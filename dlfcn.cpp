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

#include "common.hpp"

namespace dromozoa {
  namespace {
    void impl_dlopen(lua_State* L) {
      const char* file = lua_tostring(L, 1);
      int mode = luaX_check_integer<int>(L, 2);
      if (void* result = dlopen(file, mode)) {
        new_handle(L, result);
      } else {
        luaX_push(L, luaX_nil);
        luaX_push(L, dlerror());
      }
    }
  }

  void initialize_dlfcn(lua_State* L) {
    luaX_set_field(L, -1, "dlopen", impl_dlopen);

    luaX_set_field(L, -1, "RTLD_LAZY", RTLD_LAZY);
    luaX_set_field(L, -1, "RTLD_NOW", RTLD_NOW);
    luaX_set_field(L, -1, "RTLD_GLOBAL", RTLD_GLOBAL);
    luaX_set_field(L, -1, "RTLD_LOCAL", RTLD_LOCAL);

    new_handle(L, RTLD_DEFAULT);
    luaX_set_field(L, -2, "RTLD_DEFAULT");
    new_handle(L, RTLD_NEXT);
    luaX_set_field(L, -2, "RTLD_NEXT");
  }
}
