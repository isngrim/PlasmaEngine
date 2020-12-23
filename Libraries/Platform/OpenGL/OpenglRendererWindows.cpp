// MIT Licensed (see LICENSE.md).

#include "Precompiled.hpp"
#include "../OpenGL/OpenglRenderer.hpp"

namespace Plasma
{
class OpenglRendererWindows : public OpenglRenderer
{
public:
  OpenglRendererWindows(OsHandle windowHandle, String& error);
  ~OpenglRendererWindows() override;
};

OpenglRendererWindows::OpenglRendererWindows(OsHandle windowHandle, String& error)
{
  HWND window = (HWND)windowHandle;
  HDC deviceContext = GetDC(window);

  ...
  Pixel format stuff done MipHeader
  ...

  HGLRC renderContext = wglCreateContext(deviceContext);
  wglMakeCurrent(deviceContext, renderContext);

  // Call the base initialize now that we've created the OpenGL context.
  Initialize(windowHandle, deviceContext, renderContext, error);
}

OpenglRendererWindows::~OpenglRendererWindows()
{
  // Must call this before we destroy the OpenGL context.
  Shutdown();

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext((HGLRC)mRenderContext);
}

Renderer* CreateRenderer(OsHandle windowHandle, String& error)
{
  return new OpenglRendererWindows(windowHandle, error);
}

void zglSetSwapInterval(OpenglRenderer* renderer, int interval)
{
  wglSwapIntervalEXT(interval);
}

IntVec2 zglGetWindowRenderableSize(OpenglRenderer* renderer)
{
  RECT rect;
  GetClientRect((HWND)renderer->mWindow, &rect);
  IntVec2 size = IntVec2(rect.right - rect.left, rect.bottom - rect.top);
  return size;
}

void zglSwapBuffers(OpenglRenderer* renderer)
{
  SwapBuffers((HDC)renderer->mDeviceContext);
}

} // namespace Plasma
