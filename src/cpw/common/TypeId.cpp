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

//#include <stdafx.h>

#include <cpw/common/TypeId.h>
#include <cpw/common/pstdint.h>


using namespace cpw;


TypeId::TypeId()
{
	for (int i=0; i<TYPEID_SIZE; i++)
		value[i] = 0;
}


TypeId::TypeId(const std::string &id)
{
	Import(id);
}


TypeId::TypeId(const char *id)
{
	Import(std::string(id));
}


TypeId::TypeId(const TypeId& id)
{
	for (int i=0; i < TYPEID_SIZE; i++)
		value[i] = id.value[i];
}


int TypeId::size()
{
	return TYPEID_SIZE;
}


TypeId &TypeId::operator=(const TypeId& id)
{
	for (int i=0; i < TYPEID_SIZE; i++)
		value[i] = id.value[i];

	return *this;
}


const bool TypeId::operator== (const TypeId &id) const 
{
	int i=0;
	while ((i<TYPEID_SIZE) && (value[i]==id.value[i])) i++;

	if (i<TYPEID_SIZE)
		return false;
	else
		return true;
}


const bool TypeId::operator!=(const TypeId &id) const 
{
	return !operator==(id);
}


const bool TypeId::operator<(const TypeId &id) const 
{
	int i = TYPEID_SIZE-1;
	while ((i>=0) && (value[i]==id.value[i])) i--;

	if (i>=0)
		return (value[i] < id.value[i]);
	else
		return false;
}


TypeId::operator std::string()
{
	return Export();
}


std::string TypeId::GetValue()
{
	return Export();
}


std::string TypeId::GetValue() const
{	
	return Export();
}


void TypeId::SetValue(const std::string &str)
{
	Import(str);
}


uint8_t &TypeId::operator[](const int i)
{
	if (i<TYPEID_SIZE)
		return value[i];
	else
		return value[TYPEID_SIZE-1];
}


std::ostream & cpw::operator<<(std::ostream &s, const TypeId &id)
{
	s << id.GetValue();
	return s;
}


std::istream & cpw::operator>>(std::istream &s, TypeId &id)
{
	std::string str;
	s >> str;
	id.SetValue(str);
	return s;
}


void hex2int(unsigned char str[2], uint8_t *_int);

void TypeId::Import(std::string _value)
{
	if (_value.size() != (TYPEID_SIZE*2) + 4)
	{
		for (int i=0; i<TYPEID_SIZE; i++)
			this->value[i] = 0;
		return;
	}

	_value.replace(8, 1, std::string());
	_value.replace(12, 1, std::string());
	_value.replace(16, 1, std::string());
	_value.replace(20, 1, std::string());

	for (int i=0; i<TYPEID_SIZE; i++)
	{
		unsigned char str[2];
		uint8_t _int;

		str[0] = _value.substr(i*2, 1).c_str()[0];
		str[1] = _value.substr((i*2)+1, 1).c_str()[0];

		if (((str[0]>= 48 && str[0] <= 57) || (str[0]>= 65 && str[0] <= 70) ||
			(str[0]>= 97 && str[0] <= 102)) && 
			((str[1]>= 48 && str[1] <= 57) ||(str[1]>= 65 && str[1] <= 70) ||
			(str[1]>= 97 && str[1] <= 102)))
		{
			hex2int(str, &_int);
			this->value[i] = _int;
		}
		else
		{
			for (int i=0; i<TYPEID_SIZE; i++)
				this->value[i] = 0;
			return;
		}
	}
}

void int2hex(uint8_t _int, unsigned char str[3]);

std::string TypeId::Export()
{
	std::string strtemp = std::string();
	unsigned char strbyte[3];
	strbyte[2] = '\0';

	for (int i=0; i<TYPEID_SIZE; i++)
	{
		int2hex(value[i], strbyte);
		strtemp += std::string((char*)strbyte);
	}

	std::string representation;

	//time_low
	representation = strtemp.substr(0, 8)  + "-";

	//time_mid
	representation += strtemp.substr(8, 4)  + "-";

	//time_hi_and_version
	representation += strtemp.substr(12, 4)  + "-";

	//clock_seq_low
	representation += strtemp.substr(16, 4)  + "-";

	//rest
	representation += strtemp.substr(20, 12);

	return representation;
}



const std::string TypeId::Export() const
{
	std::string strtemp = std::string();
	unsigned char strbyte[3];
	strbyte[2] = '\0';

	for (int i=0; i<TYPEID_SIZE; i++)
	{
		int2hex(value[i], strbyte);
		strtemp += std::string((char*)strbyte);
	}

	std::string representation;

	//time_low
	representation = strtemp.substr(0, 8)  + "-";

	//time_mid
	representation += strtemp.substr(8, 4)  + "-";

	//time_hi_and_version
	representation += strtemp.substr(12, 4)  + "-";

	//clock_seq_low
	representation += strtemp.substr(16, 4)  + "-";

	//rest
	representation += strtemp.substr(20, 12);

	return representation;
}


void int2hex(uint8_t _int, unsigned char str[3])
{
	uint8_t mod;
	uint8_t res;
	
	res = _int / 16;
	if (res <=9)
		str[0] = res + 48;
	else
		str[0] = res + 65 - 10; //10 = decimal value for A

	mod = _int % 16;
	if (mod <= 9)
		str[1] = mod + 48;
	else
		str[1] = mod + 65 - 10; //10 = decimal value for A
	
}

void hex2int(unsigned char str[2], uint8_t *_int)
{
	*_int = 0;

	if (str[1] >= 48 && str[1] <= 57)
		*_int += str[1] - 48;
	else
	{
		if (str[1] >= 65 && str[1] <= 70)
				*_int = str[1] - 65 + 10;  //10 = decimal value for A
		else
		{
			if (str[1] >= 97 && str[1] <= 102)
				*_int = str[1] - 97 + 10;  //10 = decimal value for A
		}
	}

	if (str[0] >= 48 && str[0] <= 57)
		*_int += (str[0] - 48) << 4;
	else
	{
		if (str[0] >= 65 && str[0] <= 70)
			*_int += (str[0] - 65 + 10) << 4;  //10 = decimal value for A
		else
		{
			if (str[0] >= 97 && str[0] <= 102)
				*_int += (str[0] - 97 + 10) << 4;  //10 = decimal value for A
		}
	}
}
