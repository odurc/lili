lili
====

A doubly linked list library implemented in C. The library can be built to use only static
memory allocation, what make it interesting to employ in microcontrollers projects or other
devices used in embedded system.


Features
---

* no external dependency
* configurable to use only static memory allocation


How to install
---

Simply copy the content of src directory to your work directory.


Configuration
---

The configuration of the library is done by setting 'define' macros in the header file,
under the configuration section.

In order to use only static memory allocation, three 'define' macros need to be declared,
*LILI_ONLY_STATIC_ALLOCATION*, *LILI_MAX_LISTS* and *LILI_MAX_NODES*.

    #define LILI_ONLY_STATIC_ALLOCATION
    #define LILI_MAX_LISTS      10
    #define LILI_MAX_NODES      100

As the macros name suggest the definitions are used to set the maximum number of lists and nodes.
As expected, all objects are previously allocated as static variables and managed internally by the
library.

The default configuration, i.e. when no 'define' macros is declared (or they are kept commented out),
will use dynamic memory allocation as any regular linked list library.


How to use
---

To see details how to use the library, please check the online [API documentation](http://ricardocrudo.github.io/lili).


License
---

MIT
