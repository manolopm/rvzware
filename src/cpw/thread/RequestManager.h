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

#ifndef _RESQUESTMANAGER_
#define _RESQUESTMANAGER_

#include <vector>

#include <cpw/thread/RequestThread.h>
#include <cpw/entity/Entity.h>

#include <cpw/Export.h>


namespace cpw {


	/** 
		\brief 
		\ingroup cpw
	*/
	class CPWEXPORT RequestManager: public RequestThread
	{

	public:

		RequestManager(IRequestReceiver *visual, const std::string &cn, int npet = 30000, IStatusController *status = NULL);

		virtual ~RequestManager(void);

		virtual void Process(cpw::Request &request);

		virtual int  NumberOutRequests() {return layers.size();}

		virtual void PreProcess() {GetLayers();};

		std::vector<cpw::Entity *> &GetLayers();

	private:

		std::vector<cpw::Entity *> layers;

		std::string layer_class_name;

	};


}


#endif


