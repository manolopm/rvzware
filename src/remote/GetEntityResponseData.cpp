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

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#ifdef WIN32
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <remote/stdafx.h>

#include <cpw/ApplicationConfiguration.h>
#include <cpw/common/Sha1.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/common/Composite.h>
#include <cpw/persistent/Persistent.h>
#include <cpw/persistent/file/XmlCpwParser.h>
#include <cpw/persistent/file/FileManager.h>
#include <cpw/persistent/file/PersistentFileManager.h>

#include <remote/DataStream.h>
#include <remote/xmlParser.h>
#include <remote/GetEntityResponseData.h>


using namespace cpw::remote;


std::vector<std::string> SetTags()
{
  std::vector<std::string> aux;
  aux.push_back("components");
  aux.push_back("icon");
  aux.push_back("FireTexture");
  aux.push_back("SmokeTexture");
  aux.push_back("IgnitionPointModel");
  aux.push_back("primitive_url");
  aux.push_back("model_url");
  aux.push_back("synchs");

  return aux;
}

std::vector<std::string> GetEntityResponseData::tags = SetTags();



GetEntityResponseData::GetEntityResponseData()
{
  SetMessageType(msgTypeGetEntityResponse);
}


GetEntityResponseData::~GetEntityResponseData()
{

}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  Returns an empty DataStream in case there is an error.
 */
DataStream GetEntityResponseData::Code()
{
  try
    {
      std::string primitive_xml;
      std::map<std::string, uint32_t> binaries_url;
      std::map<uint32_t, DataStream> binaries_data;
      std::map<uint32_t, std::string> binaries_name;
      uint32_t binary_last_id = 0;
      uint32_t msg_size = 0;


      //Calculate size of message
      msg_size = 6 + cpw::TypeId::size() + 8 + 8 + 1;

      if (send)
	{
	  GetDataFromEntity(false, primitive_xml, binaries_url, binaries_data, binary_last_id);

	  msg_size += 4 + entity_xml.size() +
	    4 + children_ids.size() + (cpw::TypeId::size() * children_ids.size()) +
	    4 + primitive_xml.size();
			
	  //number of binaries
	  msg_size += 4;

	  //name of binaries
	  std::map<std::string, uint32_t>::iterator it_bin_url;
	  for (it_bin_url = binaries_url.begin(); it_bin_url != binaries_url.end(); it_bin_url++)
	    {
	      //Get the last part of the url (ie: the name)
	      std::string tmp_str = it_bin_url->first;
	      int pos;
	      if ((pos = tmp_str.rfind("/"))==-1)
		pos = tmp_str.rfind("\\");
	      tmp_str = tmp_str.substr(pos+1, tmp_str.size()-pos-1);
	      binaries_name[it_bin_url->second] = tmp_str;

	      msg_size += 4 + 4 + tmp_str.size();
	    }


	  //Count the size binaries
	  std::map<uint32_t, DataStream>::iterator it_bin;
	  for (it_bin=binaries_data.begin(); it_bin!=binaries_data.end(); it_bin++)
	    msg_size += (4 + it_bin->second.GetSize());
	}


      //Get the content into the stream
      DataStream data(msg_size);

      //control data
      data << (uint8_t) message_type;
      data << msg_size;
      data << request_id;

      //Entity's id
      data << entity_id;
		
      //timestamp of current time
      data << current_time;


      //timestamp of last synchro
      data << last_synchro;

      //Wether this message contains Entity's data
      data << send;
		
      if (send)
	{
	  data << entity_xml;

	  //Children
	  data << (uint32_t) children_ids.size();

	  std::vector<cpw::TypeId>::iterator it_ch;
	  for (it_ch=children_ids.begin(); it_ch!=children_ids.end(); it_ch++)
	    data << *it_ch;


	  //Primitive
	  data << primitive_xml;

	  //Binaries
	  data << (uint32_t) binaries_data.size();

	  std::map<uint32_t, DataStream>::iterator it_dat;
	  for (it_dat=binaries_data.begin(); it_dat!=binaries_data.end(); it_dat++)
	    {
	      //id of file
	      data << it_dat->first;

	      //Write the name of the file
	      data << binaries_name[it_dat->first];

	      //Add binary data
	      data << it_dat->second;
	    }
	}

      return data;
    }
  catch (DataStreamException)
    {
      return DataStream();
    }
}


/*!
 *  \brief Creates the message from a DataStream
 *  \param data_stream The data to be used to compose the message
 *
 *  Creates the message from a DataStream.
 *
 *  Returns a true on success. Returns false otherwise.
 */
bool GetEntityResponseData::Decode(const DataStream &data_stream)
{
  try
    {
      DataStream data(data_stream);

      //First of all, get the meta-data
      uint8_t tmp8;
      data >> tmp8;
      message_type = (MessageType) tmp8;

      uint32_t msg_size;
      data >> msg_size;

      if (data.GetSize() != msg_size)
	return false;

      data >> tmp8;
      request_id = tmp8;

      data >> entity_id;

      //Timestamp of current time
      data >> current_time;

      //Timestamp of last synch
      data >> last_synchro;

      data >> send;

      if (send)
	{
	  //the entity
	  data >> entity_xml;

	  XMLNode xMainNode;
	  xMainNode = XMLNode::parseString(entity_xml.c_str());

	  if (!xMainNode.getChildNode(0).isEmpty())
	    {
	      if (!xMainNode.getChildNode(0).getChildNode("id").isEmpty())
		{
		  if (xMainNode.getChildNode(0).getChildNode("id").nText() != 0)
		    {
		      std::istringstream str(xMainNode.getChildNode(0).getChildNode("id").getText(0));
		      cpw::TypeId entity_id_aux;
		      str >> entity_id_aux;
		      if (entity_id_aux != entity_id)
			return false;						
		    }
		  else
		    return false;
		}
	      else
		return false;
	    }
	  else
	    return false;


	  uint32_t n_child;
	  data >> n_child;

	  while (n_child > 0)
	    {
	      cpw::TypeId idtemp;
	      data >> idtemp;

	      children_ids.push_back(idtemp);

	      n_child--;
	    }

	  //the primitive
	  std::string primitive_xml;
	  data >> primitive_xml;

	  cpw::TypeId primitive_id;

	  if (primitive_xml.size() != 0)
	    {
	      xMainNode = XMLNode::parseString(primitive_xml.c_str());

	      if (!xMainNode.getChildNode(0).isEmpty())
		{
		  if (!xMainNode.getChildNode(0).getChildNode("id").isEmpty())
		    {
		      if (xMainNode.getChildNode(0).getChildNode("id").nText() != 0)
			{
			  std::istringstream str2(xMainNode.getChildNode(0).getChildNode("id").getText(0));
			  str2 >> primitive_id;
			}
		      else
			return false;
		    }
		  else
		    return false;
		}
	      else
		return false;
	    }

	  //Finally, get the binaries from the stream
	  std::map<uint32_t, std::string> binaries_names;

	  uint32_t nbin;
	  data >> nbin;

			
	  std::string root_folder =
	    cpw::ApplicationConfiguration::GetInstance()->GetEntityDirectory() + "remote/";

	  //Create folder
#ifdef WIN32
	  mkdir(root_folder.c_str());
#else
	  mkdir(root_folder.c_str(),0777);
#endif

	  while (nbin>0)
	    {
	      //binary id
	      uint32_t id;
	      data >> id;

	      //name of binary
	      std::string binary_name;
	      data >> binary_name;

	      std::string binary;
	      data >> binary;

	      std::ofstream file;
	      file.open((root_folder + binary_name).c_str(), std::ios::out | std::ios::binary);
	      file.write(binary.c_str(), binary.size());
	      file.close();
				
	      //fm.Save(binary, root_folder + "/" + binary_name, true);

	      binaries_names[id] = root_folder + binary_name;

	      nbin--;
	    }


	  //After processing the complete stream, xml files should be processesed,
	  //changing binaries ids for urls


	  //Entity
	  ChangeIdsWithUrl(entity_xml, binaries_names);

	  //Primitive
	  if (primitive_xml.size() > 0)
	    {
	      ChangeIdsWithUrl(primitive_xml, binaries_names);


	      //Save files on disk
	      cpw::FileManager fm;
	      fm.Save(primitive_xml, root_folder +
		      (std::string)primitive_id + std::string(".cel"), true);
	    }
	}

      return true;
    }
  catch (DataStreamException)
    {
      return false;
    }
}


/*!
 *  \brief Tells wheter the message is a request or not
 *
 *  Tells wheter the message is a request or not.
 */
bool GetEntityResponseData::IsRequest()
{
  return false;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool GetEntityResponseData::IsResponse()
{
  return true;
}


/*!
 *  \brief Returns the entity xml definition
 *
 *  Returns the entity id xml definition.
 */
const std::string &GetEntityResponseData::GetEntityXml()
{
  return entity_xml;
}


/*!
 *  \brief Returns the list of children's id of the entity
 *
 *  Returns the list of children's id of the entity.
 */
const std::vector<cpw::TypeId> &GetEntityResponseData::GetChildren()
{
  return children_ids;
}


/*!
 *  \brief Returns true if the message contains the entity data
 *
 *  Returns true if the message contains the entity data. Returns false in case the purpose of the
 *  request was a synchronization of the entity.
 */
bool GetEntityResponseData::GetSendEntity()
{
  return send;
}


/*!
 *  \brief Returns the timestamp of the last syncrhonization between the nodes
 *
 *  Returns the timestamp of the las syncrhonization between the nodes.
 */
uint64_t GetEntityResponseData::GetLastSynchro()
{
  return last_synchro;
}


/*!
 *  \brief Returns the current entity time set on the message
 *
 *  Returns the current entity time set on the message.
 */
uint64_t GetEntityResponseData::GetCurrTime()
{
  return current_time;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &GetEntityResponseData::GetEntityId()
{
  return entity_id;
}


/*!
 *  \brief Returns the entity xml definition
 *
 *  Returns the entity id xml definition.
 */
const std::string &GetEntityResponseData::GetEntityXml() const
{
  return entity_xml;
}


/*!
 *  \brief Returns the list of children's id of the entity
 *
 *  Returns the list of children's id of the entity.
 */
const std::vector<cpw::TypeId> &GetEntityResponseData::GetChildren() const
{
  return children_ids;
}


/*!
 *  \brief Returns true if the message contains the entity data
 *
 *  Returns true if the message contains the entity data. Returns false in case the purpose of the
 *  request was a synchronization of the entity.
 */
bool GetEntityResponseData::GetSendEntity() const
{
  return send;
}


/*!
 *  \brief Returns the timestamp of the last syncrhonization between the nodes
 *
 *  Returns the timestamp of the las syncrhonization between the nodes.
 */
uint64_t GetEntityResponseData::GetLastSynchro() const
{
  return last_synchro;
}


/*!
 *  \brief Returns the current entity time set on the message
 *
 *  Returns the current entity time set on the message.
 */
uint64_t GetEntityResponseData::GetCurrTime() const
{
  return current_time;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &GetEntityResponseData::GetEntityId() const
{
  return entity_id;
}


/*!
 *  \brief Sets the entity xml definition to be sent
 *
 *  Sets the entity id xml definition to be sent.
 */
void GetEntityResponseData::SetEntityXml(const std::string &entity)
{
  entity_xml = entity;
}


/*!
 *  \brief Specifies whether the message contains the entity's data or not
 *  \param send A boolean meaning the purpose of the request
 *
 *  Specifies whether the message contains the entity's data or not.
 */
void GetEntityResponseData::SetSendEntity(bool _send)
{
  send = _send;
}


/*!
 *  \brief Returns the timestamp of the las syncrhonization between the nodes
 *
 *  Returns the timestamp of the las syncrhonization between the nodes.
 */
void GetEntityResponseData::SetLastSynchro(const uint64_t &ts)
{
  last_synchro = ts;
}


/*!
 *  \brief Sets the current entity time to be sent
 *
 *  Sets the current entity time to be sent.
 */
void GetEntityResponseData::SetCurrTime(const uint64_t &ts)
{
  current_time = ts;
}


/*!
 *  \brief Sets the entity id
 *  \param id Entity id
 *
 *  Sets the entity id.
 */
void GetEntityResponseData::SetEntityId(const cpw::TypeId &id)
{
  entity_id = id;
}




//---------------------------------------------------------------------------
//-----------------------    AUXILIAR METHODS     ---------------------------
//---------------------------------------------------------------------------



/*!
 *  \brief Returns the id of the entity defined on the file specified
 *  \param url The url of the file containing the entity
 *
 *  Returns the id of the entity defined on the file specified.
 */
cpw::TypeId GetIdFromFile(const std::string &url)
{
  cpw::TypeId id;
  cpw::Node *root;
  cpw::PersistentFileManager persistent_manager;

  cpw::PersistentError error = persistent_manager.Load(url, &root);

  if (error == cpw::PERSISTENTOK)
    {
      if(root != NULL)
	{
	  std::string a = root->GetChildValue("id");
	  id = cpw::TypeId(a);
			
	  if (root)
	    {
	      delete root;
	      root = NULL;
	    }
	}
    }

  return id;
}


/*!
 *  \brief Returns a digest string (SHA-1) of the data specified
 *  \param memblock A pointer to a chunk of memory
 *  \param size The size of the memory chunk
 *
 *  Returns a digest string (SHA-1) of the data specified.
 */
std::string GetHashName(uint8_t *memblock, int size)
{
  //Get the name for the file. Use the Sha-1 hash value + extension
  cpw::SHA1Context cntx;
  uint8_t digest[SHA1HashSize];

  cpw::SHA1Reset(&cntx);
  cpw::SHA1Input(&cntx, (uint8_t*) memblock, size);
  cpw::SHA1Result(&cntx, digest);

  std::string name;

  for (int i=0; i<SHA1HashSize;i++)
    {
      uint8_t _int = digest[i];
      uint8_t mod;
      uint8_t res;
      char str[3];
      str[2] = '\0';
		
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

      name += std::string(str);
    }

  return name;
}


/*!
 *  \brief Auxiliar method used on codification
 *
 *  Auxiliar method used on codification.
 *
 *  The method walks the xml tree and modifies it to be sent through the network.
 */
void GetEntityResponseData::GetDataFromEntity(bool primitive, std::string &primitive_xml,
					      std::map<std::string, uint32_t> &binaries_url, std::map<uint32_t, DataStream> &binaries_data,
					      uint32_t &binary_last_id)
{
  //std::string root_folder = cpw::ApplicationConfiguration::GetInstance()->GetEntityDirectory();
  std::string root_folder = cpw::EntityRegistry::GetInstance()->GetEntity(entity_id)->GetCompoundDirectory() + "/";
  XMLNode xMainNode;

  if (primitive)
    xMainNode = XMLNode::parseString(primitive_xml.c_str());
  else
    xMainNode = XMLNode::parseString(entity_xml.c_str());

  //For each tag, find it on the tree
  std::vector<std::string>::iterator it_tag;
  for (it_tag = tags.begin(); it_tag != tags.end(); it_tag++)
    {
      XMLNode xml_aux1 = xMainNode.getChildNode(0).getChildNode(it_tag->c_str());
      if (!xml_aux1.isEmpty()) //FOUND it!!
	{
	  std::string tag_name(xml_aux1.getName());
	  if (tag_name == "synchs")
	    {
	      xml_aux1.deleteNodeContent();
	    }
	  else if (tag_name == "components") //child entities follow
	    {
	      while (xml_aux1.nChildNode() > 0)
		{
		  XMLNode xml_aux2 = xml_aux1.getChildNode(0).getChildNode("url");
		  if (!xml_aux2.isEmpty())
		    {
		      if (xml_aux2.nText() !=0)
			{
			  std::string url(xml_aux2.getText());
			  url = root_folder + url;
			  cpw::TypeId id;
			  id = cpw::EntityRegistry::GetInstance()->GetEntityFromUrl(url)->GetId();
			  if (id != cpw::TypeId())
			    children_ids.push_back(id);
			}
		    }
		  xml_aux1.getChildNode(0).deleteNodeContent();
		}
				
	    }
	  else if (tag_name == "primitive_url")
	    {
	      if (xml_aux1.nText() !=0)
		{
		  std::string url(xml_aux1.getText());
		  url = root_folder + url;
		  cpw::TypeId id(GetIdFromFile(url));
		  if (id != cpw::TypeId())
		    {
		      xml_aux1.updateText(((std::string)id).c_str());
		      XMLNode tempxnode = XMLNode::parseFile(url.c_str());
		      char *xml_string_char = tempxnode.createXMLString();
		      primitive_xml = xml_string_char;
		      if (xml_string_char)
			{
			  delete [] xml_string_char;
			  xml_string_char = NULL;
			}
		      GetDataFromEntity(true, primitive_xml, binaries_url, binaries_data, binary_last_id);
		    }
		}
	    }
	  else //binary data
	    {
	      if (xml_aux1.nText() != 0)
		{
		  std::string url1(xml_aux1.getText());

		  //Get the binary file
		  std::ifstream file;
		  file.open((root_folder + url1).c_str(),
			    std::ios::in | std::ios::binary);
		  long begin = file.tellg();
		  file.seekg (0, std::ios::end);
		  long end = file.tellg();
		  file.seekg (0, std::ios::beg);
		  int size = end-begin;
		  uint8_t * memblock = new uint8_t [size];
		  file.read((char *)memblock, size);

		  std::string url2;
		  url2 = GetHashName(memblock, size);
					
		  if (url1.rfind('.') != std::string::npos)
		    url2 += url1.substr(url1.rfind('.'));

		  uint32_t id;
		  if (binaries_url.find(url2) == binaries_url.end())
		    {
		      //Get a new id
		      id = binary_last_id++;

		      DataStream binFile(size, memblock);
		      binaries_data[id] = binFile;

		      binaries_url[url2] = id;
		    }
		  else
		    id = binaries_url[url2];

		  //substitute url with id on the tree
		  std::ostringstream str;
		  str << id;
		  xml_aux1.updateText(str.str().c_str());
		  if (memblock)
		    {
		      delete memblock;
		      memblock = NULL;
		    }
		  file.close();
		}
	    }
	}
    }

  //get the final string and insert it on the map
  char *xml_string_char = xMainNode.createXMLString();
  std::string xml_string(xml_string_char);
  if (xml_string_char)
    {
      delete [] xml_string_char;
      xml_string_char = NULL;
    }

  if (primitive)
    primitive_xml = xml_string;
  else
    entity_xml = xml_string;
}


/*!
 *  \brief Auxiliar method used on decodification
 *
 *  Auxiliar method used on decodification.
 *
 *  The method walks the xml tree and modifies it to be used locally.
 */
void GetEntityResponseData::ChangeIdsWithUrl(std::string &xml_string,
					     std::map<uint32_t, std::string> &binaries_url)
{
  std::string root_folder = cpw::ApplicationConfiguration::GetInstance()->GetEntityDirectory()
    + "/remote/";

  XMLNode xMainNode = XMLNode::parseString(xml_string.c_str());

  std::vector<std::string>::iterator it_tag;
  for (it_tag=tags.begin(); it_tag!=tags.end(); it_tag++)
    {
      XMLNode xml_aux1 = xMainNode.getChildNode(0).getChildNode(it_tag->c_str());
      if (!xml_aux1.isEmpty()) //FOUND it!!
	{
	  std::string tag_name(xml_aux1.getName());
	  if (tag_name == "primitive_url") //child entities follow
	    {
	      if (xml_aux1.nText() != 0)
		xml_aux1.updateText((root_folder + std::string(xml_aux1.getText()) + ".cel").c_str());
	    }
	  else //child is a binary
	    {
	      if (xml_aux1.nText() != 0)
		{
		  std::istringstream url(xml_aux1.getText());
		  uint32_t id;
		  url >> id;
		  xml_aux1.updateText(binaries_url[id].c_str());
		}
	    }
	}					
    }

  char *xml_string_char = xMainNode.createXMLString();
  xml_string = xml_string_char;
  if (xml_string_char)
    {
      delete [] xml_string_char;
      xml_string_char = NULL;
    }
}
