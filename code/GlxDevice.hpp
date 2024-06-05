#ifndef BBX_GRAPHICS_GLX_DEVICE_HPP__
#define BBX_GRAPHICS_GLX_DEVICE_HPP__

// C++
#include <memory>
#include <cstdint>

namespace bbx::graphics
{

class GlxDevice
{
	struct Impl;

public:
	explicit GlxDevice( void* pWindowHandle ) noexcept( false );

	~GlxDevice();

	void viewport( int x, int y, std::uint32_t width, std::uint32_t height );
	void clearColour( float r, float g, float b, float a = 1.0f );

	void render();

	void clear( std::uint32_t mask );
	void clearColourDepthStencil();

	void swapBuffers();

private:
	std::unique_ptr< Impl > m_pImpl;
};

} // namespace bbx::graphics
#endif // BBX_GRAPHICS_GLX_DEVICE_HPP__
