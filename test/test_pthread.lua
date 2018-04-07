-- Copyright (C) 2018 Tomoyuki Fujimori <moyu@dromozoa.com>
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

local is_glibc = dyld.RTLD_DEFAULT:dlsym "gnu_get_libc_version"
local registry = debug.getregistry()

assert(dyld.dlopen_pthread())
assert(dyld.RTLD_DEFAULT:dlsym "pthread_create")
local pthread_handle = registry["dromozoa.dyld.pthread"]
print(pthread_handle)
if is_glibc then
  assert(pthread_handle ~= nil)
else
  assert(pthread_handle == nil)
end

assert(dyld.dlopen_pthread())
assert(registry["dromozoa.dyld.pthread"] == pthread_handle)

assert(dyld.dlclose_pthread())
assert(registry["dromozoa.dyld.pthread"] == nil)

assert(dyld.dlclose_pthread())
assert(registry["dromozoa.dyld.pthread"] == nil)
