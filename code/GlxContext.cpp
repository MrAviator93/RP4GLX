#include "GlxContext.hpp"

// X11
#include <X11/Xlib.h>

// OpenES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

// C++
#include <vector>
#include <stdexcept>

namespace bbx::graphics
{

namespace
{

const std::vector< EGLint > kAttributeList = { EGL_RED_SIZE,
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

const std::vector< EGLint > kContextAttributes = {
	EGL_CONTEXT_CLIENT_VERSION, 3, EGL_CONTEXT_MINOR_VERSION, 1, EGL_NONE };

} // namespace

struct GlxContext::Impl
{
	EGLDisplay display;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;
};

GlxContext::GlxContext( void* pWindowHandle )
	: m_pImpl{ std::make_unique< Impl >() }
{
	if( !pWindowHandle )
	{
		throw std::runtime_error( "ERROR: Window handle is nullptr.\n" );
	}

	m_pImpl->display = ::eglGetDisplay( EGL_DEFAULT_DISPLAY );

	// Initialize EGL
	if( !::eglInitialize( m_pImpl->display, nullptr, nullptr ) )
	{
		throw std::runtime_error( "ERROR: Couldn't initialize OpenGL ES.\n" );
	}

	EGLint numCconfig{};

	// Get an appropriate EGL frame buffer configuration
	if( !::eglChooseConfig( m_pImpl->display, kAttributeList.data(), &m_pImpl->config, 1, &numCconfig ) )
	{
		throw std::runtime_error( "ERROR: Failed to choose config.\n" );
	}

	// Get an appropriate EGL frame buffer configuration
	if( !::eglBindAPI( EGL_OPENGL_ES_API ) )
	{
		throw std::runtime_error( "ERROR: Failed to bind OpenGL ES API.\n" );
	}

	// Create an EGL window surface
	m_pImpl->surface = ::eglCreateWindowSurface(
		m_pImpl->display, m_pImpl->config, *reinterpret_cast< Window* >( pWindowHandle ), NULL );
	if( m_pImpl->surface == EGL_NO_SURFACE )
	{
		throw std::runtime_error( "ERROR: Failed to create surface.\n" );
	}

	// Create an EGL rendering context
	m_pImpl->context =
		::eglCreateContext( m_pImpl->display, m_pImpl->config, EGL_NO_CONTEXT, kContextAttributes.data() );
	if( m_pImpl->context == EGL_NO_CONTEXT )
	{
		throw std::runtime_error( "ERROR: Failed to create rendering context.\n" );
	}

	// Associate the egl-context with the egl-surface
	if( !::eglMakeCurrent( m_pImpl->display, m_pImpl->surface, m_pImpl->surface, m_pImpl->context ) )
	{
		throw std::runtime_error( "ERROR: Failed to attach EGL rendering context to EGL surfaces.\n" );
	}

	// check OpenGL vendor
	// fmt::print( "OpenGL vendor: {}\n", glGetString( GL_VENDOR ) );

	// check OpenGL vendor
	// fmt::print( "OpenGL vendor: {}\n", glGetString( GL_RENDERER ) );

	// check of OpenGL version
	// fmt::print( "OpenGL version: {}\n", glGetString( GL_VERSION ) );

	// check for OpenGL extensions
	//fmt::print("OpenGL extensions: {}\n", glGetString( GL_EXTENSIONS ) );
}

GlxContext::~GlxContext()
{
	::eglDestroyContext( m_pImpl->display, m_pImpl->context );
	::eglDestroySurface( m_pImpl->display, m_pImpl->surface );
	::eglTerminate( m_pImpl->display );
}

void GlxContext::swapBuffers()
{
	::eglSwapBuffers( m_pImpl->display, m_pImpl->surface );
}

} // namespace bbx::graphics
