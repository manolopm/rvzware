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
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <string.h>
//#include <stdafx.h>

#include <cpw/persistent/file/FileManager.h>

using namespace std;
using namespace cpw;

FileManager::FileManager(void)
{
}

FileManager::~FileManager(void)
{

}


bool FileManager::IsLocal(const std::string &url)
{
  std::string url1 = url.substr(0, 4);
  return (url1 != "http");
  //return true;
}


FileError FileManager::Save(const std::string &data, const std::string &file, bool overwrite){

  if(!overwrite) {

    //test if file already exists
    std::ifstream fin(file.c_str());
		
    if(fin.is_open()) 
      {	
	fin.close();
	return FILEALREADYEXISTS;
      }
    fin.close();
		
  }

  std::ofstream fout(file.c_str());

  if(fout.is_open()) {
	
    fout << data;
    fout.close();

    return FILEOK;
  }
  else return FILEPERMISSIONDENIED;

}


FileError FileManager::Load(const std::string &url, std::string &data) {

	
  if(IsLocal(url))
    {
      std::ifstream fin(url.c_str());


      if(fin.is_open()) {

	int length;
	char * buffer;

	// get length of file:
	fin.seekg (0, ios::end);
	length = fin.tellg();
	fin.seekg (0, ios::beg);

	// allocate memory:
	buffer = new char [length];
	memset(buffer,0,length);

	// read data as a block:
	fin.read (buffer,length);
	fin.close();

	std::string temp(buffer,&buffer[length]);
	if (buffer)
	  {
	    delete[] buffer;
	    buffer = NULL;
	  }

	data.swap(temp);

	return FILEOK;
      }
      else return FILENOTFOUND;
    }
  else
    {

      //this is for remote access
      std::string proxy = "";
      bool isSSLAddress = false;
      bool result = GetMap(url, isSSLAddress, data, proxy);
		
      if(result) return FILEOK;
      else return FILENOTFOUND; //return (int)data.size();
    }
}


FileError FileManager::Delete(const std::string &file)
{
  if(std::remove(file.c_str()) != 0)

    return FILECANNOTDELETE;

  return FILEOK;
}



size_t FileManager::writeCallback(void *buffer, size_t size, size_t nmemb, void *getMapWorkingData)
{
  struct GetMapWorkingData *wd=(struct GetMapWorkingData *)getMapWorkingData;

  // Append a void buffer to memory buffer
  wd->membuf->append((char*)buffer, size*nmemb);	  
  return size*nmemb;
}



/**
 *  The function retrieves a map image, storing it in memory 
 *  TRUE if success. FALSE if any error.
 **/
bool FileManager::GetMap(const std::string &url, bool ssl, std::string &memBuf, const std::string &proxy)
{
  CURL *curl;
  CURLcode res;
  bool result = false;
  struct GetMapWorkingData wd;

  // Reset memory buffer
  wd.membuf = &memBuf;
  *(wd.membuf) = "";

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (proxy!="")
      curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());

    if (ssl)
      {
	// Disable checking server certificate
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	// Disable checking server name
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
      }
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wd);

    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
    result = true;
  }

  return result;
}


size_t FileManager::read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;

  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */
  retcode = fread(ptr, size, nmemb, (FILE*)stream);

  fprintf(stderr, "*** We read %d bytes from file\n", retcode);

  return retcode;
}


int FileManager::PutFile(const std::string &localFilename, const std::string &urlDest)
{
  CURL *curl;
  CURLcode res;
  FILE * hd_src ;

  const char *file= localFilename.c_str();;
  const char *url= urlDest.c_str();

  /* get a FILE * of the same file, could also be made with
     fdopen() from the previous descriptor, but hey this is just
     an example! */
  hd_src = fopen(file, "rb");
  fseek(hd_src, 0L, SEEK_END);
  long tam = ftell(hd_src);
  fseek(hd_src, 0L, SEEK_SET);

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* we want to use our own read function */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

    /* enable uploading */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1) ;

    /* HTTP PUT please */
    curl_easy_setopt(curl, CURLOPT_PUT, 1);

    /* specify target URL, and note that this URL should include a file
       name, not only a directory */
    curl_easy_setopt(curl,CURLOPT_URL, url);

    /* now specify which file to upload */
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

    /* provide the size of the upload, we specicially typecast the value
       to curl_off_t since we must be sure to use the correct data size */
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)tam);

    /* Now run off and do what you've been told! */
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  fclose(hd_src); /* close the local file */

  curl_global_cleanup();

  return 0;
}
