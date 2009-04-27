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
#ifndef _ISTATUS_CONTROLLER_
#define _ISTATUS_CONTROLLER_

#include <vector>


namespace cpw
{
	/** 
		\brief This interface is used to control the progression of long tasks.
		The application will show a progression control.
		\ingroup cpw
	*/
	class IStatusController 
	{
	
	public:
			
		/** Destructor */
		virtual ~IStatusController(){}
		
		/** */
		virtual int  Attach() = 0;
		
		/** */
		virtual void Detach(int id)= 0;
		
		/** */
		virtual void Clear()= 0;
		
		/** */
		virtual void Update()= 0;
		
		/** */
		virtual bool Empty()= 0;
		
		/** */
		virtual bool IsDetermined	()= 0;
		
		/** */
		virtual int  GetRange		()= 0;
		
		/** */
		virtual int	 GetValue		()= 0;
		
		/** */
		virtual int	 GetStep		()= 0;
		
		/** */
		virtual std::string GetLabel()= 0;
		
		/** */
		virtual bool GetModal       () = 0;
		
		/** 
			\param id
		*/
		virtual void Init			(const int &id) = 0;
		
		/** 
			\param id
			\param value
		*/
		virtual void SetDetermined	(const int &id, const bool &value)= 0;
		
		/** 
			\param id
			\param value
		*/
		virtual void SetRange		(const int &id, const int &value)= 0;
		
		/** 
			\param id
			\param value
		*/
		virtual void SetValue		(const int &id, const int &value)= 0;
		
		/** 
			\param id
			\param value
		*/
		virtual void SetStep		(const int &id, const int &value)= 0;
		
		/** 
			\param id
			\param value
		*/
		virtual void SetLabel		(const int &id, const std::string &value)= 0;
		
		/** 
			\param id
			\param value
		*/
		virtual void SetModal       (const int &id, const bool &value) = 0;
		
		/** 
			\param id
			\param value
		*/
		virtual void Pulse			(const int &id, const int &value)=0;
		
		/** 
			\param id
		*/
		virtual void Pulse			(const int &id)=0;

	};
}
#endif



