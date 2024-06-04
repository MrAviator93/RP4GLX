#include "GlxDevice.hpp"

#include <X11/Xlib.h>
#include <fmt/format.h>
#include <date/date.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

// C++
#include <iostream>
#include <stdexcept>

namespace bbx::graphics
{

namespace
{

static const EGLint attribute_list[] = { EGL_RED_SIZE,
										 8,
										 EGL_GREEN_SIZE,
										 8,
										 EGL_BLUE_SIZE,
										 8,
										 EGL_ALPHA_SIZE,
										 8,
										 EGL_SURFACE_TYPE,
										 EGL_WINDOW_BIT,
										 EGL_NONE };

static const EGLint context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_CONTEXT_MINOR_VERSION, 1, EGL_NONE };

} // namespace

struct GlxDevice::Impl
{
	EGLDisplay display;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;
};

GlxDevice::GlxDevice( void* pWindowHandle ) noexcept( false )
	: m_pImpl{ std::make_unique< Impl >() }
{
	// All this initialization code
	// could be moved to a new class
	// CGLXContext
	// The same applies also to Windows version (3DExplorer) project!!!
	// And then have 3 different context cpp files:
	// GLXContext.hpp followed by 3 cpp files:
	// GLXContextWin.cpp, GLXContextLin.cpp, GLXContextRPi4.cpp
	// 1. Windows, 2. Linux (General) 3. Raspberry Pi 4

	if( !pWindowHandle )
	{
		throw std::runtime_error( "ERROR: Window handle is nullptr.\n" );
	}

	m_pImpl->display = eglGetDisplay( EGL_DEFAULT_DISPLAY );

	// Initialize EGL
	if( !eglInitialize( m_pImpl->display, nullptr, nullptr ) )
	{
		throw std::runtime_error( "ERROR: Couldn't initialize OpenGL ES.\n" );
	}

	EGLint numCconfig{};

	// Get an appropriate EGL frame buffer configuration
	if( !eglChooseConfig( m_pImpl->display, attribute_list, &m_pImpl->config, 1, &numCconfig ) )
	{
		throw std::runtime_error( "ERROR: Failed to choose config.\n" );
	}

	// Get an appropriate EGL frame buffer configuration
	if( !eglBindAPI( EGL_OPENGL_ES_API ) )
	{
		throw std::runtime_error( "ERROR: Failed to bind OpenGL ES API.\n" );
	}

	// Create an EGL window surface
	m_pImpl->surface = eglCreateWindowSurface(
		m_pImpl->display, m_pImpl->config, *reinterpret_cast< Window* >( pWindowHandle ), NULL );
	if( m_pImpl->surface == EGL_NO_SURFACE )
	{
		throw std::runtime_error( "ERROR: Failed to create surface.\n" );
	}

	// Create an EGL rendering context
	m_pImpl->context = eglCreateContext( m_pImpl->display, m_pImpl->config, EGL_NO_CONTEXT, context_attributes );
	if( m_pImpl->context == EGL_NO_CONTEXT )
	{
		throw std::runtime_error( "ERROR: Failed to create rendering context.\n" );
	}

	// Associate the egl-context with the egl-surface
	if( !eglMakeCurrent( m_pImpl->display, m_pImpl->surface, m_pImpl->surface, m_pImpl->context ) )
	{
		throw std::runtime_error( "ERROR: Failed to attach EGL rendering context to EGL surfaces.\n" );
	}

	// check OpenGL vendor
	fmt::print( "OpenGL vendor: {}\n", glGetString( GL_VENDOR ) );

	// check OpenGL vendor
	fmt::print( "OpenGL vendor: {}\n", glGetString( GL_RENDERER ) );

	// check of OpenGL version
	fmt::print( "OpenGL version: {}\n", glGetString( GL_VERSION ) );

	// check for OpenGL extensions
	//fmt::print("OpenGL extensions: {}\n", glGetString( GL_EXTENSIONS ) );
}

GlxDevice::~GlxDevice()
{
	eglDestroyContext( m_pImpl->display, m_pImpl->context );
	eglDestroySurface( m_pImpl->display, m_pImpl->surface );
	eglTerminate( m_pImpl->display );
}

void GlxDevice::viewport( int x, int y, std::uint32_t width, std::uint32_t height )
{
	glViewport( x, y, width, height );
}

void GlxDevice::clearColour( float r, float g, float b, float a )
{
	glClearColor( r, g, b, a );
}

void GlxDevice::clear( std::uint32_t mask )
{
	glClear( mask );
}

void GlxDevice::clearColourDepthStencil()
{
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void GlxDevice::swapBuffers()
{
	eglSwapBuffers( m_pImpl->display, m_pImpl->surface );
}

} //namespace bbx::graphics
