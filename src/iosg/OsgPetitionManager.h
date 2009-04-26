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
#ifndef _OSGPETITIONMANAGER_
#define _OSGPETITIONMANAGER_

#include <map>
#include <OpenThreads/Mutex>
#include <osg/Texture2D>

#include <cpw/common/types.h>
#include <cpw/graphic/IScene.h>
#include <cpw/thread/IRequestReceiver.h>

#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgPetitionManager
		{
			public:
				
				OsgPetitionManager();
				~OsgPetitionManager(void);

				void NewAttendedPetition(osg::Texture2D *new_img, const unsigned long int &petition_id);

				//El visitor llamaría a estas dos
				unsigned long int NewPetition(const cpw::Point2d<float> &x0_y0, const cpw::Point2d<float> &x1_y1, unsigned int tile_size = 256, const std::string &filename = std::string());
				bool IsPetitionAttended(const unsigned long int &petition_id, osg::ref_ptr<osg::Texture2D> &out_text); //returns null if petition is not attended yet

				void SetRequestManager(cpw::IRequestReceiver *rm) {request_manager = rm;}
				//virtual void Update();

				void Clear();

				void GainSafeAccess() { petitions_mutex.lock(); }
				void ReleaseSafeAccess() { petitions_mutex.unlock(); }

			private:
				
				unsigned long int id_generator;
				OpenThreads::Mutex petitions_mutex;
				std::map<unsigned long int, osg::ref_ptr<osg::Texture2D> > pending_petitions;
				//OsgImgMixer img_mixer;

				cpw::IRequestReceiver *request_manager;
		};
	}
}

#endif


