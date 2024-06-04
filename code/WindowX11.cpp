#include "WindowX11.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
#include <X11/Xatom.h>

// C++
#include <iostream>
#include <stdexcept>

namespace bbx
{

WindowX11::WindowX11()
{
	createWindow();
}

WindowX11::~WindowX11()
{
	// TODO: Destroy all the stuff ...
}

void WindowX11::createWindow() noexcept( false )
{
	pDisplay = XOpenDisplay( reinterpret_cast< char* >( 0 ) );
	if( !pDisplay )
	{
		throw std::runtime_error( "ERROR: Failed to open display." );
	}

	auto pScreen = DefaultScreenOfDisplay( pDisplay );
	if( !pScreen )
	{
		throw std::runtime_error( "ERROR: Failed to gather screen." );
	}

	int screenID = DefaultScreen( pDisplay );
	auto rootWindow = DefaultRootWindow( pDisplay );

	XSetWindowAttributes windowAttribs;
	windowAttribs.border_pixel = BlackPixel( pDisplay, screenID );
	windowAttribs.background_pixel = BlackPixel( pDisplay, screenID );
	windowAttribs.override_redirect = true;
	windowAttribs.event_mask = ExposureMask;

	// CWBackPixel | CWColormap | CWBorderPixel | CWEventMask
	std::uint32_t mask = CWBackPixel | CWBorderPixel;

	Window w = XCreateWindow(
		pDisplay, // Display* - Specifies the connection to the X server.
		rootWindow, // Window - Specifies the parent window.
		0, // X - coordinate of top-left outside corner of the window's borders and are relative to the inside of the parent window's borders
		0, // Y - coordinate of top-left outside corner of the window's borders and are relative to the inside of the parent window's borders
		1200, // Width
		800, // Height
		0, // Border width - Specifies the width of the created window's border in pixels.
		CopyFromParent, // Specifies the window's depth.
		InputOutput, // Specifies the created window's class. You can pass InputOutput, InputOnly, or CopyFromParent.
		CopyFromParent, // Visual* - Specifies the visual type.
		mask, // Value mask - Specifies which window attributes are defined in the attributes argument.
		&windowAttribs // XSetWindowAttributes* - Specifies the structure from which the values (as specified by the value mask) are to be taken.
	);

	// Register interest in the delete window message (redicrect)
	Atom wmDelMsg = XInternAtom( pDisplay, "WM_DELETE_WINDOW", false );
	XSetWMProtocols( pDisplay, w, &wmDelMsg, 1 );
	wmDeleteMessage = wmDelMsg;

	XSelectInput( pDisplay, w, KeyPressMask | KeyReleaseMask | KeymapStateMask );

	XMapWindow( pDisplay, w );
	XMapRaised( pDisplay, w );

	XStoreName( pDisplay, w, "Raspberry Pi 4 X11 OpenGL ES" );

	auto screen1 = ScreenOfDisplay( pDisplay, 0 );

	int x = ( screen1->width - 1200 ) / 2;
	int y = ( screen1->height - 800 ) / 2;

	XMoveWindow( pDisplay, w, x, y );

	// w is uint64!!!
	std::uint64_t* a = new std::uint64_t{ static_cast< std::uint64_t >( w ) };
	m_pWindowHandle = reinterpret_cast< void* >( a );
	m_running = true;
}

std::uint32_t WindowX11::clientWidth() const
{
	XWindowAttributes gwa;
	XGetWindowAttributes( pDisplay, *reinterpret_cast< Window* >( m_pWindowHandle ), &gwa );
	return static_cast< std::uint32_t >( gwa.width );
}

std::uint32_t WindowX11::clientHeight() const
{
	XWindowAttributes gwa;
	XGetWindowAttributes( pDisplay, *reinterpret_cast< Window* >( m_pWindowHandle ), &gwa );
	return static_cast< std::uint32_t >( gwa.height );
}

void WindowX11::update()
{
	XEvent event;
	XNextEvent( pDisplay, &event );

	switch( event.type )
	{
	case ClientMessage: {
		// We need to intercept close "X" event
		if( event.xclient.data.l[ 0 ] == static_cast< long int >( wmDeleteMessage ) )
		{
			m_running = false;
			break;
		}
		break;
	}
	case KeymapNotify: {
		XRefreshKeyboardMapping( &event.xmapping );
		break;
	}
	case KeyPress: {
		if( event.xkey.keycode == 9 ) // ESP
		{
			m_running = false;
		}
		break;
	}
	case KeyRelease: {
		break;
	}
	}
}

} // namespace bbx
