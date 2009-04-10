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
#include <sstream>

//#include <stdafx.h>


#include <cpw/entity/Entity.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/Element3D.h>
#include <cpw/persistent/database/DataBase.h>
#include <cpw/persistent/database/Table.h>
#include <cpw/persistent/database/PersistentDBCallBack.h>


using namespace cpw;

//! DB constructor
	/*!
	Database constructor. 
	\param id	Identifier
	\param class_name Class name
	*/
DataBase::DataBase(const cpw::TypeId id, 
				   const std::string &class_name) : cpw::ContainerLayer(id, class_name)
{
	db_boConnected=false;//Initially no connection has been stablished
}

//! DB destructor
	/*!
	Database destructor. 
	*/
DataBase::~DataBase(void)
{
	Disconnect();
}

//! Adds entity to the layer
	/*!
	Adds entity to the layer
	\param entity Entity to be added
	*/
int DataBase::Add(Entity *entity)
{
	ContainerLayer::Add(entity);

	entity->SetPersistentCallBack(new PersistentDBCallBack);

	return 0;
}


//! DB Disconnction
	/*!
	If the connection is currently stablished the connection is finished
	*/
void DataBase::Disconnect()
{
	//Only if the connection is currently stablished
	if (db_boConnected)
	{	
		PQfinish(db_conn);
		db_boConnected=false;
	}
}

DataBase::DataBase(const DataBase &database) : cpw::ContainerLayer(database)
{
	db_boConnected=false;//Initially not connected
	Modified();
}

DataBase &DataBase::operator = (const DataBase &database)
{
	ContainerLayer::operator =(database);

	Modified();
	return *this;
}

//! Persistence manager for DB
	/*!
	Persistence manager for DB
	*/
int DataBase::CreatePersistence() 
{
	ContainerLayer::CreatePersistence();

	//Elements to be saved
	AddPersistence(std::string("db_host"), db_host);
	AddPersistence(std::string("db_user"), db_user);
	AddPersistence(std::string("db_passwd"), db_passwd);
	AddPersistence(std::string("db_name"), db_name);
	
	//Arrays are managed diversely
	AddPersistenceLevel((std::string)"db_tablenames");
	for(std::vector<std::string>::iterator i= db_tablenames.begin(); i!=db_tablenames.end(); i++)
	{
		AddPersistence(std::string("db_tablename"), *i);
	}		
	RemovePersistenceLevel();

	AddPersistenceLevel((std::string)"db_columnspertable");

	for(std::vector<std::vector<std::string> >::iterator i= db_columnspertable.begin(); i!=db_columnspertable.end(); i++)
	{
		AddPersistenceLevel((std::string)"db_columns");
		for(std::vector<std::string>::iterator j = i->begin(); j != i->end(); j++)
		{
			AddPersistence(std::string("db_columnname"), *j);
		}		
		RemovePersistenceLevel();
	}		
	RemovePersistenceLevel();

	return 0;
}

int DataBase::AdaptPersistence(cpw::Node *root)
{
	ContainerLayer::AdaptPersistence(root);

	db_host =  (root->GetChildValue("db_host"));
	db_user =  (root->GetChildValue("db_user"));
	db_passwd =  (root->GetChildValue("db_passwd"));
	db_name =  (root->GetChildValue("db_name"));


	std::vector<cpw::Node *> root_children = root->GetChildren();
	std::vector<cpw::Node *>::iterator i;

	for(i = root_children.begin(); i != root_children.end(); i++)
	{
		if ((*i)->GetName() == "db_tablenames")
		{
			std::vector<cpw::Node *> table_children = (*i)->GetChildren();
			std::vector<cpw::Node *>::iterator j;
			for(j = table_children.begin(); j != table_children.end(); j++)
			{
				db_tablenames.push_back((*j)->GetValue());
			}
		}
	}

	for(i = root_children.begin(); i != root_children.end(); i++)
	{
		if ((*i)->GetName() == "db_columnspertable")
		{
			std::vector<cpw::Node *> tables_children = (*i)->GetChildren();
			std::vector<cpw::Node *>::iterator j;
			for(j = tables_children.begin(); j != tables_children.end(); j++)
			{
				std::vector<cpw::Node *> columns_children = (*j)->GetChildren();
				std::vector<cpw::Node *>::iterator k;
				db_columnspertable.push_back(std::vector<std::string>());
				for(k = columns_children.begin(); k != columns_children.end(); k++)
				{
					db_columnspertable.back().push_back((*k)->GetValue());
				}
			}
		}
	}

	//Stablishes connection if necessary
	SetConnected(OpenConnection());
	//Read DB tables
	ReadTables();


	return 0;
}

//! Gets DB tables
	/*!
	Gets DB tables returning the available number. It also saves in the database the field names
	
	*/
int DataBase::GetDBTables()
{
	//No tables initially contained
	int ntables=0;

	//Getting the connection id of the DB
	PGconn     *conn=this->GetDBConn();
	
	//Is the connection is running
	if (this->GetConnected())
	{
		PGresult   *res;
		int nt;

		//The SQL command is composed to get the table information

		//A temporal var is used to get table info
		std::vector<std::string> tables;
	
		//SQL request
		res = PQexec(conn, "select table_name from information_schema.tables where table_schema = 'public' and TABLE_TYPE = 'BASE TABLE'");

		//If no NULL answer is got
		if(res != NULL)
		{
			int npos;
					
			ExecStatusType qstatus = PQresultStatus(res);

			//Different status require different actions
			//Currently only PGRES_TUPLES_OK status is treated
			switch(qstatus) {
				case PGRES_COMMAND_OK:
					//printf("Result completed successfully.\n");
					break;
				case PGRES_EMPTY_QUERY:
					//printf("Empty query sent to server.\n");
					break;
				case PGRES_TUPLES_OK:
					ntables	=		PQntuples(res);
		
					npos=PQfnumber(res, "table_name");

					//Getting table names
					for (nt = 0; nt < ntables; nt++) 
					{
						// Get the field values (we ignore possibility they are null!!!!)
						tables.push_back(PQgetvalue(res, nt, npos));						
					}

					//Saving table names in the related DB variable
					this->SetDBTableNames(tables);

					break;
				case PGRES_COPY_OUT: // from server
				case PGRES_COPY_IN: // to server
		//			printf("Data copy between server and client in progress.\n");
					break;
				case PGRES_BAD_RESPONSE:
		//			printf("Bad response from server.\n");
					break;
				case PGRES_NONFATAL_ERROR:
		//			printf("Non-fatal error returned from server.\n");
					break;
				case PGRES_FATAL_ERROR:
					//terminate(PQresultErrorMessage(res),conn);
					break;
				default:
					//terminate("Query status unknown, terminating.\n", conn);
					break;
			}
		}
		PQclear(res);

		//Gathering information about the different columns or fiels od each table
		std::vector<std::vector<std::string> > columnspertable;
		std::vector<std::string> columnnames;

		//Thos names are obtained for each table
		for (nt=0;nt<ntables;nt++)
		{
			//The SQL request is composed
			int ncolumns=0;
			std::string str;
			str = "SELECT column_name FROM information_schema.columns WHERE table_name = '" + tables.at(nt) + "'";
			res = PQexec(conn, str.c_str());

			columnnames.clear();
		
			//If there is a good answer
			if (PQresultStatus(res) == PGRES_TUPLES_OK)
			{
				ncolumns=PQntuples(res);
			
				int npos=PQfnumber(res, "column_name");
			
				//Every colukn/field name is read
				for (int nc = 0; nc < ncolumns; nc++) 
				{
					// Get the field values (we ignore possibility they are null!!!!) 
					columnnames.push_back(PQgetvalue(res, nc, npos));						
				}

				//Adding the name to the ttemp var
				columnspertable.push_back(columnnames);
			}

			PQclear(res);

			//Saving field info in the DB instance
			this->SetDBColumnsperTable(columnspertable);

		}
	}

	//The number of tables is returned
	return ntables;
}

//! Gets the records available in a DB table for a given selection of fields
	/*!
	Gets the records available in a DB table for a given number of fields
	\param fields Fields to be returned
	\param tablename Table to be consulted	
	*/
PGresult   * DataBase::GetDBRecordsfromTable(std::string &fields, std::string &tablename)
{
	//Getting the connection id of the DB
	PGconn     *conn=this->GetDBConn();
	
	//Is the connection is running
	if (this->GetConnected())
	{
		PGresult   *res;
		std::string    db_row;//temp used to save records
	
		//Temporal vars are used to save columns etc.
		std::vector<std::vector<std::string> > columnspertable;
		std::vector<std::string> columns;

		//The SQL command is composed to get the table information
		//SQL request to get the given fields of the selected table
		int neltos=0;
		std::string str;
		str = "SELECT " + fields + " FROM " + tablename;
		res = PQexec(conn, str.c_str());

		//Is the command was succesful
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{		
			//Compiles db_row with all the records
			for (int i = 0; i < PQntuples(res); i++) 
			{
				db_row.clear();//clear
				
				for (int  j = 0; j < PQnfields(res); j++)
				{
					db_row=db_row + PQgetvalue(res, i, j) + " ";
				}
			}	
			return res;
		}

		PQclear(res);
	}
	return NULL;
}

//! Inserts a record in the database
	/*!
	Inserts a record in the database. It should return the proper identifier error but right now it returns 0 (existing file)
	\param fields Fields to be written
	\param tablename Table to be used
	\param values Field values to be written
	*/
PersistentError DataBase::InsertDBRecordinTable(std::string &tablename,std::string &fields,std::string &values)
{
	//Is connected?
	if (GetConnected())
	{
		PGresult   *res;
	
		//the SQL request is composed
		std::string str;
		
		//Using the input arguments the request is formed
		str = "INSERT INTO " + tablename + "(" + fields + ") VALUES (" + values + ")";
		res = PQexec(GetDBConn(), str.c_str());

		//If no null answer
		if(res != NULL)
		{
			//Was the command correctly executed?
			if (PQresultStatus(res) != PGRES_COMMAND_OK)
			{		
				fprintf(stderr, "INSERT failed: %s", PQerrorMessage(GetDBConn()));
				PQclear(res);
			}
			else return (cpw::PersistentError)0;

			//PQclear(res);//Should it be done always
		}
	}
	return (cpw::PersistentError)-1;
}

//! Remove records satisfying a condition
	/*!
	Remove records satisfying a condition
	\param tablename Table to be used
	\param condition Condition to be checked
	*/
void DataBase::DeleteDBRecordsinTable(std::string &tablename,std::string &condition)
{
	//Getting the connection id of the DB
	PGconn     *conn=this->GetDBConn();
	
	//I the conenction is running
	if (this->GetConnected())
	{
		//temp vars
		PGresult   *res;
	
		//SQL request
		std::string str;
		
		str = "DELETE FROM " + tablename + condition;
		res = PQexec(conn, str.c_str());

		if (PQresultStatus(res) != PGRES_COMMAND_OK)
		{		
			fprintf(stderr, "DELETE failed: %s", PQerrorMessage(conn));
			PQclear(res);
		}
		
	}

}

//! Creates a table
	/*!
	Creates a table
	\param tablename Table to be used
	*/
PersistentError DataBase::CreateDBTable(std::string &tablename)
{
	//If the connection is running 
	if (GetConnected())
	{
		PGresult   *res;

		//SQL command composition
		std::string str;
		
		//The table is created without fields, those will be added with the first record
		
		str = "CREATE TABLE " + tablename + " ()";
		res = PQexec(GetDBConn(), str.c_str());
		
		if (PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			fprintf(stderr, "CREATE failed: %s", PQerrorMessage(GetDBConn()));
			PQclear(res);
		}
		else return (cpw::PersistentError)0;

		//PQclear(res);//Is necessary?

	}
	return (cpw::PersistentError)-1;

}

//! Modifies a table
	/*!
	Modifies a table in terms of fields and their type
	\param tablename Table to be used
	\param fieldandtype Field and new type
	*/
PersistentError DataBase::AlterDBTable(std::string &tablename,std::string &fieldandtype)
{
	//If the connection is running
	if (GetConnected())
	{
		PGresult   *res;

		//The table is modified adding a columns
		std::string str;
		
		str = "ALTER TABLE " + tablename + " add " + fieldandtype ;
		res = PQexec(GetDBConn(), str.c_str());
		
		if (PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			fprintf(stderr, "ALTER failed: %s", PQerrorMessage(GetDBConn()));
			PQclear(res);
		}
		else return (cpw::PersistentError)0;

	}
	return (cpw::PersistentError)-1;

}

//! Adds a geospatial field to a table
	/*!
	Modifies a table in terms of fields and their type
	\param tablename Table to be used
	*/
PersistentError DataBase::AddLocationColumn(std::string &tablename,std::string &columnname)
{
	//Is the connection runnning
	if (GetConnected())
	{
		PGresult   *res;

		//The SQL request is composed
		std::string str;

		//Currently only a POINT element is considered

		//select AddGeometryColumn('nombretabla','utm',4326,'POINT','2')
		//This command do not use capital letters to find the table, therefore we have to transform 
		//the tablename to avoid capitals before composing the SQL request
		std::transform(tablename.begin(), tablename.end(), tablename.begin(), tolower);
		
		str = "select AddGeometryColumn('" + tablename + "','" + columnname + "',4326,'POINT','2')";
		res = PQexec(GetDBConn(), str.c_str());
		
		//If it did not work
		/*if (PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			fprintf(stderr, "AddGeometryColumn failed: %s", PQerrorMessage(GetDBConn()));
			PQclear(res);
		}
		else*/ return (cpw::PersistentError)0;

	}
	return (cpw::PersistentError)-1;

}

//! Reads tables and records
	/*!
	Reads tables and records
	*/
void DataBase::ReadTables()
{
	//Getting DB tables
	int ntables=GetDBTables();
	
	//Reading table contents if any
	if (ntables)
	{
		//Getting table names
		std::vector<std::string> tablenames=GetDBTableNames();

		//Tables are added to the tree only if they contain geospatial data
		for (int i=0;i<ntables;i++)
		{			
			//To check if a table contains geo data
			//select * from geometry_columns where f_table_name=tablenames.at(i)
			//getting the geo field name and its type
		
			//temporal var
			std::vector<std::string> tables;
		
			//SQL request
			PGresult   *resG;
			std::string aux="select * from geometry_columns where f_table_name='" + tablenames.at(i) + "'";
			resG = PQexec(GetDBConn(), aux.c_str() );

			//NO null answer
			if(resG != NULL)
			{
				//OK answer
				if (PQresultStatus(resG) == PGRES_TUPLES_OK)
				{		
					//The table contains geo data, adds the table to the tree and its records
					if (PQntuples(resG))
					{
						//Table instance
						cpw::Table new_tablep;
						cpw::Entity* new_table = cpw::EntityFactory::GetInstance()->CreateEntity(new_tablep.GetClassName());

						cpw::Table &table= *((cpw::Table*) new_table);
						
						new_table->SetName(tablenames.at(i));

						//It is assumed that the table contains columns
						table.SetEmpty(false);

						cpw::EntityRegistry::GetInstance()->Add(new_table);
						Add(new_table);

						//For each record (indeed we expect only one per table, is has not been tested with more)
						for (int nl = 0; nl < PQntuples(resG); nl++) 
						{
							//Getting the table name
							std::string columnname;
							int ncolumn=PQfnumber(resG, "f_geometry_column");

							//geo spatial field name
							columnname=PQgetvalue(resG, nl, ncolumn);
						
							//Geo information is coded, thus it must be done the inverse operation to GeometryfromText, PostGIS function asewkt allows that
							//http://www.mapbender.org/presentations/Spatial_Data_Management_Arnulf_Christl/Spatial_Data_Management_Arnulf_Christl.pdf
							//MOre utilities http://postgis.refractions.net/docs/ch03.html#id2656610

							//The table column names are requested
							//Each column is treated accordingly (specially the geospacial info)
							std::string consulta;

							std::string str,Name;
							str = "SELECT column_name FROM information_schema.columns WHERE table_name = '" + tablenames.at(i) + "'";
							PGresult   *resC = PQexec(GetDBConn(), str.c_str());
							
							//If there are columns we compose in column the total collection of fields to be read from the table
							if (PQresultStatus(resC) == PGRES_TUPLES_OK)
							{
								//Position of the column containing the column_name
								int npos=PQfnumber(resC, "column_name");
							
								//For each column
								for (int  j = 0; j < PQntuples(resC); j++)
								{
									//The name is read
									std::string strtmp;
									strtmp=PQgetvalue(resC, j, npos);

									if (j)
									{
										if (strtmp.compare(columnname)==0)//Column with geo spatial info, thus a special treatment is needed
										{
											consulta.append(" , asText("+ columnname + ")");
										}
										else//normal column
										{
											consulta.append(" , "+ strtmp );
										}
									}
									else
									{//We assume that the first column will never be the geospatial column
								
										consulta=strtmp;
									}
								}

							}
							else  Name="name"; //Default option

							PQclear(resC);


							//Composing SQL request considering the field names just read from tha table
							//For the geospatial column:
							//asewkt gets srid and point    asText erturns only the point. asGml asSvg asKml
							PGresult   *res=GetDBRecordsfromTable((std::string&)consulta,(std::string&)tablenames.at(i));

							//If no null request
							if (res!=NULL)
							{
								if (PQresultStatus(res) == PGRES_TUPLES_OK)
								{
									//The records are added to the later-tree
									//Right now is done for a specific set of fields (that are known)
									for (int nr = 0; nr < PQntuples(res); nr++) 
									{
										//lement3d instance
										std::string strtmp;
										cpw::Element3D new_elto3dp;
										cpw::Entity* new_elto3d = cpw::EntityFactory::GetInstance()->CreateEntity(new_elto3dp.GetClassName());

										//The field id gives us the field type
										int pos=PQfnumber(res, "id");
										strtmp=PQgetvalue(res, nr, pos);
										cpw::TypeId id(strtmp);
										new_elto3d->SetID(id);

										pos=PQfnumber(res, "name");
										strtmp=PQgetvalue(res, nr, pos);
										new_elto3d->SetName(strtmp);

										pos=PQfnumber(res, "font");
										strtmp=PQgetvalue(res, nr, pos);
										new_elto3d->SetFont(strtmp);

										pos=PQfnumber(res, "text");
										strtmp=PQgetvalue(res, nr, pos);
										((cpw::Element *) new_elto3d)->SetText(strtmp);

										pos=PQfnumber(res, "description");
										strtmp=PQgetvalue(res, nr, pos);
										new_elto3d->SetDescription(strtmp);

										pos=PQfnumber(res, "primitive_url");
										strtmp=PQgetvalue(res, nr, pos);
										//To avoid problems
										std::replace(strtmp.begin(), strtmp.end(), '/', '\\');
										new_elto3d->SetPrimitiveUrl(strtmp);

										pos=PQfnumber(res, "icon");
										strtmp=PQgetvalue(res, nr, pos);
										//To avoid problems
										std::replace(strtmp.begin(), strtmp.end(), '/', '\\');
										new_elto3d->SetIcon(strtmp);

										pos=PQfnumber(res, "html");
										strtmp=PQgetvalue(res, nr, pos);
										new_elto3d->SetHtml(strtmp);

										pos=PQfnumber(res, "model_url");
										strtmp=PQgetvalue(res, nr, pos);
										//To avoid problems
										std::replace(strtmp.begin(), strtmp.end(), '/', '\\');
										((cpw::Element3D *) new_elto3d)->SetModelUrl(strtmp);

										//booleans
										pos=PQfnumber(res, "dynamic");
										strtmp=PQgetvalue(res, nr, pos);
										if (atol(strtmp.c_str()))
											new_elto3d->SetDynamic(true);
										else
											new_elto3d->SetDynamic(false);

										pos=PQfnumber(res, "visible");
										strtmp=PQgetvalue(res, nr, pos);
										if (atol(strtmp.c_str()))
											new_elto3d->SetVisible(true);
										else
											new_elto3d->SetVisible(false);

										pos=PQfnumber(res, "animate");
										strtmp=PQgetvalue(res, nr, pos);
										if (atol(strtmp.c_str()))
											new_elto3d->SetAnimate(true);
										else
											new_elto3d->SetAnimate(false);

										pos=PQfnumber(res, "published");
										strtmp=PQgetvalue(res, nr, pos);
										if (atol(strtmp.c_str()))
											new_elto3d->SetPublished(true);
										else
											new_elto3d->SetPublished(false);

										

										//Numerical values with spaces for scale
										pos=PQfnumber(res, "scale");
										strtmp=PQgetvalue(res, nr, pos);//This string contains three numerical values separates with blanks
										//First blank
										int fb=strtmp.find_first_of(" ");
										//second blank
										int sb=strtmp.find_first_of(" ",fb+1);

										//Getting the three numerical valus
										float fvals[3];
										fvals[0]=(float)atof(strtmp.substr(0,fb).c_str());
										fvals[1]=(float)atof(strtmp.substr(fb+1,sb-fb).c_str());
										fvals[2]=(float)atof(strtmp.substr(sb,strtmp.length()-sb).c_str());
										((cpw::Element *) new_elto3d)->SetScale(fvals);

										//Numerical values with spaces for oritntation
										pos=PQfnumber(res, "orientation");
										strtmp=PQgetvalue(res, nr, pos);//This string contains three numerical values separates with blanks
										//First blank position
										fb=strtmp.find_first_of(" ");
										//Second blank position
										sb=strtmp.find_first_of(" ",fb+1);

										//The three numerical values are extracted
										fvals[0]=(float)atof(strtmp.substr(0,fb).c_str());
										fvals[1]=(float)atof(strtmp.substr(fb+1,sb-fb).c_str());
										fvals[2]=(float)atof(strtmp.substr(sb,strtmp.length()-sb).c_str());
										((cpw::Element *) new_elto3d)->SetOrientation(fvals);

										//geospatial information column
										int posgeo=PQfnumber(res, "astext");
										strtmp=PQgetvalue(res, nr, posgeo);

										//The open ( is saearched
										sb=strtmp.find_first_of("(");
										//A blank i slocated
										fb=strtmp.find_first_of(" ",sb+1);
										//the final ) is located
										int lb=strtmp.find_first_of(")");

										//latitude is between the ( and bank,
										fvals[0]=(float)atof(strtmp.substr(sb+1,fb-(sb+1)).c_str());//latitude
										fvals[1]=(float)atof(strtmp.substr(fb+1,lb-fb).c_str());//lenght


										//Conversion to UTM
										int RefEllipsoid = 23;//WGS-84. See list with file "LatLong-UTM conversion.cpp" for id numbers
										UTMLL  utm_aux;
										double UTMNorthing=0.f, UTMEasting=0.f;
										char UTMZone[5];

										utm_aux.LLtoUTM(RefEllipsoid, fvals[0], fvals[1], UTMNorthing, UTMEasting,UTMZone);

										//The height was stored separately
										pos=PQfnumber(res, "utm_z");
										strtmp=PQgetvalue(res, nr, pos);//La cadena contiene los tres valores separados por espacios
										float height=(float)atof(strtmp.c_str());

										((cpw::Element *) new_elto3d)->SetUtm(UTMEasting,UTMNorthing,height);										
										
		
										//Adding the element to the layer-tree
										cpw::EntityRegistry::GetInstance()->Add(new_elto3d);
										table.Add(new_elto3d);
									}
								}
								
							}
							PQclear(res);
						}	
					}//records available
				}//OK
			}//Not null
			PQclear(resG);

		}
	}
}

//! Opens connection with DB
	/*!
	Opens connection with DB
	*/

//Falta quizás comprobar si ya está conectada antes de hacerlo de nuevo
bool DataBase::OpenConnection()
{
	PGconn     *conn;
	std::string conninfo;

	//Composing connect command
	std::string str;
	str = "hostaddr=" + GetDBHost() + " port=5432" + " user=" + GetDBUser() + " password=" + GetDBPasswd() + " dbname=" + GetDBName();
   
	if (str.c_str()!=NULL)
		conninfo = str;
    else//default connection
		conninfo = "hostaddr=10.22.144.150 port=5432 user=postgres2 password=agustin dbname=pepe";

    // Connecting with DB
    conn = PQconnectdb(conninfo.c_str());

	//Saving identifier
	SetDBConn(conn);
	//conn = PQconnectdb(conninfo);

    // Checking connection
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        Disconnect();

		return false;
    }

	return true;

}

/*
PRIMARY KEY   clave primaria

//Eliminar una tabla
//DROP TABLE customer

//Eliminar los datos de la tabla pero no la tabla
//TRUNCATE TABLE customer

//Añadir una columna a una tabla (customer), indicamos el nombre y tipo
//ALTER table customer add Gender char(1)

//Modificar el nombre de una columna
//ALTER table customer change Address Addr char(50)

//Modificar el tipo de una columna
//ALTER table customer modify Addr char(30)

//Eliminar una columna
//ALTER table customer drop Gender

//Borrar un registro 

DELETE FROM Store_Information
WHERE store_name = "Los Angeles"


*/


