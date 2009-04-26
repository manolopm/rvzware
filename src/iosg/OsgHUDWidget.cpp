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

#include <iosg/OsgHUDWidget.h>


using namespace cpw::iosg;

OsgHUDWidget::OsgHUDWidget(void)
{
}

OsgHUDWidget::~OsgHUDWidget(void)
{
}

OsgHUDWidget * OsgHUDWidget::GetChildren(const unsigned int &child_index)
{
	if (child_index >= children.size())
		return NULL;
	else
		return children[child_index];
}
