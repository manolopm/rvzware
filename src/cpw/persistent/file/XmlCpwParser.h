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

#ifndef _XMLCPWPARSER_
#define _XMLCPWPARSER_

#include <string>

#include <cpw/persistent/file/xmlParser.h>
#include <cpw/common/Composite.h>
#include <cpw/Export.h>


namespace cpw{


	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT XmlCpwParser
	{

	public:

		XmlCpwParser(void);
		~XmlCpwParser(void);

		cpw::Node * XmlToEntity(const std::string &xml_string);
		int EntityToXml(const cpw::Node* root, std::string &xml_string);


	private:

		int EntityToXml(const Node *parent, XMLNode *xml_node);
		int XmlToEntity(const XMLNode &xml_node, Node* parent);

	};

}


#endif
