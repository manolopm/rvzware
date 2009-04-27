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

#include <cpw/callback/RasterCallBack.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/ElementRaster.h>

#include <cpw/debugger.h>

using namespace cpw;



RasterCallBack::RasterCallBack()
{
}

RasterCallBack::~RasterCallBack()
{
}

void RasterCallBack::Insert(const cpw::Callable* callable)
{
}


void RasterCallBack::Update(cpw::Callable* callable)
{
}


void RasterCallBack::Delete(cpw::Callable* callable)
{
}


RasterCallBack::RasterCallBack(const RasterCallBack &rastercb)
{
}

RasterCallBack &RasterCallBack::operator = (const RasterCallBack &rastercb)
{
	return *this;
}

ICallBack* RasterCallBack::Clone() 
{
	return new RasterCallBack(*this);
}

void RasterCallBack::Animate(cpw::Callable* callable, cpw::cpwTime &time)
{
}

void RasterCallBack::Visualize(cpw::Callable* callable, const bool &value)
{
}
