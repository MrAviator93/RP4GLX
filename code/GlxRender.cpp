#include "GlxRender.hpp"
#include "GlxDevice.hpp"

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

struct GlxRender::Impl
{
	GlxDevice& device;
	GLuint VAO{};
	GLuint VBO{};
	GLuint shaderProgram{};

	explicit Impl( GlxDevice& device ) noexcept
		: device{ device }
	{ }
};

GlxRender::GlxRender( GlxDevice& device ) noexcept( false )
	: m_pImpl{ std::make_unique< Impl >( device ) }
{
	GLfloat vertices[] = { 0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f };

	GLuint vertexShader = ::glCreateShader( GL_VERTEX_SHADER );
	const char* vertexShaderSrc = kVertexShader.data();
	::glShaderSource( vertexShader, 1, &vertexShaderSrc, nullptr );
	::glCompileShader( vertexShader );

	GLuint fragmentShader = ::glCreateShader( GL_FRAGMENT_SHADER );
	const char* fragmentShaderSrc = kFragmentShader.data();
	::glShaderSource( fragmentShader, 1, &fragmentShaderSrc, nullptr );
	::glCompileShader( fragmentShader );

	m_pImpl->shaderProgram = ::glCreateProgram();
	::glAttachShader( m_pImpl->shaderProgram, vertexShader );
	::glAttachShader( m_pImpl->shaderProgram, fragmentShader );
	::glLinkProgram( m_pImpl->shaderProgram );

	::glDeleteShader( vertexShader );
	::glDeleteShader( fragmentShader );

	::glGenVertexArrays( 1, &m_pImpl->VAO );
	::glGenBuffers( 1, &m_pImpl->VBO );

	::glBindVertexArray( m_pImpl->VAO );
	::glBindBuffer( GL_ARRAY_BUFFER, m_pImpl->VBO );
	::glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	::glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void*)0 );
	::glEnableVertexAttribArray( 0 );

	::glBindVertexArray( 0 );
}

GlxRender::~GlxRender()
{
	::glDeleteVertexArrays( 1, &m_pImpl->VAO );
	::glDeleteBuffers( 1, &m_pImpl->VBO );
	::glDeleteProgram( m_pImpl->shaderProgram );
}

void GlxRender::render()
{
	::glUseProgram( m_pImpl->shaderProgram );
	::glBindVertexArray( m_pImpl->VAO );
	::glDrawArrays( GL_TRIANGLES, 0, 3 );
	::glBindVertexArray( 0 );
}

} // namespace bbx::graphics
