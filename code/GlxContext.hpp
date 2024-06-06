#ifndef BBX_GRAPHICS_GLX_CONTEXT_HPP__
#define BBX_GRAPHICS_GLX_CONTEXT_HPP__

// C++
#include <memory>

namespace bbx::graphics
{

class GlxContext
{
	struct Impl;

public:
	GlxContext( void* pWindowHandle );

	~GlxContext();

	void swapBuffers();

private:
	std::unique_ptr< Impl > m_pImpl;
};

} // namespace bbx::graphics
#endif // BBX_GRAPHICS_GLX_CONTEXT_HPP__
