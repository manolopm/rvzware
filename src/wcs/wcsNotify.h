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
#ifndef wcsNotifyContext_HEADER
#define wcsNotifyContext_HEADER
#include <iostream>
#include <string>
#include <fstream>
#include <wcs/wcsExport.h>
extern "C"
{
/**
  * Notification level enumeration.  This specifies the
  * level of the mesage being written.
  */
WCS_EXPORT enum wcsNotifyLevel
{
   wcsNotifyLevel_ALWAYS = 0,
   wcsNotifyLevel_FATAL  = 1,
   wcsNotifyLevel_WARN   = 2,
   wcsNotifyLevel_NOTICE = 3,
   wcsNotifyLevel_INFO   = 4,
   wcsNotifyLevel_DEBUG  = 5
};

/**
 * Flags to allow the user to turn off certain notification levels
 * 
 */
WCS_EXPORT enum wcsNotifyFlags
{
   wcsNotifyFlags_NONE   = 0,
   wcsNotifyFlags_FATAL  = 1,
   wcsNotifyFlags_WARN   = 2,
   wcsNotifyFlags_NOTICE = 4,
   wcsNotifyFlags_INFO   = 8,
   wcsNotifyFlags_DEBUG  = 16,
   wcsNotifyFlags_ALL    = (wcsNotifyFlags_FATAL|
                              wcsNotifyFlags_WARN|
                              wcsNotifyFlags_NOTICE|
                              wcsNotifyFlags_INFO|
                              wcsNotifyFlags_DEBUG)
};

WCS_EXPORT void wcsSetDefaultNotifyHandlers();

WCS_EXPORT void wcsSetNotifyStream(std::ostream* outputStream,
                                   wcsNotifyFlags whichLevelsToRedirect=wcsNotifyFlags_ALL);

WCS_EXPORT std::ostream* wcsGetNotifyStream(wcsNotifyLevel whichLevel);

WCS_EXPORT bool wcsIsNotifyEnabled();

WCS_EXPORT std::ostream& wcsNotify(wcsNotifyLevel level = wcsNotifyLevel_WARN);

/**
 *
 */
WCS_EXPORT void wcsSetLogFilename(const std::string& filename);


/**
 *
 */
WCS_EXPORT void wcsDisableNotify(wcsNotifyFlags notifyLevel=wcsNotifyFlags_ALL);

/**
 *
 */
WCS_EXPORT void wcsEnableNotify(wcsNotifyFlags notifyLevel=wcsNotifyFlags_ALL);


WCS_EXPORT void wcsSetNotifyFlag(wcsNotifyFlags notifyFlags);
WCS_EXPORT void wcsPushNotifyFlags();
WCS_EXPORT void wcsPopNotifyFlags();
WCS_EXPORT wcsNotifyFlags wcsGetNotifyFlags();

}
#endif
