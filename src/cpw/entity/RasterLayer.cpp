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

#include <cpw/entity/RasterLayer.h>

using namespace cpw;


RasterLayer::RasterLayer(const cpw::TypeId id, const std::string &class_name): 
    Layer(id, class_name) {

}


RasterLayer::RasterLayer(std::string nam, std::string prim, std::string htm,	
	  bool dyn, std::string desc, std::vector<Attribute> &attr) {

}


RasterLayer::RasterLayer(const RasterLayer &layer): Layer(layer) {

}

RasterLayer::~RasterLayer(){

}


RasterLayer &RasterLayer::operator = (const RasterLayer &layer) {

	Layer::operator =(layer);

	return *this;
}

void RasterLayer::SetValue(const std::string &f, const std::string &v)
{
	Layer::SetValue(f, v);
}


int RasterLayer::CreatePersistence() 
{
	Layer::CreatePersistence();
	return 0;
}

int RasterLayer::AdaptPersistence(cpw::Node *root)
{

	Layer::AdaptPersistence(root);

	return 0;
}
