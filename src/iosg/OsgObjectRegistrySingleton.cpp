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

#include <iosg/stdafx.h>

#include <iosg/OsgObjectRegistrySingleton.h>

using namespace cpw::iosg;

OsgObjectRegistrySingleton * OsgObjectRegistrySingleton::instance = NULL;

OsgObjectRegistrySingleton * OsgObjectRegistrySingleton::GetInstance(void)
{
  if (instance == NULL)
    instance = new OsgObjectRegistrySingleton;

  return instance;
}

void OsgObjectRegistrySingleton::ReleaseInstance(void)
{
  if (instance != NULL) {
    delete instance;
    instance = NULL;
  }
}

OsgObjectRegistrySingleton::OsgObjectRegistrySingleton(void)
{
  obj_reg = new OsgObjectRegistry;
}

OsgObjectRegistrySingleton::~OsgObjectRegistrySingleton(void)
{
  if (obj_reg)
    {
      delete obj_reg;
      obj_reg = NULL;
    }
}
