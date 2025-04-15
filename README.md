# Compiler errors
## Errors: no file "unistd.h" or "sys/mman.h"
Usually ```
In file included from test.c:1:0:
stdfs.h:5:28: fatal error: include/unistd.h: No such file or directory
 #include <include/unistd.h>
                            ^
compilation terminated.```
* This is just included in bsd/linux. download it [here](https://github.com/openbsd/src/tree/master).
## Errors: implicit use of [function]
I'm going to update this later
