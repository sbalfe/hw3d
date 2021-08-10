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
#include "ChiliWin.h" /* include this at the start of every windows api class to setup switches first  */


class Window
{
private:
	// singleton manages registration/cleanup of window class, we only require one 
	// therefore its a singleton, should be private apart from the name/instace getters

	class WindowClass
	{
	public:
		/*
			obtain name of the class
		*/
		static const char* GetName() noexcept;
		
		/*
			obtains current instance of window
		*/
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();

		/*
			remove copy constructor and assignment operator.
		*/
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;

		/*
			stores the name of the window 
		*/
		static constexpr const char* wndClassName = "Chili Direct3D Engine Window";

		/*
			the static instance , as its a singleton its unique
		*/
		static WindowClass wndClass;
		/*
			instance ID
		*/
		HINSTANCE hInst;
	};
public:
	Window( int width,int height,const char* name ) noexcept;
	~Window();
	/*
		remove copy and assignment operators.
	*/
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;
private:
	/*
		CALLBACK = __stdcall for winAPI

		Use this callback

		LRESULT __int64 returns integer essentially.
	*/
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	/*
		cannot directly use handle msg as a class method, must use the callback s
	*/
	LRESULT HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
};