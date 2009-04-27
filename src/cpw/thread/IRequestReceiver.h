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

#ifndef _IRESQUESTRECEIVER_
#define _IRESQUESTRECEIVER_

#include <cpw/thread/Request.h>

namespace cpw 
{


	/** 
		\brief 
		\ingroup cpw
	*/
	class IRequestReceiver
	{

		public:

			virtual ~IRequestReceiver(){}

			virtual int  ProcessRequest(Request &request) {return 0;};
		
			virtual int  SendRequest(Request &request)    {return 0;};

			virtual int  ReturnRequest(Request &request)  {return 0;};

			virtual void Clear() {}

			virtual void ClearInvoker() {}


	};


}


#endif



