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

//#include <stdafx.h>

#include <cpw/persistent/database/PersistentDBCallBack.h>
#include <cpw/persistent/database/DataBase.h>
#include <cpw/persistent/database/Table.h>
#include <cpw/persistent/Persistent.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/Element3D.h>
#include <cpw/persistent/file/FileManager.h>
#include <cpw/persistent/file/XmlCpwParser.h>


using namespace cpw;


PersistentDBCallBack::PersistentDBCallBack(void)
{

}

PersistentDBCallBack::~PersistentDBCallBack(void)
{

}


PersistentError PersistentDBCallBack::Save(cpw::Persistent *persistent, bool overwrite)
{
	PersistentError ferror = cpw::PERSISTENTNOTFOUND;
	
	Node *root = persistent->GetPersistence();

	//Gets parent info
	Entity *entity = (Entity *) persistent;
	Entity *parent = entity->GetParent(0);
	//Parent type
	cpw::Persistent *persistentParent=(cpw::Persistent *)parent;

	//Getting the node name of the node to be added
	std::string	ClassName=persistentParent->GetClassName();

	if(root != NULL)
	{

		//According to the class different actions are taken
		//Table --> added to parent DB
		//Record --> added to parent table


		//Case Element3D
		if (ClassName.compare("Element3D")==0)
		{
			//what should be done in this case
		}

		//The parent is a Table, then a record is being added
		if (ClassName.compare("Table")==0)
		{
			//Getting the element 
			cpw::Element3D *element = (cpw::Element3D *) persistent;

			//Getting the table name
			std::string	table_name=parent->GetName();

			//element descriptor
			std::vector<cpw::Node *> root_children = root->GetChildren();
			std::vector<cpw::Node *>::iterator i;

			//If the tabel is empty, it must be managed differently creating the columns

			//is the table empty?
			bool bovuota=((Table*) parent)->IsEmpty();
			if (bovuota)
			{				
				//The table is empty, an instance is created with typical geviemer columns but without content
				for(i = root_children.begin(); i != root_children.end(); i++)
				{
					//Getting name and value of current child
					std::string fieldname=(*i)->GetName();
					std::string fieldval=(*i)->GetValue();

					std::string fieldandtype;

					//According to each field
					//This must be modified if geviemer icnludes new fields

					//key
					if (fieldname.compare("id")==0)
					{
						//Composing name and type
						std::stringstream  saux;
						saux << (TYPEID_SIZE*2+4);	//coherent with TypeID
						fieldandtype=fieldname + " char(" + saux.str() + ") PRIMARY KEY";
			
						((DataBase*) parent->GetParent())->AlterDBTable(table_name,fieldandtype);
					}

					//char fields
					if (fieldname.compare("name")==0 ||
						fieldname.compare("font")==0 ||
						fieldname.compare("text")==0 )
					{
						//Compone nombre del campo y tipo char
						fieldandtype=fieldname + " char(50)";
			
						((DataBase*) parent->GetParent())->AlterDBTable(table_name,fieldandtype);
					}

					//booleans
					if (fieldname.compare("dynamic")==0 || 
						fieldname.compare("visible")==0 ||
						fieldname.compare("animate")==0 ||
						fieldname.compare("published")==0)
					{
						//Composed with name and an integer
						fieldandtype=fieldname + " integer";
			
						((DataBase*) parent->GetParent())->AlterDBTable(table_name,fieldandtype);
					}

					//memo
					if (fieldname.compare("description")==0 || 
						fieldname.compare("primitive_url")==0 ||						
						fieldname.compare("icon")==0 ||
						fieldname.compare("html")==0 ||
						fieldname.compare("model_url")==0)
					{
					//composing field and type
						fieldandtype=fieldname + " text";
			
						((DataBase*) parent->GetParent())->AlterDBTable(table_name,fieldandtype);
					}
					
					//fields with three numerical components
					//They are written as a string
					if (fieldname.compare("scale")==0 || 
						fieldname.compare("orientation")==0)
					{
						//field and type
						fieldandtype=fieldname + " char(30)";
			
						((DataBase*) parent->GetParent())->AlterDBTable(table_name,fieldandtype);
					}

					//Correct conversion must be done for geospatial info
					//p.e.
					//select AddGeometryColumn('nombretabla','utm',4326,'POINT','2')
					if (fieldname.compare("utm")==0)
					{						
						((DataBase*) parent->GetParent())->AddLocationColumn(table_name,fieldname);

						//An additional field is added to contain the height info
						//Name and type
						fieldandtype="utm_z char(30)";
						((DataBase*) parent->GetParent())->AlterDBTable(table_name,fieldandtype);
					}					
				}
				
				//After creating the table and columns, is is marked as non empty
				((Table*) parent)->SetEmpty(false);
			}//if (bovuota)

			//The table is already created the SQL request is composed

			//Filling fields and values, initially empty
			std::string fields="";
			std::string values="";

			//Considering a geviemer table we can make use of GetChildren GetValue
			bool bocoma=false;

			//Cvalues and fields are read
			for(i = root_children.begin(); i != root_children.end(); i++)
			{				
				std::string fieldname=(*i)->GetName();

				//Some fields are currently not considered: attributes, changes, creation_time y position
				//If new fields are added, the code must be updated
				if (fieldname.compare("id")==0 ||
					fieldname.compare("icon")==0 || 
					fieldname.compare("primitive_url")==0 ||
					fieldname.compare("model_url")==0 ||
					fieldname.compare("utm")==0 ||
					fieldname.compare("scale")==0 || 
					fieldname.compare("orientation")==0 ||
					fieldname.compare("description")==0 || 
					fieldname.compare("html")==0 ||
					fieldname.compare("dynamic")==0 || 
					fieldname.compare("visible")==0 ||
					fieldname.compare("animate")==0 ||
					fieldname.compare("published")==0 ||
					fieldname.compare("name")==0 ||
					fieldname.compare("font")==0 ||
					fieldname.compare("text")==0
					)
				{
					//Adding a , after the first field
					if (!bocoma)
						bocoma=true;
					else
					{
						fields=fields +",";
						values=values +",";
					}

					fields=fields + fieldname;

					//No utm field
					if (fieldname.compare("utm")!=0)
					{					
						std::string saux=(*i)->GetValue();

						//Taking care of /
						if (fieldname.compare("icon")==0 || 
							fieldname.compare("primitive_url")==0 ||
							fieldname.compare("model_url")==0)
							std::replace(saux.begin(), saux.end(), '\\', '/');

						values=values + "'" + saux + "'";
					}
					else///the field name is utm, the proper conversion is done considering the Canary Islands
					{
						double Lat,Long;
						int RefEllipsoid = 23;//WGS-84. See list with file "LatLong-UTM conversion.cpp" for id numbers
						UTMLL  utm_aux;
						//The whole zone is 28R excepting some areas of El Hierro that would be 27R (see http://www.dmap.co.uk/utmworld.htm)
						//To avoid jumps in utm coordinates, we have modified the function LLtoUTM to force always the zone 28R
						utm_aux.UTMtoLL(RefEllipsoid, element->GetUtm(1), element->GetUtm(0), "28R", Lat, Long);

						std::stringstream lats,longs,height;

						//if zone is 28R, according to http://www.cellspark.com/UTM.html it is converting properly
						lats << Lat;
						longs << Long;

						values=values + "GeomFromText('Point(" + lats.str() + " " + longs.str() + ")',4326)";

						//An additional field to store the height
						fields=fields +",utm_z";
						//Height value
						height << element->GetUtm(2);
						values=values + ",'" + height.str() + "'";
					}
				}
			}

			//Addint the record to the table
			ferror=((DataBase*) parent->GetParent())->InsertDBRecordinTable(table_name, fields, values);
		}

		//If the parent is a DB, we are adding a table
		if (ClassName.compare("DataBase")==0)
		{
			std::string	table_name=entity->GetName();

			//Adding table to a DB
			ferror=((DataBase*) parent)->CreateDBTable(table_name);

		}

		delete root;
	}
	return ferror;
}

PersistentError PersistentDBCallBack::Load(cpw::Persistent *persistent)
{
	//load the file with xml parser
	FileManager file_manager;		
	std::string xml;
	std::string url = persistent->GetUrl();

	cpw::PersistentError ferror = (PersistentError) file_manager.Load(url, xml);

	if (ferror == cpw::PERSISTENTOK)
	{
	    cpw::XmlCpwParser parser;

		Node *root = parser.XmlToEntity(xml);
		persistent->SetPersistence(root);
		delete root;
	
	}

	return ferror;

}

PersistentError PersistentDBCallBack::Update(cpw::Persistent *persistent)
{
	return Save(persistent);
}

PersistentError PersistentDBCallBack::Delete(cpw::Persistent *persistent)
{

	//delete file from file system
	FileManager file_manager;		
	std::string url = persistent->GetUrl();
	return (PersistentError) file_manager.Delete(url);

}


IPersistentCallBack* PersistentDBCallBack::Clone() 
{
	return new PersistentDBCallBack(*this);
}

