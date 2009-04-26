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

#ifndef _OSGIMAGEMIXER_
#define _OSGIMAGEMIXER_

#include <vector>
#include <map>
#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>
#include <osg/Texture2D>

#include <cpw/common/types.h>
#include <cpw/graphic/IImgMixer.h>

#include <iosg/OsgPetitionManager.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{


		/** 
			\brief Por comentar...
			\ingroup iosg
		*/
		class IOSGEXPORT OsgImgMixer :	public cpw::IImgMixer, public OpenThreads::Thread, public cpw::IRequestReceiver
		{
			public:

				OsgImgMixer(const std::string &url);
				~OsgImgMixer(void);

				
				virtual void MixImages(const std::map<int, std::string> &image_filenames, const unsigned long int &petition_id);

				void SetPetitionManager( OsgPetitionManager *pm ) { osg_petition_manager = pm;}
				OsgPetitionManager * GetPetitionManager() { return osg_petition_manager; }

				void FinishThread();

				void SetImgFolder(const std::string &_img_folder) { img_folder = _img_folder + "/"; }
				
				

			private:

				struct TPetition { std::map<int, std::string> img_files;
								  unsigned long int petition_id;	};

				virtual void MixImages(const std::vector< osg::ref_ptr<osg::Image> > &images, osg::ref_ptr<osg::Texture2D> &new_texture);
				bool LoadAllImages(const std::map<int, std::string> &img_filenames, std::vector< osg::ref_ptr<osg::Image> > &img_vec, bool reverse=false);
				virtual void run();

				std::vector< TPetition > petitions;
				OpenThreads::Mutex mutex, cond_mutex, petition_mutex;
				osg::ref_ptr<osg::Image> dummy;
				osg::ref_ptr<osg::Texture2D> dummy_tex;
				OpenThreads::Condition cond;
				bool finished;
				std::string img_folder;

				OsgPetitionManager *osg_petition_manager;

		};
	}
}

#endif


