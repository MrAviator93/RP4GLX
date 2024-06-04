#ifndef BBX_WINDOW_X11_HPP__
#define BBX_WINDOW_X11_HPP__

// C++
#include <cstdint>

struct _XDisplay;

namespace bbx
{

/// Windows window class wrapper, uses X11 library for implementation.
class WindowX11
{
	struct Impl;

public:
	WindowX11();
	~WindowX11();

	void update();

	[[nodiscard]] std::uint32_t clientWidth() const;
	[[nodiscard]] std::uint32_t clientHeight() const;
	[[nodiscard]] bool running() const { return m_running; }
	[[nodiscard]] void* handle() const { return m_pWindowHandle; }

private:
	void createWindow() noexcept( false );

private:
	_XDisplay* pDisplay{ nullptr };
	void* m_pWindowHandle{ nullptr }; // Window
	bool m_running{ false };
	std::uint64_t wmDeleteMessage;
};

} // namespace bbx
#endif // BBX_WINDOW_X11_HPP__
