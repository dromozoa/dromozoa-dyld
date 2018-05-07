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

local verbose = os.getenv "VERBOSE" == "1"

assert(dyld.dlopen_pthread())

local count = 0
local symbol = assert(dyld.RTLD_DEFAULT:dlsym "pthread_create")
if verbose then
  io.stderr:write(count, " ", tostring(symbol:get()), "\n")
end

return function ()
  count = count + 1
  local symbol = assert(dyld.RTLD_DEFAULT:dlsym "pthread_create")
  local symbol = assert(dyld.RTLD_DEFAULT:dlsym "pthread_create")
  if verbose then
    io.stderr:write(count, " ", tostring(symbol:get()), "\n")
  end
end
