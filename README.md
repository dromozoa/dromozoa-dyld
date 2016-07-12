# dromozoa-dyld

Dynamic linker utility.

## pthread workarounds

I want to solve the problem [Question about BZ#10652: Dynamic loading a library that is linked with pthread crashes app compiled without pthread in getaddrinfo](https://sourceware.org/ml/libc-alpha/2012-10/msg00224.html). There are workarounds:

1. rebuild lua with pthread.
2. use environment variable `LD_PRELOAD`.
3. use `dyld.dlopen` from lua script as below:

``` lua
local dyld = require "dromozoa.dyld"
local symbol = dyld.RTLD_DEFAULT:dlsym("pthread_create")
if not symbol or symbol:is_null() then
  dyld.dlopen("libpthread.so.0", dyld.RTLD_LAZY | dyld.RTLD_GLOBAL)
end
```
