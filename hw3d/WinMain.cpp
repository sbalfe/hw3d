/******************************************************************************************
*	Chili Direct3D Engine																  *
*	Copyright 2018 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of Chili Direct3D Engine.											  *
*																						  *
*	Chili Direct3D Engine is free software: you can redistribute it and/or modify		  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili Direct3D Engine is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili Direct3D Engine.  If not, see <http://www.gnu.org/licenses/>.    *
******************************************************************************************/
#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	/*
		call our window which starts it and the message loop 

		they share the same message loops so closing one closes all.
	*/
	Window wnd( 800,300,"Donkey Fart Box" );
	Window wnd2(300, 400, "sex");

	MSG msg;
	BOOL gResult;

	/*
		WM_CLOSE calls and returns 0, calling destructor once when the wnd goes out of scope

		ignores default windows handling of this method
	*/
	while( (gResult = GetMessage( &msg,nullptr,0,0 )) > 0 )
	{
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// check if GetMessage call itself borked
	if( gResult == -1 )
	{
		return -1;
	}

	// wParam here is the value passed to PostQuitMessage
	return msg.wParam;
}