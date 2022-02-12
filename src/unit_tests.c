#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <vkzos.h>

#ifdef _MSC_VER
int CALLBACK wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nShowCmd
) {

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
