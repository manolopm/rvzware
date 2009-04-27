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

#include <cpw/callback/Callable.h>



using namespace cpw;


Callable::Callable(void): callback_vector(), animate_callback()
{
}

Callable::Callable(const Callable &callable)
{
	std::vector<ICallBack*>::const_iterator i = callable.callback_vector.begin();
	for(;i!=callable.callback_vector.end();i++)
	{
		callback_vector.push_back((*i)->Clone());  
	}

	if(callable.animate_callback != NULL)
		animate_callback = (IGraphicCallBack *)callable.animate_callback->Clone();
	else
		animate_callback = NULL;

}

Callable &Callable::operator = (const Callable &callable)
{
	std::vector<ICallBack*>::const_iterator i = callable.callback_vector.begin();
	for(;i!=callable.callback_vector.end();i++)
	{
		callback_vector.push_back((*i)->Clone());  
	}
	if(callable.animate_callback != NULL)
		animate_callback = (IGraphicCallBack *)callable.animate_callback->Clone();
	else
		animate_callback = NULL;


	return *this;
}


Callable::~Callable(void)
{
	std::vector<ICallBack*>::iterator i = callback_vector.begin();

	for( ; i!= callback_vector.end(); i++)
		delete *i;

	if (animate_callback != NULL) delete animate_callback;
}



void Callable::GraphicInsert()
{
	std::vector<ICallBack*>::iterator i = callback_vector.begin();
	for( ; i!= callback_vector.end(); i++)
	{
		(*i)->Insert(this);
	}

}

void Callable::GraphicUpdate()
{
	std::vector<ICallBack*>::iterator i = callback_vector.begin();
	for( ; i!= callback_vector.end(); i++)
		(*i)->Update(this); //debug assertiron aqui --'
}

void Callable::GraphicDelete()
{
	std::vector<ICallBack*>::iterator i = callback_vector.begin();
	for( ; i!= callback_vector.end(); i++)
		(*i)->Delete(this);
}

void Callable::AddCallBack(cpw::ICallBack* callback)
{ 
	callback_vector.push_back(callback); 
}

void Callable::Animate(cpw::cpwTime &time)
{ 
	if (animate_callback != NULL)
		animate_callback->Animate(this, time); 
}


void Callable::SetAnimateCallBack(IGraphicCallBack *callback)
{ 
	if(animate_callback != NULL) delete animate_callback;

	animate_callback = callback; 
}

void Callable::Visualize(const bool &value)
{
	if (animate_callback != NULL)
	{
		bool a = value;
		animate_callback->Visualize(this, a);		
	}
	
	std::vector<ICallBack*>::iterator i = callback_vector.begin();
	for( ; i!= callback_vector.end(); i++)
		(*i)->Visualize(this, value);
	

}
