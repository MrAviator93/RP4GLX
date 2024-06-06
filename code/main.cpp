#include "Window.hpp"
#include "GlxDevice.hpp"

// C++
#include <vector>
#include <string_view>

int main( const int argc, const char* const* const argv )
{
	using Args = std::vector< std::string_view >;
	[[maybe_unused]] const Args args{ argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) };

	bbx::window::Window window;

	// Set-up OpenGL ES
	bbx::graphics::GlxDevice glxDevice{ window.handle() };
	glxDevice.viewport( 0, 0, 1200, 800 );
	glxDevice.clearColourDepthStencil();
	glxDevice.clearColour( 0.24, 0.14, 0.24, 1.0f );

	while( window.running() )
	{
		window.update();
		glxDevice.clearColourDepthStencil();
		glxDevice.clearColour( 0.24, 0.14, 0.24, 1.0f );
		glxDevice.render();
		glxDevice.swapBuffers();
	}

	return 0;
}
