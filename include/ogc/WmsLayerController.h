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
#ifndef _WMSLAYERCONTROLLER_
#define _WMSLAYERCONTROLLER_

#include <wms/wmsClient.h>
#include <wms/wmsCapabilitiesRoot.h>

#include "WmsLayer.h"
#include "Export.h"

#include <vector>
#include <string>
#include <iostream>


namespace cpw 
{
	namespace ogc
	{

		/** \brief Enumeration for the node types 
			\ingroup ogc
		*/
		enum NodeType {

			LEAF = 0,
			FOLDER = 1
		};

		/** \brief A basic node
			\ingroup ogc
		*/
		class Node {

		public:

			virtual ~Node() {};

			virtual int Add(Node *node) { return -1; }

			NodeType &Type()     {return type; }
			std::string &Name()  {return name;}
			std::string &Title() {return title;}

			const NodeType &Type()     const {return type; }
			const std::string &Name()  const {return name;}
			const std::string &Title() const {return title;}

			virtual std::vector<Node *> GetNodes(){ return std::vector<Node *>();}
			virtual const std::vector<Node *> GetNodes() const { return std::vector<Node *>();}

		protected:
			Node(NodeType typ, std::string nam = std::string(), std::string tit = std::string()): 
					type(typ), name(nam), title(tit){}

		private:

			NodeType type;
			std::string name;
			std::string title;
		};


		/** \brief A compound node 
			\ingroup ogc
		*/
		class Folder: public Node {

		public:
			Folder(std::string nam = std::string(), std::string tit = std::string()): 
				  Node(FOLDER, nam, tit){}

			virtual ~Folder() { 

				for(std::vector<Node *>::iterator i = nodes.begin(); i != nodes.end(); i++)
					
					delete *i;
			}

			int Add(Node *node) { nodes.push_back(node); return 0;}

			virtual std::vector<Node *> GetNodes(){return nodes;}
			virtual const std::vector<Node *> GetNodes() const {return nodes;}


		private:

			std::vector<Node *> nodes;

		};


		/** \brief A leaf node 
			\ingroup ogc
		*/
		class Leaf: public Node {

		public:
			Leaf(std::string nam = std::string(), std::string tit = std::string()): 
				  Node(LEAF, nam, tit){}

		};



		/** \brief This class controls the creation of a WMSLayer
			\ingroup ogc
		*/
		class OGCEXPORT WmsLayerController
		{
		public:
			WmsLayerController(void);
			~WmsLayerController(void);

			int ConnectToServer(const std::string &url);

			void GetInformation(std::string &info);

			void GetLayers(std::vector<std::string> &layers);

			void GetLayers(Node *layerTree);

			void GetImageFormats(std::vector<std::string> &formats);

			bool GetMap(const std::string &layer_name, int width, int height, float latmin, float lonmin,
						float latmax, float lonmax, std::string &filename, const std::string &image_format = "image/png", 
						const std::string &version = "1.0.0", 
						const std::string &srs = "EPSG:32628");
						//const std::string &srs = "EPSG:23030"); 
						//

			bool GetMap(const WmsLayer &layer, int width, int height, float latmin, float lonmin,
						float latmax, float lonmax, std::string &filename, 
						const std::string &srs = "EPSG:32628");
						//const std::string &srs = "EPSG:23030");  
						//

			wmsRefPtr<wmsCapabilitiesRoot> &GetRoot() {return wms_root;}

			void GetSRS(std::vector<std::string> &formats);

			std::string GetComposedURL() {return cu;}
			std::string GetMapURL(const WmsLayer &layer, int width, int height, float latmin, float lonmin,
								float latmax, float lonmax, const std::string &srs = "EPSG:32628") ;

			void CreateLayer(const std::string &layer_name, const std::string &format, WmsLayer &layer);

		private:

			void AddChildrenNodes(Node *layerTree, std::vector<wmsRefPtr<wmsCapabilitiesState> > &children);

			wmsRefPtr<wmsCapabilitiesRoot> wms_root;
			wmsUrl wms_url;

			std::string cu;

		};

	}

}

std::ostream &operator <<(std::ostream &s, const cpw::ogc::Folder &folder);


#endif


