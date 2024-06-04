#ifndef BBX_GRAPHICS_GLX_DEVICE_HPP__
#define BBX_GRAPHICS_GLX_DEVICE_HPP__

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

// C++
#include <cstdint>

namespace bbx::graphics
{

// OpenGL ES Device Wrapper class.
class GlxDevice
{
public:
	explicit GlxDevice( void* pWindowHandle ) noexcept( false );

	~GlxDevice();

	void viewport( int x, int y, std::uint32_t width, std::uint32_t height );
	void clearColour( float r, float g, float b, float a = 1.0f );

	void clear( std::uint32_t mask );
	void clearColourDepthStencil();

	void swapBuffers();

private:
	EGLDisplay m_display;
	EGLConfig m_config;
	EGLSurface m_surface;
	EGLContext m_context;
};

} // namespace bbx::graphics
#endif // BBX_GRAPHICS_GLX_DEVICE_HPP__
