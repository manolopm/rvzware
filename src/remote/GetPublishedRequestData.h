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

#ifndef _GETPUBLISHEDREQUESTDATA_
#define _GETPUBLISHEDREQUESTDATA_

#include <remote/RemoteExport.h>
#include <remote/DataStream.h>
#include <remote/MessageData.h>

/*!
 *  \file GetPublishedRequestData.h
 */

namespace cpw 
{ 
	namespace remote
	{

		/*!
		 *  \class GetPublishedRequestData GetPublishedRequestData.h <remote/messages/GetPublishedRequestData.h>
		 *  \ingroup remote
		 *  \brief Message used to request the tree of published entities
		 *
		 *  Message used to request the tree of published entities.
		 *
		 *  Stream contents:
		 *  - 6 bytes - Control data
		 */
		class REMOTE_EXPORT GetPublishedRequestData : public MessageData
		{
		public:
			GetPublishedRequestData();

			bool IsRequest();
			bool IsResponse();

			DataStream Code();
			bool Decode(const DataStream &_data);
		};

	}

}

#endif
