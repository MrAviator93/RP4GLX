#ifndef BBX_WINDOW_X11_HPP__
#define BBX_WINDOW_X11_HPP__

// C++
#include <memory>
#include <cstdint>

namespace bbx
{

/// Windows window class wrapper, uses X11 library for implementation.
class Window
{
	struct Impl;

public:
	Window();

	~Window();

	void update();

	[[nodiscard]] void* handle() const;
	[[nodiscard]] bool running() const;

	[[nodiscard]] std::uint32_t clientWidth() const;
	[[nodiscard]] std::uint32_t clientHeight() const;

private:
	void createWindow() noexcept( false );

private:
	std::unique_ptr< Impl > m_pImpl;
};

} // namespace bbx
#endif // BBX_WINDOW_X11_HPP__
