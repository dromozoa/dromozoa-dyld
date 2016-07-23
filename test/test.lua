-- Copyright (C) 2016 Tomoyuki Fujimori <moyu@dromozoa.com>
--
-- This file is part of dromozoa-dyld.
--
-- dromozoa-dyld is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- dromozoa-dyld is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with dromozoa-dyld.  If not, see <http://www.gnu.org/licenses/>.

local uint32 = require "dromozoa.commons.uint32"
local dyld = require "dromozoa.dyld"

assert(dyld.RTLD_LAZY)
assert(dyld.RTLD_NOW)
assert(dyld.RTLD_GLOBAL)
assert(dyld.RTLD_LOCAL)

local symbol, message = dyld.RTLD_DEFAULT:dlsym("pthread_create")
if symbol and not symbol:is_null() then
  print("dlsym(pthread_create)", symbol:get())
else
  local handle = assert(dyld.dlopen("libpthread.so.0", uint32.bor(dyld.RTLD_LAZY, dyld.RTLD_GLOBAL)))
  print("dlopen(libpthread.so.0)", handle:get())
  local symbol = assert(dyld.RTLD_DEFAULT:dlsym("pthread_create"))
  print("dlsym(pthread_create)", symbol:get())
end

local symbol, message = assert(dyld.RTLD_DEFAULT:dlsym("puts"))
assert(symbol:is_null() == false)

assert(dyld.RTLD_DEFAULT:is_default() == true)
assert(dyld.RTLD_DEFAULT:is_next() == false)

assert(dyld.RTLD_NEXT:is_default() == false)
assert(dyld.RTLD_NEXT:is_next() == true)
