/***********************************************************************
FILE WINDOWX11.HPP CONTAINS LINUX OPERATING SYSTEM WINDOW IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (24.01.2021) BY ARTUR K. 
***********************************************************************/

#ifndef BENGINE_WINDOW_X11_HPP__
#define BENGINE_WINDOW_X11_HPP__

#include <cstdint>

struct _XDisplay;

namespace BEngine
{
//-----------------------------------------------------------------------
// LINUX X11 WINDOW WRAPPER CLASS
//
// Windows window class wrapper, uses X11 library for implementation.
//-----------------------------------------------------------------------
class CWindowX11
{
	friend class CX11MessageCallback;

public:
	CWindowX11();
	~CWindowX11();

	void update();

	std::uint32_t clientWidth() const;
	std::uint32_t clientHeight() const;

	bool running() const { return m_running; }

	void* getWindowHandle() const { return m_pWindowHandle; }

private:
	void createWindow() noexcept( false );

	_XDisplay* pDisplay{ nullptr };
	void* m_pWindowHandle{ nullptr }; // Window
	bool m_running{ false };
	std::uint64_t wmDeleteMessage;
};

} // namespace BEngine
#endif // BENGINE_WINDOW_X11_HPP__
