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
#ifndef _SUBJECT_
#define _SUBJECT_

#include <vector>
#include <cpw/common/Observer.h>
#include <cpw/Export.h>

namespace cpw
{
	/** 
		\brief This class implements the subject in the Observer Pattern
		\ingroup cpw
	*/
	class CPWEXPORT Subject
	{
		public:

			/** Default constructor */
			Subject();
			
			/** Copy constructor 
				\param subject to copy
			*/
			Subject(const Subject &subject);

			/** Destructor */
			virtual ~Subject(void);

			/** Attaches a new observer to this subject
				\param observer to attach
			*/
			void Attach(Observer *observer);

			/** Detaches one observer from the list of observers
				\param observer to detach
			*/
			void Detach(Observer *observer);

			/** Notifies all the observers when the subject is modified */
			void Notify();

			/** Overload of operator =
				\param subject to copy
			*/
			Subject &operator = (const Subject &subject);

		private:

			std::vector<Observer *> observer_vec; ///< list of attached observers

	};
}

#endif
