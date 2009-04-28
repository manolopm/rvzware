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

#ifndef _ELEMENTRASTER_

#define _ELEMENTRASTER_

#include <cpw/entity/Element.h>
#include <cpw/Export.h>

namespace cpw {

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT ElementRaster: public Element
	{

	public:

		ElementRaster(const cpw::TypeId id = cpw::TypeId(), 
			    const std::string &class_name = std::string("ElementRaster"));
		ElementRaster(const ElementRaster &element);
		virtual ~ElementRaster();

		ElementRaster &operator = (const ElementRaster &element);

		virtual Entity* Clone() {return new ElementRaster(*this);}

		virtual void SetValue(const std::string &f, const std::string &v);

	protected:

		virtual int CreatePersistence();
		virtual int AdaptPersistence(Node *root);

	private:

	};

}


#endif 
