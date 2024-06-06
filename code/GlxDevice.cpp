#include "GlxDevice.hpp"
#include "GlxContext.hpp"

#include <GLES3/gl3.h>

// C++
#include <string_view>

namespace bbx::graphics
{

namespace
{

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
	std::unique_ptr< GlxContext > pContext{ nullptr };

	// Triangle data
	GLuint VAO{};
	GLuint VBO{};
	GLuint shaderProgram{};
};

GlxDevice::GlxDevice( void* pWindowHandle ) noexcept( false )
	: m_pImpl{ std::make_unique< Impl >() }
{
	m_pImpl->pContext = std::make_unique< GlxContext >( pWindowHandle );

	// Create triangle
	GLfloat vertices[] = { 0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f };

	// Create and compile the vertex shader
	GLuint vertexShader = ::glCreateShader( GL_VERTEX_SHADER );
	// const GLchar *const*string,
	const char* vertexShaderSrc = kVertexShader.data();
	::glShaderSource( vertexShader, 1, &vertexShaderSrc, nullptr );
	::glCompileShader( vertexShader );
	// checkCompileErrors( vertexShader, "VERTEX" );

	// Create and compile the fragment shader
	GLuint fragmentShader = ::glCreateShader( GL_FRAGMENT_SHADER );
	const char* fragmentShaderSrc = kFragmentShader.data();
	::glShaderSource( fragmentShader, 1, &fragmentShaderSrc, nullptr );
	::glCompileShader( fragmentShader );
	// checkCompileErrors( fragmentShader, "FRAGMENT" );

	// Link shaders to create the shader program
	m_pImpl->shaderProgram = ::glCreateProgram();
	::glAttachShader( m_pImpl->shaderProgram, vertexShader );
	::glAttachShader( m_pImpl->shaderProgram, fragmentShader );
	::glLinkProgram( m_pImpl->shaderProgram );
	// checkCompileErrors( m_pImpl->shaderProgram, "PROGRAM" );

	// Delete the shaders as they're linked into our program now and no longer necessary
	::glDeleteShader( vertexShader );
	::glDeleteShader( fragmentShader );

	// Create VAO and VBO
	::glGenVertexArrays( 1, &m_pImpl->VAO );
	::glGenBuffers( 1, &m_pImpl->VBO );

	// Bind VAO
	::glBindVertexArray( m_pImpl->VAO );

	// Bind and set VBO data
	::glBindBuffer( GL_ARRAY_BUFFER, m_pImpl->VBO );
	::glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	// Configure vertex attributes
	::glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void*)0 );
	::glEnableVertexAttribArray( 0 );

	// Unbind VAO
	::glBindVertexArray( 0 );
}

GlxDevice::~GlxDevice()
{
	::glDeleteVertexArrays( 1, &m_pImpl->VAO );
	::glDeleteBuffers( 1, &m_pImpl->VBO );
	::glDeleteProgram( m_pImpl->shaderProgram );

	m_pImpl->pContext.reset();
}

void GlxDevice::viewport( int x, int y, std::uint32_t width, std::uint32_t height )
{
	::glViewport( x, y, width, height );
}

void GlxDevice::clearColour( float r, float g, float b, float a )
{
	::glClearColor( r, g, b, a );
}

void GlxDevice::clearColourDepthStencil()
{
	::glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void GlxDevice::render()
{
	::glUseProgram( m_pImpl->shaderProgram );
	::glBindVertexArray( m_pImpl->VAO );
	::glDrawArrays( GL_TRIANGLES, 0, 3 );
	::glBindVertexArray( 0 );
}

void GlxDevice::swapBuffers()
{
	m_pImpl->pContext->swapBuffers();
}

} //namespace bbx::graphics
