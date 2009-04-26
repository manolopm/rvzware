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

#include <osg/Drawable>
#include <sstream>

#include <iosg/stdafx.h>

#include <iosg/OsgLine2Callback.h>
#include <cpw/common/ApplicationLog.h>
#include <cpw/common/Math.h>

using namespace cpw::iosg;

OsgLine2Callback::OsgLine2Callback(const std::string &defaultpath) : active(false), need_to_update(false), curve_adapter(NULL), orig(), geom(NULL), defaultpath_(defaultpath)
{
	brothers[0] = NULL;
	brothers[1] = NULL;
	width = 4;
	height = 20;
	repeat_img = 2;
}

OsgLine2Callback::~OsgLine2Callback(void)
{
}


void OsgLine2Callback::operator ()(osg::Node *node, osg::NodeVisitor *nv)
{
	
		
	if (active && !need_to_update)
	{
		//return;
	}
	else
	{
		#ifdef _DEBUG
		//std::ostringstream msg;
		//msg <<  << ;
		//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(msg.str());
		#endif

		active = true;
		need_to_update = false;
		
		SetBrothersToInactive();

		AdaptVerticesToTerrain();
	}

	osg::NodeCallback::operator ()(node, nv);
}

void OsgLine2Callback::SetBrothersToInactive()
{
	//left brothers
	OsgLine2Callback *aux = NULL;
	
	if (brothers[0] != NULL)
	{
		aux = GetLeftBrother();
	
		while (aux != NULL)
		{
			aux->active = false;
			aux = aux->GetLeftBrother();
		}
	}

	//right brothers
	if (brothers[1] != NULL)
	{
		aux = GetRightBrother();

		while (aux != NULL)
		{
			aux->active = false;
			aux = aux->GetRightBrother();
		}
	}
}

void OsgLine2Callback::AdaptVerticesToTerrain()
{
	//exit preconditions
	if (curve_adapter == NULL)
		return;

	osg::Vec3Array* vertex = (osg::Vec3Array*)geom->getVertexArray();

	if (vertex->empty())
		return;

	temp_vec1.clear();

	//clean the vertices of osg
	vertex->clear();

	//adapt the new vertices
	float aux = curve_adapter->GetStepDistance();
	curve_adapter->SetStepDistance( curve_adapter_step_distance );
	curve_adapter->Adapt(orig, temp_vec1);
	curve_adapter->SetStepDistance( aux );

///
	std::vector< cpw::Point3d<float> > new_vertex;
	std::vector< cpw::Point3d<float> > new_normal;
	std::vector< cpw::Point3d<float> >::iterator iter = temp_vec1.begin();
	for(;iter != temp_vec1.end()-1;iter++)
	{
		cpw::Point3d<float> vaux1, vaux2;
		
		vaux2.x = 0;
		vaux2.y = 0;
		vaux2.z = 1;

		vaux1.x = (iter+1)->x - iter->x;
		vaux1.y = (iter+1)->y - iter->y;
		vaux1.z = (iter+1)->z - iter->z;

		cpw::Math::NormalizeVec(vaux1);

		cpw::Point3d<float> vcross = cpw::Math::CrossProduct(vaux1, vaux2);
		cpw::Math::NormalizeVec(vcross);
		int t = -width;
		cpw::Point3d<float> a,b;
		a.x = iter->x + vcross.x*t;
		a.y = iter->y + vcross.y*t;
		a.z = iter->z + vcross.z*t;

		t=width;
		b.x = iter->x + vcross.x*t;
		b.y = iter->y + vcross.y*t;
		b.z = iter->z + vcross.z*t;

		cpw::Point3d<float> v1,v2,v3,v4;

		v1 = a;
		v1.z -= height/2;
		v2 = b;
		v2.z -= height/2;
		v3 = b;
		v3.z += height;
		v4 = a;
		v4.z += height;

		new_vertex.push_back(v1);
		new_vertex.push_back(v2);
		new_vertex.push_back(v3);
		new_vertex.push_back(v3);
		new_vertex.push_back(v4);
		new_vertex.push_back(v1);

		cpw::Plane plane;
		plane.MakeFrom3Points(v1, v2, v3);

		cpw::Point3d<float> normal;
		normal.x = plane.GetX();
		normal.y = plane.GetY();
		normal.z = plane.GetZ();
		cpw::Math::NormalizeVec(normal);
		new_normal.push_back(normal);		
		new_normal.push_back(normal);		
		new_normal.push_back(normal);		
		new_normal.push_back(normal);		
		new_normal.push_back(normal);		
		new_normal.push_back(normal);		


		if(iter+2 != temp_vec1.end())
		{
			cpw::Point3d<float> vaux3, vaux4;
			
			vaux4.x = 0;
			vaux4.y = 0;
			vaux4.z = 1;

			vaux3.x = (iter+2)->x - (iter+1)->x;
			vaux3.y = (iter+2)->y - (iter+1)->y;
			vaux3.z = (iter+2)->z - (iter+1)->z;

			cpw::Math::NormalizeVec(vaux3);

			cpw::Point3d<float> vcross2 = cpw::Math::CrossProduct(vaux3, vaux4);
			cpw::Math::NormalizeVec(vcross2);
			int t2 = -width;
			cpw::Point3d<float> c,d;
			c.x = (iter+1)->x + vcross2.x*t2;
			c.y = (iter+1)->y + vcross2.y*t2;
			c.z = (iter+1)->z + vcross2.z*t2;

			t2=width;
			d.x = (iter+1)->x + vcross2.x*t2;
			d.y = (iter+1)->y + vcross2.y*t2;
			d.z = (iter+1)->z + vcross2.z*t2;

			cpw::Point3d<float> v1_,v2_,v3_,v4_;

			v1_ = c;
			v1_.z -= height/2;
			v2_ = d;
			v2_.z -= height/2;
			v3_ = d;
			v3_.z += height;
			v4_ = c;
			v4_.z += height;


			new_vertex.push_back(v2);
			new_vertex.push_back(v2_);
			new_vertex.push_back(v3_);
			new_vertex.push_back(v3_);
			new_vertex.push_back(v3);
			new_vertex.push_back(v2);
			///
			{
			cpw::Plane plane;
			plane.MakeFrom3Points(v2, v2_, v3_);

			cpw::Point3d<float> normal;
			normal.x = plane.GetX();
			normal.y = plane.GetY();
			normal.z = plane.GetZ();
			cpw::Math::NormalizeVec(normal);
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);
			}
			///

			new_vertex.push_back(v3);
			new_vertex.push_back(v3_);
			new_vertex.push_back(v4_);
			new_vertex.push_back(v4_);
			new_vertex.push_back(v4);
			new_vertex.push_back(v3);	
			///
			{
			cpw::Plane plane;
			plane.MakeFrom3Points(v1, v2, v3);

			cpw::Point3d<float> normal;
			normal.x = plane.GetX();
			normal.y = plane.GetY();
			normal.z = plane.GetZ();
			cpw::Math::NormalizeVec(normal);
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);
			}
			///

			new_vertex.push_back(v4_);
			new_vertex.push_back(v1_);
			new_vertex.push_back(v1);
			new_vertex.push_back(v1);
			new_vertex.push_back(v4);
			new_vertex.push_back(v4_);
			///
			{
			cpw::Plane plane;
			plane.MakeFrom3Points(v4, v1_, v1);

			cpw::Point3d<float> normal;
			normal.x = plane.GetX();
			normal.y = plane.GetY();
			normal.z = plane.GetZ();
			cpw::Math::NormalizeVec(normal);
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);
			}
			///
		}
		else
		{
			cpw::Point3d<float> vaux1, vaux2;
			
			vaux2.x = 0;
			vaux2.y = 0;
			vaux2.z = 1;

			vaux1.x = iter->x - (iter+1)->x;
			vaux1.y = iter->y - (iter+1)->y ;
			vaux1.z = iter->z - (iter+1)->z ;

			cpw::Math::NormalizeVec(vaux1);

			cpw::Point3d<float> vcross = cpw::Math::CrossProduct(vaux2, vaux1);
			cpw::Math::NormalizeVec(vcross);
			int t = -width;
			cpw::Point3d<float> a,b;
			a.x = (iter+1)->x + vcross.x*t;
			a.y = (iter+1)->y + vcross.y*t;
			a.z = (iter+1)->z + vcross.z*t;

			t=width;
			b.x = (iter+1)->x + vcross.x*t;
			b.y = (iter+1)->y + vcross.y*t;
			b.z = (iter+1)->z + vcross.z*t;

			cpw::Point3d<float> _v1,_v2,_v3,_v4;

			_v1 = a;
			_v1.z -= height/2;
			_v2 = b;
			_v2.z -= height/2;
			_v3 = b;
			_v3.z += height;
			_v4 = a;
			_v4.z += height;


			new_vertex.push_back(v2);
			new_vertex.push_back(_v2);
			new_vertex.push_back(_v3);
			new_vertex.push_back(_v3);
			new_vertex.push_back(v3);
			new_vertex.push_back(v2);
			///
			{
			cpw::Plane plane;
			plane.MakeFrom3Points(v2, _v2, _v3);

			cpw::Point3d<float> normal;
			normal.x = plane.GetX();
			normal.y = plane.GetY();
			normal.z = plane.GetZ();
			cpw::Math::NormalizeVec(normal);
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);
			}
			///


			new_vertex.push_back(v3);
			new_vertex.push_back(_v3);
			new_vertex.push_back(_v4);
			new_vertex.push_back(_v4);
			new_vertex.push_back(v4);
			new_vertex.push_back(v3);
			///
			{
			cpw::Plane plane;
			plane.MakeFrom3Points(v3, _v3, _v4);

			cpw::Point3d<float> normal;
			normal.x = plane.GetX();
			normal.y = plane.GetY();
			normal.z = plane.GetZ();
			cpw::Math::NormalizeVec(normal);
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);
			}
			///

	
			new_vertex.push_back(_v4);
			new_vertex.push_back(_v1);
			new_vertex.push_back(v1);
			new_vertex.push_back(v1);
			new_vertex.push_back(v4);
			new_vertex.push_back(_v4);
			///
			{
			cpw::Plane plane;
			plane.MakeFrom3Points(_v4, _v1, v1);

			cpw::Point3d<float> normal;
			normal.x = plane.GetX();
			normal.y = plane.GetY();
			normal.z = plane.GetZ();
			cpw::Math::NormalizeVec(normal);
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);
			}
			///

		
			new_vertex.push_back(_v3);
			new_vertex.push_back(_v2);
			new_vertex.push_back(_v1);
			new_vertex.push_back(_v1);
			new_vertex.push_back(_v4);
			new_vertex.push_back(_v3);
			///
			{
			cpw::Plane plane;
			plane.MakeFrom3Points(_v3, _v2, _v1);

			cpw::Point3d<float> normal;
			normal.x = plane.GetX();
			normal.y = plane.GetY();
			normal.z = plane.GetZ();
			cpw::Math::NormalizeVec(normal);
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);		
			new_normal.push_back(normal);
			}
			///

		}
	}	

///
	geom->removePrimitiveSet(0,1);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES ,0,new_vertex.size()));	
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	osg::Vec3Array* normal_array = (osg::Vec3Array*)geom->getNormalArray();
	if(normal_array == NULL) normal_array = new osg::Vec3Array();
	normal_array->clear();
	
	std::vector< cpw::Point3d<float> >::iterator normal_iter = new_normal.begin();

	//fill again the osg vec
	for (std::vector< cpw::Point3d<float> >::iterator iter = new_vertex.begin();
		 iter != new_vertex.end();
		 iter++, normal_iter++)
	{
		vertex->push_back( osg::Vec3(iter->x, iter->y, iter->z) );	
		normal_array->push_back(osg::Vec3(normal_iter->x, normal_iter->y, normal_iter->z));
		//normal_array->push_back(osg::Vec3(0, 0, 1));		
	}	
	geom->setNormalArray(normal_array);

	RepeatUV();
}

void OsgLine2Callback::SetGeometry(osg::Geometry* g) 
{ 
	geom = g; 

	osg::Vec3Array* vertex = (osg::Vec3Array*)geom->getVertexArray();

	if (vertex->empty())
		return;

	for (int i=0; i<(signed)vertex->size(); i++)
		orig.push_back( cpw::Point3d<float>((*vertex)[i].x(), (*vertex)[i].y(), (*vertex)[i].z()) );

}

void OsgLine2Callback::SetNewInitialVertex(const std::vector<cpw::Point3d<float> > &draw_array)
{
	if(!orig.empty())
		orig.clear();
	for (int i=0; i<(signed)draw_array.size(); i++)
		orig.push_back(draw_array[i]);//.x(), draw_array[i].y(), draw_array[i].z()) );

	//if(active)
	//	AdaptVerticesToTerrain();
}

void OsgLine2Callback::RepeatUV()
{
		osg::ref_ptr<osg::Vec2Array> texcoords;
		unsigned int nvertices = geom->getVertexArray()->getNumElements();
		//osg::Vec3Array* vertex = (osg::Vec3Array*)geom->getVertexArray();
		texcoords = new osg::Vec2Array(nvertices);

		osg::ref_ptr<osg::StateSet> stateset = geom->getOrCreateStateSet();
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

		//the texture
		//std::string default_directory = "C:\\OSG\\svn-itc\\trunk\\src\\geviemer\\config\\default";
		std::string icon_path = defaultpath_;//default_directory;
		icon_path += "/brick_wall_tile.png";

		osg::ref_ptr<osg::Image> dummy_image = osgDB::readImageFile(icon_path.c_str());
		osg::ref_ptr<osg::Texture2D> dummy_tex = new osg::Texture2D;
		dummy_tex.get()->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
		dummy_tex.get()->setImage(dummy_image.get());
		dummy_tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		dummy_tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
int j =0;
bool incr=false;
		for (unsigned int i=0; i <= texcoords->size()-6; i+=6)
		{
			float x = 0.0f;
			float y = 0.0f;

			if((((i-j) % (18)) != 0) || i==0)
			{
				x = 0;
				y = repeat_img;
				(*texcoords.get())[i].set(x, y);

				x = repeat_img;
				y = repeat_img;
				(*texcoords.get())[i+1].set(x, y);

				x = repeat_img;
				y = 0;
				(*texcoords.get())[i+2].set(x, y);

				x = repeat_img;
				y = 0;
				(*texcoords.get())[i+3].set(x, y);

				x = 0;
				y = 0;
				(*texcoords.get())[i+4].set(x, y);

				x = 0;
				y = repeat_img;
				(*texcoords.get())[i+5].set(x, y);

				if(incr) j+=6;
				incr = false;
			}
			else
			{
				incr=true;
				x = 0;
				y = 0;
				(*texcoords.get())[i].set(x, y);

				x = 0;
				y = repeat_img;
				(*texcoords.get())[i+1].set(x, y);

				x = repeat_img;
				y = repeat_img;
				(*texcoords.get())[i+2].set(x, y);

				x = repeat_img;
				y = repeat_img;
				(*texcoords.get())[i+3].set(x, y);

				x = repeat_img;
				y = 0;
				(*texcoords.get())[i+4].set(x, y);

				x = 0;
				y = 0;
				(*texcoords.get())[i+5].set(x, y);
			}
		}

		geom->setTexCoordArray(0, texcoords.get());
		stateset.get()->setTextureAttributeAndModes(0,&(*dummy_tex),osg::StateAttribute::ON);
}
