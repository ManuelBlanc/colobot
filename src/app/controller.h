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

/**
 * \file app/controller.h
 * \brief CController class
 */

#pragma once

#include "common/event.h"
#include "common/singleton.h"

class CApplication;
class CRobotMain;
namespace Ui {
class CMainDialog;
}

/**
 * \class CController
 * \brief Entry point into CRobotMain and CMainDialog
 */
class CController : public CSingleton<CController>
{
public:
    CController(CApplication* app, bool loadProfile = true);
    ~CController();
    
    //! Return CApplication instance
    CApplication*    GetApplication();
    //! Return CRobotMain instance
    CRobotMain*      GetRobotMain();
    //! Return CMainDialog instance
    Ui::CMainDialog* GetMainDialog();
    
    //! Event processing
    void ProcessEvent(Event &event);
    
    //! Start the application
    void StartApp();
    //! Starts the simulation, loading the given scene
    void StartGame(std::string cat, int chap, int lvl);

private:
    CApplication*    m_app;
    CRobotMain*      m_main;
    Ui::CMainDialog* m_dialog;
};