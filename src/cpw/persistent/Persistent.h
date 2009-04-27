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
#ifndef _PERSISTENT_
#define _PERSISTENT_

#include <string>
#include <map>

#include <cpw/common/Composite.h>
#include <cpw/persistent/IPersistentCallBack.h>
#include <cpw/common/Subject.h>
#include <cpw/common/TypeId.h>
#include <cpw/Export.h>


namespace cpw {

	/** 
		\brief This class utilizes a callback for the persistence of entities through serialization
		\ingroup cpw
	*/
	class CPWEXPORT Persistent: public Subject
	{

	public:

		/** Destructor */
		virtual ~Persistent();

		/** Returns the persistent tree */
		Node *GetPersistence();

		/** Returns the properties tree */
		Node *GetProperties();

		/** Initialize the Entity with the information in the persistent tree 
			\param root is the top level node of the persistent tree
		*/
		void  SetPersistence(Node *root);

		/** Returns true if the Entity has been modified */
		bool isModified();

		/** Returns true if the Entity has been modified */
		const bool isModified() const;

		/** Change the modified flag to false*/ 
		void Saved();

		/** Returns the Entity ID*/
		cpw::TypeId &GetId();

		/** Returns the Entity class name */
		std::string &GetClassName();

		/** Returns the Entity url */
		std::string GetUrl();

		/** Returns the Entity url */
		std::string &GetCompoundDirectory();

		/** Returns the Entity class name */
		const cpw::TypeId &GetId()  const;
		
		/** Returns the Entity class name */
		const std::string &GetClassName() const;
		
		/** Returns the Entity url */
		const std::string GetUrl() const;

		/** Returns the Entity url */
		const std::string &GetCompoundDirectory() const;
		
		/** Sets the Entity url
			\param url of the Entity
		*/
		void SetUrl(const std::string &url);

		/** Sets the Entity url
			\param url of the Entity
		*/
		void SetCompoundDirectory(const std::string &url);

		/** Sets the Entity ID
			\param id 
		*/
		void SetID(const cpw::TypeId id);
		
		/** Sets the class name of the Entity
			\param class_name
		*/
		void SetClassName(const std::string &class_name);

		/** Template method pattern (to be redefined in every child class)
			It is usefull to introduce the value of an attribute as a pair of strings (field - value)
			\param field attribute to modify
			\param value for the attribute
		*/
		virtual void SetValue(const std::string &field, const std::string &value);

		/** Saves the Entity using the persistent callback
			\param overwrite 
		*/
		PersistentError Save(bool overwrite = false);

		/** Loads the Entity using the persistent callback
		*/		
		PersistentError Load();
		
		/** Updates the Entity using the persistent callback
		*/		
		PersistentError Update();
		
		/** Deletes the Entity using the persistent callback
		*/		
		PersistentError Delete();

		/** Sets the persistent callback */
		void SetPersistentCallBack(IPersistentCallBack *callback); 

	protected:

		/** Default constructor 
			\param id Entity identifier
			\param class_nam Entity class name
		*/
		Persistent(const cpw::TypeId id = cpw::TypeId(), const std::string &class_nam = std::string("Persistent"));

		/** Copy constructor 
			\param persistent to copy
		*/
		Persistent(const Persistent &persistent);

		/** Overload of operator =
			\param persistent to copy
		*/
	    Persistent &operator = (const Persistent &persistent);

		/** Creates the persistent tree visiting the Entity hierarchy.
		    Template method pattern (to be redefined in every child class).
		*/
		virtual int CreatePersistence();

		/** Creates the persistent tree visiting the Entity hierarchy.
		    Template method pattern (to be redefined in every child class).
		*/
		virtual int CreateProperties();

		/** Inserts the persistent tree into the Entity hierarchy.
		    Template method pattern (to be redefined in every child class).
			\param root is the top level node of the persistent tree
		*/
		virtual int AdaptPersistence(Node *root);

		/** Initialize the persistent tree
			\param name of the top level node
		*/
		void InitPersistence(const std::string &name);

		/** Adds a new node to the persistent tree
			\param name of the new node
			\param value of the node
		*/
		void AddPersistence(const std::string &name, const std::string &value);

		/** Adds a new node to the persistent tree
			\param name of the new node
			\param attrs a set of attributes (field + value)
		*/
		void AddPersistence(const std::string &name, const std::map<std::string, std::string> &attrs);

		/** Adds a new folder node to the persistent tree
			\param name of the new node
		*/
		void AddPersistenceLevel(const std::string &name);

		/** Goes back to the previous folder node in the persistent tree */
		void RemovePersistenceLevel();

		/** Returns the modified flag */
		void Modified();
	
		/** Returns the global path to the file
			\param url is the path to compose with the entity global path
		*/
		const std::string ComposeDirectory(const std::string &url) const;

		/** Returns the relative path to the file
			\param url is the url for which we want to find the relative path with respect to the entity path
		*/
     	const std::string RelativeDirectory(const std::string &url) const;

		/** Returns the path without intermediate ../ or ./
			\param path is the url to clean
		*/
		void CleanPath(std::string &path) const;

	private:

		cpw::TypeId id;			  ///< Entity identifier

		std::string class_name;   ///< Entity class name

		std::string url;		  ///< Entity url

		std::string filename;		  ///< Entity filename

		std::string compound_directory; ///< Complete directory of the Entity

		bool modified;			  ///< Modified flag (true if the Entity has been modified)

		Node *root;				  ///< Top level node of the persistent tree

		std::vector<Node *> composite_stack;	   ///< Structure of nodes for the persistent tree 

		IPersistentCallBack *persistent_callback;  ///< Persistent callback

		std::string ToLower(const std::string &str) const;



	};

}


#endif
