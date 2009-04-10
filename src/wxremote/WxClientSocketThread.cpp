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
#include <string>
#include <wx/event.h>
#include <wx/socket.h>
#include <wx/sckstrm.h>

#include "stdafx.h"

#include <cpw/common/pstdint.h>
#include <cpw/common/RemoteNode.h>
#include <remote/DataStream.h>
#include <remote/ConnectionManager.h>

#include <wxremote/WxSocketEvent.h>
#include <wxremote/WxClientSocketThread.h>


using namespace cpw::wxremote;

double CurrentTime()
{
	return clock() * CLK_TCK / 1000.;
}

/*!
 *  \param socket The wxWidgets socket
 *  \param main_thread A pointer to an event handler in the main thread
 *
 *  The thread is not run inmediatly. You need to call wxThread::Run for that
 */
WxClientSocketThread::WxClientSocketThread(wxSocketClient *socket, wxEvtHandler *main_thread) :
	wxThread(), socket(socket), main_thread(main_thread), finished(false)
{
	if (socket->IsConnected())
	{
		connected = true;

		//TCP_NODELAY options reduces the lag of the connection by dissabling the Naggle algorithm
		//Warning!!! - SetOption and GetOption are not documented, so probably not supported either and
		//it can dissapear from future versions of wxWidgets
		//SetOption returns false when it can't set the option
		int flag = 1;
		socket->SetOption(IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
	}
	else
		connected = false;
}


WxClientSocketThread::~WxClientSocketThread()
{
	delete socket;
}


/*!
 *  \brief Main loop of the thread
 *
 *  Main loop of the thread.
 */
wxThread::ExitCode WxClientSocketThread::Entry()
{
	socket->SetTimeout(10); //Set timeout for operations in 10 seconds
	while (!TestDestroy() && !finished)
	{
		if (!connected)
			WaitForConnect();
		else
		{
			if (!socket->IsConnected())
				SendDisconnected();
			else
			{
				ProcessDelayedWrite();

				//Check messages from main thread
				WxSocketEvent evt;
				int timeout = ReceiveTimeout(1, evt);
				if (timeout != wxMSGQUEUE_TIMEOUT)
				{
					//Process message from main thread
					if (evt.GetType() == sockTypeDisconnection)
						Disconnect();
					if (evt.GetType() == sockTypeWrite)
						WriteMessage(evt);
				}
				else
					//No message from main thread, so try to read a message
					ReadMessage();
			}
		}
		//Yield();
	}

	while(!TestDestroy())
		Yield();

	return EXIT_SUCCESS;
}



void WxClientSocketThread::WriteMessage(WxSocketEvent evt)
{
	cpw::remote::DataStream data(evt.GetData());
	uint8_t *temp = (uint8_t*)data.GetDataPointer();

	int message_type = (int)temp[0];
	if (message_type == cpw::remote::msgTypeSetValue)
	{
		//Delay this kind of messages
		cpw::remote::SetValueData *message =
			new cpw::remote::SetValueData();
		message->Decode(data);
		cpw::TypeId entity_id = message->GetEntityId();
		std::string field = message->GetField();
		delayed_msg[std::pair<cpw::TypeId, std::string>(entity_id, field)] =
			data;
	}
	else
	{
		int tempsize = data.GetSize();
		wxSocketFlags flags = socket->GetFlags();
		socket->SetFlags(wxSOCKET_WAITALL);
		wxSocketOutputStream *stream = new wxSocketOutputStream(*socket);
		stream->Write(temp, tempsize);
		delete stream;
		socket->SetFlags(flags);
	}
}


void WxClientSocketThread::Disconnect()
{
	socket->Close();
	SendDisconnected();
}


void WxClientSocketThread::SendDisconnected()
{
	wxIPV4address addr;
	std::string hostname;
	int port;

	if	(socket->GetPeer(addr))
	{
		hostname = addr.IPAddress().To8BitData();
		port = addr.Service();
	}

	WxSocketEvent evt;
	evt.SetType(sockTypeDisconnection);
	evt.SetNode(cpw::RemoteNode(hostname, port));
	main_thread->AddPendingEvent(evt);
	finished = true;
}



void WxClientSocketThread::ReadMessage()
{
	//Try to read data from socket
	//Yield(); //avoid closing problems
	if (socket->IsData() && !TestDestroy())
	{
		//First of all, we read the size of the message
		uint8_t temp[6];
		if (socket->Peek(temp, 6).LastCount() != 6)
		{
			//Discard message
			//socket->Discard();
			return;
		}

		//else, everything looks fine
		uint32_t msg_size;

		((uint8_t*)&msg_size)[0] = temp[1];
		((uint8_t*)&msg_size)[1] = temp[2];
		((uint8_t*)&msg_size)[2] = temp[3];
		((uint8_t*)&msg_size)[3] = temp[4];
		msg_size = cpw::remote::DataStream::FromNetworku32(msg_size);

		uint8_t *data_array = new uint8_t[msg_size];
		wxUint32 read_size = 0;

		int skip_yield = 0;
		while (read_size < msg_size)
		{
			if (TestDestroy())
			{
				delete [] data_array;
				return;
			}

			socket->Read(&(data_array[read_size]), (msg_size - read_size));

			if (socket->Error())
			{
				//Somehow socket->Error() == true can mean == wxSOCKET_NOERROR
				if (socket->LastError() != wxSOCKET_TIMEDOUT &&
					socket->LastError() != wxSOCKET_NOERROR)
				{
					//something went wrong
					socket->Discard();
					delete [] data_array;
					return;
				}
			}

			read_size += socket->LastCount();

			//if the message is too big, yield every certain number of reads
			//or the UI will feel sluggish
			if (read_size < msg_size && msg_size > 100)
			{
				if (skip_yield % 5==0)
				{
					skip_yield = 0;
					Yield();
				}
				else
					skip_yield++;
			}
		}

		cpw::remote::DataStream data(read_size, data_array);
		delete [] data_array;

		wxIPV4address addr;
		std::string hostname;
		int port;

		//Obtenemos la dirección del host remoto, para actualizar la interfaz
		if	(socket->GetPeer(addr))
		{
			hostname = addr.IPAddress().To8BitData();
			port = addr.Service();
		}

		if (TestDestroy())
			return;

		/*int message_type = (int)data[0];
		if (message_type == cpw::remote::msgTypeSetValue)
		{
			//Delay this kind of messages
			cpw::remote::SetValueData *message =
				new cpw::remote::SetValueData();
			message->Decode(data);
			cpw::TypeId entity_id = message->GetEntityId();
			std::string field = message->GetField();
			delayed_msg[std::pair<cpw::TypeId, std::string>(entity_id, field)] =
				data;
		}
		else
		{*/
			WxSocketEvent evt;
			evt.SetType(sockTypeRead);
			evt.SetData(data);
			evt.SetNode(cpw::RemoteNode(hostname, port));
			main_thread->AddPendingEvent(evt);
		//}
	}
}

void WxClientSocketThread::WaitForConnect()
{
	WxSocketEvent evt;
	int timeout = ReceiveTimeout(1, evt);
	if (timeout != wxMSGQUEUE_TIMEOUT)
	{
		if (evt.GetType() == sockTypeNewConnection && !TestDestroy())
		{
			//Create the object with IP address
			cpw::RemoteNode node = evt.GetNode();
			wxIPV4address addr;
			addr.Hostname(wxString::From8BitData(node.GetIPAddress().c_str()));
			addr.Service(node.GetPort());

			//Call "Connect" with wait =false so it doesn't block the thread indefinitly
			if (socket->Connect(addr, true))
			{
				WxSocketEvent evt;
				evt.SetType(sockTypeNewConnection);
				evt.SetNode(node);
				main_thread->AddPendingEvent(evt);	


				//TCP_NODELAY options reduces the lag of the connection by dissabling the Naggle algorithm
				//Warning!!! - SetOption and GetOption are not documented, so probably not supported either and
				//it can dissapear from future versions of wxWidgets
				//SetOption returns false when it can't set the option
				int flag = 1;
				socket->SetOption(IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
			}
			
			connected = true; //If unsuccess, next iteration this thread will send
							  //a disconnection event to the main thread
		}
	}
}



void WxClientSocketThread::ProcessDelayedRead()
{
	if (!delayed_msg.empty())
	{
		std::map<std::pair<cpw::TypeId, std::string>, cpw::remote::DataStream>::iterator it, itaux;
		for (it = delayed_msg.begin(); it!=delayed_msg.end() && !TestDestroy();)
		{
			double last_msg = 0;
			std::map<std::pair<cpw::TypeId, std::string>, double>::iterator it_lt;
			it_lt = last_processed_msg.find(it->first);
			if (it_lt != last_processed_msg.end())
				last_msg = it_lt->second;
			if (CurrentTime() >= last_msg + 90)
			{
				wxIPV4address addr;
				std::string hostname;
				int port;

				//Obtenemos la dirección del host remoto, para actualizar la interfaz
				if	(socket->GetPeer(addr))
				{
					hostname = addr.IPAddress().To8BitData();
					port = addr.Service();
				}
				WxSocketEvent evt;
				evt.SetType(sockTypeRead);
				evt.SetData(it->second);
				evt.SetNode(cpw::RemoteNode(hostname, port));
				main_thread->AddPendingEvent(evt);
				//Yield();
				last_processed_msg[it->first] = CurrentTime();
				itaux = it;
				itaux++;
				delayed_msg.erase(it);
				it = itaux;
			}
			else
				it++;
		}
	}
}



void WxClientSocketThread::ProcessDelayedWrite()
{
	if (!delayed_msg.empty())
	{
		std::map<std::pair<cpw::TypeId, std::string>, cpw::remote::DataStream>::iterator it, itaux;
		for (it = delayed_msg.begin(); it!=delayed_msg.end() && !TestDestroy();)
		{
			double last_msg = 0;
			std::map<std::pair<cpw::TypeId, std::string>, double>::iterator it_lt;
			it_lt = last_processed_msg.find(it->first);
			if (it_lt != last_processed_msg.end())
				last_msg = it_lt->second;
			if (CurrentTime() >= last_msg + 90)
			{
				wxIPV4address addr;
				std::string hostname;
				int port;

				//Obtenemos la dirección del host remoto, para actualizar la interfaz
				if	(socket->GetPeer(addr))
				{
					hostname = addr.IPAddress().To8BitData();
					port = addr.Service();
				}

				cpw::remote::DataStream data = it->second;
				uint8_t *temp = (uint8_t*)data.GetDataPointer();

				int tempsize = data.GetSize();
				wxSocketFlags flags = socket->GetFlags();
				socket->SetFlags(wxSOCKET_WAITALL);
				wxSocketOutputStream *stream = new wxSocketOutputStream(*socket);
				stream->Write(temp, tempsize);
				delete stream;
				socket->SetFlags(flags);

				//Yield();
				last_processed_msg[it->first] = CurrentTime();
				itaux = it;
				itaux++;
				delayed_msg.erase(it);
				it = itaux;
			}
			else
				it++;
		}
	}
}