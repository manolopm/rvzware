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
#include <string>
#include <vector>

#include <stdafx.h>

#include <cpw/entity/Entity.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/entity/Element3D.h>
#include <cpw/persistent/database/DataBase.h>
#include <cpw/persistent/database/Table.h>

#include <cpw/ApplicationConfiguration.h>
#include <controllers/PrimitiveController.h>
#include <controllers/PersistentController.h>
#include <controllers/DataBaseController.h>



using namespace cpw::controllers;



//! DB Controller constructor
DataBaseController::DataBaseController()
{

}

//! DB Controller destructor
DataBaseController::~DataBaseController()
{
}

//! Adds a database to the layer
	/*!
	Adds a database to the layer tree (still under development)
		\param parent	wxWindow to be used as parent
		\param layer_tree Layer used to include the database
	*/
void DataBaseController::AddDataBase(wxWindow* parent, cpw::LayerTree &layer_tree)
{

	//Database instance
	cpw::DataBase new_databasep;
	cpw::Entity* new_database = cpw::EntityFactory::GetInstance()->CreateEntity(new_databasep.GetClassName());
	
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
	new_database->SetIcon(icon_path + "/folder.bmp");
  
	cpw::DataBase &DB= *((cpw::DataBase*) new_database);
	
	//Here an interface must be used to setup the connection

	//At this moment the connection is fixed making use of a local database for debugging
	//instead of launching the proper interface for that purpose
	//Name assigned to the connection
	DB.SetName("EjemploDB");
	//IP address
	DB.SetDBHost("127.0.0.1");//localhost
	//User
	DB.SetDBUser("postgres");
	//password
	DB.SetDBPasswd("cavacan07");
	//Database name (multiple DBs can be available in the same machine)
	DB.SetDBName("g4wd");
	
	//The connection is stablished
	DB.SetConnected(ConnectDB(DB));

	//Checking if the DB has some tables. In that case they are added to the layer-tree
	AddTablesToTree(DB);
	
	//Registering the connection
	Register(new_database);

	layer_tree.AddToActiveLayer(new_database);

	//Persistence management
	PersistentController persistent;

	cpw::PersistentError ferror = persistent.Save(parent, new_database, ".cla", false);
}


//! Adds a table to the layer
	/*!
	Adds a table to the layer tree (still under development)
		\param parent	wxWindow to be used as parent
		\param layer_tree Layer used to include the database
	*/
void DataBaseController::CreateTable(wxWindow* parent, cpw::LayerTree &layer_tree)
{
	
	cpw::DataBase db;

	//Checking the right location to add a table
	if(layer_tree.GetLastContainer()->GetClassName() == db.GetClassName())
	{
		//A Table instance is created
		cpw::Table new_table;
		cpw::Entity* table = cpw::EntityFactory::GetInstance()->CreateEntity(new_table.GetClassName());
		
		//The icon isassigned
		std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
		table->SetIcon(icon_path + "/folder.bmp");
	  
		//Here an interface must be used to setup the table name
		//Currently a fixed name is used
		table->SetName("Tabla1");

		//Registering the table
		Register(table);

		layer_tree.AddToActiveLayer(table);

		//Persistence management
		PersistentController persistent;

		cpw::PersistentError ferror = persistent.Save(parent, table, ".cla", false);
	}
}



/*************************************************************************************

Database access routines

*************************************************************************************/
//! Disconnects a given database connection
	/*!
	The provided database connection is disconnected
		\param DB	Database to be disconnected
	*/
void DataBaseController::DisconnectDB(cpw::DataBase &DB)
{
	//Obtiene el identificador de conexión del DataBase proporcionado
	DB.Disconnect();
}

//! Connects to a given database
	/*!
	Stablishes a connection with the given database. The function returns true if the operation is succesful
		\param DB	Database to be disconnected
	*/
bool DataBaseController::ConnectDB(cpw::DataBase &DB)  
{

	return DB.OpenConnection();
}


//! Adds the tables contained in the database to the layer-tree
	/*!
	Reads the table information of a given DB, adding them to the layer-tree
		\param DB	Database to be read
	*/
void DataBaseController::AddTablesToTree(cpw::DataBase &DB)
{

	DB.ReadTables();
}
