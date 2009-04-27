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

#ifndef _DATABASE_
#define _DATABASE_

#include <string>
#include <vector>

#include <algorithm>	
#include <cctype>       

#include <cpw/entity/ContainerLayer.h>
#include <cpw/Export.h>

#include <libpq-fe.h> //libpq cpw::utils

namespace cpw
{

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT DataBase : public cpw::ContainerLayer
	{
		public:

			DataBase(const cpw::TypeId id = cpw::TypeId(), 
					 const std::string &class_name = "DataBase");
			
			virtual ~DataBase(void);

			DataBase(const DataBase &DataBase);

			DataBase &operator = (const DataBase &database);

			virtual int Add(Entity *entity);


			virtual Entity* Clone() {return new DataBase(*this);}

			//Added functions 
			std::string &GetDBHost() { return db_host; }
			std::string &GetDBUser() { return db_user; }
			std::string &GetDBPasswd() { return db_passwd; }
			std::string &GetDBName() { return db_name; }
			PGconn *GetDBConn() { return db_conn; }
			bool GetConnected() { return db_boConnected;}
			std::vector<std::string> &GetDBTableNames() { return db_tablenames; }
			std::vector<std::vector<std::string> > &GetDBColumnsperTable() { return db_columnspertable; }

			void SetDBHost(const std::string &host) { db_host = host; }
			void SetDBUser(const std::string &user) { db_user = user; }
			void SetDBPasswd(const std::string &passwd) { db_passwd = passwd; }
			void SetDBName(const std::string &name) { db_name = name; }
			void SetDBConn(PGconn *conn) { db_conn = conn; }
			void SetConnected(bool connected) { db_boConnected = connected; }
			void SetDBTableNames(const std::vector<std::string> &tablenames) { db_tablenames = tablenames; }
			void SetDBColumnsperTable(const std::vector<std::vector<std::string> > &columnspertable) { db_columnspertable = columnspertable; }

			void Disconnect();

			//DB management
			bool OpenConnection();		//Stablishes the connection with the DB
			int GetDBTables();			//Number of tables
			void ReadTables();			//Read the records of all the tables
			PGresult   * GetDBRecordsfromTable(std::string &fields,std::string &tablename);
			void DeleteDBRecordsinTable(std::string &tablename,std::string &condition);
			PersistentError InsertDBRecordinTable(std::string &tablename,std::string &columns,std::string &values);
			PersistentError CreateDBTable(std::string &tablename);
			PersistentError AlterDBTable(std::string &tablename,std::string &fieldandtype);
			PersistentError AddLocationColumn(std::string &tablename,std::string &columnname);

		
		private:
			//! Host name
			std::string db_host;
			//! User name used for the connection
			std::string db_user;
			//! Password used fot the connection
			std::string db_passwd;
			//! Database name used
			std::string db_name;
			//! Connection handle
			PGconn     *db_conn;
			//! Table names
			std::vector<std::string> db_tablenames;
			//! Array containing the column names of each table
			std::vector<std::vector<std::string> > db_columnspertable;
			//! TRUE if the connection is stablished
			bool		db_boConnected;
			
			protected:
			
			virtual int CreatePersistence();

			virtual int AdaptPersistence(cpw::Node *root);

	};

}
#endif
