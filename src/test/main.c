#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <vkzos.h>
#include <assert.h>
#include <conio.h>

#include <vulkan\vulkan_win32.h>
#include "rectangle.h"

#ifdef _MSC_VER

#define NUM_FRAMES_IN_FLIGHT 3

float* vertexData, * textureCoordsData;
uint32_t* indexData;
uint32_t pipeline_index = 100;
uint32_t image_index = 0;
VkBuffer vertex_buffer = VK_NULL_HANDLE;
VkDeviceMemory vertex_buffer_memory = VK_NULL_HANDLE;
VkBuffer index_buffer = VK_NULL_HANDLE;
VkDeviceMemory index_buffer_memory = VK_NULL_HANDLE;

VkVertexInputBindingDescription binding_desc[1];
VkVertexInputAttributeDescription attrib_desc[1];
VkCommandBuffer command_buffer[NUM_FRAMES_IN_FLIGHT];
uint32_t current_frame_index = 0;

BOOL closing = FALSE;

int set_input_state_callback(VkPipelineVertexInputStateCreateInfo*
  inputStateCreateInfo) {

  memset(binding_desc, 0, 1 * sizeof(VkVertexInputBindingDescription));

  binding_desc[0].binding = 0;
  binding_desc[0].stride = 4 * sizeof(float);

  memset(attrib_desc, 0, 1 * sizeof(VkVertexInputAttributeDescription));

  attrib_desc[0].binding = 0;
  attrib_desc[0].location = 0;
  attrib_desc[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
  attrib_desc[0].offset = 0;

  inputStateCreateInfo->vertexBindingDescriptionCount = 1;
  inputStateCreateInfo->vertexAttributeDescriptionCount = 1;
  inputStateCreateInfo->pVertexBindingDescriptions = binding_desc;
  inputStateCreateInfo->pVertexAttributeDescriptions = attrib_desc;

  return 1;
}

int set_pipeline_layout_callback(VkPipelineLayoutCreateInfo*
  pipelineLayoutCreateInfo) {

  pipelineLayoutCreateInfo->pSetLayouts = NULL;
  pipelineLayoutCreateInfo->setLayoutCount = 0;

  return 1;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

  //other code
  AllocConsole();
  freopen("CONIN$", "r", stdin);
  freopen("CONOUT$", "w", stdout);
  freopen("CONOUT$", "w", stderr);

  switch (message) {

  case WM_PAINT:

    vkz_acquire_next_image(pipeline_index, &image_index, &current_frame_index);
    vkz_wait_gpu_cpu_fence(current_frame_index);
    
    //vkz_destroy_draw_command_buffer(&command_buffer[current_frame_index]);
    
    if (command_buffer[current_frame_index] == VK_NULL_HANDLE) {
      vkz_begin_draw_command_buffer(&command_buffer[current_frame_index]);
      vkz_bind_pipeline_to_command_buffer(pipeline_index, &command_buffer[current_frame_index]);
      VkDeviceSize binding = 0;
      vkCmdBindVertexBuffers(command_buffer[current_frame_index], 0, 1, &vertex_buffer, &binding);
      vkCmdBindIndexBuffer(command_buffer[current_frame_index], index_buffer, 0, VK_INDEX_TYPE_UINT32);
      vkCmdDrawIndexed(command_buffer[current_frame_index], 6, 1, 0, 0, 0);
      vkz_end_draw_command_buffer(&command_buffer[current_frame_index]);
    }
    vkz_draw(&command_buffer[current_frame_index]);
    vkz_present_next_image();

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

    for (int idx = 0; idx < NUM_FRAMES_IN_FLIGHT; ++idx) {
      vkz_wait_gpu_cpu_fence(idx);
      vkz_destroy_draw_command_buffer(&command_buffer[idx]);
    }

    vkz_destroy_buffer(vertex_buffer, vertex_buffer_memory);
    vkz_destroy_buffer(index_buffer, index_buffer_memory);
    vkz_destroy_pipeline(pipeline_index);

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
  _In_ int nShowCmd) {

  vertexData = malloc(16 * sizeof(float));
  textureCoordsData = malloc(8 * sizeof(float));
  indexData = malloc(6 * sizeof(uint32_t));


  createRectangle(-0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
    vertexData, indexData, textureCoordsData);


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
  windowClass.lpszClassName = L"vkzos unit tests";
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

  HWND hWnd = CreateWindowExW(0, L"vkzos unit tests", L"vkzos unit tests", WS_OVERLAPPEDWINDOW, windowX, windowY,
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

  if (!vkz_init(NUM_FRAMES_IN_FLIGHT)) {
    MessageBox(NULL, "Failed to initialise Vulkan", "Error", MB_OK);
  }

  vkz_set_width_height(width, height);

  vkz_create_sync_objects();

  if (!vkz_create_swapchain()) {
    MessageBox(NULL, "Failed to create Vulkan surface", "Error", MB_OK);
  }

  if (!vkz_create_pipeline("..\\..\\resources\\shaders\\vertexShader.spv",
    "..\\..\\resources\\shaders\\fragmentShader.spv",
    set_input_state_callback, set_pipeline_layout_callback,
    &pipeline_index)) {
    MessageBox(NULL, "Failed to create Pipeline", "Error", MB_OK);
  }
  
  if (!vkz_create_buffer(&vertex_buffer,
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    16 * sizeof(float),
    &vertex_buffer_memory,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
    MessageBox(NULL, "Failed to create postition buffer", "Error", MB_OK);
  }

  VkBuffer staging_buffer;
  VkDeviceMemory staging_buffer_memory;

  if (vkz_create_buffer(&staging_buffer,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    16 * sizeof(float),
    &staging_buffer_memory,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
    void* staging_data = NULL;
    vkMapMemory(vkz_logical_device, staging_buffer_memory, 0,
      VK_WHOLE_SIZE,
      0, &staging_data);
    memcpy(staging_data, vertexData, 16 * sizeof(float));
    vkUnmapMemory(vkz_logical_device, staging_buffer_memory);

    vkz_copy_buffer(staging_buffer, vertex_buffer,
      16 * sizeof(float));

    vkz_destroy_buffer(staging_buffer, staging_buffer_memory);
  }
  else {
    MessageBox(NULL, "Failed to create staging buffer for vertices", "Error", MB_OK);
  }

  if (!vkz_create_buffer(&index_buffer,
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    6 * sizeof(uint32_t),
    &index_buffer_memory,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
    MessageBox(NULL, "Failed to create index buffer", "Error", MB_OK);
  }

  if (vkz_create_buffer(&staging_buffer,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    6 * sizeof(uint32_t),
    &staging_buffer_memory,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
    void* staging_data;
    vkMapMemory(vkz_logical_device, staging_buffer_memory, 0,
      VK_WHOLE_SIZE,
      0, &staging_data);
    memcpy(staging_data, indexData, 6 * sizeof(uint32_t));
    vkUnmapMemory(vkz_logical_device, staging_buffer_memory);

    vkz_copy_buffer(staging_buffer, index_buffer,
      6 * sizeof(uint32_t));

    vkz_destroy_buffer(staging_buffer, staging_buffer_memory);
  }
  else {
    MessageBox(NULL, "Failed to create staging buffer for indices", "Error", MB_OK);
  }
  

  for (int idx = 0; idx < NUM_FRAMES_IN_FLIGHT; ++idx) {
    command_buffer[idx] = VK_NULL_HANDLE;
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

  
  vkz_destroy_swapchain();
  vkz_destroy_sync_objects();
  vkz_shutdown();
  free(indexData);
  free(vertexData);
  free(textureCoordsData);
  printf("Press a key to close.");
  _getch();
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
