#pragma once
#include <unordered_map>
#include <Windows.h>

class WindowsMessageMap
{
public:
	WindowsMessageMap();
	std::string operator()( DWORD msg,LPARAM lp,WPARAM wp ) const;
private:

	/*
		maps windows message map ids to the string which is the name of the message.
	*/
	std::unordered_map<DWORD,std::string> map;
};