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

#ifndef wcsReferenced_HEADER
#define wcsReferenced_HEADER
#include <wcs/wcsExport.h>

class WCS_EXPORT wcsReferenced
{
 public:
  wcsReferenced() 
    {
      theRefCount=0;
    }
  wcsReferenced(const wcsReferenced&)
    {
      theRefCount=0;
    }
   
  inline wcsReferenced& operator = (const wcsReferenced&) { return *this; }
   

  /*! increment the reference count by one, indicating that 
    this object has another pointer which is referencing it.*/
  inline void ref() const { ++theRefCount; }
   
  /*! decrement the reference count by one, indicating that 
    a pointer to this object is referencing it.  If the
    reference count goes to zero, it is assumed that this object
    is no longer referenced and is automatically deleted.*/
  inline void unref() const;
   
  /*! decrement the reference count by one, indicating that 
    a pointer to this object is referencing it.  However, do
    not delete it, even if ref count goes to 0.  Warning, unref_nodelete() 
    should only be called if the user knows exactly who will
    be resonsible for, one should prefer unref() over unref_nodelete() 
    as the later can lead to memory leaks.*/
  inline void unref_nodelete() const { --theRefCount; }
   
  /*! return the number pointers currently referencing this object. */
  inline int referenceCount() const { return theRefCount; }
   
   
 protected:
  virtual ~wcsReferenced();
  mutable int theRefCount;
};

inline void wcsReferenced::unref() const
{
  --theRefCount;
  if (theRefCount==0)
    {
      if (this)
	{
	  delete this;
	}
    }
}

#endif
