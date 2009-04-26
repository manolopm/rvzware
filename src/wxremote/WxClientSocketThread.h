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

#ifndef _WXCLIENTSOCKETTHREAD_
#define _WXCLIENTSOCKETTHREAD_

#include <wx/event.h>
#include <wx/socket.h>
#include <wx/thread.h>

#include <wxremote/msgqueue.h>

#include <remote/DataStream.h>
#include <wxremote/WxSocketEvent.h>

/*!
 *  \file WxClientSocketThread.h
 */

namespace cpw 
{ 
	namespace wxremote
	{
		/*!
		 *  \class WxClientSocketThread WxClientSocketThread.h <wxremote/WxClientSocketThread.h>
		 *  \ingroup wxremote
		 *  \brief This class is a thread that deals with a wxWidgets socket
		 *
		 *  This class is a thread that deals with a wxWidgets socket.
		 */
		class WxClientSocketThread : public wxThread, public wxMessageQueue<WxSocketEvent>
		{
		public:
			WxClientSocketThread(wxSocketClient *_socket, wxEvtHandler *_main_thread);
			~WxClientSocketThread();

			wxThread::ExitCode Entry();
				
		private:
			void WriteMessage(WxSocketEvent evt);
			void Disconnect();
			void SendDisconnected();
			void ReadMessage();
			void WaitForConnect();
			void ProcessDelayedRead();
			void ProcessDelayedWrite();

			wxEvtHandler *main_thread;  //Link to handle events
			wxSocketClient *socket;
			bool finished;
			bool connected;
			std::map<std::pair<cpw::TypeId, std::string>, cpw::remote::DataStream> delayed_msg;
			std::map<std::pair<cpw::TypeId, std::string>, double> last_processed_msg;

		};
	}
}

#endif

