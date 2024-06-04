/***********************************************************************
1. Install the following libs: 
sudo apt-get install libx11-dev
sudo apt-get install libglew-dev - not needed !!!
sudo apt-get install mesa-utils
sudo apt install libglm-dev

2. Don't forget to link the following libs: 
-lX11 -lGL -lGLEW
***********************************************************************/

#include "WindowX11.hpp"
#include "GlxDevice.hpp"

#include <fmt/format.h>
#include <date/date.h>

// Window related libs
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
#include <X11/Xatom.h>

// OpenGL related libs
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

// Maths libraries
#include <glm/vec3.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <math.h>

int main( const int argc, const char* const* const argv )
{
	fmt::print( "Welcome to Raspbery PI 4 OpenGL!\n" );

	bbx::WindowX11 window;

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
		glxDevice.swapBuffers();
	}

	return 0;
}
