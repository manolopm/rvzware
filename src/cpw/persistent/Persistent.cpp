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

#include <sstream>
#include <ctype.h>
#include <boost/filesystem.hpp>


//#include <stdafx.h>  

#include <cpw/persistent/Persistent.h>



using namespace cpw;

Persistent::Persistent(const cpw::TypeId _id, const std::string &class_nam): 
			 Subject(), id(_id), class_name(class_nam), modified(false), root(), composite_stack(), persistent_callback(), compound_directory()
{
}

Persistent::Persistent(const Persistent &persistent): Subject(persistent),
			 id(persistent.id), class_name(persistent.class_name), modified(persistent.modified),
		     root(persistent.root), composite_stack(persistent.composite_stack), compound_directory(persistent.compound_directory)
{

	if(persistent.persistent_callback != NULL)
		persistent_callback = (IPersistentCallBack *)persistent.persistent_callback->Clone();
	else
		persistent_callback = NULL;


}


Persistent &Persistent::operator = (const Persistent &persistent)
{

	Subject::operator = (persistent);

	id				   = persistent.id;
	class_name         = persistent.class_name;
	root               = persistent.root;
	composite_stack    = persistent.composite_stack;
	modified           = persistent.modified;
	compound_directory = persistent.compound_directory;

	if(persistent.persistent_callback != NULL)
		persistent_callback = (IPersistentCallBack *)persistent.persistent_callback->Clone();
	else
		persistent_callback = NULL;


	return *this;
}


Persistent::~Persistent() 
{
  if (persistent_callback != NULL)
    {
      delete persistent_callback;
      persistent_callback = NULL;
    }

}


Node *Persistent::GetPersistence() 
{ 
	CreatePersistence(); 
	return root; 
}

Node *Persistent::GetProperties() 
{ 
	CreateProperties();
	return root; 
}


void  Persistent::SetPersistence(Node *root)
{ 
	AdaptPersistence(root);
}

bool Persistent::isModified() 
{
	return modified;
}

const bool Persistent::isModified() const 
{
	return modified;
}

void Persistent::Saved() 
{
	modified = false;
}

cpw::TypeId &Persistent::GetId() 
{
	return id;
}


std::string &Persistent::GetClassName() 
{
	return class_name;
}

std::string Persistent::GetUrl() 
{ 
	return ComposeDirectory(filename); 
}

std::string &Persistent::GetCompoundDirectory() 
{ 
	return compound_directory; 
}

const cpw::TypeId &Persistent::GetId()  const 
{
	return id;
}


const std::string &Persistent::GetClassName() const 
{
	return class_name;
}

const std::string Persistent::GetUrl() const 
{ 
	return ComposeDirectory(filename); 
}

const std::string &Persistent::GetCompoundDirectory() const 
{ 
	return compound_directory; 
}


void Persistent::SetUrl(const std::string &str) 
{ 
	url = str;

	SetCompoundDirectory(url);

	Modified();
}


void Persistent::SetCompoundDirectory(const std::string &str) 
{ 
	boost::filesystem::path path(str);	
	boost::filesystem::path directory     = path.branch_path();
	boost::filesystem::path previous_path = boost::filesystem::current_path();
	boost::filesystem::path actual_path   = (directory.has_root_path())? directory : previous_path / directory;

	std::string str2 = actual_path.string();
	CleanPath(str2);
	actual_path = str2;

	compound_directory = actual_path.string();

	filename = path.leaf();
}


void Persistent::SetID(const cpw::TypeId _id) 
{
	id = _id; 
	Modified();
}


void Persistent::SetClassName(const std::string &cn) 
{
	class_name = cn; 
	Modified();
}


void Persistent::SetValue(const std::string &f, const std::string &v)
{

	if(f == "url") 
		SetUrl(v); 
	else if (f == "id")
	{
		cpw::TypeId id;
		std::istringstream str(v);
		str >> id;
		SetID(id); 
	}
	else if(f == "class_name")
		SetClassName(v); 
}


void Persistent::Modified() 
{ 
	modified = true; 
	Subject::Notify();
}


int Persistent::CreatePersistence() 
{	
	
	//This initializes the persistent tree (only to be done in this class)
	InitPersistence(class_name);

	std::ostringstream str2;
	str2.precision(60);
	str2 << id;
	AddPersistence((std::string)"id", str2.str());

	return 0;
}

int Persistent::CreateProperties() 
{	
	
	//This initializes the persistent tree (only to be done in this class)
	InitPersistence("Entity");

	std::ostringstream str2;
	str2.precision(60);
	str2 << id;
	AddPersistenceLevel("General");
	AddPersistence((std::string)"Identifier", str2.str());

	return 0;
}


int Persistent::AdaptPersistence(Node *root)
{

	class_name = root->GetName();

	std::istringstream str (root->GetChildValue("id"));

	str >> id;

	return 0;

}



void Persistent::InitPersistence(const std::string &name) 
{

	root = new Composite(name);
	
	composite_stack.push_back(root);

}

void Persistent::AddPersistence(const std::string &name, const std::string &value) 
{	
	composite_stack.back()->Add(new LeafNode(name, value));
}

void Persistent::AddPersistence(const std::string &name, const std::map<std::string, std::string> &attrs)
{	
	composite_stack.back()->Add(new LeafNode(name, attrs));
}


		
void Persistent::AddPersistenceLevel(const std::string &name) 
{
	
	Node *newNode = new Composite(name);

	//we add the new subtree to the parent tree
	composite_stack.back()->Add(newNode);
	
	//we take the new composite as the new root
	composite_stack.push_back(newNode);

}

void Persistent::RemovePersistenceLevel() 
{

	//pop up the last composite node from the stack
	composite_stack.pop_back();

}


PersistentError Persistent::Save(bool overwrite)
{
	if(persistent_callback != NULL)
		return persistent_callback->Save(this, overwrite);
	else
		return PERSISTENTNOTAVAILABLE;
}

PersistentError Persistent::Load()
{
	if(persistent_callback != NULL)
		return persistent_callback->Load(this);
	else
		return PERSISTENTNOTAVAILABLE;}

PersistentError Persistent::Update()
{
	if(persistent_callback != NULL)
		return persistent_callback->Update(this);
	else
		return PERSISTENTNOTAVAILABLE;}

PersistentError Persistent::Delete()
{
	if(persistent_callback != NULL)
		return persistent_callback->Delete(this);
	else
		return PERSISTENTNOTAVAILABLE;
}

void Persistent::SetPersistentCallBack(IPersistentCallBack *persist)
{
  if(persistent_callback != NULL)
    {
      delete persistent_callback;
      persistent_callback = NULL;
    }

  persistent_callback = persist; 

}


const std::string Persistent::ComposeDirectory(const std::string &url) const
{
	boost::filesystem::path path(url);

	boost::filesystem::path actual_path = (path.has_root_name())? path : compound_directory / path;

	return actual_path.string();
}


const std::string Persistent::RelativeDirectory(const std::string &url) const
{
	typedef boost::filesystem::path boost_path;

	boost_path path(url);
	boost_path actual_path = (path.has_root_name())? path : compound_directory / path;
	
	std::string str = actual_path.string();
	CleanPath(str);
	actual_path = str;
	
	boost_path path2(compound_directory);
	boost_path::const_iterator i = actual_path.begin();
	boost_path::const_iterator j = path2.begin();

	while(i != actual_path.end() && j != path2.end() && 
		  ToLower(*i) == ToLower(*j) )
	{
		i++;
		j++;
	}

	boost_path out_path;

	if(j != path2.end())
	{
		while(j != path2.end()) 
		{ 
			out_path /= "../"; 
			j++; 
		}
	}

	while(i != actual_path.end()) 
	{ 
		out_path /= *i; 
		
		i++; 
	}

	return out_path.string();
}

std::string Persistent::ToLower(const std::string &str) const 
{
	std::string out_str;

	std::transform(str.begin(), str.end(), std::back_inserter(out_str), std::ptr_fun <int, int> (tolower));

	return out_str;
}

void Persistent::CleanPath(std::string &str) const
{
	boost::filesystem::path path(str);
	boost::filesystem::path out_path;
	
	boost::filesystem::path::const_iterator i = path.begin();

	while(i != path.end()) 
	{ 
		if(*i == "..")
		{
			//test if the leaf is a slash or the actual directory
			if(out_path.leaf() == ".")
				out_path.remove_leaf();

			out_path.remove_leaf();
		}
		else if(*i != ".")
			out_path /= *i; 
		
		i++; 
	}
	str = out_path.string();
}

