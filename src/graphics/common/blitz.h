// * This file is part of the COLOBOT source code
// * Copyright (C) 2001-2008, Daniel ROUX & EPSITEC SA, www.epsitec.ch
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program. If not, see  http://www.gnu.org/licenses/.

// blitz.h

#pragma once


#include "common/misc.h"
#include "common/struct.h"


class CInstanceManager;
class CD3DEngine;
class CTerrain;
class CCamera;
class CSound;



const float BLITZPARA	= 200.0f;		// radius of lightning protection
const int BLITZMAX		= 50;

enum BlitzPhase
{
	BPH_WAIT,
	BPH_BLITZ,
};



class CBlitz
{
public:
	CBlitz(CInstanceManager* iMan, CD3DEngine* engine);
	~CBlitz();

	void		Flush();
	bool		EventProcess(const Event &event);
	bool		Create(float sleep, float delay, float magnetic);
	bool		GetStatus(float &sleep, float &delay, float &magnetic, float &progress);
	bool		SetStatus(float sleep, float delay, float magnetic, float progress);
	void		Draw();

protected:
	bool		EventFrame(const Event &event);
	CObject*	SearchObject(D3DVECTOR pos);

protected:
	CInstanceManager*	m_iMan;
	CD3DEngine*			m_engine;
	CTerrain*			m_terrain;
	CCamera*			m_camera;
	CSound*				m_sound;

	bool		m_bBlitzExist;
	float		m_sleep;
	float		m_delay;
	float		m_magnetic;
	BlitzPhase	m_phase;
	float		m_time;
	float		m_speed;
	float		m_progress;
	D3DVECTOR	m_pos;
	FPOINT		m_shift[BLITZMAX];
	float		m_width[BLITZMAX];
};
