/***********************************************************************
FILE IWINDOW.H CONTAINS INTERFACE WINDOW CLASS IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (30.12.2020) BY ARTUR K. 
***********************************************************************/

#ifndef BENGINE_WINDOW_INTERFACE_HPP__
#define BENGINE_WINDOW_INTERFACE_HPP__

namespace BEngine
{
//-----------------------------------------------------------------------
// WINDOW INTERFACE CLASS
//
// Base abstract window class that must be inherited
// by other window implementations.
//-----------------------------------------------------------------------
class IWindow
{
public:
	IWindow() = default;
	virtual ~IWindow() = default;

	virtual void* getWindowHandle() = 0;
};

} // namespace BEngine
#endif // BENGINE_WINDOW_INTERFACE_HPP__
