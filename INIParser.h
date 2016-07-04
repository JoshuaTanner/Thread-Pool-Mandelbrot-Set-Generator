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

#ifndef __INIPARSER_H__
#define __INIPARSER_H__
#include <map>
#include <fstream>


//Parser class for reading info from INI
class CINIParser
{
private:
	
	std::map<std::string, std::string> m_mapPairs;	
	std::string m_strFilename;	
	std::fstream m_filestream;

public:
	
	CINIParser();
	~CINIParser();

	void setFileName(std::string _sFileName);
		
	std::string ReturnMapVal(std::string _key);
	int GetMapSize();

	//Read in info from file
	bool LoadIniFile(const char* _pcFilename);
	
	//Get values of different types
	bool GetStringValue(const char* _pcSection, const char* _pcKey, std::string& _rStrValue);	
	bool GetIntValue(const char* _pcSection, const char* _pcKey, int& _riValue);
	bool GetFloatValue(const char* _pcSection, const char* _pcKey, float& _rfValue);	
	bool GetBoolValue(const char* _pcSection, const char* _pcKey, bool& _rbValue);

	
};

#endif
