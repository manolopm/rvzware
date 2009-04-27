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

//#include <stdafx.h>

#include <cpw/persistent/file/XmlCpwParser.h>


#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



using namespace cpw;


XmlCpwParser::XmlCpwParser(void)
{

}

XmlCpwParser::~XmlCpwParser(void)
{

}


// Conversion from XML to Nodes 

int XmlCpwParser::XmlToEntity(const XMLNode &xml_node, Node* parent) 
{
	XMLNode xml_aux;

	for (int n = 0; n < xml_node.nChildNode(); n++)
	{
		xml_aux = xml_node.getChildNode(n);
		if(xml_aux.nChildNode() != 0)
		{
			cpw::Composite *subparent = new cpw::Composite(std::string(xml_aux.getName()));
			XmlToEntity(xml_aux, subparent);
			parent->Add(subparent);
		}
		else
		{
			cpw::LeafNode *leaf;
			if (xml_aux.getText() != NULL)
				leaf = new cpw::LeafNode(std::string(xml_aux.getName()), std::string(xml_aux.getText()));
			else
			{
				if(xml_aux.nAttribute() == 0)
					leaf = new cpw::LeafNode(std::string(xml_aux.getName()), std::string(""));
				else
				{
					std::map<std::string, std::string> attr;
					
					for(int i=0; i< xml_aux.nAttribute(); i++ )
					{
						attr[xml_aux.getAttributeName(i)] = xml_aux.getAttributeValue(i);
					}
					leaf = new cpw::LeafNode(std::string(xml_aux.getName()),attr);
				}
			}
			
			parent->Add(leaf);
		}
	}
	
	return 0;
}




cpw::Node * XmlCpwParser::XmlToEntity(const std::string &xml_string)
{
	cpw::Node *root = NULL;
	XMLNode xMainNode = XMLNode::parseString(xml_string.c_str());


	if (!xMainNode.isEmpty())
	{
		
		XMLNode xml_aux = xMainNode.getChildNode(0);
		
		root = new cpw::Composite(std::string(xml_aux.getName()));
		XmlToEntity(xml_aux, root);
	}

	return root;

}


// Conversion from Nodes to XML 
int XmlCpwParser::EntityToXml(const Node *parent, XMLNode *xml_node)
{
	XMLNode xml_aux = xml_node->addChild(parent->GetName().c_str());

	if (parent->isContainer())
	{
		const std::vector<Node *> children = parent->GetChildren();
		for(std::vector<Node *>::const_iterator i= children.begin(); i != children.end(); i++)
		{
			EntityToXml(*i, &xml_aux);		
		}
	}
	else
	{
		if (parent->GetNumAttributes() == 0)
			xml_aux.addText(parent->GetValue().c_str());
		else
		{
			const std::map<std::string, std::string> *attrs = parent->GetAttributes();
			std::map<std::string, std::string>::const_iterator i= attrs->begin();
			for ( ; i != attrs->end() ; i++ )
				xml_aux.addAttribute(i->first.c_str(), i->second.c_str());
		}
		
	}

	return 0;
}


int XmlCpwParser::EntityToXml(const Node* root, std::string &xml_string)
{
	XMLNode xml_root=XMLNode::createXMLTopNode("xml",TRUE);
    xml_root.addAttribute("version","1.0");
    xml_root.addAttribute("encoding","UTF-8");

	std::vector<Node *> child = root->GetChildren();
	XMLNode xml_node= xml_root.addChild(root->GetName().c_str());

	for(std::vector<Node *>::iterator i= child.begin(); i != child.end(); i++)
	{
		EntityToXml(*i, &xml_node);		
	}

	char *t = xml_root.createXMLString();
    xml_string = t;
	free(t);

	return 0;
}



