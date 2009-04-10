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
#ifndef _MODEL3DCALLBACK_
#define _MODEL3DCALLBACK_

#include <cpw/callback/IGraphicCallBack.h>
#include <cpw/common/types.h>
#include <cpw/Export.h>


namespace cpw
{

	/** 
		\brief This class implements the IGraphicCallBack interface for the Element3D entity
		\ingroup cpw
	*/
	class CPWEXPORT Model3DCallBack : public IGraphicCallBack
	{
		public:

			/** Default constructor */
			Model3DCallBack();

			/** Copy constructor
				\param callback
			*/	
			Model3DCallBack(const Model3DCallBack &callback);
			
			/** Destructor */
			virtual ~Model3DCallBack();

			/** Overload of operator = 
				\param callback
			*/
			Model3DCallBack &operator = (const Model3DCallBack &callback);

			/** Implementation of the Insert method
				\param callable
			*/
			virtual void Insert(const cpw::Callable* callable);

			/** Implementation of the Update method
				\param callable
			*/
			virtual void Update(cpw::Callable* callable);

			/** Implementation of the Delete method
				\param callable
			*/
			virtual void Delete(cpw::Callable* callable);

			/** Implementation of the Animate method
				\param callable
			*/
			virtual void Animate(cpw::Callable* callable, cpw::cpwTime &time);

			/** Implementation of the Visualize method
				\param callable
			*/
			virtual void Visualize(cpw::Callable* callable, const bool &value);


			/** Implementation of the Clone method */
			virtual ICallBack* Clone();
	};
}

#endif
