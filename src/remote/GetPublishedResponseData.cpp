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
#include <list>
#include <string>

#include "stdafx.h"

#include <cpw/common/pstdint.h>
#include <remote/DataStream.h>
#include <remote/GetPublishedResponseData.h>

using namespace cpw::remote;

void TreeFlatten(PublishedNode *root_node, std::list<uint8_t> &stream);
void TreeDeflatten(PublishedFolder *root_node, std::list<uint8_t> &stream);


/*!
 *  Sets the corresponding MessageType
 */
GetPublishedResponseData::GetPublishedResponseData() : entities(NULL)
{
	SetMessageType(msgTypeGetPublishedResponse);
}


GetPublishedResponseData::~GetPublishedResponseData()
{
	if (entities != NULL)
		delete entities;
}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  Returns an empty DataStream in case there is an error.
 */
DataStream GetPublishedResponseData::Code()
{
	try
	{
		std::list<uint8_t> stream;

		TreeFlatten(entities, stream);
		stream.push_back(1);

		uint32_t msg_size = stream.size() + 6;

		DataStream data(msg_size);

		data << (uint8_t) message_type;
		data << msg_size;
		data << request_id;

		std::list<uint8_t>::iterator i;
		for (i = stream.begin(); i != stream.end(); ++i)
			data << *i;

		return data;
	}
	catch (DataStreamException)
	{
		return DataStream();
	}
}


/*!
 *  \brief Creates the message from a DataStream
 *  \param data_stream The data to be used to compose the message
 *
 *  Creates the message from a DataStream.
 *
 *  Returns a true on success. Returns false otherwise.
 */
bool GetPublishedResponseData::Decode(const DataStream &data_stream)
{
	try
	{
		DataStream data(data_stream);
		//Get the control data
		uint8_t tmp8;
		data >> tmp8;
		message_type = (MessageType) tmp8;

		uint32_t msg_size;
		data >> msg_size;

		if (data.GetSize() != msg_size)
			return false;

		data >> tmp8;
		request_id = tmp8;

		PublishedFolder *tmp_root = new PublishedFolder("Published Entities", cpw::TypeId());

		std::list<uint8_t> stream;

		int i = 6;
		while (i<data.GetSize())
			stream.push_back(data[i++]);

		bool first=true;

		if (stream.size() > 0)
		{
			unsigned char front;
			do
			{
				TreeDeflatten(tmp_root, stream);
				front = stream.front();
				stream.pop_front();
				if (front==2)
				{
					front = stream.front();//should be  = 1
					stream.pop_front();
				}

				if (first)
				{
					if (tmp_root->GetNodes().size()==1)
					{
						entities = *(tmp_root->GetNodes().begin());
						tmp_root->DetachChildren();
						delete tmp_root;
						tmp_root = (PublishedFolder*)entities;
					}
					else
						entities = tmp_root;
						//otherwise the message is incorrect, but for now it doesn't handle this error
					first = false;
				}


			} while (front!=1);
		}

		return true;
	}
	catch (DataStreamException)
	{
		return false;
	}
}


/*!
 *  \brief Tells wheter the message is a request or not
 *
 *  Tells wheter the message is a request or not.
 */
bool GetPublishedResponseData::IsRequest()
{
	return false;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool GetPublishedResponseData::IsResponse()
{
	return true;
}


/*!
 *  \brief Returns the root of the tree contained in the message
 *
 *  Returns the root of the tree contained in the message.
 */
PublishedNode *GetPublishedResponseData::GetEntities()
{
	return entities;
}


/*!
 *  \brief Returns the root of the tree contained in the message
 *
 *  Returns the root of the tree contained in the message.
 */
PublishedNode *GetPublishedResponseData::GetEntities() const
{
	return entities;
}


/*!
 *  \brief Sets the tree to be contained in the message
 *  \param root_node The root of the tree
 *
 *  Sets the tree to be contained in the message.
 */
void GetPublishedResponseData::SetEntities(PublishedNode *root_node)
{
	entities = root_node;
}


/*!
 *  \brief Method used to flatten the tree
 *  \param root_node The root tree
 *  \param stream The resulting stream of data
 *
 *  Method used to flatten the tree.
 */
void TreeFlatten(PublishedNode *root_node, std::list<uint8_t> &stream)
{
	cpw::TypeId id;
	id = root_node->GetId();

	for (int i=0; i < cpw::TypeId::size(); i++)
		stream.push_back(id[i]);

	std::string name = root_node->GetName();

	stream.push_back((uint8_t)name.size());
	for (uint32_t i=0;i<name.size();i++)
		stream.push_back(name[i]);

	if (!root_node->IsLeaf())
	{
		std::vector<PublishedNode *> children = ((PublishedFolder*)root_node)->GetNodes();
		
		if (children.size()>0)
			stream.push_back(3);
		else
			stream.push_back(4);

		std::vector<PublishedNode *>::iterator i;
		for (i = children.begin(); i < children.end(); ++i)
		{
			TreeFlatten(*i,stream);
			if ((i+1)==children.end())
				stream.push_back(2);
			else
				stream.push_back(0);
		}
	}
}


/*!
 *  \brief Method used to deflatten the tree
 *  \param root_node The root of the tree being constructed
 *  \param stream The stream of data used as input
 *
 *  Method used to deflatten the tree.
 */
void TreeDeflatten(PublishedFolder *root_node, std::list<uint8_t> &stream)
{
	cpw::TypeId id;

	for (int i=0; i < cpw::TypeId::size(); i++)
	{
		id[i] = stream.front();
		stream.pop_front();
	}
	
	uint32_t name_size = stream.front();
	stream.pop_front();

	std::string name;
	for (;name_size>0;name_size--)
	{
		name += stream.front();
		stream.pop_front();
	}

	switch (stream.front())
	{
	case 0:
		{
			PublishedLeaf *node = new PublishedLeaf(name, id);
			root_node->Add(node);
			break;
		}
	case 1:
		break; //error - It shouldn't get this point. Otherwise it is a wrong stream
	case 2:
		{
			PublishedLeaf *node = new PublishedLeaf(name, id);
			root_node->Add(node);
			break;
		}
	case 3:
		{
			stream.pop_front(); //consume byte
			PublishedFolder *node = new PublishedFolder(name, id);
			unsigned char front;
			do
			{
				TreeDeflatten(node, stream);
				front = stream.front();
				stream.pop_front();
				if (front != 0 && front != 2)
					return;
			} while (front != 2);
			root_node->Add(node);
			break;
		}
	case 4:
		{
			//empty tree
			stream.pop_front();
			PublishedFolder *node = new PublishedFolder(name, id);
			root_node->Add(node);
			break;
		}
	default:
		break; //error - It shouldn't get this point. Otherwise it is a wrong stream
	}
}

