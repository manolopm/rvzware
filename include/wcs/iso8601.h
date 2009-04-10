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


#ifndef WCS_ISO8601_H
#define WCS_ISO8601_H

#include <string>

typedef struct wcsMyTime_t {
	int year;
	int month;
	int day;
	int hour;
	int min;
	float sec;
} wcsMyTime;

// Returns true if the first date is less than the second date.
bool operator<(const wcsMyTime & a, const wcsMyTime & b);

// Are two times equal?
bool operator==(const wcsMyTime & a, const wcsMyTime & b);
bool operator!=(const wcsMyTime & a, const wcsMyTime & b);

// month is in range [1-12].  Returns 1 if there is a leap day this month/year.
int wcs_leap_days(int year, int month);

// Adds two times together.  This is more useful if one of the times
// is actually an interval.
wcsMyTime operator+(const wcsMyTime & a, const wcsMyTime & b);

// Find the absolute difference between two times.  Returns the interval
// between the two times.
wcsMyTime operator-(const wcsMyTime & a, const wcsMyTime & b);


// Convert a time in string format to a wcsMyTime struct.  It's pretty 
// forgiving to what it calls a time.
void wcs_parse_time(wcsMyTime & t, const std::string & str);

// Convert a period or interval into a wcsMyTime struct.
void wcs_parse_period(wcsMyTime & t, const std::string & str);


// Produce a date/time in standard ISO8601 form from a wcsMyTime struct.
std::string wcs_iso8601_time(const wcsMyTime & t);

// Produce a date/time in standard ISO8601 form from a string.
std::string wcs_iso8601_time(const std::string & str);

// Produce a time period from a wcsMyTime struct.
std::string wcs_iso8601_period(const wcsMyTime & t);

#endif
