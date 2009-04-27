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

#ifndef _CPWUTILSEXPORT_
#define _CPWUTILSEXPORT_


	#if defined(_MSC_VER)
	    #pragma warning( disable : 4244 )
	    #pragma warning( disable : 4251 )
	    #pragma warning( disable : 4267 )
	    #pragma warning( disable : 4275 )
	    #pragma warning( disable : 4290 )
	    #pragma warning( disable : 4786 )
	    #pragma warning( disable : 4305 )
	    #pragma warning( disable : 4996 )
	#endif


	#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
	#ifdef CPWUTILS_EXPORTS_STATIC
	#    define FIREEXPORT
	#else
	#	ifdef CPWUTILS_EXPORTS
	#		define CPWUTILS_EXPORT __declspec(dllexport)
	#	else
	#       define CPWUTILS_EXPORT __declspec(dllimport)
	#   endif
	#endif
#else
	#ifdef CPWUTILS_EXPORTS_STATIC
	#    define FIREEXPORT
	#else
	#	ifdef CPWUTILS_EXPORTS
	#		define CPWUTILS_EXPORT 
	#	else
	#       define CPWUTILS_EXPORT 
	#   endif
	#endif
#endif
#endif

