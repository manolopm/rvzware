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

#ifndef _SORTED_
#define _SORTED_

#include <cpw/common/Loggable.h>
#include <cpw/Export.h>

#include <string>

namespace cpw
{
	/** 
		\brief This class allows to sort the entities  
		\ingroup cpw
	*/
	class CPWEXPORT Sorted: public Loggable
	{
		public:

			/** Default constructor 
				\param id unique object identifier
				\param class_name class name of the object
			*/
			Sorted(const cpw::TypeId id = cpw::TypeId(), const std::string &class_name = std::string("Sorted"));
			
			/** Copy constructor
				\param sorted to copy
			*/
			Sorted(const Sorted &sorted);

			/** Destructor */
			virtual ~Sorted(void);

			/** Overload of operator =
				\param sorted to copy
			*/
			Sorted &operator = (const Sorted &sorted);

			/** Returns the position of the Entity */
			int GetPosition() { return position;}

			/** Sets a new position 
				\param p the new position
			*/
			void SetPosition(const int p) { position = p;}

			/** Sends the Entity to the first position */
			void SetFirst();
			
			/** Sends the Entity to the last position */
			void SetLast();

			/** Modifies the value of a field
				\param field
				\param value
			*/
			virtual void SetValue(const std::string &field, const std::string &value);

			/** Compares two sorted entities and returns true if the left is smaller than the right 
				\param sorted to compare with
			*/
			inline bool operator < (const Sorted &sorted);

			/** Compares two sorted entities and returns true if the left is bigger than the right 
				\param sorted to compare with
			*/
			inline bool operator > (const Sorted &sorted);

		protected:

			/** Adds the information of the class to the persistent tree */
			virtual int CreatePersistence();
		
			/** Loads the information of the class from the persistent tree
				\param root is the top level node of the persistent tree
			*/
			virtual int AdaptPersistence(Node *root);

		private:

			static int min;  ///< the minimum value for sorting
			static int max;  ///< the maximum value for sorting
			int position;    ///< the position of the current Entity

	};
}

#endif
