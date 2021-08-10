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
#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"


class Window
{
public:
	class Exception : public ChiliException
	{
	public:
		/*
			exception takes in the line, file and HRESULT which is a default error hadnling thing for windows
		*/
		Exception( int line,const char* file,HRESULT hr ) noexcept;

		/* what overloaded of course */
		const char* what() const noexcept override;

		/* should really have been overriden */
		virtual const char* GetType() const noexcept;

		/* figure out what the error was, returns descriptive string */
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;

		/* just returns HRESULT error code */
		HRESULT GetErrorCode() const noexcept;

		/* calls translate error code with the current hr stored.*/
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
/*
* ensure window class is private
*/
private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
		static constexpr const char* wndClassName = "Chili Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window( int width,int height,const char* name );
	~Window();
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
};


// error exception helper macro

/*
	helps us find the file and line number where the exception was thrown frome 

	__LINE__ = line number

	__FILE__ = File thrown from

	just takes in some HRESULT etc, this is called when ever an error occurs 

	get last error just passes in the previous threads error.
*/

#define CHWND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )

/*
	some dont return an erro code therefore call get last to obtain what it actually was
*/

#define CHWND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )