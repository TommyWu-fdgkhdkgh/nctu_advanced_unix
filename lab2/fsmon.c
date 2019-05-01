#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DLSYM(X) dlsym(RTLD_NEXT, (X))

int  (*new_fprintf)(FILE *restrict stream, const char *restrict format, ...);
ssize_t (*new_readlink)(const char *pathname, char *buf, size_t bufsiz);
struct dirent *(*new_readdir)(DIR *dirp);
FILE *(*new_fopen)(const char *pathname, const char *mode);
int (*new_fclose)(FILE *stream);
size_t (*new_fwrite)(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
int (*new_dup)(int oldfd);

int output = 0;
char *filename;


/*
 * char type >>> print string
 * fd(int), FILE *, DIR * >>> print string
 * struct stat >>> type, size
 */


__attribute__((constructor(111))) static void construct(){

  new_fprintf = DLSYM("fprintf");
  new_readlink= DLSYM("readlink");
  new_readdir = DLSYM("readdir");
  new_fopen   = DLSYM("fopen");
  new_fclose  = DLSYM("fclose");
  new_fwrite  = DLSYM("fwrite");
  new_dup     = DLSYM("dup");

  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
}


/*
 *  <<<fd2filename
 */
void fd2filename(int fd, char buf[], int bufsiz){
  new_readlink = DLSYM("readlink");

  if(fd==0){
    strcpy(buf, "<STDIN>\x00");
    return;
  }else if(fd==1){
    strcpy(buf, "<STDOUT>\x00");
    return;
  }else if(fd==2){
    strcpy(buf, "<STDERR>\x00");
    return;
  }

  char path[256];
  memset(path,0,256); 
  sprintf(path,"/proc/self/fd/%d",fd);
  new_readlink(path, buf, bufsiz);
}

/*
 * <<<FILE2filename
 */
void FILE2filename(FILE *st, char buf[]){
  new_readlink = DLSYM("readlink");

  int fd = fileno(st);
  //printf("\n\nfd:%d\n\n",fd);
  if(fd==0){
    strcpy(buf, "<STDIN>\x00");
    return;
  }else if(fd==1){
    strcpy(buf, "<STDOUT>\x00");
    return;
  }else if(fd==2){
    strcpy(buf, "<STDERR>\x00");
    return;
  }

  char path[256];
  memset(path, 0, 256);
  sprintf(path, "/proc/self/fd/%d", fd);
  int result = new_readlink(path, buf, 256);
}

/*
 * <<<DIR2filename
 */
void DIR2filename(DIR *dirp, char buf[]){
  new_readlink = DLSYM("readlink");  

  int fd = dirfd(dirp);
  char path[256];
  memset(path, 0, 256);
  sprintf(path, "/proc/self/fd/%d", fd);
  int result = new_readlink(path, buf, 256);
}

/*
 *  <<<fputs_unlocked
 */
int fputs_unlocked(const char *s, FILE *stream){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_fputs_unlocked)(const char *s, FILE *stream);
  new_fputs_unlocked = DLSYM("fputs_unlocked");
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");


  int result = new_fputs_unlocked(s, stream);

  char name[256];
  memset(name, 0, 256); 
  FILE2filename(stream, name);

  if(output==0){
    new_fprintf(stderr, "# fputs_unlocked(0x%x, \"%s\") = %d\n", s, name, result);
  } else {
    //open file
    //write
    //close
    FILE *pFile;
    pFile = new_fopen(filename,"a+");
    new_fprintf(pFile, "# fputs_unlocked(0x%x, \"%s\") = %d\n", s, name, result);
    new_fclose(pFile);
  }
  return result;
}

/*
 *  <<<fflush
 */
int fflush(FILE *stream){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_fflush)(FILE *stream);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");


  new_fflush = DLSYM("fflush");

  int result = new_fflush(stream);

  char name[256];
  memset(name, 0, 256); 
  FILE2filename(stream, name);

  if(output==0){
    new_fprintf(stderr ,"# fflush(\"%s\") = %d\n", name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename,"a+");
    new_fprintf(pFile, "# fflush(\"%s\") = %d\n", name, result);
    new_fclose(pFile);
  } 

  return result;
}

/*
 *  <<<fflush_unlocked
 */
int fflush_unlocked(FILE *stream){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_fflush_unlocked)(FILE *stream);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

 
  new_fflush_unlocked = DLSYM("fflush_unlocked");

  int result = new_fflush_unlocked(stream);
  
  char name[256];
  memset(name, 0, 256); 
  FILE2filename(stream, name);

  if(output==0){
    new_fprintf(stderr, "# fflush_unlocked(\"%s\") = %d\n", name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# fflush_unlocked(\"%s\") = %d\n", name, result);
    new_fclose(pFile);
  }
 
  return result;
}

/*
 *  <<<fwrite_unlocked
 */
size_t fwrite_unlocked(const void *ptr, size_t size, size_t n, FILE *stream){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  size_t (*new_fwrite_unlocked)(const void *ptr, size_t size, size_t n, FILE *stream);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_fwrite_unlocked = DLSYM("fwrite_unlocked");

  int result = new_fwrite_unlocked(ptr, size, n, stream);

  char name[256];
  memset(name, 0, 256); 
  FILE2filename(stream, name);
   
  if(output==0){
    new_fprintf(stderr , "# fwrite_unlocked(0x%x, %d, %d, \"%s\") = %d\n", ptr, size, n, name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile , "# fwrite_unlocked(0x%x, %d, %d, \"%s\") = %d\n", ptr, size, n, name, result);
    new_fclose(pFile);
  }
 

  return result;
}


/*
 * <<<rmdir
 */

int rmdir(const char *pathname){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_rmdir)(const char *pathname);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_rmdir = DLSYM("rmdir");

  int result = new_rmdir(pathname);

  if(output==0){
    new_fprintf(stderr, "# rmdir(\"%s\") = %d\n", pathname, result); 
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# rmdir(\"%s\") = %d\n", pathname, result);
    new_fclose(pFile);
  }

  return result;
}

/*
 * <<<mkdir
 */
int mkdir(const char *pathname, mode_t mode){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_mkdir)(const char *pathname, mode_t mode);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_mkdir = DLSYM("mkdir");

  int result = new_mkdir(pathname, mode);
  
  if(output==0){
    new_fprintf(stderr , "# mkdir(\"%s\", %d) = %d\n", pathname, mode, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# mkdir(\"%s\", %d) = %d\n", pathname, mode, result);
    new_fclose(pFile); 
  } 
 
  return result;
}


/*
 * <<<symlink
 */
int symlink(const char *target, const char *linkpath){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_symlink)(const char *target, const char *linkpath);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_symlink = DLSYM("symlink");

  int result = new_symlink(target, linkpath);
 
  if(output==0){
    new_fprintf(stderr, "# symlink(\"%s\", \"%s\") = %d\n", target, linkpath, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# symlink(\"%s\", \"%x\") = %d\n", target, linkpath, result);
    new_fclose(pFile); 
  }
  return result;
}



/*
 * <<<readlink
 */
ssize_t readlink(const char *pathname, char *buf, size_t bufsiz){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  ssize_t (*new_readlink)(const char *pathname, char *buf, size_t bufsiz);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_readlink = DLSYM("readlink");

  int result = new_readlink(pathname, buf, bufsiz);

  if(output==0){
    new_fprintf(stderr, "# readlink(\"%s\", \"%s\", %d) = %d\n", pathname, buf, bufsiz, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# readlink(\"%s\", \"%s\", %d) = %d\n", pathname, buf, bufsiz, result);
    new_fclose(pFile);
  }   
  return result;
}




/*
 * <<<unlink
 */
int unlink(const char *pathname){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_unlink)(const char *pathname);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_unlink = DLSYM("unlink");

  int result = new_unlink(pathname);

  if(output==0){
    new_fprintf(stderr, "# unlink(\"%s\") = %d\n", pathname, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# unlink(\"%s\") = %d\n", pathname, result);
    new_fclose(pFile);
  }
  return result;
}



/*
 * <<<link
 */
int link(const char *oldpath, const char *newpath){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_link)(const char *oldpath, const char *newpath);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_link = DLSYM("link");

  int result = new_link(oldpath, newpath);

  if(output==0){
    new_fprintf(stderr, "# link(\"%s\", \"%s\") = %d\n", oldpath, newpath, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# link(\"%s\", \"%s\") = %d\n", oldpath, newpath, result);
    new_fclose(pFile);
  }
  return result;
}


/*
 * <<<rename 
 */
int rename(const char *oldpath, const char *newpath){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_rename)(const char *oldpath, const char *newpath);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_rename = DLSYM("rename");

  int result = new_rename(oldpath, newpath);

  if(output==0){
    new_fprintf(stderr, "# rename(\"%s\", \"%s\") = %d\n", oldpath, newpath, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# rename(\"%s\", \"%s\") = %d\n", oldpath, newpath, result);
    new_fclose(pFile);
  } 
  return result;
}

/*
 * <<<remove
 */
int remove(const char *pathname){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_remove)(const char *pathname);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_remove = DLSYM("remove");

  int result = new_remove(pathname);

  if(output==0){
    new_fprintf(stderr, "# remove(\"%s\") = %d\n", pathname, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# remove(\"%s\") = %d\n", pathname, result);
    new_fclose(pFile);
  }
  return result;
}


/*
 * <<<chmod
 */
int chmod(const char *pathname, mode_t mode){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_chmod)(const char *pathname, mode_t mode);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_chmod = DLSYM("chmod");

  int result = new_chmod(pathname, mode); 

  if(output==0){
    new_fprintf(stderr, "# chmod(\"%s\", %d) = %d\n", pathname, mode, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# chmod(\"%s\", %d) = %d\n", pathname, mode, result);
    new_fclose(pFile);
  } 

  return result;
}


/*
 * <<<chown
 */
int chown(const char *pathname, uid_t owner, gid_t group){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_chown)(const char *pathname, uid_t owner, gid_t group);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_chown = DLSYM("chown");

  int result = new_chown(pathname, owner, group);
  if(output==0){
    new_fprintf(stderr, "# chown(\"%s\", %d, %d) = %d\n", pathname, owner, group, result); 
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+"); 
    new_fprintf(pFile, "# chown(\"%s\", %d, %d) = %d\n", pathname, owner, group, result); 
    new_fclose(pFile);
  } 
  return result;
}


/*
 * <<<chdir
 */
int chdir(const char *path){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_chdir)(const char *path);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_chdir = DLSYM("chdir");

  int result = new_chdir(path); 

  if(output==0){
    new_fprintf(stderr, "# chdir(\"%s\") = %d\n", path, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# chdir(\"%s\") = %d\n", path, result);
    new_fclose(pFile);
  }  
  return result;
}



/*
 * <<<fprintf
 */

int fprintf(FILE *restrict stream, const char *restrict format,...){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_vfprintf)(FILE *restrict stream, const char *restrict , ...);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_vfprintf = DLSYM("vfprintf");
  
  va_list ap;
  va_start(ap, format);

  int result;
  result = new_vfprintf(stream, format, ap);

  va_end(ap);

  char name[256];
  memset(name, 0, 256); 
  FILE2filename(stream, name);

  if(output==0){
    new_fprintf(stderr, "# fprintf(\"%s\", \"%s\", ...) = %d\n", name, format, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# fprintf(\"%s\", \"%s\", ...) = %d\n", name, format, result);
    new_fclose(pFile);
  }
  return result;
}




/*
 * <<<fread
 */
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  size_t (*new_fread)(void *ptr, size_t size, size_t nmemb, FILE *stream);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");
  
  new_fread = DLSYM("fread");
  

  size_t result = new_fread(ptr, size, nmemb, stream);

  char name[256];
  memset(name, 0, 256); 
  FILE2filename(stream, name);

  if(output==0){
    new_fprintf(stderr, "# fread(0x%x, %d, %d, %s) = %d\n", ptr, size, nmemb, name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# fread(0x%s, %d, %d, %s) = %d\n", ptr, size, nmemb, name, result);
    new_fclose(pFile); 
  }
  
  return result;
}

/*
 * <<<fwrite
 */
size_t fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  size_t (*new_fwrite)(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");
 
  new_fwrite = DLSYM("fwrite");

  size_t result = new_fwrite(ptr, size, nitems, stream); 

  char name[256];
  memset(name, 0, 256); 
  FILE2filename(stream, name);

  if(output==0){
    new_fprintf(stderr, "# fwrite(0x%x, %d, %d, \"%s\") = %d\n", ptr, size, nitems, name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# fwrite(0x%x, %d, %d, \"%s\") = %d\n", ptr, size, nitems, name, result);
    new_fclose(pFile);
  }

  return result;
}

/*
 * <<<fgetc
 */
int fgetc(FILE *stream){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_fgetc)(FILE *stream);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_fgetc = DLSYM("fgetc");

  int result = new_fgetc(stream); 

  char name[256];
  memset(name, 0, 256); 
  FILE2filename(stream, name);

  if(output==0){
    new_fprintf(stderr, "# fgetc(\"%s\") = %d\n", name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# fgetc(\"%s\") = %d\n", name, result);
    new_fclose(pFile); 
  }
  
  return result;
}

/*
 * <<<fgets
 */
char *fgets(char *s, int size, FILE *stream){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  char *(*new_fgets)(char *s, int size, FILE *stream);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_fgets = DLSYM("fgets");

  char *result = new_fgets(s, size, stream);

  char name[256];
  memset(name, 0, 256); 
  FILE2filename(stream, name);
  
  if(output==0){
    new_fprintf(stderr, "fgets(\"%s\", %d, \"%s\") = 0x%x\n", s, size, name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "fgets(\"%s\", %d, \"%s\") = 0x%x\n", s, size, name, result);
    new_fclose(pFile); 
  }

  return result;
}

/*
 * <<<fscanf
 */


int fscanf(FILE *restrict stream, const char *restrict format, ...){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_fscanf)(FILE *restrict stream, const char *restrict format, ...);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  va_list args;
  va_start(args, format);

  new_fscanf = DLSYM("fscanf");

  
  int result;
  result = new_fscanf(stream, format,args);

  char name[256];
  memset(name, 0, 256);
  FILE2filename(stream, name);

  if(output==0){
    new_fprintf(stderr, "# fscanf(\"%s\", \"%s\", ...) = %d\n", name, format, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# fscanf(\"%s\", \"%s\", ...) = %d\n", name, format, result);
    new_fclose(pFile);
  }


  va_end(args);

  return result;
}


/*
 * <<<__isoc99_fscanf
 */
int __isoc99_fscanf(FILE *restrict stream, const char *restrict format, ...){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }
  int (*new_vfscanf)(FILE *restrict stream, const char *restrict format, ...);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  va_list args;
  va_start(args, format);

  new_vfscanf = DLSYM("vfscanf");
  
  int result;
  result = new_vfscanf(stream, format, args);

  char name[256];
  memset(name, 0, 256);
  FILE2filename(stream, name);
  
  if(output==0){
    new_fprintf(stderr, "# fscanf(\"%s\", \"%s\", ...) = %d\n", name, format, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# fscanf(\"%s\", \"%s\", ...) = %d\n", name, format, result);
    new_fclose(pFile); 
  }
  va_end(args);

  return result;
}


/*
 * <<<closedir
 */
int closedir(DIR *dirp){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_closedir)(DIR *dirp);
  int result; 
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  new_closedir = DLSYM("closedir");

  char name[256];
  memset(name, 0, 256);
  DIR2filename(dirp, name);

  result = new_closedir(dirp);
 
  if(output==0){ 
    new_fprintf(stderr, "# closedir(\"%s\") = %d\n", name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# closedir(\"%s\") = %d\n", name, result);
    new_fclose(pFile); 
  }

  return result;
}

/*
 * <<<opendir
 */
DIR *opendir(const char *name){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  DIR *(*new_opendir)(const char *name);
   new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  DIR *result;

  new_opendir = DLSYM("opendir");

  result = new_opendir(name);

  char nn[256];
  memset(nn, 0, 256);
  DIR2filename(result, nn);
  
  if(output==0){
    new_fprintf(stderr, "# opendir(\"%s\") = \"%s\"\n", name, nn);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# opendir(\"%s\") = \"%s\"\n", name, nn);
    new_fclose(pFile);
  }


  return result;
}

/*
 * <<<readdir
 */
struct dirent *readdir(DIR *dirp){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  struct dirent *result;
  result = new_readdir(dirp);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  char name[256];
  memset(name, 0, 256);
  DIR2filename(dirp, name);

  if(result!=NULL){
    if(output==0){
        new_fprintf(stderr, "# readdir(\"%s\") = %s\n", name, result->d_name);
    } else {
      FILE *pFile;
      pFile = new_fopen(filename, "a+");
      new_fprintf(pFile, "# readdir(\"%s\") = %s\n", name, result->d_name);
      new_fclose(pFile);
    }
  }
  return result;
}


/*
 * <<<creat
 */
int creat(const char *pathname, mode_t mode){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int(*new_creat)(const char *pathname, mode_t mode);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  int result;

  new_creat = DLSYM("creat");

  result = new_creat(pathname, mode);

  if(output==0){
    new_fprintf(stderr, "# creat(\"%s\", %d) = %d\n", pathname, mode, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# creat(\"%s\", %d) = %d\n", pathname, mode, result);
    new_fclose(pFile);
  }

  return result;
}

/*
 * <<<open
 */
int open(const char *pathname, int flags, ...){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_open)(const char *pathname, int flags, ...);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  int result;

  new_open = DLSYM("open");

  va_list args;
  va_start(args, flags);

  if((flags & O_CREAT) || (flags & O_TMPFILE)){
    //mode_t會有東西
    mode_t m = va_arg(args, mode_t);
    result = new_open(pathname, flags, m);
  } else {
    result = new_open(pathname, flags);
  }
  
  if(output==0){
    new_fprintf(stderr, "# open(\"%s\", %d) = %d\n", pathname, flags, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# open(\"%s\", %d) = %d\n", pathname, flags, result);
    new_fclose(pFile);
  }
  

  /*
    The mode argument specifies the file mode bits be applied when
              a new file is created.  This argument must be supplied when
              O_CREAT or O_TMPFILE is specified in flags; if neither O_CREAT
              nor O_TMPFILE is specified, then mode is ignored.
  */

  va_end(args);

  return result;
}



/* 
 * <<<dup
 */
int dup(int fd){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_dup)(int fd);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");


  char name[256];
  memset(name, 0, 256);
  fd2filename(fd, name, 256);

  int result;
  new_dup = DLSYM("dup");
  result = new_dup(fd);


  if(output==0){
    new_fprintf(stderr, "# dup(\"%s\") = %d\n", name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# dup(\"%s\") = %d\n", name, result);
    new_fclose(pFile);
  }

  return result;
}



/*
 * <<<dup2
 * 在這個function把參數變字串感覺很怪
 */
int dup2(int oldfd, int newfd){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_dup2)(int oldfd, int newfd);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  int result;

  new_dup2  = DLSYM("dup2");
  result = new_dup2(oldfd, newfd);



  if(output==0){
    new_fprintf(stderr, "# dup2(%d, %d) = %d\n", oldfd, newfd, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# dup2(%d, %d) = %d\n", oldfd, newfd, result);
    new_fclose(pFile);
  }
  return result;
}


/*
 * <<<close
 * 似乎也有可能會把stderr的fd給關掉，試著處理一下
 */
int close(int fd){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_close)(int fd);
  //new_fprintf = DLSYM("fprintf"); 
  //new_fopen   = DLSYM("fopen"); 
  //new_fclose  = DLSYM("fclose");
  //new_close = DLSYM("close");
  void *handle = dlopen("libc.so.6", RTLD_LAZY);
  if(handle != NULL){
    new_close = dlsym(handle, "close");
    new_fopen = dlsym(handle, "fopen");
    new_fclose = dlsym(handle, "fclose");
    new_fprintf = dlsym(handle, "fprintf");
  }

  int result;

  int fd_stderr = new_dup(2);
  FILE *file_stderr = fdopen(fd_stderr, "a+");


  char name[256];
  memset(name, 0, 256);
  fd2filename(fd, name, 256);

  result = new_close(fd);


  //ls -al不斷core dump的問題被這一段給解決了，終究不知道為什麼
  if(file_stderr==NULL){
    return result;
  }

  if(output==0){
    new_fprintf(file_stderr, "# close(\"%s\") = %d\n", name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# close(\"%s\") = %d\n", name, result);
    new_fclose(pFile);
  }

  //new_fclose(file_stderr);

  return result;
}



/*
 * <<<lstat
 * 注意符號不一樣
 */
int lstat(const char *pathname, struct stat *statbuf){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_lstat)(const char *pathname, struct stat *statbuf); 
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  int result;

  new_lstat = DLSYM("lstat");
  result = new_lstat(pathname, statbuf);

  if(output==0){
    new_fprintf(stderr, "# lstat(\"%s\", 0x%x {mode=%d, size=%d}) = %d\n", pathname, statbuf, statbuf->st_mode, statbuf->st_size, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# lstat(\"%s\", 0x%x {mode=%d, size=%d}) = %d\n", pathname, statbuf, statbuf->st_mode, statbuf->st_size, result);
    new_fclose(pFile); 
  }

  return result;
}

/*
 * <<<__lxstat
 * 注意符號不一樣
 */
int __lxstat(int ver, const char *pathname, struct stat *statbuf){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_lstat)(int ver, const char *pathname, struct stat *statbuf); 
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  int result;

  new_lstat = DLSYM("__lxstat");
  result = new_lstat(ver, pathname, statbuf);

  if(output==0){
    new_fprintf(stderr, "# lstat(%d, \"%s\", 0x%x {mode=%d, size=%d}) = %d\n", ver, pathname, statbuf, statbuf->st_mode, statbuf->st_size, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# lstat(%d, \"%s\", 0x%x {mode=%d, size=%d}) = %d\n", ver, pathname, statbuf, statbuf->st_mode, statbuf->st_size, result);
    new_fclose(pFile); 
  }

  return result;
}



/*
 * <<<stat
 * 注意符號不一樣
 */
int stat(const char *pathname, struct stat *statbuf){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_stat)(const char *pathname, struct stat *statbuf);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  int result;

  new_stat = DLSYM("stat");
  result = new_stat(pathname, statbuf);

  if(output==0){
    new_fprintf(stderr, "# stat(\"%s\", 0x%x {mode=%d, size=%d}) = %d\n", pathname, statbuf, statbuf->st_mode, statbuf->st_size, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# stat(\"%s\", 0x%x {mode=%d, size=%d}) = %d\n", pathname, statbuf, statbuf->st_mode, statbuf->st_size, result);
    new_fclose(pFile);
  }


  return result;
}


/*
 * <<<__xstat
 * 注意符號不一樣
 */
int __xstat(int ver, const char *pathname, struct stat *statbuf){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_stat)(int ver, const char *pathname, struct stat *statbuf);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  int result;

  new_stat = DLSYM("__xstat");
  result = new_stat(ver, pathname, statbuf);

  if(output==0){
    new_fprintf(stderr, "# stat(%d, \"%s\", 0x%x {mode=%d, size=%d}) = %d\n", ver, pathname, statbuf, statbuf->st_mode, statbuf->st_size, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# stat(%d, \"%s\", 0x%x {mode=%d, size=%d}) = %d\n", ver, pathname, statbuf, statbuf->st_mode, statbuf->st_size, result);
    new_fclose(pFile);
  }
  return result;
}

/*
 * <<<pwrite
 */
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  ssize_t (*new_pwrite)(int fd, const void *buf, size_t count, off_t offset);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  ssize_t result;

  new_pwrite = DLSYM("pwrite");
  result = new_pwrite(fd, buf, count, offset);

  if(output==0){
    new_fprintf(stderr, "# pwrite(%d, 0x%x, %d, %d) = %d\n", fd, buf, count, offset, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# pwrite(%d, 0x%x, %d, %d) = %d\n", fd, buf, count, offset, result);
    new_fclose(pFile); 
  }

  return result;
}


/*
 * <<<fopen
 */
FILE *fopen(const char *pathname, const char *mode){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
    //printf("not NULL\n");
  } else {
    //printf("NULL\n");
  }

  //new_fopen   = DLSYM("fopen");
  
  void *handle = dlopen("libc.so.6", RTLD_LAZY);
  if(handle != NULL) {
    new_fopen = dlsym(handle, "fopen");
  }
  
  new_fprintf = DLSYM("fprintf"); 
  new_fclose  = DLSYM("fclose");

  FILE *result;
 
  result = new_fopen(pathname, mode);

  
  char name[256];
  memset(name, 0, 256);
  FILE2filename(result, name);


  if(output==0){
    new_fprintf(stderr, "# fopen(\"%s\", \"%s\") = \"%s\"\n", pathname, mode, name);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# fopen(\"%s\", \"%s\") = \"%s\"\n", pathname, mode, name);
    new_fclose(pFile);
  }

  return result;
}



/*
 * <<<fclose
 * 可能會把stderr關掉，要記得特別處理
 */
int fclose(FILE *stream){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  int (*new_fclose)(FILE *stream);
  int result;

  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");
  new_dup     = DLSYM("dup");
  
  int fd_stderr = new_dup(2);
  FILE *file_stderr = fdopen(fd_stderr, "a+");

  char name[256];
  memset(name, 0, 256);
  FILE2filename(stream, name);

  new_fclose = DLSYM("fclose");
  result = new_fclose(stream);

  if(output==0){
    new_fprintf(file_stderr, "# fclose(\"%s\") = %d\n", name, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# fclose(\"%s\") = %d\n", name, result);
    new_fclose(pFile);
  }

  new_fclose(file_stderr);

 
  return result;
}


/*
 * <<<read
 */
ssize_t read(int fd, void *buf, size_t nbytes){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  ssize_t (*new_read)(int fd, void *buf, size_t nbytes);
  ssize_t result;
  new_read = dlsym(RTLD_NEXT,"read");
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");
  result = new_read(fd, buf, nbytes);

  char name[256];
  memset(name, 0, 256);
  fd2filename(fd, name, 256);

  if(output==0){
    new_fprintf(stderr, "# read(\"%s\", 0x%x, %d) = %d\n", name, buf, nbytes, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# read(\"%s\", 0x%x, %d) = %d\n", name, buf, nbytes, result);
    new_fclose(pFile);
  }
  return result;
}


/*
 *  <<<write
 */
ssize_t write (int fd, const void *buf, size_t nbytes){
  filename = getenv("MONITOR_OUTPUT");
  if(filename!=NULL){
    output = 1;
  }

  
  ssize_t result;
  ssize_t (*new_write)(int fd, const void *buf, size_t nbytes);
  new_write = DLSYM("write");
  result = new_write(fd, buf, nbytes);
  new_fprintf = DLSYM("fprintf"); 
  new_fopen   = DLSYM("fopen"); 
  new_fclose  = DLSYM("fclose");

  if(output==0){
    new_fprintf(stderr, "# write(%d, 0x%x, %d) = %d\n", fd, buf, nbytes, result);
  } else {
    FILE *pFile;
    pFile = new_fopen(filename, "a+");
    new_fprintf(pFile, "# write(%d, 0x%x, %d) = %d\n", fd, buf, nbytes, result);
    new_fclose(pFile); 
  } 

  return result;
}


// a test function
/*
int puts(const char *message){

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
}*/
