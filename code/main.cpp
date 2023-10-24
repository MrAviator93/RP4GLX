/***********************************************************************
FILE MAIN.CPP CONTAINS EXPERIMENTAL CODE TO CREATE X11 WINDOW

http://manpages.ubuntu.com/manpages/bionic/en/man3/XCreateWindow.3.html
https://jan.newmarch.name/LinuxSound/Diversions/RaspberryPiOpenGL/
https://wiki.maemo.org/SimpleGL_example


1. Install the following libs: 
sudo apt-get install libx11-dev
sudo apt-get install libglew-dev - not needed !!!
sudo apt-get install mesa-utils
sudo apt install libglm-dev

2. Don't forget to link the following libs: 
-lX11 -lGL -lGLEW

FILE UTMOST REVIEW DONE ON (23.01.2021) BY ARTUR K. 
***********************************************************************/

#include "WindowX11.hpp"
#include "GLXDevice.hpp"

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

// There is an issue with fmt/format (fmtlib)
// https://github.com/fmtlib/fmt/issues/795

int main( int argc, char* argv[] )
{
	fmt::print( "Welcome to Raspbery PI 4 OpenGL!\n" );

	BEngine::CWindowX11 window;

	// Set-up OpenGL ES
	BEngine::Graphics::CGLXDevice glxDevice{ window.getWindowHandle() };
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
