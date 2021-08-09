#include <Windows.h>

/*
	returns integer which signals a success or failure
	https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain

	CALLBACK - use a different calling convention for this function 
		> use __stdcall > passes paramters on the stack differently.
		https://docs.microsoft.com/en-us/cpp/cpp/stdcall?view=msvc-160
*/
int CALLBACK WinMain(
	/*
		handle to current instance of our application
	*/
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,

	/*
		receive extra command line parameters from this class
		c++ receives as parsed array of tokens 
	*/

	LPSTR     lpCmdLine,

	/*
		describes how the window should be shown for creating the program, ignore for now, 
		https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	*/
	int       nCmdShow )
{
	/* defines the class name */
	const auto pClassName = "hw3dbutts";
	// register window class
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassa
	WNDCLASSEX wc = { 0 };

	/* byte size of the structure which is just sizeof(wc) in */
	wc.cbSize = sizeof( wc );

	/*
		style to apply to our class
		https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles 

		requires bitwise operation to combnine various styles

		CS_OWNDC > allocates unqiue device context for each window created in the class.
			> used for spawing multiple windows at once 
	*/
	wc.style = CS_OWNDC;

	/*
		handles all messages to the window 
			 > determines behaviour and look of the current window. 
			 
		DefWindowProc > default procedure to handle the important messages 
	*/
	wc.lpfnWndProc = DefWindowProc;

	/* extra bytes after allocation of window class structure */
	wc.cbClsExtra = 0;

	/* 
		dont need extra data stored on the windows api side of things 
	*/
	wc.cbWndExtra = 0;

	/*
		handle to the windows instace we created above 
		contains the windows procedure for the class 
	*/
	wc.hInstance = hInstance;

	/* handle to class icon, must be handle to icon resource
		if null > system provides default
	*/
	wc.hIcon = nullptr;

	/* get default cursor selection  */
	wc.hCursor = nullptr;

	/*
		used for paiting , creates a handle for HBRUSH 
		color value must be one of the types specified below.
		gdi for drawing. 

		draws the background for the window 

		we dont want this feature as we are configuring the window with direct3D 
	*/
	wc.hbrBackground = nullptr;

	/*
		creates a default menu for the window 
	*/
	wc.lpszMenuName = nullptr;

	/*
		when we create window instaces this is what we refer to it as .
	*/
	wc.lpszClassName = pClassName;

	/*
		icon configured as the little symbol to appear in our windw
	*/
	wc.hIconSm = nullptr;

	/*
		takes constant pointer to our WNDCLASSEX struct containing various data require for our windows class 
	*/
	RegisterClassEx( &wc );
	// create window instance, Ex standing for extended functionality. 

	/*
		https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa

		parameter:

		dwExStyle = 0
			extended window style created
			possible styles: https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
				- examples such as drag and drop / transparent windows etc.

		pClasName = pclassname stated above which was the registered name for our class 

		lpWindowname = our windows name

		dwStyle , apply various styles to use
			- ws_caption > window contains a title bar
			- ws_minimizebox > contains standard minimize button
			- sysmenu > has a window menu on the title bar itself

		x,y position on screen
		width, height > width and height defining the resolution of the screen.

		handle to parent = nullptr as we have a single window and not another window spawning it.
		
		handle to menu of which we dont have any therefore nullptr 

		hInstance = the instance of the module associated with this window defined in WinMain entry point
		
		custom params = nullptr as we dont currently pass any data to the window
	*/
	HWND hWnd = CreateWindowEx(
		0,pClassName,
		"Happy Hard Window",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200,200,640,480,
		nullptr,nullptr,hInstance,nullptr
	);

	/*
		if the above hwnd fails it returns null, otherwise it returns a handle to the window created.
	*/

	// show the damn window
	// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow

	/*
		takes handle to window 

		nCmdShow defines how to control the window when its shown, ignored at start

		SW_SHOW just activates the window to display in the current size and position.
	*/
	ShowWindow( hWnd,SW_SHOW );
	while( true );
	return 0;
}