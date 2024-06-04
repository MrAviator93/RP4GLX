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
#include <string_view>

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

constexpr std::string_view kVertexShader = R"(
#version 310 es
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

constexpr std::string_view kFragmentShader = R"(
#version 310 es
precision mediump float;
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

} // namespace

struct GlxDevice::Impl
{
	EGLDisplay display;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	// Triangle data
	GLuint VAO{};
	GLuint VBO{};
	GLuint shaderProgram{};
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

	// Create triangle
	GLfloat vertices[] = { 0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f };

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
	// const GLchar *const*string,
	const char* vertexShaderSrc = kVertexShader.data();
	glShaderSource( vertexShader, 1, &vertexShaderSrc, nullptr );
	glCompileShader( vertexShader );
	// checkCompileErrors( vertexShader, "VERTEX" );

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	const char* fragmentShaderSrc = kFragmentShader.data();
	glShaderSource( fragmentShader, 1, &fragmentShaderSrc, nullptr );
	glCompileShader( fragmentShader );
	// checkCompileErrors( fragmentShader, "FRAGMENT" );

	// Link shaders to create the shader program
	m_pImpl->shaderProgram = glCreateProgram();
	glAttachShader( m_pImpl->shaderProgram, vertexShader );
	glAttachShader( m_pImpl->shaderProgram, fragmentShader );
	glLinkProgram( m_pImpl->shaderProgram );
	// checkCompileErrors( m_pImpl->shaderProgram, "PROGRAM" );

	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );

	// Create VAO and VBO
	glGenVertexArrays( 1, &m_pImpl->VAO );
	glGenBuffers( 1, &m_pImpl->VBO );

	// Bind VAO
	glBindVertexArray( m_pImpl->VAO );

	// Bind and set VBO data
	glBindBuffer( GL_ARRAY_BUFFER, m_pImpl->VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	// Configure vertex attributes
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void*)0 );
	glEnableVertexAttribArray( 0 );

	// Unbind VAO
	glBindVertexArray( 0 );
}

GlxDevice::~GlxDevice()
{
	glDeleteVertexArrays( 1, &m_pImpl->VAO );
	glDeleteBuffers( 1, &m_pImpl->VBO );
	glDeleteProgram( m_pImpl->shaderProgram );

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

void GlxDevice::render()
{
	glUseProgram( m_pImpl->shaderProgram );
	glBindVertexArray( m_pImpl->VAO );
	glDrawArrays( GL_TRIANGLES, 0, 3 );
	glBindVertexArray( 0 );
}

void GlxDevice::swapBuffers()
{
	eglSwapBuffers( m_pImpl->display, m_pImpl->surface );
}

} //namespace bbx::graphics
