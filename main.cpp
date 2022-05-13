#include <windows.h>

bool active=true;
bool startExecute=false;
bool pause=true;
unsigned long long tickCount;
unsigned long long lastTickCount;

bool Initialize();
bool Loop(unsigned long long dt);
bool CleanUp();

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		case WM_SIZE:
		{
			switch(wParam)
			{
				case SIZE_MINIMIZED:
					active=false;
					return 0;
				case SIZE_MAXIMIZED:
					active=true;
					return 0;
				case SIZE_RESTORED:
					active=true;
					return 0;
			}
			break;
		}
		
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=true;						// Program Is Active
			}
			else
			{
				active=false;						// Program Is No Longer Active
			}
			return 0;								// Return To The Message Loop
		}
		case WM_LBUTTONDOWN:
		{
			startExecute=true;
			pause=false;
			HDC hdc=GetDC(hwnd);
			Rectangle(hdc,10,30,100,20);
			TextOut(hdc,10,10,"executing=true",strlen("executing=true"));
			ReleaseDC(hwnd,hdc);
			return 0;
		}
		
		case WM_RBUTTONDOWN:
		{
			startExecute=false;
			pause=true;
			HDC hdc=GetDC(hwnd);
			Rectangle(hdc,10,10,100,20);
			TextOut(hdc,10,30,"executing=false",strlen("executing=false"));
			ReleaseDC(hwnd,hdc);
		}
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	Initialize();
	
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	
	pause:
		pause=false;
		while(GetMessage(&msg, NULL, 0, 0) >= 0) { /* If no error is received... */
			if(msg.message==WM_QUIT)
				goto end;
			TranslateMessage(&msg); /* Translate key codes to chars if present */
			DispatchMessage(&msg); /* Send it to WndProc */
			if(startExecute)
				break;
		}
	startExecute=false;
	
	tickCount=GetTickCount();
	lastTickCount=tickCount;
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				goto end;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			//Loop
			if (active)								// Program Active?
			{
				tickCount = GetTickCount ();				// Get The Tick Count
				Loop(tickCount - lastTickCount);
				lastTickCount = tickCount;			// Set Last Count To Current Count
				if(pause)
					goto pause;
			}
			else
			{
				WaitMessage();
			}
		}
	}
	end:
		CleanUp();
		
	return msg.wParam;
}

bool Initialize()
{
	
}

//called only when it is activated
bool Loop(unsigned long long dt)
{
	//Get Network data,etc,process and Render
	//Send CapturedData
}

bool CleanUp()
{
	
}
