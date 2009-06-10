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

#include <sstream>

#include <iosg/stdafx.h>

#include <iosg/gui/OsgWidget.h>
#include <iosg/OsgObjectRegistrySingleton.h>

#include <cpw/common/ApplicationLog.h>



using namespace cpw::iosg; 

unsigned int OsgIWidget::id=0;


OsgIWidget::OsgIWidget(const std::string &url, const cpw::Point3d<float> &_position, const cpw::Point3d<float> &_size, const cpw::Point3d<float> &_rotation, const TAnchor &_anchor) : object(NULL), visible(true)
{
  SetDefaultPath(url);

  unsigned int new_id = RequestId();
  std::ostringstream temp;
  temp << new_id;
  id_string = temp.str();
	
  osg::Matrix m;
  m.makeIdentity();
  mt = new osg::MatrixTransform();
  mt->setMatrix(m);

  anchor.x = _anchor.x;
  anchor.y = _anchor.y;
  anchor.w = _anchor.w;
  anchor.h = _anchor.h;
  ResizeScreen(20, 20);
  SetPosition(_position);
  SetSize(_size);
  scale.z = scale.y = scale.x = 1.0;
  SetRotation(_rotation);


  UpdateTransformationMatrix();
}

OsgIWidget::~OsgIWidget(void)
{
}

void OsgIWidget::SetObject(const std::string &filename)
{
  if (object == NULL)
    {
      std::vector<osg::MatrixTransform *> mt_vec;
		
      //      osg::ref_ptr<osg::Node> object;

		
      //if (obj_reg == NULL)
      //	object = osgDB::readNodeFile(filename);
      //else
      OsgObjectRegistrySingleton *instance =
	OsgObjectRegistrySingleton::GetInstance();
      if (instance) {
	OsgObjectRegistry *registry =
	  instance->GetObjReg();
	if (registry) {
	  osg::Node *node =
	    registry->GetObjectFromFile(filename);
	  if (node) {
	    object = (osg::Node *) node->clone(osg::CopyOp::DEEP_COPY_ALL);
	  }
	}
      }


      if (object == NULL)
	{	//ERROR, hay que controlar esto
	  return;
	}

      osg::StateSet* stateset = object->getOrCreateStateSet();
      stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
      object->setStateSet(stateset);

      OsgVisitorFirstGeode vfg;
      vfg.apply(*(object.get()));

      if (vfg.GetFoundGeode() == NULL)
	{	//ERROR, hay que controlar esto
	  return;
	}

      osg::Geode *geode = vfg.GetFoundGeode();

      //grab the transformation
      osg::Node *temp_node = geode;

      while (temp_node->getNumParents() != 0)
	{
	  temp_node = temp_node->getParent(0);
	  std::string pname = temp_node->className();
	  if (temp_node->className() == std::string("MatrixTransform"))
	    mt_vec.push_back((osg::MatrixTransform *) temp_node);
	}

      osg::Matrix tt_matrix;
      tt_matrix.makeIdentity();

      for (std::vector<osg::MatrixTransform *>::iterator i = mt_vec.begin();
	   i != mt_vec.end();
	   i++)
	{
	  tt_matrix = tt_matrix * (*i)->getMatrix();
	}

      osg::BoundingBox bb = geode->getBoundingBox();
		
      x_max = bb.xMax();
      x_min = bb.xMin();
      y_max = bb.yMax();
      y_min = bb.yMin();

      osg::Vec3 min(bb.xMin(), bb.yMin(), bb.zMin());
      osg::Vec3 max(bb.xMax(), bb.yMax(), bb.zMax());
      min = min * tt_matrix;
      max = max * tt_matrix;

      cpw::Point3d<float> new_scale;
      float xsize = max.x() - min.x();
      float ysize = max.y() - min.y();
      float zsize = max.z() - min.z();
      new_scale.x = GetSize().x / xsize;
      new_scale.y = GetSize().y / ysize;
      new_scale.z = GetSize().z / zsize;
      SetScale(new_scale);

      sw = new osg::Switch();
      sw->setAllChildrenOn();
      sw->addChild(object.get());
      GetMatrixTransform()->addChild(sw.get());

      MarkTree(id_string, GetMatrixTransform());
		
      UpdateTransformationMatrix();
    }
}


void OsgIWidget::Move(const int &x, const int &y, const int &z)
{
}

void OsgIWidget::Rotate(const int &x, const int &y, const int&z)
{
}

void OsgIWidget::Scale(const int &x, const int &y, const int &z)
{
}

void OsgIWidget::Update(osg::Matrix *mp)
{
  //aqui es donde llega con la matriz propagada, no hace nada :
  UpdateTransformationMatrix(mp);
		
}

void OsgIWidget::UpdateTransformationMatrix(osg::Matrix *mp)
{
  osg::Matrix m = mt->getMatrix();
  osg::Matrix m_pos, m_scale, m_rotx, m_roty, m_rotz;
	
  m_rotx.makeRotate(rot.x, osg::Vec3(1.0, 0.0, 0.0));
  m_roty.makeRotate(rot.y, osg::Vec3(0.0, 1.0, 0.0));
  m_rotz.makeRotate(rot.z, osg::Vec3(0.0, 0.0, 1.0));
  m_scale.makeScale(osg::Vec3(scale.x, scale.y, scale.z));
  m_pos.makeTranslate(osg::Vec3(absolute_position.x, absolute_position.y, absolute_position.z));
  mt->setMatrix(m_scale * m_rotx * m_roty * m_rotz * m_pos);

  if (mp != NULL)
    {
      //mt->setMatrix(mt->getMatrix() * (*mp));
      osg::Matrix final_matrix = m_scale * m_rotx * m_roty * m_rotz * m_pos * (*mp);
      osg::Vec3 trans = final_matrix.getTrans();
      osg::Vec3 scale = final_matrix.getScale();
      //osg::Vec3 rotate = final_matrix.getR

      mt->setMatrix(final_matrix);
      //mt->setMatrix(*mp);
    }
  /*
    std::ostringstream debug_message;
    debug_message << "ap " << id_string << ": " <<  absolute_position.x << ", " << absolute_position.y;
    //cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(debug_message.str());
    */
}

void OsgIWidget::SetPosition(const cpw::Point3d<float> &_position)
{ 
  position = _position; 

  absolute_position.x = (screen_x_size * anchor.w) + anchor.x + position.x;
  absolute_position.y = (screen_y_size * anchor.h) + anchor.y + position.y;
  absolute_position.z = position.z;
	
  UpdateTransformationMatrix();
}
void OsgIWidget::SetSize(const cpw::Point3d<float> &_size)
{ 
	
  cpw::Point3d<float> percent = _size;
  percent.x /= size.x;
  percent.y /= size.y;
  percent.z /= size.z;
  size = _size;
	
  //if (object != NULL)
  //{
  scale.x *= percent.x;
  scale.y *= percent.y;
  scale.z *= percent.z;
  ////scale.x = size.x / (radius*2); 
  ////scale.y = size.y / (radius*2); 
  ////scale.z = size.z / (radius*2); 
  //}
  UpdateTransformationMatrix();
}
void OsgIWidget::SetRotation(const cpw::Point3d<float> &_rot)					   
{ 
  rot = _rot;
  UpdateTransformationMatrix();
}
			

void OsgIWidget::ResizeScreen(const int &x, const int &y)
{
  screen_x_size = x;
  screen_y_size = y;

  SetPosition(position);
}

void OsgIWidget::SetAnchor(const float &w, const float &h, const int &offset_x, const int &offset_y)
{
  anchor.w = w;
  anchor.h = h;
  anchor.x = offset_x;
  anchor.y = offset_y;
  SetPosition(position);
  //UpdateTransformationMatrix();
}

void OsgIWidget::RotateX(const float &angle)
{
  rot.x += angle;
  UpdateTransformationMatrix();
}
void OsgIWidget::RotateY(const float &angle)
{
  rot.y += angle;
  UpdateTransformationMatrix();
}
void OsgIWidget::RotateZ(const float &angle)
{
  rot.z += angle;
  UpdateTransformationMatrix();
}


void OsgIWidget::MarkTree(const std::string &str, osg::Node *node)
{
  node->setName(str);

  osg::Group *group = dynamic_cast<osg::Group *>(node);

  if (group != NULL)
    {
      for (unsigned int i=0; i<group->getNumChildren(); i++)
	MarkTree(str, group->getChild(i));
    }
  else if (node->className() == std::string("Geode"))
    {
      osg::Geode *geode = (osg::Geode *) node;
      unsigned int drawable_count = geode->getNumDrawables();

      for (unsigned int i=0; i<drawable_count; i++)
	{
	  geode->getDrawable(i)->setName(str);
	}
    }	
}

void OsgIWidget::GetBoundingBox(float &xmax, float &xmin, float &ymax, float &ymin)
{
  xmax = x_max;
  xmin = x_min;

  ymax = y_max;
  ymin = y_min;
}

void OsgIWidget::SetVisible(const bool &_visible)
{ 
  visible = _visible;

  if (sw != NULL)
    {
      if (visible)
	sw->setAllChildrenOn();
      else
	sw->setAllChildrenOff();
    }
		
}
