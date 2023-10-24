/***********************************************************************
FILE GLXDEVICE.HPP CONTAINS SIMPLE GLX-EGL DEVICE IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (24.01.2021) BY ARTUR K. 
***********************************************************************/

#ifndef BENGINE_GRAPHICS_GLX_DEVICE_HPP__
#define BENGINE_GRAPHICS_GLX_DEVICE_HPP__

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#include <cstdint>

namespace BEngine::Graphics 
{
    //-----------------------------------------------------------------------
	// OPEN GLX DEVICE
	//
	// OpenGL ES Device Wrapper class.
    //-----------------------------------------------------------------------
    class CGLXDevice
    {
        public:
            explicit CGLXDevice( void* pWindowHandle ) noexcept ( false );

            ~CGLXDevice();

            void viewport(int x, int y, std::uint32_t width, std::uint32_t height);
            void clearColour(float r, float g, float b, float a = 1.0f);

            void clear( std::uint32_t mask );
            void clearColourDepthStencil();

            void swapBuffers();

        private:
            EGLDisplay m_display;
            EGLConfig m_config;
            EGLSurface m_surface;
            EGLContext m_context;
    };

} //namespace BEngine::Graphics 
#endif // BENGINE_GRAPHICS_GLX_DEVICE_HPP__
