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

#include <cpw/entity/Layer.h>


using namespace cpw;


Layer::Layer(const cpw::TypeId id, 
			 const std::string &class_name): Entity(id, class_name) 
{

}


Layer::Layer(const cpw::TypeId id, const std::string &nam, 
			 const std::string &prim, const std::string &htm,	
			 const bool dyn, const bool vis, const bool anim, const std::string &desc, 
			 const std::vector<Attribute> &attr, const std::string &class_name): 
                Entity(id, nam, prim, htm, dyn, vis, anim, desc, attr, class_name) {

}


Layer::Layer(const Layer &layer): Entity(layer) {

}

Layer::~Layer(){

}


Layer &Layer::operator = (const Layer &layer) {

	Entity::operator = (layer);

	return *this;
}

void Layer::SetValue(const std::string &f, const std::string &v)
{
	Entity::SetValue(f, v);
}


int Layer::CreatePersistence() {

	Entity::CreatePersistence();	
	return 0;
}


int Layer::AdaptPersistence(Node *root)
{

	Entity::AdaptPersistence(root);

	return 0;

}
