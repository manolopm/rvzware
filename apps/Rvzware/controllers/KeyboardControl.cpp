/*
 * CAPAWARE 3D Geographical Multilayer Framework Free Software
 * Copyright (C) 2009 Instituto Tecnológico de Canarias (ITC) and Universidad de Las Palmas de Gran Canaria (ULPGC)
 *
 * This file is part of CAPAWARE.
 *
 * CAPAWARE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * CAPAWARE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The CAPAWARE development team
*/
#include <stdafx.h>

#include <controllers/KeyboardControl.h>


using namespace cpw::controllers;


cpw::controllers::KeyboardControl* KeyboardControl::instance = NULL;



KeyboardControl *KeyboardControl::GetInstance()
{
	if(instance == NULL)
		instance = new KeyboardControl;

	return instance;
}

void KeyboardControl::ReleaseInstance()
{
	if (instance != NULL)
	{
		delete instance;
		instance = NULL;
	}
}
			

KeyboardControl::KeyboardControl(void)
{
	//keys = new bool[KEYBOARD_SIZE];
	//keys = (bool *) malloc(sizeof(bool) * KEYBOARD_SIZE);
	//memset(keys, 0, KEYBOARD_SIZE);

	for (int i=0; i<KEYBOARD_SIZE; i++)
		keys[i] = false;
}

KeyboardControl::~KeyboardControl(void)
{
	int debug;
	debug = 1;
}


void KeyboardControl::KeyDown(int k)
{
	if ((k < 0) || (k >= KEYBOARD_SIZE))
		return;

	keys[k] = true; 
}

void KeyboardControl::KeyUp(int k)
{
	if ((k < 0) || (k >= KEYBOARD_SIZE))
		return;

	keys[k] = false; 

}
		
bool KeyboardControl::KeyPressed(int k)
{
	if ((k < 0) || (k >= KEYBOARD_SIZE))
		return false;

	return keys[k];
}
