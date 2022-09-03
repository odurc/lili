lili
====

A doubly linked list library implemented in C. By the default the is configured to use only static
memory allocation which makes its use interesting for microcontrollers. However, it's also possible
to use dynamic memory allocation and configure your own `malloc` and `free` functions.

Features
---

* push at and pop from functions supporting negative index
* configurable static or dynamic memory allocation
* user configurable memory allocation functions
* no external dependency
* easy to use and setup

How to install
---

There is no installation, simply copy the content of `src` directory to your work directory and
adjust your build file or IDE as necessary.

How to use
---

To see details how to use the library, please check the online
[API documentation](http://odurc.gitlab.io/lili).

Configuration
---

The configuration of the library is done by setting 'define' macros in the header file,
under the configuration section.

By default the library is configured to use only static memory allocation via define' macros as
shown below.

```c
#define LILI_ONLY_STATIC_ALLOCATION
#define LILI_MAX_LISTS      10
#define LILI_MAX_NODES      100
```

As the macros name suggest the definitions are used to enable static memory usage, set the maximum
number of lists and maximum number of nodes, respectively.
All objects are previously allocated as static variables and managed internally by the library. Note
that the maximum number of nodes is general and not per list.

When the macros above are not defined (or commented out) the library uses dynamic memory allocation
and by default `malloc` and `free` are used to manage memory. To change this behavior to use your
own functions, define the macros as the example below and include the header for your library.

```c
#include "FreeRTOS.h"
//...
#define MALLOC(n)       pvPortMalloc(n)
#define FREE(p)         vPortFree(p)
```

Note that the macros are intentionally not prefixed with `LILI_` to facilitate a global definition
in case you use multiple libraries with this same concept of supporting custom `malloc` and `free`
functions. In this case, you can define the macros above on `config.h` for example and it would be
enough to add `#include "config.h` to `lili.h`.

License
---

MIT
