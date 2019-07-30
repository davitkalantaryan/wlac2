/*
 *	File: argument_parser.cpp
 *
 *	Created on: 25 Nov, 2016
 *	Created by: Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements the class for parsing 
 *  command line arguments
 *  boost has classes for this purpose,
 *  but sometime dependency from boost is not needed
 *
 */

//#include "stdafx.h"
#ifdef __cplusplus
#include "common_argument_parser.hpp"

common::argument_parser::argument_parser()
{}

common::argument_parser::~argument_parser()
{}


common::argument_parser&  common::argument_parser::operator<<(const std::string& a_optionName)
{
	return AddOption(a_optionName,0,"");
}


common::argument_parser& common::argument_parser::AddOption(const std::string& a_optionName, int a_isArg, const std::string& a_defValue)
{
	SInput aInp;
	aInp.defaultValue = a_defValue;
	aInp.isArg = a_isArg;
	m_htOptionsIn2[a_optionName] = aInp;
	return *this;
}


const char* common::argument_parser::operator[](const char* a_option_name)
{
	if (m_htOptionsIn2.count(a_option_name)){
		return m_htOptionsFound[a_option_name].c_str();
	}
	return NULL;
}


std::string common::argument_parser::HelpString()const
{
	return "Should be implemented";
}


#else   // #ifdef __cplusplus
#endif  // #ifdef __cplusplus
