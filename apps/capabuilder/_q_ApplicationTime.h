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


#ifndef _APPLICATIONTIME_
#define _APPLICATIONTIME_

#include <string>
#include <wx/datetime.h>

#include <cpw/common/Subject.h>


/** 
	\brief Controls the application time and serves this information to different observers.
	Implements the Observer Pattern
*/

class ApplicationTime : public cpw::Subject
{
	public:

		static ApplicationTime * GetInstance();
		static void ReleaseInstance();	

		void On();
		void Off();
		void Pause();
		void Resume();
		
		float & GetMultiplier() { return multiplier; }
		void SetMultiplier(const float &new_multiplier) { multiplier = new_multiplier; }
		
		void Update();
		
		wxDateTime & GetCurrentTime() { return time1; }
		wxDateTime & GetPastTime() { return time0; }
		wxDateTime & GetTransformedTime() { return ttime; }
		wxDateTime & GetInitDate() { return init_date; }
		wxDateTime & GetEndDate() { return end_date; }

		void SetTransformedTime(const wxDateTime &new_ttime);

		void SetPeriod(const wxDateTime &_init_date, const wxDateTime &_end_date);
		void UnsetPeriod() { established_period = false;}
		bool GetPeriodEstablished() { return established_period; }

	private:

		ApplicationTime(void);
		~ApplicationTime(void);

		static ApplicationTime *instance;

		wxDateTime time0, time1, ttime, init_date, end_date;
		bool established_period;

		float multiplier;
		std::string date;
		unsigned int hour, seconds;
		bool running;
		int minute_notify;


};

#endif