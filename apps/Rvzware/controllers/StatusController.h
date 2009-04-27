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

#ifndef _STATUS_CONTROLLER_
#define _STATUS_CONTROLLER_

#include <map>

#include <OpenThreads/Mutex>
#include <cpw/common/IStatusController.h>
#include <cpw/common/Subject.h>

#include <gui/UIProgressionModalDialog.h>

namespace cpw
{

	namespace controllers
	{
		/** 
			\brief This controllers shows the progress bar during the execution of a process
			\ingroup cpw
		*/
		struct TraceableData
		{
			bool valid;
			bool determined;
			int range;
			int value;
			int step;
			std::string label;
			bool modal;
		};


		/** 
			\brief 
			\ingroup cpw
		*/
		class StatusController : public IStatusController, Subject
		{
		
		public:
				
			StatusController();
			~StatusController();

			int  Attach();
			void Detach(int traceableItem);
			void Clear();
			void Update();

			bool Empty			()					{return TraceableList.empty();};
			bool IsDetermined	()					{return determined;}
			int  GetRange		()					{return range;}
			int	 GetValue		()					{return value;}
			int	 GetStep		()					{return step;}	
			std::string GetLabel()					{return label;}	
			virtual bool GetModal()					{return (modalTraceableItem!=0);};
			TraceableData GetModalTraceableItem();

			void Init			(const int &id);
			void SetDetermined	(const int &id, const bool &value = true);
			void SetRange		(const int &id, const int &value = 100);
			void SetValue		(const int &id, const int &value = 0);
			void SetStep		(const int &id, const int &value = 1);
			void SetLabel		(const int &id, const std::string &value);
			void SetModal       (const int &id, const bool &value = false);
			void Pulse			(const int &id, const int &value);
			void Pulse			(const int &id);


		private:

			std::map< int, TraceableData > TraceableList;
			int modalTraceableItem;

			bool determined;
			int range;
			int value;
			int step;
			std::string label;

			OpenThreads::Mutex mutex;

		};
	}
}

#endif

