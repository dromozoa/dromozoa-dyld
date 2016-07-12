// Copyright (C) 2016 Tomoyuki Fujimori <moyu@dromozoa.com>
//
// This file is part of dromozoa-dyld.
//
// dromozoa-dyld is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dromozoa-dyld is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with dromozoa-dyld.  If not, see <http://www.gnu.org/licenses/>.

#include <dlfcn.h>

#include <dromozoa/bind.hpp>

namespace dromozoa {
  void initialize(lua_State* L) {
    luaX_set_field(L, -1, "RTLD_LAZY", RTLD_LAZY);
    luaX_set_field(L, -1, "RTLD_NOW", RTLD_NOW);
    luaX_set_field(L, -1, "RTLD_GLOBAL", RTLD_GLOBAL);
    luaX_set_field(L, -1, "RTLD_LOCAL", RTLD_LOCAL);
  }
}

extern "C" int luaopen_dromozoa_dyld(lua_State* L) {
  lua_newtable(L);
  dromozoa::initialize(L);
  return 1;
}
