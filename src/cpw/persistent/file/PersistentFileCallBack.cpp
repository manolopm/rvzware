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
//#include <stdafx.h>

#include <cpw/persistent/Persistent.h>
#include <cpw/persistent/file/FileManager.h>
#include <cpw/persistent/file/XmlCpwParser.h>
#include <cpw/persistent/file/PersistentFileCallBack.h>


using namespace cpw;


PersistentFileCallBack::PersistentFileCallBack(void)
{

}

PersistentFileCallBack::~PersistentFileCallBack(void)
{

}


PersistentError PersistentFileCallBack::Save(cpw::Persistent *persistent, bool overwrite)
{
	PersistentError ferror = cpw::PERSISTENTNOTFOUND;

	Node *root = persistent->GetPersistence();

	if(root != NULL)
	{
		XmlCpwParser parser;
		std::string       xml;

		parser.EntityToXml(root, xml);

		delete root;

		FileManager fout;
		std::string url = persistent->GetUrl();
		ferror = (PersistentError) fout.Save(xml, url, overwrite); 
		if (ferror == cpw::PERSISTENTOK) persistent->Saved();

	}
	return ferror;

}

PersistentError PersistentFileCallBack::Load(cpw::Persistent *persistent)
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

PersistentError PersistentFileCallBack::Update(cpw::Persistent *persistent)
{
	return Save(persistent);
}

PersistentError PersistentFileCallBack::Delete(cpw::Persistent *persistent)
{
	//delete file from file system
	FileManager file_manager;		
	std::string url = persistent->GetUrl();
	return (PersistentError) file_manager.Delete(url);

}


IPersistentCallBack* PersistentFileCallBack::Clone() 
{
	return new PersistentFileCallBack(*this);
}
