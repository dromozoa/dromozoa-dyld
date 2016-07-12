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

print(dyld.dlopen("no such file.so", uint32.bor(dyld.RTLD_LAZY, dyld.RTLD_LOCAL)))

print(dyld.RTLD_DEFAULT:dlsym("pthread_mutex_lock"))

-- local f = dyld.handle.dlsym(dyld.RTLD_DEFAULT, "pthread_mutex_lock")
-- print(f, f:is_null())
