# 作業要求

Monitor File and Directory Activities of Dynamically Linked Programs

In this homework, we are going to practice library injection and API hijacking. Please implement a "library call monitor" (LCM) program that is able to show the activities of an arbitrary binary running on a Linux operating system. You have to implement your LCM as a shared library and inject the shared library into a process by using LD_PRELOAD. You have to dump the library calls as well as the passed parameters and the returned values. Please monitor file and directory relevant functions listed in the section "Minimum Requirements" below. The result should be output to either stderr or a filename, e.g., "fsmon.log". By default, the output is written to stderr. But you may write the output to a filename specified by an environment variable "MONITOR_OUTPUT".

You have to compile your source codes and generate a shared object. You don't have to implement any monitored program by yourself. Instead, you should work with those binaries already installed in the system, or the test cases provided by the instructor.


