#include <windows.h>

#include <iostream>
#include <stdio.h>

#include <GL3/gl3w.h>
#include <GL/freeglut.h>

#include "Renderer.h"
#include "Resource.h"

const int MAX_LOADSTRING = 100;

namespace
{
   HINSTANCE hInst;                     // instance of the application
   HWND hWnd;                           // window
   HDC hDC;                             // drawing context
   HGLRC hRC;                           // rendering context

   char szTitle[MAX_LOADSTRING];       // ihe title bar text
   char szWindowClass[MAX_LOADSTRING]; // the main window class name
}

// Forward declarations
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
extern "C" LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
extern "C" INT_PTR CALLBACK About( HWND, UINT, WPARAM, LPARAM );
void EnableOpenGL( HWND hWnd, HDC* hDC, HGLRC* hRC );
void DisableOpenGL( HWND hWnd, HDC hDC, HGLRC hRC );
void InitGlew();

// display callback
void display()
{
   glClear( GL_COLOR_BUFFER_BIT );
   glutSwapBuffers();
}

int WinMain( HINSTANCE hInstance,     // instance of the application
             HINSTANCE hPrevInstance, // previous instance (NULL)
             LPTSTR    lpCmdLine,     // command line
             int       nCmdShow )     // how to display window
{
// MessageBox( 0, "Hello, Windows", "MinGW Test Program", MB_OK );

   strcpy( szTitle, "gcctest" );
   strcpy( szWindowClass, "GCCTEST" );

   ATOM result = MyRegisterClass( hInstance );
   if (result == 0)
   {
      DWORD error = GetLastError();
      HRESULT hr = HRESULT_FROM_WIN32( error );
      printf( "MyRegisterClass failed with error code %lu, hr %lx\n", error, hr );
      return FALSE;
   }

   // Perform application initialization
   if (!InitInstance( hInstance, nCmdShow ))
   {
      printf( "InitInstance failed\n" );
      return FALSE;
   }

   // Shortcut keys
   HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_GLTEST ) );

   EnableOpenGL( hWnd, &hDC, &hRC );
   gl3wInit();

   Renderer rend;

   timeBeginPeriod( 1 );
   long start_ms = timeGetTime();

   // Main message loop
   bool quit = false;
   MSG msg;
   while (!quit)
   {
      if (PeekMessage( &msg,   // where to store message
                       NULL,   // any window we own
                       0,      // min message value
                       0,      // max message value (0, 0) means all)
                       PM_REMOVE ))
      {
         // If TranslateAccelerator() succeeds, it's because it translated this
         // message to a different one, so we should ignore it.
         if (!TranslateAccelerator( msg.hwnd, hAccelTable, &msg ))
         {
            if (msg.message == WM_QUIT) quit = true;
            TranslateMessage( &msg ); // may post character message if this was a key message
            DispatchMessage( &msg );  // send to window procedure (we specified this to be WndProc)
         }
      }
      else
      {
         rend.Poll( timeGetTime() - start_ms );
         SwapBuffers( hDC );
      }
   }

   timeEndPeriod( 1 );

   // "If the function succeeds, terminating when it receives a
   // WM_QUIT message, it should return the exit value contained in
   // that message's wParam parameter."
   return (int) msg.wParam;
} 

void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
   PIXELFORMATDESCRIPTOR pfd;
   int iFormat;

   // get the device context (DC)
   *hDC = GetDC( hWnd );

   // set the pixel format for the DC
   ZeroMemory( &pfd, sizeof( pfd ) );
   pfd.nSize = sizeof( pfd );
   pfd.nVersion = 1;
   pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 24;
   pfd.cDepthBits = 16;
   pfd.iLayerType = PFD_MAIN_PLANE;
   iFormat = ChoosePixelFormat( *hDC, &pfd );
   SetPixelFormat( *hDC, iFormat, &pfd );

   // create and enable the render context (RC)
   *hRC = wglCreateContext( *hDC );
   if (!hRC)
   {
      printf( "EnableOpenGL failed\n" );
   }
   wglMakeCurrent( *hDC, *hRC );
}

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
   wglMakeCurrent( NULL, NULL );
   wglDeleteContext( hRC );
   ReleaseDC( hWnd, hDC );
}

ATOM MyRegisterClass( HINSTANCE hInstance )
{
   WNDCLASS wc;
   ZeroMemory( &wc, sizeof( wc ) );

   wc.style          = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc    = WndProc; // message processing function
   wc.hInstance      = hInstance;
   wc.lpszClassName  = szWindowClass;

   // RegisterClass registers a window class for CreateWindow.
   return RegisterClass( &wc );
}

// ---------------------------------------------------------------------
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow( szWindowClass,       // matches call to RegisterClassEx
                        szTitle,             // title
                        WS_OVERLAPPEDWINDOW, // style
                        CW_USEDEFAULT,       // x
                        0,                   // y (ignored)
                        768,                 // width
                        768,                 // height
                        NULL,                // parent
                        NULL,                // handle to menu
                        hInstance,           // our instance
                        NULL );              // pointer to extra parameters

   if (!hWnd)
   {
      DWORD error = GetLastError();
      HRESULT hr = HRESULT_FROM_WIN32( error );
      printf( "InitInstance failed with error code %lu, hr %lx\n", error, hr );
      return FALSE;
   }

   ShowWindow( hWnd, nCmdShow );
   UpdateWindow( hWnd );

   return TRUE;
}

// ---------------------------------------------------------------------
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_KEYDOWN  - keyboard messages
//  WM_PAINT    - paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
extern "C"
LRESULT CALLBACK WndProc( HWND hWnd,      // window handle
                          UINT message,   // message
                          WPARAM wParam,  // more message info
                          LPARAM lParam ) // yet more message info
{
   int wmId /* , wmEvent */ ;

   switch (message)
   {
      case WM_COMMAND:
      {
         wmId    = LOWORD(wParam); // menu ID or accel ID
         // wmEvent = HIWORD(wParam); // whether it's menu or accel
         // Parse the menu selections:
         switch (wmId)
         {
            case IDM_ABOUT:
            {
               DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
               break;
            }

            case IDM_EXIT:
            {
               DisableOpenGL( hWnd, hDC, hRC );
               DestroyWindow( hWnd );
               break;
            }

            default:
               // Pass it up the chain
               return DefWindowProc( hWnd, message, wParam, lParam );
         }
         break;
      }

      case WM_KEYDOWN:
      {
         switch (wParam)
         {
            case VK_ESCAPE:
            {
               SendMessage( hWnd, WM_COMMAND, IDM_EXIT, 0 );
               break;
            }
            default:
               break;
         }
         break;
      }

      case WM_PAINT:
      {
         PAINTSTRUCT ps;
         /* HDC hdc = */ BeginPaint( hWnd, &ps );
         // Add any drawing code here
         EndPaint( hWnd, &ps );
         break;
      }
      case WM_DESTROY:
         PostQuitMessage( 0 );  // sends WM_QUIT
         break;

      default:
         return DefWindowProc( hWnd, message, wParam, lParam );
   }
   return 0;
}

// ---------------------------------------------------------------------

// Message handler for about box.
// Returns whether it handled the message
extern "C" 
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
   UNREFERENCED_PARAMETER(lParam);
   switch (message)
   {
      case WM_INITDIALOG:
         return (INT_PTR) TRUE; // set keyboard focus

      case WM_COMMAND:
         if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
         {
            EndDialog( hDlg, LOWORD( wParam ) );
            return (INT_PTR) TRUE;
         }
         break;
   }
   return (INT_PTR) FALSE;
}

#if 0
int main( int argc, char **argv )
{
   glutInit( &argc, argv );
   // request version 4.1
   glutInitContextVersion( 4, 1 );
   // core profile 
   glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
   glutInitContextProfile( GLUT_CORE_PROFILE );

   // double buffered, depth, color w/ alpha
   glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
   glutInitWindowSize( 640, 480 );
   glutCreateWindow( "OpenGL Test" );

   if (gl3wInit()) {
      std::cerr << "Failed to initialize." << std::endl;
      return -1;
   }
   if (!gl3wIsSupported( 4, 1 )) {
      std::cerr << "OpenGL 4.1 not supported" << std::endl;
      return -1;
   }

	std::cout << "OpenGL " << glGetString( GL_VERSION ) << "\nGLSL "
             << glGetString( GL_SHADING_LANGUAGE_VERSION );
   glClearColor( 0, 0, 0, 0 );
   glutDisplayFunc( display );
   glutMainLoop();
   return 0;
}
#endif
