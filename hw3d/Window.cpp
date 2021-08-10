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


// Window Class Stuff, just defines wndClass here. 
Window::WindowClass Window::WindowClass::wndClass; // calls default constructor below 

Window::WindowClass::WindowClass() noexcept
	:
	/*
		obtain handle to current instance, without having to take in a parameter

		NULL returns handle to current file instance executing it. 
	*/
	hInst( GetModuleHandle( nullptr ) )
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup; /* special method for handling messages, this is just the setup however for the pointer to window class */
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance(); /* call for the instance*/
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName(); /* get name from windows class */
	wc.hIconSm = nullptr;
	RegisterClassEx( &wc ); /* register the class*/
}

Window::WindowClass::~WindowClass()
{
	/* just unregister the class after window is destroyed */
	UnregisterClass( wndClassName,GetInstance() );
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

/*
	getter for instance
*/
HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}


// Window Stuff

/*
	creates height / width / name
*/
Window::Window( int width,int height,const char* name ) noexcept
{

	/*
		take the width and height as basis of client region only 

		the non client border region is taken to be ignored in this sense

		https://docs.microsoft.com/en-us/windows/win32/api/windef/ns-windef-rect

		left and top = x , y of top left

		right and bottom = x and y of bottom right.
	*/
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	/*
		takes in a RECT we defined based on client size and calculates the actual size
		required based on how are styling is defined .
	*/

	AdjustWindowRect( &wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE );
	// create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(),name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		/* allow windows to determine where the window actually starts up, use system defaults here */
		CW_USEDEFAULT,CW_USEDEFAULT,wr.right - wr.left,wr.bottom - wr.top,
		nullptr,nullptr,WindowClass::GetInstance(),this /* pass pointer to window instance as our lParam here*/
	);
	// show window

	/*
		create the window with the instance defined as default ,ignored for the first call
	*/
	ShowWindow( hWnd,SW_SHOW);
}

Window::~Window()
{
	/*
		destroy window instance.
	*/
	DestroyWindow( hWnd );
}

/*
	all windows message are sent here.
*/
LRESULT CALLBACK Window::HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side

	/*
		https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-nccreate
		sent prior to WM_CREATE when window is first created.

		the l params of WM_NCCREATE are assocaited with the window just created

		here we are setting the values of windows pointers in the API backend to our specific instance
		and attaching a thunk function

	*/
	if( msg == WM_NCCREATE )
	{
		// extract ptr to window class from creation data

		/*
			CREATESTRUCTW* is the stuct type for window creation
			
		*/
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

		/*
			static case pCreate to creata a window pointer from lpCreateParams
			stores the this pointer to window created in window constructor
		*/
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// set WinAPI-managed user data to store ptr to window class

		/*
			can set data stored in windows API side
			https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptra

			can alter style, instance etc.

			we choose to alter GWLP_USERDATA, which is the user associated data with the window
			data inteded for use by the application that created the window, intially value of zero. 

			set this data to pWnd which is our data associated with creating the windown instnace in the first place.

			creates a link between our class and the window itself.
		*/

		SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(pWnd) );

		// set message proc to normal (non-setup) handler now that setup is finished

		/*
			set the GWLP_WNDPROC which is the windows procedure to deal with incoming messages 

			pass in our handle message thunk which deals with all the messages.
		*/
		SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk) );
		// forward message to window class handler

		/*
			use the pointer we have to call windows functions now
		*/
		return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	// rarely going to occur.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
	job just to invoke our handle message member function
*/
LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	// retrieve ptr to window class

	/*
		fetches the user data we defined in the setup and uses this as the window pointer instance

		so everytime this is called it fetches the specified window instance data to be 

		the class we use.
	*/
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr( hWnd,GWLP_USERDATA ));
	// forward message to window class handler
	return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
}


LRESULT Window::HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	switch( msg )
	{
	// we don't want the DefProc to handle this message because
	// we want our destructor to destroy the window, so return 0 instead of break
	// as wParam is 0 we xit the program as gResult returns a value less than 0
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd,msg,wParam,lParam );
}