#ifndef BBX_GRAPHICS_GLX_RENDER_HPP__
#define BBX_GRAPHICS_GLX_RENDER_HPP__

// C++
#include <memory>

namespace bbx::graphics
{

class GlxDevice;

class GlxRender
{
	struct Impl;

public:
	explicit GlxRender( GlxDevice& device ) noexcept( false );

	~GlxRender();

	void render();

private:
	std::unique_ptr< Impl > m_pImpl;
};

} // namespace bbx::graphics
#endif // BBX_GRAPHICS_GLX_RENDER_HPP__
