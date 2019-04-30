#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
int puts(const char *message)
{
  int (*new_puts)(const char *message);
  int result;
  new_puts = dlsym(RTLD_NEXT, "puts");
  if(strcmp(message, "Hello world!n") == 0)
  {
    result = new_puts("Goodbye, cruel world!\n");
  }
  else
  {
    result = new_puts(message);
  }
  return result;
}

/*
  RTLD_NEXT:

  dlsym:

  -fPIC 
  -shared:a shared library by specifying the -shared and -fPIC compile flags

  -ldl:link against libdl using the -ldl flag

  -D_GNU_SOURCE:D_GNU_SOURCE flag is specified to satisfy #ifdef conditions that allow us to use the RTLD_NEXT enum
  

*/
