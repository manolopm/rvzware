/* -*-c++-*- libwms - Copyright (C) since 2004 Garrett Potts 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the libwms Public License (WMSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * libwms Public License for more details.
 */
#ifndef wcsRefPtr_HEADER
#define wcsRefPtr_HEADER
#include <wcs/wcsExport.h>

template<class T>
class wcsRefPtr
{
public:
  typedef T element_type;
   
  wcsRefPtr() :thePtr(0L) {}
  wcsRefPtr(T* t):thePtr(t)              { if (thePtr) thePtr->ref(); }
  wcsRefPtr(const wcsRefPtr& rp):thePtr(rp.thePtr)  { if (thePtr) thePtr->ref(); }
  ~wcsRefPtr()                           { if (thePtr) thePtr->unref(); thePtr=0; }
   
  inline wcsRefPtr& operator = (const wcsRefPtr& rp)
  {
    if (thePtr==rp.thePtr) return *this;
    T* tmpPtr = thePtr;
    thePtr = rp.thePtr;
    if (thePtr) thePtr->ref();
    // unref second to prevent any deletion of any object which might
    // be referenced by the other object. i.e rp is child of the
    // original _ptr.
    if (tmpPtr) tmpPtr->unref();
    return *this;
  }
   
  inline wcsRefPtr& operator = (T* ptr)
  {
    if (thePtr==ptr) return *this;
    T* tmpPtr = thePtr;
    thePtr = ptr;
    if (thePtr) thePtr->ref();
    // unref second to prevent any deletion of any object which might
    // be referenced by the other object. i.e rp is child of the
    // original thePtr.
    if (tmpPtr) tmpPtr->unref();
    return *this;
  }
   
  // comparison operators for wcsRefPtr.
  inline bool operator == (const wcsRefPtr& rp) const { return (thePtr==rp.thePtr); }
  inline bool operator != (const wcsRefPtr& rp) const { return (thePtr!=rp.thePtr); }
  inline bool operator < (const wcsRefPtr& rp) const { return (thePtr<rp.thePtr); }
  inline bool operator > (const wcsRefPtr& rp) const { return (thePtr>rp.thePtr); }
   
  // comparion operator for const T*.
  inline bool operator == (const T* ptr) const { return (thePtr==ptr); }
  inline bool operator != (const T* ptr) const { return (thePtr!=ptr); }
  inline bool operator < (const T* ptr) const { return (thePtr<ptr); }
  inline bool operator > (const T* ptr) const { return (thePtr>ptr); }
   
   
  inline T& operator*()  { return *thePtr; }
   
  inline const T& operator*() const { return *thePtr; }
   
  inline T* operator->() { return thePtr; }
   
  inline const T* operator->() const   { return thePtr; }
   
  inline bool operator!() const	{ return thePtr==0L; }
   
  inline bool valid() const	{ return thePtr!=0L; }
   
  inline T* get() { return thePtr; }
   
  inline const T* get() const { return thePtr; }
   
  /** take control over the object pointed to by ref_ptr, unreference but do not delete even if ref count goes to 0,
   * return the pointer to the object.
   * Note, do not use this unless you are 100% sure your code handles the deletion of the object correctly, and
   * only use when absolutely required.*/
  inline T* take() { return release();}
   
  inline T* release() { T* tmp=thePtr; if (thePtr) thePtr->unref_nodelete(); thePtr=0; return tmp;}
   
private:
  T* thePtr;
};

#endif
