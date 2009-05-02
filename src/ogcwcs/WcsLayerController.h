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

#ifndef _WCSLAYERCONTROLLER_
#define _WCSLAYERCONTROLLER_

#include <wcs/wcsClient.h>
#include <wcs/wcsCapabilitiesRoot.h>

#include <ogcwcs/WcsLayer.h>
#include <ogcwcs/Export.h>

#include <vector>
#include <string>
#include <iostream>


namespace cpw 
{
  namespace ogcwcs
  {

    /** \brief Enumeration for the node types 
	\ingroup ogcwcs
    */
    enum NodeType {

      LEAF = 0,
      FOLDER = 1
    };

    /** \brief A basic node
	\ingroup ogcwcs
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
	\ingroup ogcwcs
    */
    class Folder: public Node {

    public:
    Folder(std::string nam = std::string(), std::string tit = std::string()): 
      Node(FOLDER, nam, tit){}

      virtual ~Folder() { 

	for(std::vector<Node *>::iterator i = nodes.begin(); i != nodes.end(); i++)
				  
	  if (*i)
	    {
	      delete *i;
	      *i= NULL;
	    }
      }

      int Add(Node *node) { nodes.push_back(node); return 0;}

      virtual std::vector<Node *> GetNodes(){return nodes;}
      virtual const std::vector<Node *> GetNodes() const {return nodes;}


    private:

      std::vector<Node *> nodes;

    };


    /** \brief A leaf node 
	\ingroup ogcwcs
    */
    class Leaf: public Node {

    public:
    Leaf(std::string nam = std::string(), std::string tit = std::string()): 
      Node(LEAF, nam, tit){}

    };



    /** \brief This class controls the creation of a WCSLayer
	\ingroup ogcwcs
    */
    class OGCWCSEXPORT WcsLayerController
    {
    public:
      WcsLayerController(void);
      ~WcsLayerController(void);

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

      bool GetMap(const WcsLayer &layer, int width, int height, float latmin, float lonmin,
		  float latmax, float lonmax, std::string &filename, 
		  const std::string &srs = "EPSG:32628");
      //const std::string &srs = "EPSG:23030");  
      //

      bool DescribeCoverage(const std::string &folder);

      wcsRefPtr<wcsCapabilitiesRoot> &GetRoot() {return wcs_root;}

      void GetSRS(std::vector<std::string> &formats);

      std::vector<std::string> WCS_CRS(const std::string &formats);

      std::string GetComposedURL() {return cu;}
      std::string GetMapURL(const WcsLayer &layer, int width, int height, float latmin, float lonmin,
			    float latmax, float lonmax, const std::string &srs = "EPSG:32628") ;

      void CreateLayer(const std::string &layer_name, const std::string &format, WcsLayer &layer);

    private:

      void AddChildrenNodes(Node *layerTree, std::vector<wcsRefPtr<wcsCapabilitiesState> > &children);
      void ProcessDescribeCoverage(const std::string &folder, std::string &str1);

      wcsRefPtr<wcsCapabilitiesRoot> wcs_root;
      //wcsRefPtr<wcsDescribeRoot> wcs_desc_root;
      wcsUrl wcs_url;

      std::string cu;

      std::map<std::string, std::vector<std::string> > wcs_crs;

    };

  }

}

std::ostream &operator <<(std::ostream &s, const cpw::ogcwcs::Folder &folder);


#endif
