/*
 * Rvzware based in CAPAWARE 3D
 *
 * Rvzware is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * Rvzware is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The Rvzware development team
 */

#include <stdafx.h>
#include <controllers/StatusController.h>

using namespace cpw::controllers;


StatusController::StatusController() 
: cpw::Subject()
{
	determined = true;
	range = 0;
	value = 0;
	step  = 0;
	label = "";
	modalTraceableItem = 0;
	TraceableList.clear();
}

StatusController::~StatusController()
{
	Clear();
}


int StatusController::Attach()
{

	mutex.lock();

	TraceableData newTraceableData;
	int newId = TraceableList.size() + 1;
	TraceableList[newId] = newTraceableData;
	
	mutex.unlock();

	Init(newId);

	Update();


	return newId;
}

void StatusController::Detach(int traceableItem)
{
	mutex.lock();

	if (!TraceableList.empty()) 
	{
		std::map<int, TraceableData>::iterator iter = TraceableList.find(traceableItem);
		
		if(iter != TraceableList.end())
			
			TraceableList.erase(iter);

		mutex.unlock();

		Update();

	}
	else mutex.unlock();
}

void StatusController::Clear()
{
	mutex.lock();

	if (!TraceableList.empty()) 
	{
		TraceableList.clear();
		Update();
	}
	mutex.unlock();
}

void StatusController::Update()
{
	mutex.lock();
	value = 0;
	range = 0;
	step  = 0;
	label = "";
	determined = true;
	modalTraceableItem = 0;
	if (!TraceableList.empty()) 
	{
		std::map<int, TraceableData>::const_iterator iter = TraceableList.begin();
		int i = 0;
		float percent = 0;
		for(;iter != TraceableList.end(); iter++)
		{
			TraceableData traceable = TraceableList[iter->first];
			determined &= traceable.determined;
			label = traceable.label;
			if (traceable.range > range) 
				range = traceable.range;
			if (step == 0)
				step  = traceable.step;
			else 
				if (((traceable.step  > 0)) && ((traceable.step  < step) )) 
					step  = traceable.step;
			if (traceable.range != 0)
				percent += traceable.value / (float)traceable.range;
			if (traceable.modal)
				modalTraceableItem = iter->first;
			i++;
		}
		if (i > 0) value = (int)( (percent/(float)i)*range);
	}
	mutex.unlock();

	Notify();
}

TraceableData StatusController::GetModalTraceableItem()
{
	mutex.lock();

	if (modalTraceableItem > 0)
	{
		mutex.unlock();
		return TraceableList[modalTraceableItem];
	}
	TraceableData notvalid;
	notvalid.valid = false;

	mutex.unlock();

	return notvalid;
}

void StatusController::Init (const int &id)
{
	mutex.lock();
	TraceableList[id].valid = true;
	TraceableList[id].determined = true;
	TraceableList[id].label = "";
	TraceableList[id].modal = false;
	TraceableList[id].range = 100;
	TraceableList[id].step = 1;
	TraceableList[id].value = 0;
	mutex.unlock();

}

void StatusController::SetDetermined (const int &id, const bool &value)
{
	mutex.lock();
	TraceableList[id].determined = value;
	mutex.unlock();

	Update();
}

void StatusController::SetRange (const int &id, const int &value)
{
	mutex.lock();
	TraceableList[id].range = value;
	mutex.unlock();

	Update();
}

void StatusController::SetValue	(const int &id, const int &value)
{
	mutex.lock();

	TraceableList[id].value = value;
	if (TraceableList[id].value < 0) TraceableList[id].value = 0;
	if (TraceableList[id].determined)
	{
		if (value > TraceableList[id].range) TraceableList[id].value  = TraceableList[id].range;
	}
	else 
		TraceableList[id].value = TraceableList[id].value % TraceableList[id].range;
	
	mutex.unlock();

	Update();
}

void StatusController::SetStep (const int &id, const int &value)
{
	mutex.lock();
	TraceableList[id].step = value;
	mutex.unlock();

	Update();
}

void StatusController::SetLabel (const int &id, const std::string &value)
{
	mutex.lock();
	TraceableList[id].label = value;
	mutex.unlock();

	Update();
}

void StatusController::SetModal (const int &id, const bool &value)
{
	mutex.lock();
	TraceableList[id].modal = value;
	mutex.unlock();

	Update();
}

void StatusController::Pulse (const int &id, const int &value)
{
	mutex.lock();
	TraceableList[id].value += value;
	if (TraceableList[id].value < 0) TraceableList[id].value = 0;
	if (TraceableList[id].determined)
	{
		if (value > TraceableList[id].range) TraceableList[id].value  = TraceableList[id].range;
	}
	else 
		TraceableList[id].value = TraceableList[id].value % TraceableList[id].range;
	mutex.unlock();

	Update();
}

void StatusController::Pulse (const int &id)
{
	mutex.lock();
	
	TraceableList[id].value += TraceableList[id].step;
	if (TraceableList[id].value < 0) TraceableList[id].value = 0;
	if (TraceableList[id].determined)
	{
		if (value > TraceableList[id].range) TraceableList[id].value  = TraceableList[id].range;
	}
	else 
		TraceableList[id].value = TraceableList[id].value % TraceableList[id].range;
	
	mutex.unlock();

	Update();
}
