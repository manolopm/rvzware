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
#ifndef _CONTAINERLAYERCALLBACK_
#define _CONTAINERLAYERCALLBACK_

#include <cpw/callback/IGraphicCallBack.h>
#include <cpw/Export.h>

namespace cpw
{
	/** 
		\brief This class defines the behaviour of the ContainerLayer callbacks 
		\ingroup cpw
	*/
	class CPWEXPORT ContainerLayerCallBack : public IGraphicCallBack
	{
		public:
			/** Default constructor */
			ContainerLayerCallBack();

			/** Copy constructor
				\param callback
			*/
			ContainerLayerCallBack(const ContainerLayerCallBack &callback);		
			
			/** Destructor */
			~ContainerLayerCallBack(void);

			/** Overload of operator = 
				\param callback
			*/
			ContainerLayerCallBack &operator = (const ContainerLayerCallBack &callback);

			/** Virtual function to implement the prototype pattern */
			virtual ContainerLayerCallBack* Clone();

			/** Allows to insert a callable ...
				\param callable
			*/
			virtual void Insert(const cpw::Callable* callable);

			/** Allows to ...
				\param callable
			*/
			virtual void Update(cpw::Callable* callable);

			/** Allows to ...
				\param callable
			*/
			virtual void Delete(cpw::Callable* callable);
	};
}

#endif