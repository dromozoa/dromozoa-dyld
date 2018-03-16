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
end

assert(dyld.RTLD_DEFAULT)
assert(dyld.RTLD_NEXT)

if dyld.RTLD_DEFAULT:dlsym "pthread_create" then
  print "pthread_create found"
else
  print "pthread_create not found"
  assert(dyld.dlopen("libpthread.so.0", dyld.RTLD_LAZY + dyld.RTLD_GLOBAL))
  assert(dyld.RTLD_DEFAULT:dlsym "pthread_create")
end
