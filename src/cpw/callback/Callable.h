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
#ifndef _CALLABLE_
#define _CALLABLE_

#include <vector>
#include <cpw/callback/ICallBack.h>
#include <cpw/callback/IGraphicCallBack.h>
#include <cpw/common/types.h>
#include <cpw/Export.h>

/**
	\namespace cpw Capaware \brief Capaware library for basic layer management
    \defgroup cpw Capaware library
 */



namespace cpw 
{

	class CPWEXPORT ICallBack;
	class CPWEXPORT IGraphicCallBack;

	/** 
		\brief This class allows to introduce several callbacks into the Entity class 
		\ingroup cpw
	*/
	class CPWEXPORT Callable
	{

		public:

			/** Constructor */
			Callable();					     

			/** Copy Constructor 
				Clones every callback from the \a callable Entity
				\param callable is the entity to be copied
			*/
			Callable(const Callable &callable);  

			/** Destructor 
			    Frees the set of callbacks if they have been initialized
			*/
			~Callable();

			/** Inserts the graphical representation of the entity into the scene */
			void GraphicInsert();

			/** Updates the graphical representation of the entity in the scene */
			void GraphicUpdate();

			/** Removes the graphical representation of the entity in the scene */
			void GraphicDelete();
			
			/** Sets the graphical representation of the entity in the scene */
			void GraphicSetVisible();

			/** Sets the graphical representation of the entity in the scene 
				\param value indicates the value of the visible flag (true or false)
			*/
			void Visualize(const bool &value);

			/** Sets the graphical representation of the entity in the scene 
				\param time indicates the current execution time
			*/
			void Animate(cpw::cpwTime &time);

			/** Sets the graphical representation of the entity in the scene 
			*/
			bool IsAnimated() { if (animate_callback != NULL) return true; return false; }

			/** Sets the graphical representation of the entity in the scene 
				\param callback
			*/
			void AddCallBack(cpw::ICallBack* callback);

			/** Sets the graphical representation of the entity in the scene 
				\param callback
			*/
			void SetAnimateCallBack(IGraphicCallBack *callback); 

			/** Sets the graphical representation of the entity in the scene 
				\param callable
			*/
			Callable &operator = (const Callable &callable);


		private:

			/** \todo Revisar lo del vector de callbacks --> mejor es un método para cada cosa */
			std::vector<cpw::ICallBack*> callback_vector;  ///< Contains a collection of callbacks

			/** \todo Integrar el animate_callback con los callback gráficos */
			IGraphicCallBack *animate_callback;			   ///< Is the callback for graphic animations 
			
	};
}

#endif

