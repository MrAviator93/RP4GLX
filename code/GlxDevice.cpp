#include "GlxDevice.hpp"
#include "GlxContext.hpp"

#include <GLES3/gl3.h>

namespace bbx::graphics
{

struct GlxDevice::Impl
{
	std::unique_ptr< GlxContext > pContext{ nullptr };
};

GlxDevice::GlxDevice( void* pWindowHandle ) noexcept( false )
	: m_pImpl{ std::make_unique< Impl >() }
{
	m_pImpl->pContext = std::make_unique< GlxContext >( pWindowHandle );
}

GlxDevice::~GlxDevice()
{
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

void GlxDevice::swapBuffers()
{
	m_pImpl->pContext->swapBuffers();
}

} //namespace bbx::graphics
