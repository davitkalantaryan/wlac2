/*
 *	File: namedpipeclient.cpp
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
#include "namedpipeclient.h"

NamedPipeClient::NamedPipeClient(const char* a_pipe_name, int a_timeoutMS)
{
	if (a_pipe_name != NULL)
		ConnectToNamedPipeC(a_pipe_name, a_timeoutMS);
}


int NamedPipeClient::ConnectToNamedPipeC(const char* a_pipe_name, int a_nTimeoutMS)
{
	int nIteration(0);
	
	ClosePipe();
	m_Pipe = CreateFileA(a_pipe_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	while ((m_Pipe == INVALID_HANDLE_VALUE)&&(nIteration<a_nTimeoutMS))
	{
		++nIteration;
		GetLastError();
		Sleep(1);
		m_Pipe = CreateFileA(a_pipe_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	}
	
	if (m_Pipe == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}
	return 0;
}
