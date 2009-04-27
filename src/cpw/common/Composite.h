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

#ifndef _COMPOSITE_
#define _COMPOSITE_

#include <string>
#include <vector>
#include <map>
#include <cpw/Export.h>


namespace cpw {


	/** 
		\brief
		\ingroup cpw
	*/
	class CPWEXPORT Node
	{

	public:

		Node(): name(){}
		
		Node(const std::string &nam):name(nam){}
		
		virtual ~Node(){}

		virtual void Add(Node *node) {}
		
		virtual std::vector<Node *> GetChildren() {return std::vector<Node *>();}
		virtual const std::vector<Node *> GetChildren() const {return std::vector<Node *>();}

		virtual std::string GetChildValue(const std::string &name) {return std::string();}
		virtual const std::string GetChildValue(const std::string &name) const {return std::string();}

		virtual bool isContainer() = 0;
		virtual const bool isContainer() const = 0;

		std::string &GetName() { return name; }
		const std::string &GetName() const { return name; }

		virtual std::string GetValue() {return std::string();}
		virtual const std::string GetValue() const {return std::string();}

		virtual int GetNumAttributes () {return 0;}
		virtual const int GetNumAttributes () const {return 0;}
		virtual std::map<std::string, std::string> *GetAttributes() {return NULL;}
		virtual const std::map<std::string, std::string> *GetAttributes() const {return NULL;}

		virtual std::map<std::string, std::string> *GetChildAttributes(const std::string &name) {return NULL;}
		virtual const std::map<std::string, std::string> *GetChildAttributes(const std::string &name) const {return NULL;}


	private:

		std::string name;
	};



	/** 
		\brief
		\ingroup cpw
	*/
	class CPWEXPORT LeafNode: public Node {
	
	public:

		LeafNode(): Node(), value(), attributes() {}
		
		LeafNode(const std::string &nam, const std::string &val):Node(nam), value(val){}

		LeafNode(const std::string &nam, const std::map<std::string, std::string> &attr):Node(nam), attributes(attr){}

		virtual ~LeafNode(){}

		bool isContainer() {return false;}
		const bool isContainer() const {return false;}

		std::string GetValue() {return value;}
		const std::string GetValue() const {return value;}

		int GetNumAttributes () {return attributes.size();}
		virtual const int GetNumAttributes () const {return attributes.size();}
		std::map<std::string, std::string> *GetAttributes() {return &attributes;}
		const std::map<std::string, std::string> *GetAttributes() const {return &attributes;}

	private:
		std::string value;
		std::map<std::string, std::string> attributes;

	};



	/** 
		\brief
		\ingroup cpw
	*/
	class CPWEXPORT Composite: public Node  {

	public:
		Composite(): Node(), components(){}

		Composite(const std::string &nam):Node(nam), components(){}

		Composite(const std::string &nam, const std::vector<Node *> &cmps):Node(nam), components(cmps){}

		virtual ~Composite(){

			for(std::vector<Node *>::iterator i = components.begin(); i != components.end(); i++)

				delete *i;
		}


		virtual void Add(Node *node){ components.push_back(node); }

		virtual std::vector<Node *> GetChildren() {return components;}
		virtual const std::vector<Node *> GetChildren() const {return components;}

		virtual std::string GetChildValue(const std::string &name) 
		{
			std::string value;
			std::vector<Node *>::iterator i = components.begin();
			while ((i!=components.end()) && ((*i)->GetName() != name)) i++;

			if(i != components.end()) return (*i)->GetValue();
			else return std::string();
		}


		bool isContainer() {return true;}
		const bool isContainer() const {return true;}

		virtual std::map<std::string, std::string> *GetChildAttributes(const std::string &name) 
		{
			std::vector<Node *>::iterator i = components.begin();
			while ((i!=components.end()) && ((*i)->GetName() != name)) i++;

			if(i != components.end()) return (*i)->GetAttributes();
			else return NULL;
		}
		virtual const std::map<std::string, std::string> *GetChildAttributes(const std::string &name)const
		{
			std::vector<Node *>::const_iterator i = components.begin();
			while ((i!=components.end()) && ((*i)->GetName() != name)) i++;

			if(i != components.end()) return (*i)->GetAttributes();
			else return NULL;
		}


	private:

		std::vector<Node *> components;

	};


}

#endif
