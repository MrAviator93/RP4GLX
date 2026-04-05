#include "GlxRender.hpp"
#include "GlxDevice.hpp"

#include <GLES3/gl3.h>

// C++
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

namespace bbx::graphics
{

namespace
{

std::optional< std::string > loadShaderSource( const std::filesystem::path& shaderPath )
{
	std::ifstream file( shaderPath, std::ios::in | std::ios::binary );
	if( !file )
	{
		return std::nullopt;
	}

	std::ostringstream content;
	content << file.rdbuf();
	return content.str();
}

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

	const std::filesystem::path shaderDir = "data/shaders";
	const auto vertexShaderSource = loadShaderSource( shaderDir / "triangle.vert" );
	if( !vertexShaderSource )
	{
		throw std::runtime_error( "Failed to load vertex shader: " + ( shaderDir / "triangle.vert" ).string() );
	}

	GLuint vertexShader = ::glCreateShader( GL_VERTEX_SHADER );
	const char* vertexShaderSrcCStr = vertexShaderSource->c_str();
	::glShaderSource( vertexShader, 1, &vertexShaderSrcCStr, nullptr );
	::glCompileShader( vertexShader );

	const auto fragmentShaderSource = loadShaderSource( shaderDir / "triangle.frag" );
	if( !fragmentShaderSource )
	{
		throw std::runtime_error( "Failed to load fragment shader: " + ( shaderDir / "triangle.frag" ).string() );
	}

	GLuint fragmentShader = ::glCreateShader( GL_FRAGMENT_SHADER );
	const char* fragmentShaderSrcCStr = fragmentShaderSource->c_str();
	::glShaderSource( fragmentShader, 1, &fragmentShaderSrcCStr, nullptr );
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
