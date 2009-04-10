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
#include "stdafx.h"

#include <controllers/RelativeDirectory.h>


using namespace cpw::controllers;

RelativeDirectory::RelativeDirectory(const std::string &url)
{
	boost::filesystem::path path(url);
	
	directory     = path.branch_path();
	previous_path = boost::filesystem::current_path();
	actual_path   = (directory.has_root_name())? directory : previous_path / directory;

	if(boost::filesystem::exists(actual_path))
	{
		actual_path = boost::filesystem::current_path();

		filename = actual_path / path.leaf();

		is_valid = true;
	}
	else
	{
		filename = actual_path / path.leaf();

		is_valid = false;
	}
}

RelativeDirectory::~RelativeDirectory(void)
{
	if(boost::filesystem::exists(previous_path))
	
		previous_path = boost::filesystem::current_path();

}

