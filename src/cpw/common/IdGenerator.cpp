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

#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

//#include <stdafx.h>

#include <cpw/common/pstdint.h>
#include <cpw/common/Sha1.h>
#include <cpw/common/IdGenerator.h>



using cpw::IdGenerator;

time_t cpw::IdGenerator::last_id_generated = 0;

cpw::TypeId cpw::IdGenerator::namespc = cpw::TypeId();


IdGenerator::IdGenerator(void)
{
}

IdGenerator::~IdGenerator(void)
{
}

void IdGenerator::SetNamespace(std::string _namespace)
{
	cpw::SHA1Context cntx;
	uint8_t digest[SHA1HashSize+1];

	cpw::SHA1Reset(&cntx);
	cpw::SHA1Input(&cntx, (uint8_t*) _namespace.c_str(), _namespace.size());
	cpw::SHA1Result(&cntx, digest);
	
	digest[SHA1HashSize] = '\0';

	namespc[0] = digest[0];//time_low
	namespc[1] = digest[1];//time_low
	namespc[2] = digest[2];//time_low
	namespc[3] = digest[3];//time_low
	namespc[4] = digest[4];//time_mid
	namespc[5] = digest[5];//time_mid
	namespc[6] = digest[6];//time_hi_and_version
	namespc[7] = 80 | (15 & (digest[7]));//time_hi_and_version (with version on 4 most significant bits set to version 5)
	namespc[8] = 128 | (63 & (digest[8])); //clock_seq_hi_and_reserved (with 10 at the beginning)
	namespc[9] = digest[9];//clock_seq_low
	namespc[10] = digest[10];
	namespc[11] = digest[11];
	namespc[12] = digest[12];
	namespc[13] = digest[13];
	namespc[14] = digest[14];
	namespc[15] = digest[15];
}

cpw::TypeId  IdGenerator::GetNewId()
{
	cpw::SHA1Context cntx;
	uint8_t digest[SHA1HashSize+1];

	cpw::SHA1Reset(&cntx);

	const time_t seconds = time (NULL);
	time_t id = clock () % CLOCKS_PER_SEC + seconds * CLOCKS_PER_SEC;

	while (id == last_id_generated)
		id = clock () % CLOCKS_PER_SEC + seconds * CLOCKS_PER_SEC;

	last_id_generated = id;
	
	std::stringstream str;
	str << id;

	SetNamespace(namespc);

	std::string msg(((std::string)namespc) + str.str());

	cpw::SHA1Input(&cntx, (uint8_t*) msg.c_str(), msg.size());
	cpw::SHA1Result(&cntx, digest);

	digest[SHA1HashSize] = '\0';
	
	cpw::TypeId new_id;

	new_id[0] = digest[0];//time_low
	new_id[1] = digest[1];//time_low
	new_id[2] = digest[2];//time_low
	new_id[3] = digest[3];//time_low
	new_id[4] = digest[4];//time_mid
	new_id[5] = digest[5];//time_mid
	new_id[6] = digest[6];//time_hi_and_version
	new_id[7] = 80 | (15 & (digest[7]));//time_hi_and_version (with version on 4 most significant bits set to version 5)
	new_id[8] = 128 | (63 & (digest[8])); //clock_seq_hi_and_reserved (with 10 at the beginning)
	new_id[9] = digest[9];//clock_seq_low
	new_id[10] = digest[10];
	new_id[11] = digest[11];
	new_id[12] = digest[12];
	new_id[13] = digest[13];
	new_id[14] = digest[14];
	new_id[15] = digest[15];

	return new_id;	
}
