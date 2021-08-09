#include <Windows.h>


/*
	returns 32 bit integer values

	must define calling convention of __stdcall

	must pass in handle to window its processing the message of

	must take the actual message structure

	wparam and lparam are message dependent. 64 bit integers
*/

/*
	meaning of result depends also on what you are handling 
*/
LRESULT CALLBACK WndProc( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
	/*
		switch case for dealing with each of the messages. 
	*/
	switch( msg )
	{
	case WM_CLOSE:
		/*
			indicate to the system that a thread has made a request to termiante

			exit code define by us

			returns wParam as the exit we can use to determine the exit code
		*/
		PostQuitMessage( 69 );
		break;
	}

	/*
		handle to window, msg structure and wide and long params.

		this is called here to deal with any default behaviour that was not handled in our current logic.
	*/

	return DefWindowProc( hWnd,msg,wParam,lParam );
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	const auto pClassName = "hw3dbutts";
	// register window class
	WNDCLASSEX wc = { 0 };
	wc.style = CS_OWNDC;
	wc.cbSize = sizeof(wc);
	/*
		example , does not close the window even after pressing X 
		does not assume anything about number of windows to act on therefore must be defined
	*/
	wc.lpfnWndProc = WndProc; // Define our custom window procedure, away from the default one to be our open handler. 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx( &wc );
	// create window instance
	HWND hWnd = CreateWindowEx(
		0,pClassName,
		"Happy Hard Window",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200,200,640,480,
		nullptr,nullptr,hInstance,nullptr
	);
	// show the damn window
	ShowWindow( hWnd,SW_SHOW );

	// message pump

	/*
		tagMSG structure

		hwnd > handle to current window
		message > actual message identifier , low word only, high word for system only

		wParam, lparam > context of the actual message, have different definitions for each message.

		time > when the message was sent 

		point > obtain the position of the cursor for every message that was received. 

		lprivate > not used at all
	*/
	MSG msg;
	BOOL gResult;
	/*
		GetMessage parameters
		https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage

		lpmsg > pointer to msg structure that receives message info from the threads message queue

		hWnd > pass in nullptr , this retrieves the message for any window belonging to the current thread
			> both window messages and thread message are processed. 
			> i.e get all messages of our .exe

		filtermin , filter max > the range of message to pull of the queue 
			> set both to 0 to process any message.

				
		return: if the message is not WM_QUIT then its non zero value, return 0 otherwise of course.
			> -1 on error
	*/

	while( (gResult = GetMessage( &msg,nullptr,0,0 )) > 0 )
	{
		
		/*
			translates virtual key message into charcter messagees 
			https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow

			takes pointer to our MSG message struct, its a const and will generate auxiliary messages
				that are passed to the queue again.
		*/
		TranslateMessage( &msg );

		/*
			pass message to the window proc related to this message 
			https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessage
		*/
		DispatchMessage( &msg );
	}

	/*
		return -1 if get message error has ocurred.
	*/
	if( gResult == -1 )
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}