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

#ifndef _PUBLISHEDTREE_
#define _PUBLISHEDTREE_

#include <string>
#include <vector>

#include <cpw/common/TypeId.h>

/*!
 *  \file PublishedTree.h
 */

namespace cpw 
{ 
  namespace remote
  {
    /*!
     *  \class PublishedNode PublishedTree.h <remote/utils/PublishedTree.h>
     *  \brief This is the base class for the PublishedTree structure
     *  \ingroup remote
     *
     *  This is the base class for the PublishedTree structure.
     *
     *  This structure is used to deal with GetPublished messages. It provides the means to create
     *  a tree with each node having a string to be presented to the user (name) and a TypeId
     *  identifying the corresponding Entity.
     */
    class PublishedNode
    {
    public:
      virtual ~PublishedNode() {};

      /*!
       *  \brief Returns the string to be presented to the user
       *
       *  Returns the string to be presented to the user.
       */
      std::string &GetName() { return name; }

      /*!
       *  \brief Returns identifier of the entity
       *
       *  Returns identifier of the entity.
       */
      cpw::TypeId &GetId() { return id; }



      /*!
       *  \brief Returns the string to be presented to the user
       *
       *  Returns the string to be presented to the user.
       */
      const std::string &GetName() const { return name; }

      /*!
       *  \brief Returns identifier of the entity
       *
       *  Returns identifier of the entity.
       */
      const cpw::TypeId &GetId() const { return id; }



      /*!
       *  \brief Sets the string to be presented to the user
       *  \param name The string to be used
       *
       *  Sets the string to be presented to the user.
       */
      void SetName(std::string name) { this->name = name; }

      /*!
       *  \brief Sets the identifier of the entity
       *  \param id The id to be used
       *
       *  Sets the identifier of the entity.
       */
      void SetId(cpw::TypeId id) { this->id = id; }



      /*!
       *  \brief Tells whether the object is a PublishedLeaf or not
       *
       *  Tells whether the object is a PublishedLeaf or not.
       */
      virtual bool IsLeaf() = 0;

      /*!
       *  \brief Tells whether the object is a PublishedLeaf or not
       *
       *  Tells whether the object is a PublishedLeaf or not.
       */
      virtual const bool IsLeaf() const = 0;

    protected:
      std::string name;
      cpw::TypeId id;
    };







    /*!
     *  \class PublishedFolder PublishedTree.h <remote/utils/PublishedTree.h>
     *  \brief This class implements a node in the PublishedTree which contains other nodes
     *  \ingroup remote
     *
     *  This class implements a node in the PublishedTree which contains other nodes.
     */
    class PublishedFolder: public PublishedNode
    {
    public:
      /*!
       *  \param name The string to be presented to the user
       *  \param id The identifier of the entity
       */
      PublishedFolder(const std::string &name = std::string(), const cpw::TypeId &id = cpw::TypeId())
	{ this->name = name; this->id = id; }

      virtual ~PublishedFolder()
	{ 
	  for(std::vector<PublishedNode *>::iterator i = nodes.begin(); i != nodes.end(); i++)
	    if (*i)
	      {
		delete *i;
		*i = NULL;
	      }
	}



      /*!
       *  \brief Tells whether the object is a PublishedLeaf or not
       *
       *  Tells whether the object is a PublishedLeaf or not.
       */
      virtual bool IsLeaf() { return false; }

      /*!
       *  \brief Tells whether the object is a PublishedLeaf or not
       *
       *  Tells whether the object is a PublishedLeaf or not.
       */
      virtual const bool IsLeaf() const { return false; }



      /*!
       *  \brief Appends a new child node
       *  \param node The node to be appended
       *
       *  Appends a new child node.
       */
      bool Add(PublishedNode *node) { nodes.push_back(node); return 0;}

      /*!
       *  \brief Erases al children nodes
       *
       *  Erases al children nodes.
       */
      void DetachChildren() { nodes.clear(); }



      /*!
       *  \brief Returns the list of children nodes
       *
       *  Returns the list of children nodes.
       */
      virtual std::vector<PublishedNode *> &GetNodes() { return nodes; }

      /*!
       *  \brief Returns the list of children nodes
       *
       *  Returns the list of children nodes.
       */
      virtual const std::vector<PublishedNode *> &GetNodes() const { return nodes; }

    private:
      std::vector<PublishedNode *> nodes;
    };







    /*!
     *  \class PublishedLeaf PublishedTree.h <remote/utils/PublishedTree.h>
     *  \brief This class is a functor implementing a callback
     *  \ingroup remote
     *
     *  This class is a functor implementing a callback.
     *
     */
    class PublishedLeaf: public PublishedNode
    {
    public:
      /*!
       *  \param name The string to be presented to the user
       *  \param id The identifier of the entity
       */
      PublishedLeaf(const std::string &name = std::string(), const cpw::TypeId &id = cpw::TypeId())
	{ this->name = name; this->id = id; }


      /*!
       *  \brief Tells whether the object is a PublishedLeaf or not
       *
       *  Tells whether the object is a PublishedLeaf or not.
       */
      virtual bool IsLeaf() { return true; }

      /*!
       *  \brief Tells whether the object is a PublishedLeaf or not
       *
       *  Tells whether the object is a PublishedLeaf or not.
       */
      virtual const bool IsLeaf() const { return true; }
    };
  }

}

#endif
