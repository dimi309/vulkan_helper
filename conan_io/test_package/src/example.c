#include <stdio.h>
#include <vkzos.h>

int main (int argc, char *argv[])
{
  uint32_t num = 0;

  if (vkz_create_instance("title", NULL, num)) {
    printf("Ok\n\r");

  }
  else {
    printf("Failed\n\r");
  }
  return EXIT_SUCCESS;
}
