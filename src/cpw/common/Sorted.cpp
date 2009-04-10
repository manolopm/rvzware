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
#include <sstream>

//#include <stdafx.h>

#include <cpw/common/Sorted.h>


using namespace cpw;


int Sorted::min = -1;
int Sorted::max = 1;
			

Sorted::Sorted(const cpw::TypeId id, const std::string &class_name): Loggable(id, class_name), position()
{
	SetFirst();
}


Sorted::Sorted(const Sorted &sorted): Loggable(sorted), position(sorted.position)
{
	SetFirst();
}


Sorted::~Sorted(void)
{

}


Sorted &Sorted::operator = (const Sorted &sorted)
{
	Loggable::operator = (sorted);

	position = sorted.position;

	return *this;
}


void Sorted::SetFirst()
{
	position = min--;

	Modified("position", position);
}

void Sorted::SetLast()
{
	position = max++;

	Modified("position", position);
}

void Sorted::SetValue(const std::string &f, const std::string &v)
{
	if(f == "position")
	{
		int p;
		std::istringstream str(v);
		str >> p;
		SetPosition(p);
	}
	else
		Loggable::SetValue(f, v);

}


inline bool Sorted::operator < (const Sorted &sorted)
{
	return position < sorted.position;
}

inline bool Sorted::operator > (const Sorted &sorted)
{
	return position > sorted.position;
}


int Sorted::CreatePersistence() {

	Loggable::CreatePersistence();

	//Aggregate the class attributes to the persistence
	std::ostringstream str;
	str << position;
	AddPersistence((std::string)"position", str.str());

	return 0;
}


int Sorted::AdaptPersistence(Node *root)
{
	Loggable::AdaptPersistence(root);

	std::istringstream str(root->GetChildValue("position"));
	str >> position;

	if(position <= min) min = position - 1; 
	if(position >= max) max = position + 1; 

	return 0;
}
