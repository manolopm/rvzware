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

#include <sstream>
#include <string>

//#include <stdafx.h>

#include <cpw/persistent/database/Table.h>
#include <cpw/persistent/database/DataBase.h>
#include <cpw/persistent/database/PersistentDBCallBack.h>

using namespace cpw;

Table::Table(const cpw::TypeId id, 
				   const std::string &class_name) : cpw::ContainerLayer(id, class_name)
{
	//Initially table without columns
	boEmptyTable=true;
}

Table::~Table(void)
{
}

Table::Table(const Table &table) : cpw::ContainerLayer(table)
{
	Modified();
}

Table &Table::operator = (const Table &table)
{
	ContainerLayer::operator =(table);


	Modified();
	return *this;
}

int Table::Add(Entity *entity)
{
	ContainerLayer::Add(entity);

	entity->SetPersistentCallBack(new PersistentDBCallBack);

	return 0;
}

int Table::CreatePersistence() 
{
	ContainerLayer::CreatePersistence();

	return 0;
}

int Table::AdaptPersistence(cpw::Node *root)
{
	ContainerLayer::AdaptPersistence(root);

	return 0;
}
