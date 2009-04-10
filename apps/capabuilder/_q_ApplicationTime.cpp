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

#include "stdafx.h"
#include "ApplicationTime.h"


ApplicationTime * ApplicationTime::instance = NULL;

ApplicationTime::ApplicationTime(void) : time0(wxDateTime::UNow()),
									     time1(wxDateTime::UNow()),
										 ttime(wxDateTime::UNow()),
										 running(true),
										 multiplier(1.0f),
										 established_period(false),
										 minute_notify(0)
{
	
}

ApplicationTime::~ApplicationTime(void)
{
}

ApplicationTime * ApplicationTime::GetInstance()
{
	if (instance == NULL)
		instance = new ApplicationTime;

	return instance;
}
void ApplicationTime::ReleaseInstance()
{
	if (instance != NULL)
		delete instance;
}

void ApplicationTime::Update()
{
	if (!running)
	{
		Notify();
		return;
	}
	
	wxDateTime aux(wxDateTime::UNow());
	wxTimeSpan time_span1(aux.GetHour(), aux.GetMinute(), aux.GetSecond(), aux.GetMillisecond());
	wxTimeSpan time_span2(time1.GetHour(), time1.GetMinute(), time1.GetSecond(), time1.GetMillisecond());
	wxTimeSpan difference = time_span1.Subtract(time_span2);
	difference *= (int)multiplier;

	ttime.Add(difference);

	if (established_period)
	{
		if (ttime.IsLaterThan(end_date))
			ttime = end_date;
		else
			if (ttime.IsEarlierThan(init_date))
				ttime = init_date;
	}

	time0 = time1;
	time1 = aux;
	
	Notify(); //update the observers
	
}


void ApplicationTime::On()
{
	running = true;
	time0 = wxDateTime::UNow();
	time1 = wxDateTime::UNow();
}
void ApplicationTime::Off()
{
	running = false;
}
void ApplicationTime::Pause()
{
	running = false;
}
void ApplicationTime::Resume()
{
	running = true;
	time0 = time1;
	time1 = wxDateTime::UNow();
}

void ApplicationTime::SetPeriod(const wxDateTime &_init_date, const wxDateTime &_end_date)
{
	init_date = _init_date;

	end_date = _end_date;

	established_period = true;	
}


void ApplicationTime::SetTransformedTime(const wxDateTime &new_ttime)
{ 
	ttime = new_ttime;
	Notify(); //update the observers
}