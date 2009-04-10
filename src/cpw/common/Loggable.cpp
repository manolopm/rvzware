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

#include <cpw/common/Loggable.h>

using namespace cpw;


time_t CurrentTime()
{
	tzset();
	return (time(NULL) - timezone) * 1000;
}



time_t Loggable::start_time = CurrentTime();

Loggable::Loggable(const cpw::TypeId id, const std::string &class_name)
	: Persistent(id, class_name), start_dt(0), otr(false)
{
	creation_time = CurrentTime();
}


Loggable::Loggable(const Loggable &loggable)
	: Persistent(loggable), creation_time(loggable.creation_time), start_dt(loggable.start_dt),
	otr(false)
{

}


Loggable::~Loggable(void)
{

}


Loggable &Loggable::operator = (const Loggable &loggable)
{
	Persistent::operator = (loggable);

	changes = loggable.changes;

	creation_time = loggable.creation_time;

	start_dt = loggable.start_dt;

	return *this;
}


void Loggable::Modified(const std::string &attribute, const std::string &val)
{
	if (!otr)
	{
		time_t dt;
		clock_t a = clock();
		dt = start_dt + (((double)clock() / CLOCKS_PER_SEC) * 1000);

		changes.push_back(Change(attribute, val, dt));
	}

	Persistent::Modified();
}

void Loggable::Modified(const std::string &attribute, bool val)
{
	std::ostringstream str;
	str << val;

	Modified(attribute, str.str());
}

void Loggable::Modified(const std::string &attribute, int val)
{
	std::ostringstream str;
	str << val;

	Modified(attribute, str.str());
}

void Loggable::Modified(const std::string &attribute, unsigned long int val)
{
	std::ostringstream str;
	str << val;

	Modified(attribute, str.str());
}

void Loggable::Modified(const std::string &attribute, float val)
{
	std::ostringstream str;
	str << val;

	Modified(attribute, str.str());
}

void Loggable::Modified(const std::string &attribute, float val1, float val2)
{
	std::ostringstream str;
	str << val1 << " " << val2;

	Modified(attribute, str.str());
}


void Loggable::Modified(const std::string &attribute, float val1, float val2, float val3)
{
	std::ostringstream str;
	str << val1 << " " << val2 << " " << val3;

	Modified(attribute, str.str());
}

void Loggable::Modified(const std::string &attribute, double val1, double val2, double val3)
{
	std::ostringstream str;
	str << val1 << " " << val2 << " " << val3;

	Modified(attribute, str.str());
}



const Change Loggable::GetLastChange() const
{
	if (changes.size() == 0)
		return Change("", "", 0);
	return changes.back();
}


const Change Loggable::GetLastChange(const std::string &field) const
{
	std::vector<Change>::const_reverse_iterator it = changes.rbegin();

	while (it != changes.rend())
	{
		if (it->field == field)
			return *it;
		else
			it++;
	}

	return Change();
}


time_t Loggable::GetCurrTime()
{
	time_t dt;
	clock_t a = clock();
	dt = start_dt + (((double)clock() / CLOCKS_PER_SEC) * 1000);

	return dt;
}


time_t Loggable::GetCurrTime() const
{
	time_t dt;
	clock_t a = clock();
	dt = start_dt + (((double)clock() / CLOCKS_PER_SEC) * 1000);

	return dt;
}


void Loggable::SetCurrTime(const time_t &ts)
{
	start_dt = ts - (((double)clock() / CLOCKS_PER_SEC) * 1000);
}


const std::vector<Change> Loggable::GetChangesSince(time_t dt) const
{
	std::vector<Change>::const_iterator it = changes.begin();

	while (it != changes.end())
	{
		if (it->time_stamp > dt)
			return std::vector<Change>(it, changes.end());
		else
			it++;
	}

	return std::vector<Change>();
}


bool Loggable::CheckCollision(const Change &change)
{
	std::vector<Change>::iterator it = changes.begin();

	while (it != changes.end())
	{
		if (it->time_stamp == change.time_stamp && it->field == it->field)
			return true;
	}

	return false;
}


bool Loggable::AddChange(const Change &change, bool replace)
{
	std::vector<Change>::iterator it = changes.end()-1;
	int pos = changes.size()-1;

	while (pos >= 0)
	{
		if (it->time_stamp <= change.time_stamp)
		{
			if (it->time_stamp == change.time_stamp)
			{
				if (it->field == change.field)
				{
					if (replace)
					{
						it->value = change.value;
						return true;
					}
					else
						return false;
				}
				else
				{
					it--;
					pos--;
				}
			}
			else
			{
				if (pos == changes.size()-1)
				{
					changes.push_back(change);
				}
				else
				{
					it++;
					changes.insert(it, change);
				}
				return true;
			}
		}
		else
		{
			it--;
			pos--;
		}
	}

	changes.push_back(change);
	return true;
}


bool Loggable::GetRegisterChanges()
{
	return !otr;
}


void Loggable::SetRegisterChanges(bool reg)
{
	otr = !reg;
}

void Loggable::SetValue(const std::string &f, const std::string &v)
{
	Persistent::SetValue(f, v);
}



int Loggable::CreatePersistence() {

	Persistent::CreatePersistence();

	std::ostringstream str;
	str << creation_time;
	AddPersistence((std::string)"creation_time", str.str());

	//Aggregate the class attributes to the persistence
	AddPersistenceLevel((std::string)"changes");

	for(std::vector<Change>::iterator i = changes.begin(); i != changes.end(); i++) 
	{
		AddPersistenceLevel((std::string)"change");
		std::ostringstream str;
		str << i->time_stamp;
		AddPersistence((std::string)"time_stamp", str.str());
		AddPersistence((std::string)"field", i->field);
		AddPersistence((std::string)"value", i->value);
		RemovePersistenceLevel();
	}

	time_t dt;
	clock_t a = clock();
	dt = start_dt + (((double)clock() / CLOCKS_PER_SEC) * 1000);

	AddPersistenceLevel((std::string)"change");
	std::ostringstream str2;
	str2 << dt;
	AddPersistence((std::string)"time_stamp", str2.str());
	AddPersistence((std::string)"field", std::string());
	AddPersistence((std::string)"value", std::string());
	RemovePersistenceLevel();

	RemovePersistenceLevel();

	return 0;
}


int Loggable::AdaptPersistence(Node *root)
{
	Persistent::AdaptPersistence(root);

	std::vector<cpw::Node *> &components = root->GetChildren();
	std::vector<cpw::Node *>::iterator i = components.begin();
	
	i = components.begin();
	while ((i != components.end()) && ((*i)->GetName() != std::string("changes"))) i++;

	changes.clear();
	if (i != components.end())
	{
		std::vector<cpw::Node *> &_changes = (*i)->GetChildren();
		std::vector<cpw::Node *>::iterator j = _changes.begin();
		for(;j!=_changes.end();j++)
		{
			Change change;
			std::vector<cpw::Node *> change_comp = (*j)->GetChildren();

			std::istringstream str((*j)->GetChildValue("time_stamp"));
			str >> change.time_stamp;
			
			change.field = (*j)->GetChildValue("field");
			change.value = (*j)->GetChildValue("value");
			changes.push_back(change);
		}
	}

	i = components.begin();
	while ((i != components.end()) && ((*i)->GetName() != std::string("creation_time"))) i++;

	if (i == components.end())
	{
		creation_time = CurrentTime();
		start_dt = 0;

		return 0; //changes are ignored as the timestamps doesn't make sense without creation_time
	}

	std::istringstream str((*i)->GetValue());
	str >> creation_time;

	if (start_time > creation_time + changes.back().time_stamp)
		start_dt = start_time - creation_time;
	else
		start_dt = changes.back().time_stamp + 50;

	return 0;
}
