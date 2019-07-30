/*
 *	File: namedpideclient.h
 *
 *	Created on: Sep 24, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions ...
 *		1) ...
 *		2) ...
 *		...
 *
 *
 */
#ifndef __namedpipeclient_h__
#define __namedpipeclient_h__

#define DEFAULT_WAIT_TIME	100

#include "namedpipebase.h"

class NamedPipeClient : public NamedPipeBase
{
public:
	NamedPipeClient(const char* pipe_name = NULL, int timeoutMS = DEFAULT_WAIT_TIME);
	virtual ~NamedPipeClient(){}

	int		ConnectToNamedPipeC(const char* pipe_name, int timeoutMS);
};


#endif  // #ifndef __namedpipeclient_h__
