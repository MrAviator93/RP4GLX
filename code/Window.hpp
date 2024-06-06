#ifndef BBX_WINDOW_WINDOW_HPP__
#define BBX_WINDOW_WINDOW_HPP__

// C++
#include <memory>
#include <cstdint>

namespace bbx::window
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

} // namespace bbx::window
#endif // BBX_WINDOW_WINDOW_HPP__
