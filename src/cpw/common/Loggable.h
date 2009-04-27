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
#ifndef _LOGGABLE_
#define _LOGGABLE_

#include <cpw/persistent/Persistent.h>
#include <cpw/Export.h>

#include <ctime>
#include <vector>
#include <string>

namespace cpw
{

	/** 
		\brief Registers a change on the Log when an Entity has changed
		\ingroup cpw
	
	*/
	struct Change
	{
		std::string field;  ///< Specifies the Entity field that has been changed
		std::string value;  ///< The new value of the field
		time_t time_stamp;  ///< The time when the change has occurred

		/** Default constructor */
		Change(): field(), value(), time_stamp(0) {}

		/** Constructor */
		Change(const std::string &f, const std::string &v, const time_t ts): field(f), value(v), time_stamp(ts) {}
	};


	/** This class is used to register the changes on the Entity fields
		\ingroup cpw
	*/
	class CPWEXPORT Loggable: public Persistent
	{

		public:

			/** Default constructor 
				\param id unique object identifier
				\param class_name class name of the object
			*/
			Loggable(const cpw::TypeId id = cpw::TypeId(), const std::string &class_name = std::string("Loggable"));
			
			/** Copy constructor 
				\param loggable is the entity to copy
			*/
			Loggable(const Loggable &loggable);
			
			/** Destructor */
			virtual ~Loggable();

			/** Overload of operator = 
				\param loggable is the entity to copy
			*/
			Loggable &operator = (const Loggable &loggable);

			/** Sets a new value for a given field
				\param field that is being changed
				\param value that is newly assigned
			*/
			virtual void SetValue(const std::string &field, const std::string &value);

			/** Returns the last change on the entity */
			const Change GetLastChange() const;

			/** Returns the last change for a given field
				\param field to obtain the las changed value
			*/
			const Change GetLastChange(const std::string &field) const;

			/** Returns the current time of the Entity */
			time_t GetCurrTime();

			/** Returns the current time of the Entity */
			time_t GetCurrTime() const;
			
			/** Sets the current time of the Entity */
			void SetCurrTime(const time_t &ts);

			/** Returns all the changes from a given time
				\param dt
			*/
			const std::vector<Change> GetChangesSince(time_t dt) const;

			/** Tests whether there is a collision between the registered changes and a given change
				\param change
			*/
			bool CheckCollision(const Change &change);

			/** Adds a change to the Loggable
				\param change to be introduced
				\param replace an existing change in case of collision
			*/
			bool AddChange(const Change &change, bool replace);

			/** Returns off the record mode */
			bool GetRegisterChanges();
			
			/** Sets off the record mode */
			void SetRegisterChanges(bool reg);

		protected:

			/** Adds the information of the class to the persistent tree */
			virtual int CreatePersistence();

			/** Loads the information of the class from the persistent tree
				\param root is the top level node of the persistent tree
			*/
			virtual int AdaptPersistence(Node *root);

			/** Registers a new change when it occurres
				\param attribute that is changed
				\param value the new value of the attribute
			*/
			void Modified(const std::string &attribute = std::string(),
				const std::string &val = std::string());

			/** Registers a new change when it occurres
				\param attribute that is changed
				\param value the new value of the attribute
			*/
			void Modified(const std::string &attribute, bool val);

			/** Registers a new change when it occurres
				\param attribute that is changed
				\param value the new value of the attribute
			*/
			void Modified(const std::string &attribute, int val);

			/** Registers a new change when it occurres
				\param attribute that is changed
				\param value the new value of the attribute
			*/
			void Modified(const std::string &attribute, unsigned long int val);

			/** Registers a new change when it occurres
				\param attribute that is changed
				\param value the new value of the attribute
			*/
			void Modified(const std::string &attribute, float val);

			/** Registers a new change when it occurres
				\param attribute that is changed
				\param value the new value of the attribute
			*/
			void Modified(const std::string &attribute, float val1, float val2);

			/** Registers a new change when it occurres 
				\param attribute that is changed
				\param val1 the new value of the attribute
				\param val2 the new value of the attribute
				\param val3 the new value of the attribute
			*/
			void Modified(const std::string &attribute, float val1, float val2, float val3);
			void Modified(const std::string &attribute, double val1, double val2, double val3);

		private:
			time_t creation_time;	       ///< creation time of the entity
			time_t start_dt;			   ///< stat time of the entity
			static time_t start_time;	   ///< start system time
			std::vector<Change> changes;   ///< list of changes
			bool otr;					   ///< Off the record mode

	};
}

#endif
