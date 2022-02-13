#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <vkzos.h>
#include <assert.h>

#include <vulkan\vulkan_win32.h>

#ifdef _MSC_VER

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {

    case WM_PAINT:
      break;
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    {
      int alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
      switch (wParam) {
      case VK_ESCAPE:
        PostQuitMessage(0);
        break;
      case VK_RETURN:
       
        break;
      }
    }
    break;

    case WM_SYSCHAR:
      break;

    case WM_SIZE:
    {
      RECT clientRect;
      memset(&clientRect, 0, sizeof(RECT));
      GetClientRect(hwnd, &clientRect);
      
    }
    break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProcW(hwnd, message, wParam, lParam);
    }
  return 0;
}

int CALLBACK wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nShowCmd
) {

  const char* extensions[] = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };

  if (!vkz_create_instance("title", extensions, 2)) {
    MessageBox(NULL, "Failed to create Vulkan instance", "Error", MB_OK);
  }

  WNDCLASSEXW windowClass;
  memset(&windowClass, 0, sizeof(WNDCLASSEXW));
  windowClass.cbSize = sizeof(WNDCLASSEX);
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = &WndProc;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = LoadIcon(hInstance, NULL);
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  windowClass.lpszMenuName = NULL;
  windowClass.lpszClassName = L"small3d unit tests";
  windowClass.hIconSm = LoadIcon(hInstance, NULL);
  ATOM atom = RegisterClassExW(&windowClass);
  assert(atom > 0);


  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);

  int width = 800, height = 600;
  RECT windowRect = { 0, 0, width, height };

  AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

  int windowWidth = windowRect.right - windowRect.left;
  int windowHeight = windowRect.bottom - windowRect.top;

  // center
  
  int windowX = (screenWidth - windowWidth) / 2;
  int windowY = (screenHeight - windowHeight) / 2;

  HWND hWnd = CreateWindowExW(0, L"small3d unit tests", L"small3d unit tests", WS_OVERLAPPEDWINDOW, windowX, windowY,
    windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

  VkWin32SurfaceCreateInfoKHR createInfo;
  memset(&createInfo, 0, sizeof(VkWin32SurfaceCreateInfoKHR));
  createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  createInfo.pNext = NULL;
  createInfo.hinstance = hInstance;
  createInfo.hwnd = hWnd;
  if (vkCreateWin32SurfaceKHR(vkz_instance, &createInfo, NULL, &vkz_surface) != VK_SUCCESS) {
    MessageBox(NULL, "Failed to create Vulkan surface", "Error", MB_OK);
    return 1;
  }

  if (!vkz_init(3)) {
    MessageBox(NULL, "Failed to initialise Vulkan", "Error", MB_OK);
  }

  vkz_set_width_height(width, height);

  vkz_create_sync_objects();

  if (!vkz_create_swapchain()) {
    MessageBox(NULL, "Failed to create Vulkan surface", "Error", MB_OK);
  }

  ShowWindow(hWnd, SW_SHOW);

  MSG msg;
  memset(&msg, 0, sizeof(msg));

  while (msg.message != WM_QUIT)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  vkz_destroy_sync_objects();
  vkz_shutdown();

  return 0;
}
#else
int main(int argc, char** argv) {

  uint32_t num = 0;

  if (vkz_create_instance("title", NULL, num)) {
    printf("Ok\n\r");

  }
  else {
    printf("Failed\n\r");
  }
  return EXIT_SUCCESS;
}
#endif
