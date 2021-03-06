/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2014, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsiteс.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

#include "graphics/engine/modelmanager.h"

#include "app/app.h"

#include "common/logger.h"

#include "graphics/engine/engine.h"

#include <cstdio>

template<> Gfx::CModelManager* CSingleton<Gfx::CModelManager>::m_instance = nullptr;

namespace Gfx {

CModelManager::CModelManager(CEngine* engine)
{
    m_engine = engine;
}

CModelManager::~CModelManager()
{
}

bool CModelManager::LoadModel(const std::string& fileName, bool mirrored)
{
    GetLogger()->Debug("Loading model '%s'\n", fileName.c_str());

    CModelFile modelFile;

    if (CApplication::GetInstance().IsDebugModeActive(DEBUG_MODELS))
        modelFile.SetPrintDebugInfo(true);

    if (!modelFile.ReadModel("models/" + fileName))
    {
        GetLogger()->Error("Loading model '%s' failed\n", fileName.c_str());
        return false;
    }

    ModelInfo modelInfo;
    modelInfo.baseObjRank = m_engine->CreateBaseObject();
    modelInfo.triangles = modelFile.GetTriangles();

    if (mirrored)
        Mirror(modelInfo.triangles);

    FileInfo fileInfo(fileName, mirrored);
    m_models[fileInfo] = modelInfo;

    std::vector<VertexTex2> vs(3, VertexTex2());

    for (int i = 0; i < static_cast<int>( modelInfo.triangles.size() ); i++)
    {
        int state = modelInfo.triangles[i].state;
        std::string tex1Name = "objects/"+modelInfo.triangles[i].tex1Name;
        if(modelInfo.triangles[i].tex1Name.empty())
            tex1Name.clear();
        std::string tex2Name = modelInfo.triangles[i].tex2Name;

        if (modelInfo.triangles[i].variableTex2)
        {
            state |= ENG_RSTATE_DUAL_BLACK;

            /*TODO: This seems to be not used by Colobot
            if (texNum >= 1 && texNum <= 10)
                state |= ENG_RSTATE_DUAL_BLACK;

            if (texNum >= 11 && texNum <= 20)
                state |= ENG_RSTATE_DUAL_WHITE;*/
            tex2Name = m_engine->GetSecondTexture();
        }

        vs[0] = modelInfo.triangles[i].p1;
        vs[1] = modelInfo.triangles[i].p2;
        vs[2] = modelInfo.triangles[i].p3;

        m_engine->AddBaseObjTriangles(modelInfo.baseObjRank, vs, ENG_TRIANGLE_TYPE_TRIANGLES,
                                      modelInfo.triangles[i].material, state,
                                      tex1Name, tex2Name,
                                      modelInfo.triangles[i].lodLevel, false);
    }

    return true;
}

bool CModelManager::AddModelReference(const std::string& fileName, bool mirrored, int objRank)
{
    auto it = m_models.find(FileInfo(fileName, mirrored));
    if (it == m_models.end())
    {
        if (!LoadModel(fileName, mirrored))
            return false;

        it = m_models.find(FileInfo(fileName, mirrored));
    }

    m_engine->SetObjectBaseRank(objRank, (*it).second.baseObjRank);

    return true;
}

bool CModelManager::AddModelCopy(const std::string& fileName, bool mirrored, int objRank)
{
    auto it = m_models.find(FileInfo(fileName, mirrored));
    if (it == m_models.end())
    {
        if (!LoadModel(fileName, mirrored))
            return false;

        it = m_models.find(FileInfo(fileName, mirrored));
    }

    int copyBaseObjRank = m_engine->CreateBaseObject();
    m_engine->CopyBaseObject((*it).second.baseObjRank, copyBaseObjRank);
    m_engine->SetObjectBaseRank(objRank, copyBaseObjRank);

    m_copiesBaseRanks.push_back(copyBaseObjRank);

    return true;
}

bool CModelManager::IsModelLoaded(const std::string& fileName, bool mirrored)
{
    return m_models.count(FileInfo(fileName, mirrored)) > 0;
}

int CModelManager::GetModelBaseObjRank(const std::string& fileName, bool mirrored)
{
    auto it = m_models.find(FileInfo(fileName, mirrored));
    if (it == m_models.end())
        return -1;

    return (*it).second.baseObjRank;
}

void CModelManager::DeleteAllModelCopies()
{
    for (int baseObjRank : m_copiesBaseRanks)
    {
        m_engine->DeleteBaseObject(baseObjRank);
    }

    m_copiesBaseRanks.clear();
}

void CModelManager::UnloadModel(const std::string& fileName, bool mirrored)
{
    auto it = m_models.find(FileInfo(fileName, mirrored));
    if (it == m_models.end())
        return;

    m_engine->DeleteBaseObject((*it).second.baseObjRank);

    m_models.erase(it);
}

void CModelManager::UnloadAllModels()
{
    for (auto& mf : m_models)
        m_engine->DeleteBaseObject(mf.second.baseObjRank);

    m_models.clear();
}

void CModelManager::Mirror(std::vector<ModelTriangle>& triangles)
{
    for (int i = 0; i < static_cast<int>( triangles.size() ); i++)
    {
        VertexTex2  t = triangles[i].p1;
        triangles[i].p1 = triangles[i].p2;
        triangles[i].p2 = t;

        triangles[i].p1.coord.z = -triangles[i].p1.coord.z;
        triangles[i].p2.coord.z = -triangles[i].p2.coord.z;
        triangles[i].p3.coord.z = -triangles[i].p3.coord.z;

        triangles[i].p1.normal.z = -triangles[i].p1.normal.z;
        triangles[i].p2.normal.z = -triangles[i].p2.normal.z;
        triangles[i].p3.normal.z = -triangles[i].p3.normal.z;
    }
}

float CModelManager::GetHeight(std::vector<ModelTriangle>& triangles, Math::Vector pos)
{
    const float limit = 5.0f;

    for (int i = 0; i < static_cast<int>( triangles.size() ); i++)
    {
        if ( fabs(pos.x - triangles[i].p1.coord.x) < limit &&
             fabs(pos.z - triangles[i].p1.coord.z) < limit )
            return triangles[i].p1.coord.y;

        if ( fabs(pos.x - triangles[i].p2.coord.x) < limit &&
             fabs(pos.z - triangles[i].p2.coord.z) < limit )
            return triangles[i].p2.coord.y;

        if ( fabs(pos.x - triangles[i].p3.coord.x) < limit &&
             fabs(pos.z - triangles[i].p3.coord.z) < limit )
            return triangles[i].p3.coord.y;
    }

    return 0.0f;
}


}

