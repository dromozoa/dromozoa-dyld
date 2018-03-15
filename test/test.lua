-- Copyright (C) 2016,2018 Tomoyuki Fujimori <moyu@dromozoa.com>
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

local dyld = require "dromozoa.dyld"

local names = {
  "RTLD_LAZY";
  "RTLD_NOW";
  "RTLD_GLOBAL";
  "RTLD_LOCAL";
}
for i = 1, #names do
  local name = names[i]
  local value = assert(dyld[name])
  assert(type(value) == "number")
  print(name, dyld[name])
end

local symbol, message = dyld.RTLD_DEFAULT:dlsym("pthread_create")
if not symbol then
  print(message)
  local handle = assert(dyld.dlopen("libpthread.so.0", dyld.RTLD_LAZY + dyld.RTLD_GLOBAL))
  print("dlopen(libpthread.so.0)", handle:get())
  local symbol = assert(dyld.RTLD_DEFAULT:dlsym("pthread_create"))
  print("dlsym(pthread_create)", symbol:get())
end

local symbol, message = assert(dyld.RTLD_DEFAULT:dlsym("puts"))
print(symbol:get())
