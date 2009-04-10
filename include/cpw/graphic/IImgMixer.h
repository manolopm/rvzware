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
#ifndef _IIMGMIXER_
#define _IIMGMIXER_

#include <vector> 
#include <map> 

#include <cpw/Export.h>
#include <cpw/common/types.h>

namespace cpw
{
	

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	struct CPWEXPORT ImgMixerRequest
	{
		unsigned long int id;
		std::map<int, std::string> tile_ids;
	};


	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class IImgMixer
	{
		public:

			virtual ~IImgMixer(void){};

			//virtual IImage * MixImages(const std::vector<std::string> &image_filenames) = 0;
			virtual void MixImages(const std::map<int, std::string> &image_filenames, const unsigned long int &petition_id) = 0;

			//virtual bool NewRequest(const ImgMixerRequest &new_request) = 0;
			//virtual bool NewRequestedImg(const unsigned int &petition_id, const std::string &img_id)=0;

	};
}

#endif