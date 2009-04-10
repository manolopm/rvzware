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

#include <cpw/thread/IRequestReceiver.h>

#include <OpenThreads/Mutex>

#include <osg/Group>
#include <osg/Geometry>
#include <osg/Drawable>


namespace cpw
{
	namespace iosg
	{
		class OsgSceneBuilder
		{
			public:

				OsgSceneBuilder();
				~OsgSceneBuilder();

				void SetRequestManager(cpw::IRequestReceiver *rm) {request_manager = rm;}
				void SetWCSRequestManager(cpw::IRequestReceiver *rm) {wcs_request_manager = rm;}
				osg::Geode* RequestAttended(cpw::Request &request);
				void SetCoords(const double &_up_left_corner_x, const double &_up_left_corner_y, const double &_down_right_corner_x, const double &_down_right_corner_y)
				{
					up_left_corner_x = _up_left_corner_x;
					up_left_corner_y = _up_left_corner_y;
					down_right_corner_x = _down_right_corner_x;
					down_right_corner_y = _down_right_corner_y;
				}
				unsigned long int NewPetition(const cpw::Point2d<float> &x0_y0, const cpw::Point2d<float> &x1_y1, unsigned int tile_width = 256, unsigned int tile_height = 256, const bool force=false, const std::string &filename = std::string());

				unsigned long int NewPetitionWCS(const cpw::Point2d<double> &x0_y0, const cpw::Point2d<double> &x1_y1, const unsigned int &resx=200, const unsigned int &resy=200);
				
				void RequestAttended(unsigned long int id);
				int NumPendingPetitions();

				void GetMutex();
				void ReleaseMutex();

				void Generate(cpw::Request &request);
				bool Generate() {return generate;}
				void SetCoordsMinMax(double _xmin, double _ymin, double _xmax, double _ymax) {xmin = _xmin; ymin = _ymin; xmax = _xmax; ymax = _ymax;}
				cpw::Point2d<double> GetCoordsMin() {return cpw::Point2d<double>(xmin, ymin);}
				void SetRequestMap(std::map<int, std::vector<unsigned long int> > _rm);
				void SetRequestDEMMap(std::map<int, std::vector<unsigned long int> > _rdm);
				std::vector<std::string> GetOsgdemParam() {return osgdem_param;}

				//void SetCoordsMinMax(double _xmin, double _ymin, double _xmax, double _ymax) {xmin = _xmin; ymin = _ymin; xmax = _xmax; ymax = _ymax;}
				//cpw::Point2d<double> GetCoordsMin() {return cpw::Point2d<double>(xmin, ymin);}

			private:

				cpw::IRequestReceiver *request_manager;
				cpw::IRequestReceiver *wcs_request_manager;
				unsigned long int id_generator;
				double up_left_corner_x, up_left_corner_y, down_right_corner_x, down_right_corner_y;

				osg::Geometry* geom;

				OpenThreads::Mutex petitions_mutex, extern_mutex;
				std::vector<unsigned long int> pending_petitions;

				std::vector<std::string> osgdem_param;
				int petitions;
				bool generate;
				std::vector<cpw::Request> corrupt_images;
				std::vector<osg::Timer_t> startTick;
				int lod;
				bool no_more;

				int jur;

				double xmin, ymin;
				double xmax, ymax;
				double res_x, res_y;
				float la_v;

				bool GenerateImage(cpw::Request &request);
				bool TestImage(cpw::Request &request);
				bool ComposeImage(cpw::Request &request);
				float CalculateV(cpw::Request &request, double &x, double &y);
				bool GenerateImageFromRAW(cpw::Request &request);
				bool CheckRAW(cpw::Request &request);
				std::map<int, std::vector<unsigned long int> > request_map;
				std::map<int, std::vector<unsigned long int> > request_dem;
				std::map<int, std::vector<std::string> > request_file_map;
				std::map<int, std::vector<cpw::Request> > request_mapp;
				std::vector<cpw::Request> backup;
				std::vector<cpw::Request> backup_height;
				std::vector<unsigned long int> wcs_petitions;

				//std::vector<float> temp_pixel_buffer;

				float maxh, height_factor;
				//bool generate;


		};
	}
}
