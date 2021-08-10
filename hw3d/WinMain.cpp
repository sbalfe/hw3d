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
	try
	{
		Window wnd( 800, 600, "Donkey Fart Box" );

		MSG msg;
		BOOL gResult;
		while( (gResult = GetMessage( &msg,nullptr,0,0 )) > 0 )
		{
			// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// check if GetMessage call itself borked
		if( gResult == -1 )
		{
			throw CHWND_LAST_EXCEPT();
		}

		// wParam here is the value passed to PostQuitMessage
		return msg.wParam;
	}
	/*
		check for defualt chilli exception 
		https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox

		handle to window owner = nullptr

		what message to dispaly = e.what()

		dialog box title = e.getType()

		utype = mb_ok, mb_iconexclamation
			first = display ok button 
			second = exclamation mark appears 
	*/
	catch( const ChiliException& e )
	{
		MessageBox( nullptr,e.what(),e.GetType(),MB_OK | MB_ICONEXCLAMATION );
	}
	/*
	  catch any standard exception
	*/
	catch( const std::exception& e )
	{
		MessageBox( nullptr,e.what(),"Standard Exception",MB_OK | MB_ICONEXCLAMATION );
	}
	/*
		catch unknown exceptions
	*/
	catch( ... )
	{
		MessageBox( nullptr,"No details available","Unknown Exception",MB_OK | MB_ICONEXCLAMATION );
	}
	return -1;
}