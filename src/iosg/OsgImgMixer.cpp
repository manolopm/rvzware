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
#include <stdlib.h>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/ref_ptr>

#include "stdafx.h"

#include <iosg/OsgImgMixer.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/common/ApplicationLog.h>


using namespace cpw::iosg;

OsgImgMixer::OsgImgMixer(const std::string &url) : finished(false)
{
	setSchedulePriority(THREAD_PRIORITY_MIN); 

	std::string icon_path = url;
	icon_path += "dummyRGBA.tif";

	dummy = osgDB::readImageFile(icon_path.c_str());
	dummy_tex = new osg::Texture2D;
	dummy_tex.get()->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
	dummy_tex.get()->setImage(dummy.get());

}

OsgImgMixer::~OsgImgMixer(void)
{
	finished = true;

    while(isRunning())
    {
		cond_mutex.lock();
		cond.signal();
		cond_mutex.unlock();
    }

}

void OsgImgMixer::MixImages(const std::vector< osg::ref_ptr<osg::Image> > &images,
							osg::ref_ptr<osg::Texture2D> &new_texture)
{
	osg::ref_ptr<osg::Image> new_img = new osg::Image();
	if(images.empty())
	{
		// si hubo un error, devuelvo la X
		
		osg::Image* image = osgDB::readImageFile ("..\\wms_error.png");
		new_texture = new osg::Texture2D(image);
	/*
	int resx, resy;
		resx = resy = 128;
		unsigned int img_bytes_count = resx * resy * 4;
		unsigned char *new_pixel_buffer = new unsigned char[img_bytes_count];
		memset((void *) new_pixel_buffer, 0, img_bytes_count); //faster than a "for" all the life ;)
		new_img->setImage(resx, resy, 1, dummy->getInternalTextureFormat(), dummy->getPixelFormat(),
		    					dummy->getDataType(), new_pixel_buffer, dummy->getAllocationMode()); 
	*/
	}
	else
	{
		int resx, resy;
		float inv_255 = 1.0f / 255.0f;
		float alpha, alpha_acum;
		osg::Image *first_img = images.begin()->get();
		resx = first_img->s();
		resy = first_img->t();
		unsigned int img_bytes_count = resx * resy * 4;
		unsigned char *new_pixel_buffer = /*new unsigned char[img_bytes_count];*/ (unsigned char *) malloc( sizeof(unsigned char) * img_bytes_count );
		unsigned char *destiny_pixel, *origin_pixel;

		//initialize the pixels_buffer
		memset((void *) new_pixel_buffer, 0, img_bytes_count); //faster than a "for" all the life ;)
		new_img->setImage(resx, resy, 1, dummy->getInternalTextureFormat(), dummy->getPixelFormat(),
		    			  dummy->getDataType(), new_pixel_buffer, dummy->getAllocationMode());
		
		std::vector< unsigned int > pixel_sizes;
		std::vector< unsigned char * > images_data;
		
		for (std::vector< osg::ref_ptr<osg::Image> >::const_iterator k = images.begin();
			 k != images.end();
			 k++)
		{
			pixel_sizes.push_back( (*k)->getPixelSizeInBits() );
			images_data.push_back( (*k)->data() );
		}

		destiny_pixel = new_img->data();
		unsigned int bpp=0;

		float inv_8 = 1.0f / 8.0f; 

		for (unsigned int i=0; i < (unsigned int)(resx * resy); i++)
		{
			for (unsigned int j=0; j < images.size(); j++)
			{
				// miramos cuantos bits tiene por pixel, para ver si hay canal alfa
				bpp = pixel_sizes[j];
				origin_pixel = images_data[j];

				if (origin_pixel[3] != 0)
				{
					////normalize the alpha value to 0 - 1
					//alpha = (bpp>24)? ((float)origin_pixel[3]) * inv_255 : 1;

					//// compute RGB
					//destiny_pixel[0] = (unsigned char)((float)origin_pixel[0] * alpha) + ((1.0f - alpha) * ((float) destiny_pixel[0]));
					//destiny_pixel[1] = (unsigned char)((float)origin_pixel[1] * alpha) + ((1.0f - alpha) * ((float) destiny_pixel[1]));
					//destiny_pixel[2] = (unsigned char)((float)origin_pixel[2] * alpha) + ((1.0f - alpha) * ((float) destiny_pixel[2]));
					//					
					//// compute alpha
					//alpha_acum = (float) destiny_pixel[3] + (bpp>24)? origin_pixel[3] : 255;
					//destiny_pixel[3] += (alpha_acum>255)? 255 : (unsigned char) alpha_acum;

					//normalize the alpha value to 0 - 1
					if (bpp > 24)
					{
						alpha = ((float)origin_pixel[3]) * inv_255;

						// compute RGB
						destiny_pixel[0] = (unsigned char)((float)origin_pixel[0] * alpha) + ((1.0f - alpha) * ((float) destiny_pixel[0]));
						destiny_pixel[1] = (unsigned char)((float)origin_pixel[1] * alpha) + ((1.0f - alpha) * ((float) destiny_pixel[1]));
						destiny_pixel[2] = (unsigned char)((float)origin_pixel[2] * alpha) + ((1.0f - alpha) * ((float) destiny_pixel[2]));

						alpha_acum = (float) destiny_pixel[3] + origin_pixel[3];
						destiny_pixel[3] = (alpha_acum>255)? 255 : (unsigned char) alpha_acum;
					}
					else
					{
						alpha = 1;
						// compute RGB
						destiny_pixel[0] = origin_pixel[0] ;
						destiny_pixel[1] = origin_pixel[1] ;
						destiny_pixel[2] = origin_pixel[2] ;
						destiny_pixel[3] = 255;
					}
				}

				//int debug = (int) (bpp * inv_8);
				images_data[j] += (int) (bpp * inv_8);
			}
			destiny_pixel += 4;
		}
		new_texture = new osg::Texture2D(new_img.get());
	}

	// la siguiente instrucción la he subido solo al else
	//new_texture = new osg::Texture2D(new_img.get());
}

void OsgImgMixer::MixImages(const std::map<int, std::string> &image_filenames, const unsigned long int &petition_id)
{
	TPetition new_petition;
	new_petition.img_files = image_filenames;
	new_petition.petition_id = petition_id;

	petition_mutex.lock();
	petitions.push_back(new_petition);
	petition_mutex.unlock();

	cond_mutex.lock();
	cond.signal();
	cond_mutex.unlock();
}

void OsgImgMixer::run()
{
	//this->detach();

	//loop when petitions pending, otherwise sleep
	do
	{
		if (!petitions.empty())
		{
			petition_mutex.lock();
			TPetition petition_to_process;
			std::vector<osg::ref_ptr<osg::Image> > images;			
			osg::ref_ptr<osg::Texture2D> result;// = NULL;

			petition_to_process = petitions.back();
			petitions.pop_back();

			petition_mutex.unlock(); //to let other processes acquire the mutex

			bool ok = LoadAllImages(petition_to_process.img_files, images, false);
			if (!ok) { 
				printf ("\n\n ¡¡¡¡¡¡¡¡¡¡¡¡¡error en el LoadAllImages!!!!!!!!!!!!!\n\n");
				images.clear();
			}

			//if (images.size() > 0)
			//{
			MixImages(images, result);
			//}
		
			if (osg_petition_manager != NULL)
			{
				osg_petition_manager->NewAttendedPetition(result.get(), petition_to_process.petition_id);
				//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("ImgMixer mixed an image");
			}
		}
		else
		{
			cond_mutex.lock();
			cond.wait(&cond_mutex); //sleep until is called again
			cond_mutex.unlock();
		}
	}while(!testCancel() && !finished);
}

void OsgImgMixer::FinishThread()
{ 
	cond_mutex.lock();
	finished = true; 
	cond.signal();
	cond_mutex.unlock();

}

bool OsgImgMixer::LoadAllImages(const std::map<int, std::string> &img_filenames, std::vector< osg::ref_ptr<osg::Image> > &img_vec, bool reverse)
{
	bool ok = true;

	img_vec.clear();

	//load all the images
	std::map<int, std::string>::const_reverse_iterator i = img_filenames.rbegin();
	for ( ; i != img_filenames.rend(); i++)
	{
		std::string img_filename = img_folder + i->second;
		
		osg::ref_ptr<osg::Image> temp = osgDB::readImageFile(img_filename);
		
		if (temp != NULL)
			img_vec.push_back(temp);
		else
			ok = false;
	}

	return ok;
}
