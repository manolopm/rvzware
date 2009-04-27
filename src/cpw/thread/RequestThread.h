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

#ifndef _REQUESTTHREAD_
#define _REQUESTTHREAD_

#include <string>
#include <map>

#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>
#include <OpenThreads/Condition>

#include <cpw/thread/IRequestReceiver.h>
#include <cpw/common/IStatusController.h>
#include <cpw/Export.h>


namespace cpw
{


	/** 
		\brief 
		\ingroup cpw
	*/
	class CPWEXPORT RequestThread: public OpenThreads::Thread, public cpw::IRequestReceiver
	{

	public:

		RequestThread(IRequestReceiver *visual, int npet = 30000, IStatusController *status = NULL);

		virtual ~RequestThread(void);

		int ProcessRequest(cpw::Request &request);

		int DecrementNid(const long int id);

		int ReturnRequest(cpw::Request &request);

		int SendRequest(cpw::Request &request);

		void AddThread(RequestThread *thread);

		virtual void run();

		void Stop();

		//Methods to be reimplemented inside the derived clasess

		//This method is called when there is a new incoming request to process
		virtual void Process(cpw::Request &request) {}

		//This method is called before sending the result of a request to the calling RequestReceiver
		virtual void ProcessReturn(cpw::Request &request) {};
		
		//This method is called after the thread is awaken when one request arrives
		virtual void PreProcess() {};

		virtual int NumberOutRequests() {return 1;}

		void Clear();
		void ClearInvoker();


		//Progress control
					
		virtual bool IsDetermined	(){return true;}
		virtual int  GetRange		(){return 100;}
		virtual int	 GetValue		(){return 10;}
		virtual int	 GetStep		(){return 1;}
		virtual std::string GetLabel(){return "";}
		virtual bool GetModal       (){return false;}

		void Initialize();

		virtual void Modify(cpw::Entity *entity);


	private:
		void ClearRequests();

		int max_request;
		int requests;
		unsigned long int request_number;
		bool stop;

		std::map<long int, cpw::Request> incoming_requests; 
		std::map<long int, cpw::Request> outgoing_requests;

		cpw::IRequestReceiver      *in_thread;
		std::vector<RequestThread*> out_thread;
		std::vector<RequestThread*>::iterator next_thread;

		OpenThreads::Mutex mutex;
		OpenThreads::Mutex cond_mutex;  
		OpenThreads::Condition cond;

		IStatusController *status_controller;
		int status_id;
		float status_value;

	};


}

#endif



