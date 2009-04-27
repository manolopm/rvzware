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

#include <stdafx.h>

#include <controllers/PrimitiveController.h>
#include <controllers/PersistentController.h>

#include <cpw/ApplicationConfiguration.h>
#include <cpw/entity/ContainerLayer.h>

using namespace cpw::controllers;


PrimitiveController::PrimitiveController(void)
{

}

PrimitiveController::~PrimitiveController(void)
{

}		


std::string PrimitiveController::GetDefaultContainerLayer(wxWindow* parent, cpw::EntityFactory &entity_factory, cpw::IPersistentManager *persistent_manager)
{
	cpw::ContainerLayer default_clp;
	cpw::Entity *primitive = entity_factory.CreateEntity(default_clp.GetClassName());

	std::string &default_path = ApplicationConfiguration::GetInstance()->GetDefaultDirectory();
	primitive->SetName("DefaultContainerLayerPrimitive");
	primitive->SetIcon(default_path + "default.png");

	std::string file_name = default_path + std::string("DefaultContainerLayerPrimitive.cla");

	primitive->SetUrl(file_name);

	PersistentController persistent;

	persistent.Save(parent, primitive, true);

	entity_factory.Register(primitive);

	return file_name;
}


