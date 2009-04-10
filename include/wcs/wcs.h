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
#ifndef wcs_HEADER
#define wcs_HEADER
#include <string>
#include "wcsExport.h"

/**
 *  \defgroup libwcs Libwcs library
 */


WCS_EXPORT std::string wcsUpcase(const std::string& input);

#ifdef __cplusplus
extern "C"
{
#endif
WCS_EXPORT bool wcsStringContains(const std::string& srcStr, const std::string& searchStr);
WCS_EXPORT void wcsInitialize();
WCS_EXPORT void wcsFinalize();
#ifdef __cplusplus
}
#endif
   
#endif
