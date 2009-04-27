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

#ifndef _GETPUBLISHEDRESPONSEDATA_
#define _GETPUBLISHEDRESPONSEDATA_

#include <list>
#include <string>

#include <cpw/common/pstdint.h>
#include <remote/RemoteExport.h>
#include <remote/DataStream.h>
#include <remote/MessageData.h>
#include <remote/PublishedTree.h>

/*!
 *  \file GetPublishedResponseData.h
 */

namespace cpw 
{ 
	namespace remote
	{

		/*!
		 *  \class GetPublishedResponseData GetPublishedResponseData.h <remote/messages/GetPublishedResponseData.h>
		 *  \ingroup remote
		 *  \brief Message used to send the tree of published entities
		 *
		 *  Message used to send the tree of published entities.
		 *
		 *  Stream contents:
		 *  - 6 bytes - Control data
		 *  - ? bytes - Flatten tree
		 *
		 *	----  METHOD USED FOR TREE FLATTERING  ------
		 *
		 *	A message containing a tree of published entities is formed this way:
		 *	-# Entity: id (4 bytes), number_of_chars_of_name (1 byte), name_string (number_of_char_of_name bytes)
		 *	-# Label indicating tree structure (1 byte). Possible values are:
		 *		-# Next sibling. Indicating that next we will find an entity
		 *		-# End of tree
		 *		-# End siblings. Next, another label should be expected
		 *		-# Begin children.
		 */
		class REMOTE_EXPORT GetPublishedResponseData : public MessageData
		{
		public:
			GetPublishedResponseData();
			~GetPublishedResponseData();

			bool IsRequest();
			bool IsResponse();

			DataStream Code();
			bool Decode(const DataStream &_data);

			PublishedNode *GetEntities();

			PublishedNode *GetEntities() const;

			void SetEntities(PublishedNode *root_node);

		private:
			PublishedNode *entities;
		};

	}

}

#endif

