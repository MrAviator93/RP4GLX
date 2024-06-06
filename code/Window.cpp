#include "Window.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
#include <X11/Xatom.h>

// C++
#include <iostream>
#include <stdexcept>

namespace bbx::window
{

struct Window::Impl
{
	_XDisplay* pDisplay{ nullptr };
	void* pWindowHandle{ nullptr }; // Window
	bool running{ false };
	std::uint64_t wmDeleteMessage;
};

Window::Window()
	: m_pImpl{ std::make_unique< Impl >() }
{
	createWindow();
}

Window::~Window()
{
	// TODO: Destroy all the stuff ...
}

void Window::createWindow() noexcept( false )
{
	auto pDisplay = ::XOpenDisplay( reinterpret_cast< char* >( 0 ) );
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

	::XSetWindowAttributes windowAttribs;
	windowAttribs.border_pixel = BlackPixel( pDisplay, screenID );
	windowAttribs.background_pixel = BlackPixel( pDisplay, screenID );
	windowAttribs.override_redirect = true;
	windowAttribs.event_mask = ExposureMask;

	// CWBackPixel | CWColormap | CWBorderPixel | CWEventMask
	std::uint32_t mask = CWBackPixel | CWBorderPixel;

	::Window w = ::XCreateWindow(
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
	::Atom wmDelMsg = ::XInternAtom( pDisplay, "WM_DELETE_WINDOW", false );
	::XSetWMProtocols( pDisplay, w, &wmDelMsg, 1 );
	m_pImpl->wmDeleteMessage = wmDelMsg;

	::XSelectInput( pDisplay, w, KeyPressMask | KeyReleaseMask | KeymapStateMask );

	::XMapWindow( pDisplay, w );
	::XMapRaised( pDisplay, w );

	::XStoreName( pDisplay, w, "Raspberry Pi 4 & 5 X11 OpenGL ES" );

	auto screen1 = ScreenOfDisplay( pDisplay, 0 );

	int x = ( screen1->width - 1200 ) / 2;
	int y = ( screen1->height - 800 ) / 2;

	::XMoveWindow( pDisplay, w, x, y );

	m_pImpl->pDisplay = pDisplay;

	// w is uint64!!!
	std::uint64_t* a = new std::uint64_t{ static_cast< std::uint64_t >( w ) };
	m_pImpl->pWindowHandle = reinterpret_cast< void* >( a );
	m_pImpl->running = true;
}

void* Window::handle() const
{
	return m_pImpl->pWindowHandle;
}

bool Window::running() const
{
	return m_pImpl->running;
}

std::uint32_t Window::clientWidth() const
{
	::XWindowAttributes gwa;
	::XGetWindowAttributes( m_pImpl->pDisplay, *reinterpret_cast< ::Window* >( m_pImpl->pWindowHandle ), &gwa );
	return static_cast< std::uint32_t >( gwa.width );
}

std::uint32_t Window::clientHeight() const
{
	::XWindowAttributes gwa;
	::XGetWindowAttributes( m_pImpl->pDisplay, *reinterpret_cast< ::Window* >( m_pImpl->pWindowHandle ), &gwa );
	return static_cast< std::uint32_t >( gwa.height );
}

void Window::update()
{
	::XEvent event;
	::XNextEvent( m_pImpl->pDisplay, &event );

	switch( event.type )
	{
	case ClientMessage: {
		// We need to intercept close "X" event
		if( event.xclient.data.l[ 0 ] == static_cast< long int >( m_pImpl->wmDeleteMessage ) )
		{
			m_pImpl->running = false;
			break;
		}
		break;
	}
	case KeymapNotify: {
		::XRefreshKeyboardMapping( &event.xmapping );
		break;
	}
	case KeyPress: {
		if( event.xkey.keycode == 9 ) // ESP
		{
			m_pImpl->running = false;
		}
		break;
	}
	case KeyRelease: {
		break;
	}
	}
}

} // namespace bbx::window
