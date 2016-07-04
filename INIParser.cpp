/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	INIParser.h
Description	:	INI parser header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/


#include "INIParser.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/***********************
* Contructor: contructor ini parser
* @author: Joshua Tanner
* @parameter: none
* @return: 
********************/
CINIParser::CINIParser()
{

}

/***********************
* Destructor: destroy ini parser
* @author: Joshua Tanner
* @parameter: none
* @return:
********************/
CINIParser::~CINIParser()
{

}

/***********************
* setFileName: set the filename
* @author: Joshua Tanner
* @parameter: string filename
* @return:void
********************/
void CINIParser::setFileName(std::string _sFileName)
{
	m_strFilename = _sFileName;
}

/***********************
* ReturnMapVal: retrieve value from map
* @author: Joshua Tanner
* @parameter: string key
* @return: string
********************/
std::string CINIParser::ReturnMapVal(std::string _key)
{
	std::string value = m_mapPairs[_key];
	return(value);
}

/***********************
* GetMapSize: return size of map
* @author: Joshua Tanner
* @parameter: 
* @return: int
********************/
int CINIParser::GetMapSize()
{
	return(m_mapPairs.size());
}

/***********************
* LoadIniFile: load values from file
* @author: Joshua Tanner
* @parameter: filename
* @return: bool
********************/
bool CINIParser::LoadIniFile(const char* _pcFilename)
{
	m_strFilename = _pcFilename;
	m_filestream.open(m_strFilename, std::ios::in);//Set stream mode to in

	if (m_filestream.is_open())
	{
		string input;
		string section;
		string key;
		string variable;
		string value;	
	
		while (!m_filestream.eof())
		{			
			getline(m_filestream, input);

			if (input[0] == '[')//Indications a section
			{
				section = input;
			}
			else if (input[0] != '[')//Indicates a key/value
			{				
				
				int i = input.find('=');						
				//Split up the input
				key = input.substr(0, i - 1);
				value = input.substr(i + 2, input.length() - i - 1);
				m_mapPairs.insert(std::make_pair((section + "|" + key), value));	
			}
				
		}	
		return(true);
		m_filestream.close();
	}	
	else
	{
		cout << "Not loaded\n";
		return false;
	}
	
}


/***********************
* GenerateKey: combine section and key
* @author: Joshua Tanner
* @parameter: section and key
* @return: string
********************/
string GenerateKey(const char* _pcSection, const char* _pcKey)
{
	string section = _pcSection;
	string key = _pcKey;
	return(section + '|' + key);
}

/***********************
* GetStringValue: return a string value
* @author: Joshua Tanner
* @parameter: section, key, string to populate
* @return: bool
********************/
bool CINIParser::GetStringValue(const char* _pcSection, const char* _pcKey, std::string& _rStrValue)
{
	//Return string value associated with this string
	string key = GenerateKey(_pcSection, _pcKey);
	//Size is incorect
	if (m_mapPairs.find(key) == m_mapPairs.end())
	{
		std::cout << "Key not found\n";
		return false;
	}
	else
	{
		std::cout << "The value for the key " << _pcKey << " is: " << m_mapPairs[key] << std::endl;
		_rStrValue = m_mapPairs[key];
		return true;
	}
}

/***********************
* GetIntValue: return an int value
* @author: Joshua Tanner
* @parameter: section, key, int to populate
* @return: bool
********************/
bool CINIParser::GetIntValue(const char* _pcSection, const char* _pcKey, int& _riValue)
{
	//Return int value associated with this string
	string key = GenerateKey(_pcSection, _pcKey);
	if (m_mapPairs.find(key) == m_mapPairs.end())
	{
		cout << "Key not found\n";
		return false;
	}
	else
	{
		string value = m_mapPairs[key];
		if (isdigit(value[0]))
		{
			cout << "The value for the key " << _pcKey << " is: " << m_mapPairs[key] << std::endl;
			_riValue = stoi(m_mapPairs[key]);
			return true;
		}
		else
		{
			cout << "Value for this key is not an integer\n";
			return false;
		}
	
	}
}

/***********************
* GetFloatValue: return a float value
* @author: Joshua Tanner
* @parameter: section, key, float to populate
* @return: bool
********************/
bool CINIParser::GetFloatValue(const char* _pcSection, const char* _pcKey, float& _rfValue)
{
	string key = GenerateKey(_pcSection, _pcKey);
	if (m_mapPairs.find(key) == m_mapPairs.end())
	{
		std::cout << "Key not found\n";
		return false;
	}
	else
	{
		string value = m_mapPairs[key];
		if (isdigit(value[0]))
		{
			std::cout << "The value for the key " << _pcKey << " is: " << m_mapPairs[key] << std::endl;
			_rfValue = stof(m_mapPairs[key]);
			return true;
		}
		else
		{
			cout << "Value for this key is not a float\n";
			return false;
		}
	}
}

/***********************
* GetBoolValue: return a bool value
* @author: Joshua Tanner
* @parameter: section, key, bool to populate
* @return: bool
********************/
bool CINIParser::GetBoolValue(const char* _pcSection, const char* _pcKey, bool& _rbValue)
{
	string key = GenerateKey(_pcSection, _pcKey);;
	if (m_mapPairs.find(key) == m_mapPairs.end())
	{
		std::cout << "Key not found\n";
		return false;
	}
	else
	{
		std::cout << "The value for the key " << _pcKey << " is: " << m_mapPairs[key] << std::endl;
		if (m_mapPairs[key] == "true")
		{
			_rbValue = true;
			return true;
		}
		else if (m_mapPairs[key] == "false")
		{
			_rbValue = false;
			return true;
		}
		else
		{
			cout << "Not a boolean\n";
			return false;
		}
		
	}
}

