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

#include <iostream>
#include <fstream>

#include <OpenThreads/Barrier>

//#include <stdafx.h>

#include <cpw/thread/RequestThread.h>
#include <cpw/entity/EntityRegistry.h>

#define WHEEL_FACTOR 5.0f

using namespace cpw;

RequestThread::RequestThread(IRequestReceiver *in, int npet, IStatusController *status): in_thread(in), request_number(1), max_request(npet), 
			out_thread(), next_thread(), incoming_requests(), outgoing_requests(), stop(false), status_controller(status), requests(1)
{
	setSchedulePriority(THREAD_PRIORITY_MIN);

	if(status_controller != NULL)
	{
		status_id = status_controller->Attach();
		status_value = 100;
		status_controller->SetValue(status_id,(const int &)status_value);
	}
}

RequestThread::~RequestThread()
{
	Stop();	

	for(std::vector<RequestThread *>::iterator i = out_thread.begin(); i != out_thread.end(); i++)
	{
		delete *i;	
		*i = NULL;
	}
	if(status_controller != NULL)
		status_controller->Detach(status_id);

}

void RequestThread::Modify(cpw::Entity *entity)
{

	for(std::vector<RequestThread *>::iterator i = out_thread.begin(); i != out_thread.end(); i++)

		(*i)->Modify(entity);	
}


void RequestThread::AddThread(RequestThread *thread) 
{   
	out_thread.push_back(thread);

	next_thread = out_thread.begin();

	if(!thread->isRunning())
		
		thread->start();

}

void RequestThread::run() 
{

	do {
		
		mutex.lock();

		if(!incoming_requests.empty()) 
		{
			std::map<long int, cpw::Request>::iterator i = incoming_requests.begin(); 
			long int id = i->first;
			cpw::Request request = i->second;
			incoming_requests.erase(i);

			request.nid = NumberOutRequests();

			outgoing_requests[id] = request;

			request.id = id;

			mutex.unlock();

			Process(request);
		}
		else 
		{
			mutex.unlock();

			cond_mutex.lock();
			cond.wait(&cond_mutex);
			cond_mutex.unlock();

			PreProcess();
		}

	}while (!stop && !testCancel());

}


void RequestThread::Stop() 
{
   //mutex.lock();
   //in_thread = NULL;
   stop = true;
   //mutex.lock();

   //Clear();

   for(std::vector<RequestThread *>::iterator i = out_thread.begin(); i != out_thread.end(); i++)
   	(*i)->Stop();

   while(isRunning())
   {
		cond_mutex.lock();
		cond.signal();
		cond_mutex.unlock();		
   }
}


int RequestThread::ProcessRequest(cpw::Request &request)
{
	mutex.lock();
	incoming_requests[request_number] = request;

	if(status_controller != NULL)
	{
		requests++;
		status_value = 100 - ((requests - 1)*WHEEL_FACTOR);
		if (status_value < 1) status_value = 1;
		status_controller->SetValue(status_id,(const int &)status_value);
		status_controller->SetLabel(status_id, "Downloading data...");
	}

	if((int)incoming_requests.size() >= max_request)

		incoming_requests.erase(incoming_requests.begin());

	request_number++;
	mutex.unlock();

	cond_mutex.lock();
	cond.signal();
	cond_mutex.unlock();

    return 0;
}

int RequestThread::DecrementNid(const long int id)
{
	return --(outgoing_requests[id].nid);
}

int RequestThread::ReturnRequest(cpw::Request &request) 
{
	mutex.lock();

	if(!stop)
	{
		ProcessReturn(request);

		long int id = request.id;

		outgoing_requests[id].file = request.file;
		int position = request.layer->GetPosition();
		outgoing_requests[id].tiles[position] = request.file;
		outgoing_requests[id].composed_url = request.composed_url;

		if(request.error)
			outgoing_requests[id].error = true;
		
		if( DecrementNid(id) == 0)
		{
			if(status_controller != NULL)
			{
				requests--;
				if (requests <1) requests = 1;
				status_value = 100 - ((requests - 1)*WHEEL_FACTOR);
				status_controller->SetValue(status_id, (const int &)status_value);
				status_controller->SetLabel(status_id, "Data received");
				if(status_value > 100)
					status_controller->SetValue(status_id, 100);


			}

			in_thread->ReturnRequest(outgoing_requests[id]);

			outgoing_requests.erase(id);
		}
	}
	mutex.unlock();

	return 0;
}

int RequestThread::SendRequest(cpw::Request &request)
{
	if(!out_thread.empty())
	{
		(*next_thread)->ProcessRequest(request);

		next_thread++;

		if(next_thread == out_thread.end()) next_thread = out_thread.begin();
	}

	return 0;
}

void RequestThread::Clear()
{
	ClearRequests();
	requests = 1;

	for(std::vector<RequestThread *>::iterator i = out_thread.begin(); i != out_thread.end(); i++)

		(*i)->Clear();

}

void RequestThread::ClearInvoker()
{
	ClearRequests();	
	in_thread->ClearInvoker();

}

void RequestThread::Initialize()
{
	Clear();
	in_thread->ClearInvoker();
}


void RequestThread::ClearRequests()
{
	mutex.lock();
	incoming_requests.clear(); 
	outgoing_requests.clear();
	mutex.unlock();
}

