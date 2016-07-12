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

#include <dromozoa/bind.hpp>

namespace dromozoa {
  void initialize_handle(lua_State* L);
  void initialize_symbol(lua_State* L);
  void initialize_dlfcn(lua_State* L);

  void initialize(lua_State* L) {
    initialize_handle(L);
    initialize_symbol(L);
    initialize_dlfcn(L);
  }
}

extern "C" int luaopen_dromozoa_dyld(lua_State* L) {
  lua_newtable(L);
  dromozoa::initialize(L);
  return 1;
}
