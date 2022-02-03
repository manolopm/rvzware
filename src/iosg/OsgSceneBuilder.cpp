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


#include <iosg/stdafx.h>
#include <iosg/OsgSceneBuilder.h>


///////////////////// builder //////////////////
//#include <vpb/Commandline>
//#include <vpb/DataSet>
//#include <vpb/DatabaseBuilder>
//#include <vpb/System>
//#include <vpb/Version>
//#include <vpb/FileUtils>
//
//#include <osgDB/ReadFile>
//#include <osgDB/WriteFile>
//
//#include <iostream>
///////////////////// end builder //////////////////

#include <ogc/wms/WmsLayerController.h>

//////PJ
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
//////

// para poder hacer el printf
#include <cpw/common/ApplicationLog.h>
#define printf cpw::ApplicationLog::GetInstance()->GetLogger()->printf

using namespace cpw::iosg;

OsgSceneBuilder::OsgSceneBuilder(void) : request_manager(NULL), id_generator(0),
                                         up_left_corner_x(0), up_left_corner_y(0), 
										 down_right_corner_x(0), down_right_corner_y(0),/*, extern_mutex(), petitions_mutex()*/
										 petitions(0),corrupt_images(), startTick(), lod(1), 
											no_more(false),request_file_map(), request_map(), request_mapp(), la_v(0), backup(), wcs_petitions(), request_dem(), backup_height(),
											maxh(0), height_factor(0),generate(false),osgdem_param()
{
	//extern_mutex.unlock();
	//petitions_mutex.unlock();
}

OsgSceneBuilder::~OsgSceneBuilder(void)
{
}

unsigned long int OsgSceneBuilder::NewPetition(const cpw::Point2d<float> &x0_y0, const cpw::Point2d<float> &x1_y1,  unsigned int tile_width, unsigned int tile_height, const bool force, const std::string &filename)
{
	unsigned long int id_to_return;
	if (request_manager == NULL)
		return 0;

	petitions_mutex.lock();
		id_generator++;		
		pending_petitions.push_back(id_generator);
		id_to_return = id_generator;
	petitions_mutex.unlock();
	
	//fill the request
	cpw::Request request;
	request.id = id_to_return;
	request.xo = x0_y0.x;
	request.yo = x0_y0.y;
	request.xf = x1_y1.x;
	request.yf = x1_y1.y;
	request.tile_size = tile_width;
	request.tile_width = tile_width;
	request.tile_height = tile_height;
	if(request.tile_height == 0)
		request.tile_height = tile_width;
	request.force = force;

	std::stringstream str;
	if(!filename.empty())
	{
		str << filename << "_" << tile_width<< "_" << tile_height;
	}
	else
	{
		str << "tile_" << request.xo << "_" << request.xf << "_" << request.yo << "_" << request.yf << "_" << tile_width<< "_" << tile_height;
	}
	
	request.tile_id = str.str();
	//request.file = std::string("9A0EE935-7E02-0C55-AFE9-3BBF17211572/")+str.str();

	request_manager->ProcessRequest(request);
	//wcs_request_manager->ProcessRequest(request);


	return id_to_return;
}

unsigned long int OsgSceneBuilder::NewPetitionWCS(const cpw::Point2d<double> &x0_y0, const cpw::Point2d<double> &x1_y1, const unsigned int &resx, const unsigned int &resy)
{
	unsigned long int id_to_return;

	if (request_manager == NULL)
		return 0;

	petitions_mutex.lock();
		id_generator++;		
		pending_petitions.push_back(id_generator);
		id_to_return = id_generator;
	petitions_mutex.unlock();
	
	//fill the request
	cpw::Request request;
	request.id = id_to_return;
	request.xo = x0_y0.x;
	request.yo = x0_y0.y;
	request.xf = x1_y1.x;
	request.yf = x1_y1.y;
	request.tile_size   = 0;
	request.tile_width  = resx;
	request.tile_height = resy;
	request.force = true;

	std::stringstream str;
	str << "tile_" << request.xo << "_" << request.xf << "_" << request.yo << "_" << request.yf << "_" << resx<< "_" << resy;

	
	request.tile_id = str.str();

	wcs_request_manager->ProcessRequest(request);

	wcs_petitions.push_back(id_to_return);

	return id_to_return;

}

osg::Geode* OsgSceneBuilder::RequestAttended(cpw::Request &request)
{
	RequestAttended(request.id);

	int mio = 0;

	float x0 = request.xo;
	float y0 = request.yo;
	float x1 = request.xf;
	float y1 = request.yf;


	osg::Vec3f v1(y0,x0, 50);
	osg::Vec3f v2(y1,x0, 50);
	osg::Vec3f v3(y1,x1, 50);

	osg::Vec3f v4(y1,x1, 50);
	osg::Vec3f v5(y0,x1, 50);
	osg::Vec3f v6(y0,x0, 50);

	//osg::Vec3f v1(x0,y0, 50);
	//osg::Vec3f v2(x1,y0, 50);
	//osg::Vec3f v3(x1,y1, 50);

	//osg::Vec3f v4(x1,y1, 50);
	//osg::Vec3f v5(x0,y1, 50);
	//osg::Vec3f v6(x0,y0, 50);

	osg::Geode* miGeode = new osg::Geode();
	osg::Geometry* geom = new osg::Geometry();
   
	miGeode->addDrawable(geom); 

	osg::Vec3Array* vertex = new osg::Vec3Array();
	vertex->clear();

	vertex->push_back(v1);
	vertex->push_back(v2);
	vertex->push_back(v3);

	vertex->push_back(v4);
	vertex->push_back(v5);
	vertex->push_back(v6);


///
	geom->removePrimitiveSet(0,1);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES ,0,vertex->size()));	
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::Vec3Array* normal_array = (osg::Vec3Array*)geom->getNormalArray();
	if(normal_array == NULL) normal_array = new osg::Vec3Array();
	normal_array->clear();

	normal_array->push_back(osg::Vec3(0, 0, 1));		
	normal_array->push_back(osg::Vec3(0, 0, 1));		
	normal_array->push_back(osg::Vec3(0, 0, 1));		
	normal_array->push_back(osg::Vec3(0, 0, 1));		
	normal_array->push_back(osg::Vec3(0, 0, 1));		
	normal_array->push_back(osg::Vec3(0, 0, 1));		

	geom->setVertexArray(vertex);
	geom->setNormalArray(normal_array);



	//pega la textura
		osg::ref_ptr<osg::Vec2Array> texcoords;
		unsigned int nvertices = geom->getVertexArray()->getNumElements();

		texcoords = new osg::Vec2Array(nvertices);

		osg::ref_ptr<osg::StateSet> stateset = geom->getOrCreateStateSet();

		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

		//the texture
		std::string error_text = "./wms_error.png";
		
		float repeat_img = 1;

		osg::ref_ptr<osg::Image> dummy_image = osgDB::readImageFile((request.file).c_str());

		if(dummy_image == NULL)
			dummy_image = osgDB::readImageFile(error_text.c_str());
		
		dummy_image->flipVertical();
		osg::ref_ptr<osg::Texture2D> dummy_tex = new osg::Texture2D;
		dummy_tex.get()->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
		dummy_tex.get()->setImage(dummy_image.get());
		dummy_tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		dummy_tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

		stateset.get()->removeTextureAttribute(0,&(*dummy_tex));
		

		(*texcoords.get())[0].set(0,1);
		(*texcoords.get())[1].set(1,1);
		(*texcoords.get())[2].set(1,0);

		(*texcoords.get())[3].set(1,0);
		(*texcoords.get())[4].set(0,0);
		(*texcoords.get())[5].set(0,1);



		geom->setTexCoordArray(0, texcoords.get());
		stateset.get()->setTextureAttributeAndModes(0,&(*dummy_tex),osg::StateAttribute::ON);
		//stateset.get()->setMode(GL_LIGHTING, true);
		//stateset.get()->setMode(GL_CULL_FACE,osg::StateAttribute::ON);

		geom->setStateSet(stateset.get());



	return miGeode;

}


void OsgSceneBuilder::RequestAttended(unsigned long int id)
{
	petitions_mutex.lock();

	std::vector<unsigned long int>::iterator it = pending_petitions.begin();
	while((it != pending_petitions.end()) &&  (*it != id))
	{
		it++;
	}
	if(it != pending_petitions.end())
		pending_petitions.erase(it);

	petitions_mutex.unlock();

}

int OsgSceneBuilder::NumPendingPetitions()
{
	unsigned int n;
	petitions_mutex.lock();
	
		n = pending_petitions.size();
		
	petitions_mutex.unlock();

	return n;

}

void OsgSceneBuilder::GetMutex()
{
	extern_mutex.lock();
}

void OsgSceneBuilder::ReleaseMutex()
{
	extern_mutex.unlock();
}


void OsgSceneBuilder::Generate(cpw::Request &request)
{
	//scene_builder.GetMutex();
	//	double duration = osg::Timer::instance()->delta_s(startTick.front(), osg::Timer::instance()->tick())*scene_builder.NumPendingPetitions();
	//	startTick.erase(startTick.begin());
	//	ss << "Pending Petitions: "<< scene_builder.NumPendingPetitions()<< " (Remainig time: "<<duration<<" sec.)";
	//	//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(ss.str());
	//scene_builder.ReleaseMutex();
				
	if(!TestImage(request))
	{
		corrupt_images.push_back(request);
		GenerateImage(request);			
	}

	if(request.tile_size != 0)
	{
		backup.push_back(request);
	}
	else
	{					
		// preparar la linea de comando para que encajen las alturas y las imagenes
		backup_height.push_back(request);
	}
		

	if(NumPendingPetitions() == 0)			
	{
		//startTick.clear();

		if(!corrupt_images.empty())
		{					
			//SAVE LOG WITH CORRUPT IMAGES
			std::string dir;
			if(!backup.empty())
			{
				cpw::Request request = backup.front();
				int pos = (request.file).rfind("/",(request.file).size());
				dir = (request.file).substr(0,pos);
			}
			else
			{
				dir = "";
			}
			
			std::ofstream fout(std::string(dir+"\\corrupt_images.txt").c_str());
			if(fout.is_open()) 
			{
				for(std::vector<cpw::Request>::iterator it = corrupt_images.begin();
					it != corrupt_images.end();
					it++)
				{
					fout << it->file << " " << it->xo << " " << it->yo << " " << it->xf << " " << it->yf << "\n";
				}
				fout.close();
			}

			std::stringstream ss_err_img;
			ss_err_img << "Total corrupt images:" << corrupt_images.size();
			cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(ss_err_img.str());

			std::stringstream ss_err_img2;
			ss_err_img2 << "You can see a log with corrupt images in: " << dir<<"\\corrupt_images.txt";

			corrupt_images.clear();
		}

////////////////////////////////////////////////////////////////////////////
		for(std::vector<cpw::Request>::iterator aux = backup_height.begin();
			aux != backup_height.end();
			aux++)
		{
			cpw::Request request = *aux;

			GenerateImageFromRAW(request);

			//if((!request_dem.empty()) && ( backup_height.size() == request_dem.size()*request_dem[0].size()))
			{

				double minx, miny;
				minx = miny = 0.0;
				la_v = CalculateV(request, minx, miny);

				osgdem_param.push_back("--xx");
				std::stringstream p1;
				p1.precision(5);					
				p1<< 25;  // peligroooooo;
				osgdem_param.push_back(p1.str());  

				osgdem_param.push_back("--yy");
				std::stringstream p2;
				p2.precision(5);
				p2<< 25;  // peligroooooo;
				osgdem_param.push_back(p2.str());

				if(minx != 0.0)
				{
					osgdem_param.push_back("--xt");
					std::stringstream p;
					p.precision(5);						
					p<< minx - xmin ;
					osgdem_param.push_back(p.str());
				}

				if(miny != 0.0)
				{
					osgdem_param.push_back("--yt");
					std::stringstream p;
					p.precision(5);						
					p<< miny - ymin ;
					osgdem_param.push_back(p.str());
				}

				osgdem_param.push_back("-d");
				std::stringstream p3;
				p3.precision(10);
				p3<<request.file;
				osgdem_param.push_back(p3.str());
			}
		}
///////////////////
		for(std::vector<cpw::Request>::iterator aux = backup.begin();
			aux != backup.end();
			aux++)
		{
			cpw::Request request = *aux;

			if( ComposeImage(request) )
			{
				std::map<int, std::vector<cpw::Request> >::iterator it_mapr = request_mapp.begin();
				for( ; it_mapr != request_mapp.end() ; it_mapr++)
				{
					if(it_mapr->second.size() > 1) 
					{
						osgdem_param.push_back("--xx");
						osgdem_param.push_back("1");
						osgdem_param.push_back("--yy");
						osgdem_param.push_back("1");
						osgdem_param.push_back("--xt");
						std::stringstream p1;
						p1.precision(10);
						p1<<it_mapr->second.front().yo-xmin;
						osgdem_param.push_back(p1.str());
						osgdem_param.push_back("--yt");
						std::stringstream p2;
						p2.precision(10);
						p2<<it_mapr->second.front().xo-ymin;
						osgdem_param.push_back(p2.str());
						osgdem_param.push_back("-t");
						std::stringstream p3;
						p3.precision(10);
						p3<<it_mapr->second.front().file;
						osgdem_param.push_back(p3.str());
						
					}
					else
					{
						std::vector<cpw::Request>::iterator itv = it_mapr->second.begin();
						for( ; itv != it_mapr->second.end() ; itv++)
						{
							osgdem_param.push_back("--xx");
							osgdem_param.push_back("1");
							osgdem_param.push_back("--yy");
							osgdem_param.push_back("1");
							osgdem_param.push_back("--xt");
							std::stringstream p1;
							p1.precision(10);
							p1<< ((*itv).yo)-xmin;
							osgdem_param.push_back(p1.str());
							osgdem_param.push_back("--yt");
							std::stringstream p2;
							p2.precision(10);
							p2<<((*itv).xo)-ymin;
							osgdem_param.push_back(p2.str());
							osgdem_param.push_back("-t");
							std::stringstream p3;
							p3.precision(10);
							p3<<((*itv).file);
							osgdem_param.push_back(p3.str());
						}
					}
				}				
			}					
		}
////////////////////////////////////////////////////////////////////////////
		std::stringstream ss;
		ss << "Pending Petitions: "<< NumPendingPetitions();
		cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(ss.str());

		//Calculate the suitable LOD level for generation process
		int dim = (int)std::max(abs(xmax - xmin),abs(ymax - ymin));
		lod = 2;
		while (dim > 1000)
		{
			dim = dim/2;
			lod++;
		}
		if(lod > 10) // > 10 ITS VERY LARGE GENERATION PROCESS
			lod = 10;
		else if(lod < 3)
			lod = 3;

		//lod++;
		std::string sr;
		if(!backup.empty())
			sr = backup.front().file;
		else
			sr = "";

		int pos = sr.rfind("/",sr.size());
		std::string dir;
		if(pos != -1)
			dir = sr.substr(0,pos);
		else
			dir = "";
		std::string ooo;
		ooo = dir;
		dir = dir+"\\terrain\\terrain.ive" ;

		osgdem_param.push_back("--tile-image-size");
		osgdem_param.push_back("1024");
		//osgdem_param.push_back("--compressed");
		osgdem_param.push_back("-l");
		std::stringstream p1;
		p1.precision(10);
		p1<<lod;
		osgdem_param.push_back(p1.str());
		osgdem_param.push_back("-v");
		std::stringstream p2;
		p2.precision(10);
		p2<<la_v;
		osgdem_param.push_back(p2.str());
		osgdem_param.push_back("-o");
		osgdem_param.push_back(dir);

		osgdem_param.insert(osgdem_param.begin(),"capabuilder");	

		std::stringstream ssp;
		ssp.precision(15);
		std::vector<std::string>::iterator it = osgdem_param.begin();
		for( ; it != osgdem_param.end() ; it++)
		{
			ssp << *it << " ";
		}

		//SAVE THE GENERATION COMMAND
		std::ofstream fout(std::string(ooo+"\\osgdem.txt").c_str());
		if(fout.is_open()) 
		{
			fout << ssp.str() << "\n";
			fout.close();
		}


		cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("Download Finished.");

		cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("Generation Command:");
		cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(ssp.str());


		//preview  = true;
		generate = true;
		no_more = false;

		backup.clear();
		request_map.clear();
		request_file_map.clear();
		request_mapp.clear();
		//osgdem_param.clear();

		generate = false;

	}
}

bool OsgSceneBuilder::TestImage(cpw::Request &request)
{
	if(request.tile_size == 0)
	{
		//osg::ref_ptr<osg::Image> new_img = osgDB::readImageFile(request.file);
		//if(new_img == NULL)
		//{
			//return GenerateImageFromRAW(request);
		return CheckRAW(request);
		//}
		//return true;
	}

	osg::ref_ptr<osg::Image> new_img = osgDB::readImageFile(request.file);
	if(new_img != NULL)
	{
		std::string filename = request.file;
		int pos = filename.rfind(".", filename.size());
		std::string name = filename.substr(0,pos);
		std::string ext  = filename.substr(pos,filename.size());
		//if(ext == ".jpg")
		//{
		//	name = name+".png";
		//	request.file = name;
		//	bool conv = osgDB::writeImageFile(*new_img, name);
		//}

		return true;
	}

	return false;
}


void OsgSceneBuilder::SetRequestMap(std::map<int, std::vector<unsigned long int> > _rm)
{
	request_map = _rm;

	request_file_map.clear();
	request_mapp.clear();

	for(std::map<int, std::vector<unsigned long int> >::iterator it= request_map.begin();
		it != request_map.end();
		it++)
	{
		std::vector<std::string> sv(it->second.size(),"");

		request_file_map.insert(std::make_pair<int, std::vector<std::string> >((int)it->first,(std::vector<std::string>)sv));
		request_mapp.insert(std::make_pair<int, std::vector<cpw::Request> >((int)it->first,(std::vector<cpw::Request>)it->second.size()));
	}
}


void OsgSceneBuilder::SetRequestDEMMap(std::map<int, std::vector<unsigned long int> > _rdm)
{
	request_dem = _rdm;

}


bool OsgSceneBuilder::ComposeImage(cpw::Request &request)
{
	int pos = -1;

	std::map<int, std::vector<unsigned long int> >::iterator it = request_map.begin();
	std::vector<unsigned long int>::iterator v_it;
	int count = 0;
	while((it!=request_map.end()) && (pos == -1))
	{
		v_it = it->second.begin();
		count = 0;
		while((v_it != it->second.end()) && (*v_it != request.id))
		{
			v_it++;
			count++;
		}

		if(v_it!=it->second.end())
			pos = it->first;
		else
			it++;
	}

	if(it!=request_map.end())
	{
		(request_file_map[pos])[count] = request.file;
		(request_mapp[pos])[count] = request;
	}

	for(std::map<int, std::vector<std::string> >::iterator aux=request_file_map.begin();
		aux != request_file_map.end();
		aux++)
	{
		std::vector<std::string>::iterator v_aux = aux->second.begin();
		while(v_aux != aux->second.end()  &&  *v_aux!="")
			v_aux++;

		if(v_aux != aux->second.end())
			return false;
	}
	

////////////////Composicion de imagenes////////////////////////////////////////////////////////		

	std::map<int, std::vector<cpw::Request> >::iterator iter = request_mapp.begin();

	for(; iter != request_mapp.end() ; iter++)
	{
		if(iter->second.empty()) return false;

		std::stringstream ss;			
		std::string filename = request.file;
		int pos = filename.rfind("/", filename.size());
		std::string dir = filename.substr(0,pos);
		ss << dir <<"/composed_img"<< iter->first <<".png" ;

		osg::ref_ptr<osg::Image> new_img = osgDB::readImageFile(ss.str());
		if(new_img != NULL)
		{
			cpw::Request r = iter->second.front();
			r.file = ss.str();
			(iter->second)[0] = r;

			continue;
		}

		if(iter->second.size() > 1)   
		{
			try 
			{

				std::vector< osg::ref_ptr<osg::Image> > images;
				std::vector<cpw::Request>::iterator img_iter = iter->second.begin();
				for( ; img_iter != iter->second.end() ; img_iter++)
				{
					osg::ref_ptr<osg::Image> img_tmp = osgDB::readImageFile((*img_iter).file);
					images.push_back(img_tmp);
				}

				unsigned int bpp_initial = (images.front()->getPixelSizeInBits())/8;
				osg::Image *first_img = images.front().get();
				
				int resx, resy;
				osg::ref_ptr<osg::Image> img = new osg::Image();		
				
				resx = first_img->s();
				resy = first_img->t();

				int ry = iter->second.back().xf - iter->second.front().xo;
				int rx = iter->second.back().yf - iter->second.front().yo;
				
				unsigned int img_bytes_count = (rx * ry * bpp_initial);
				unsigned char *new_pixel_buffer =  (unsigned char *) malloc( sizeof(unsigned char) * img_bytes_count );
				unsigned char *destiny_pixel, *origin_pixel;

				int new_resx = images.size(); 
				int new_resy = 1; 
				

				new_resx = rx / resx ;
				if(rx%resx>200) new_resx++;
				new_resy = ry / resy ;
				if(ry%resy>200) new_resy++;

				//initialize the pixels_buffer
				memset((void *) new_pixel_buffer, 0, img_bytes_count); //faster than a "for" all the life ;)
				img->setImage(rx, ry, 1, first_img->getInternalTextureFormat(), first_img->getPixelFormat(),
		    					  first_img->getDataType(), new_pixel_buffer, first_img->getAllocationMode());

				
				std::vector< unsigned int > pixel_sizes;
				std::vector< unsigned char * > images_data;
				
				for (std::vector< osg::ref_ptr<osg::Image> >::const_iterator k = images.begin();
					 k != images.end();
					 k++)
				{
					pixel_sizes.push_back( (*k)->getPixelSizeInBits() );
					images_data.push_back( (*k)->data() );
				}

				destiny_pixel = img->data();
				unsigned int bpp=0;
				
				for (unsigned int j=0; j < (unsigned int)new_resy; j++)
				{
					resy = images[j*new_resx]->t();
					for (unsigned int i=0; i < (unsigned int)(resy); i++)
					{

						for (unsigned int b=0; b < (unsigned int)new_resx; b++)
						{
							origin_pixel = images_data[b+(j*new_resx)];
							
							// ALPHA ??
							bpp = pixel_sizes[b+(j*new_resx)];

							resx = images[b+(j*new_resx)]->s();
							origin_pixel = origin_pixel+(resx*i*(bpp/8));

							//bpp = 24;
							for (unsigned int a=0; a <(unsigned int)(resx); a++)
							{						
								// compute RGB
								destiny_pixel[0] = origin_pixel[0] ;
								destiny_pixel[1] = origin_pixel[1] ;
								destiny_pixel[2] = origin_pixel[2] ;
								if(bpp > 24)
								{
									destiny_pixel[3] = 255;
									origin_pixel  += 4;
									destiny_pixel += 4;
								}
								else
								{				
									origin_pixel  += 3;
									destiny_pixel += 3;
								}
							}
						}
					}				
				}

				osgDB::writeImageFile(*img, ss.str());

				cpw::Request r = iter->second.front();
				r.file = ss.str();
				(iter->second)[0] = r;

			} //end try
			catch(...)
			{				
				return false;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////////////////

	return true;

}


bool OsgSceneBuilder::GenerateImage(cpw::Request &request)
{
	osg::ref_ptr<osg::Image> img = new osg::Image();		

	int resx, resy;
	resy = request.xf - request.xo;
	resx = request.yf - request.yo;

	if(request.tile_size == 0)
	{
		resx /= request.tile_width;
		resy /= request.tile_height;
		//if(GenerateImageFromRAW(request)) return true;
	}

	unsigned int img_bytes_count = (resx * resy * 3);
	unsigned char *new_pixel_buffer =  (unsigned char *) malloc( sizeof(unsigned char) * img_bytes_count );

	//initialize the pixels_buffer
	memset((void *) new_pixel_buffer, 255, img_bytes_count); //faster than a "for" all the life ;)

	img->setImage(resx,resy, 1, 6407, 6407, 5121, new_pixel_buffer, osg::Image::USE_NEW_DELETE);

	return osgDB::writeImageFile(*img, request.file);
}


bool OsgSceneBuilder::CheckRAW(cpw::Request &request)
{
	std::ifstream fin(std::string(request.file+".txt").c_str());

	if (!fin.is_open())
	{
		fin.close();
		return false;
	}

	float h;
	h =  0.0;
	std::string aux1;
	float aux2;
	fin >> aux1 >> aux2;  //ncols

	if(aux1.find("<") != -1)
	{
		fin.close();
		return false;
	}

	fin >> aux1 >> aux2;  //nrows
	fin >> aux1 >> aux2/*xmin*/;  //xllcorner
	fin >> aux1 >> aux2/*ymin*/;  //yllcorner
	fin >> aux1 >> aux2;  //cellsize
	fin >> aux1 >> aux2;  //nodata
	while (!fin.eof())
	{
		fin >> h;
		if((h > maxh) )
			maxh = h;
	}
	fin.close();
return true;
	//return maxh/255.0;

}

float OsgSceneBuilder::CalculateV(cpw::Request &request, double &vxmin, double &vymin)
{
	std::ifstream fin(std::string(request.file+".txt").c_str());

	if (!fin.is_open())
	{
		fin.close();
		return 0;
	}

	float h/*, maxh*/;
	h  = 0.0;
	std::string aux1;
	float aux2;
	fin >> aux1 >> aux2;  //ncols

	if(aux1.find("<") != -1)
	{
		fin.close();
		return 0;
	}

	fin >> aux1 >> aux2;  //nrows
	fin >> aux1 >> vxmin;  //xllcorner
	fin >> aux1 >> vymin;  //yllcorner
	fin >> aux1 >> aux2;  //cellsize
	fin >> aux1 >> aux2;  //nodata
	while (!fin.eof())
	{
		fin >> h;
		if((h > maxh) )
			maxh = h;
	}
	fin.close();
	std::stringstream ss;
	ss << "Max height: " << maxh;
	cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(ss.str());
	return maxh/255.0;
}


bool OsgSceneBuilder::GenerateImageFromRAW(cpw::Request &request)
{
		std::string name = request.file;
		name = name+".txt";

		std::ifstream text_file(name.c_str());
		//if(!text_file.is_open()) return false;

		if (text_file.is_open())
		{
			float ncols;
			float nrows;
			float xllcorner;
			float yllcorner;
			float cellsize;
			float novalue;
			std::string aux;

			float height_factor = 255/maxh;

			//std::ifstream text_file(name.c_str());
			text_file >> aux >> ncols;
			if(aux.find("<") != -1)
				return true;
			text_file >> aux >> nrows;
			text_file >> aux >> xllcorner;
			text_file >> aux >> yllcorner;
			text_file >> aux >> cellsize;
			text_file >> aux >> novalue;

			osg::ref_ptr<osg::Image> img = new osg::Image();		
			unsigned int img_bytes_count = (ncols * nrows );
			unsigned char *new_pixel_buffer =  (unsigned char *) malloc( img_bytes_count );

			int i = 0;
			int j = 0;
			int k = 0;
			for(i=0; i<nrows; i++)
			{			
				for(j=0; j<ncols; j++)
				{
					float aux;
					unsigned short int corto = 0;
					if(!text_file.eof())
					{
						text_file >> aux;
						if(aux < 0.001)
							aux = 0;
						corto = aux * height_factor;
						if(corto > 255)
							corto = 255;
						if(corto < 0)
							corto = 0;
						new_pixel_buffer[k] = corto;				
						k++;
					}
					else
					{
						free(new_pixel_buffer);
						text_file.close();
						return false;
					}

				}
			}

			img->setImage(ncols,nrows, 1, GL_LUMINANCE8 /*InternalTextureFormat*/, GL_LUMINANCE /*PixelFormat*/, GL_BYTE /*DataType*/, 
						  new_pixel_buffer /*pixels*/, osg::Image::USE_NEW_DELETE /*AllocationMode*/);
			osgDB::writeImageFile(*img, request.file);

			text_file.close();	
			return true;
		}
		else
			return false;
		
}



