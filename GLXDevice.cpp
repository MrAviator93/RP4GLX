
#include "GLXDevice.hpp"

#include <X11/Xlib.h>
#include <fmt/format.h>
#include <date/date.h>

#include <iostream>
#include <stdexcept>

namespace BEngine::Graphics 
{
    static const EGLint attribute_list[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT,
        EGL_NONE
    };

    static const EGLint context_attributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 1,
        EGL_NONE
    };

    CGLXDevice::CGLXDevice( void* pWindowHandle ) noexcept ( false )
    {
        // All this initialization code
        // could be moved to a new class
        // CGLXContext
        // The same applies also to Windows version (3DExplorer) project!!!
        // And then have 3 different context cpp files:
        // GLXContext.hpp followed by 3 cpp files:
        // GLXContextWin.cpp, GLXContextLin.cpp, GLXContextRPi4.cpp
        // 1. Windows, 2. Linux (General) 3. Raspberry Pi 4

        if ( !pWindowHandle )
        {
            throw std::runtime_error( "ERROR: Window handle is nullptr.\n" );
        }

        m_display = eglGetDisplay( EGL_DEFAULT_DISPLAY );

        // Initialize EGL
        if ( !eglInitialize( m_display, nullptr, nullptr ) )
        {
            throw std::runtime_error( "ERROR: Couldn't initialize OpenGL ES.\n" );
        }

        EGLint num_config {};

        // Get an appropriate EGL frame buffer configuration
        if ( !eglChooseConfig( m_display, attribute_list, &m_config, 1, &num_config ) )
        {
            throw std::runtime_error( "ERROR: Failed to choose config.\n" );
        }

        // Get an appropriate EGL frame buffer configuration 
        if ( !eglBindAPI( EGL_OPENGL_ES_API ) )
        {
            throw std::runtime_error( "ERROR: Failed to bind OpenGL ES API.\n" );
        }

        // Create an EGL window surface
        m_surface = eglCreateWindowSurface ( m_display, m_config, *reinterpret_cast< Window* >( pWindowHandle ), NULL );
        if ( m_surface == EGL_NO_SURFACE )
        {
            throw std::runtime_error( "ERROR: Failed to create surface.\n" );
        }

        // Create an EGL rendering context
        m_context = eglCreateContext( m_display, m_config, EGL_NO_CONTEXT, context_attributes );
        if ( m_context == EGL_NO_CONTEXT )
        {
            throw std::runtime_error( "ERROR: Failed to create rendering context.\n" );
        }

        // Associate the egl-context with the egl-surface
        if ( !eglMakeCurrent( m_display, m_surface, m_surface, m_context ) )
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

    CGLXDevice::~CGLXDevice()
    {
        eglDestroyContext( m_display, m_context );
        eglDestroySurface( m_display, m_surface );
        eglTerminate( m_display );
    }

    void CGLXDevice::viewport(int x, int y, std::uint32_t width, std::uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void CGLXDevice::clearColour(float r, float g, float b, float a )
    {
        glClearColor(r, g, b, a);
    }

    void CGLXDevice::clear( std::uint32_t mask )
    {
        glClear( mask );
    }

    void CGLXDevice::clearColourDepthStencil()
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void CGLXDevice::swapBuffers()
    {
        eglSwapBuffers( m_display, m_surface );
    }

} //namespace BEngine::Graphics 
