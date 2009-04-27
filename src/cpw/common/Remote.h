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

#ifndef _REMOTE_
#define _REMOTE_

#include <map>
#include <vector>
#include <string>
#include <ctime>

#include <cpw/Export.h>
#include <cpw/common/Sorted.h>
#include <cpw/common/RemoteNode.h>
#include <cpw/common/pstdint.h>

namespace cpw
{

	/** 
		\brief This class allows an Entity to be remote
		\ingroup cpw
	*/
	class CPWEXPORT Remote : public Sorted
	{
	public:

		/** Default constructor */
		Remote(const cpw::TypeId id = cpw::TypeId(), const std::string &class_name = std::string("Remote"));

		/** Copy constructor 
			\param remote to copy
		*/
		Remote(const Remote &remote);

		/** Destructor */
		virtual ~Remote();

		/** Overload of operator =
			\param remote to copy
		*/
		Remote &operator= (const Remote &remote);

		/** Returns the last time the entity was synchronized with the remote server 
			\param node represents the remote node
		*/
		time_t GetLastSynch(const RemoteNode &node);
		
		/** Sets the last time the entity was synchronized with the remote server 
			\param node represents the remote node
			\param last_synch the time of last synchronization
		*/
		void SetLastSynch(const RemoteNode &node, time_t last_synch);

		/** Return the remote nodes */
		std::vector<RemoteNode> GetNodes();

	protected:

    	/** Adds the information of the class to the persistent tree */	
		virtual int CreatePersistence();

		/** Loads the information of the class from the persistent tree
			\param root is the top level node of the persistent tree
		*/
		virtual int AdaptPersistence(Node *root);

	private:

		std::map<RemoteNode, time_t> synchs; ///< list to register the different synchronizations with every remote node
	};
}

#endif
