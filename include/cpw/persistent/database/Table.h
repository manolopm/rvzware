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
#ifndef _TABLE_
#define _TABLE_

#include <string>
#include <vector>

#include <cpw/entity/ContainerLayer.h>
#include <cpw/Export.h>

namespace cpw
{

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT Table : public cpw::ContainerLayer
	{
		public:

			Table(const cpw::TypeId id = cpw::TypeId(), 
					 const std::string &class_name = "Table");
			
			virtual ~Table(void);

			Table(const Table &table);

			Table &operator = (const Table &table);

			virtual Entity* Clone() {return new Table(*this);}

			virtual int Add(Entity *entity);

			void SetEmpty(bool boEmtpy){boEmptyTable=boEmtpy;}
			bool IsEmpty()
			{
				return boEmptyTable;
			}

		private:
			bool boEmptyTable;
			

		protected:
			
			virtual int CreatePersistence();

			virtual int AdaptPersistence(cpw::Node *root);

	};

}
#endif