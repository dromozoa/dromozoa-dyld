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
    class handle {
    public:
      explicit handle(void* ptr) : ptr_(ptr) {}

      void* get() const {
        return ptr_;
      }

    private:
      void* ptr_;
    };

    handle* check_handle(lua_State* L, int arg) {
      return luaX_check_udata<handle>(L, arg, "dromozoa.dyld.handle");
    }

    void impl_dlclose(lua_State* L) {
      if (dlclose(check_handle(L, 1)) == 0) {
        luaX_push_success(L);
      } else {
        luaX_push(L, luaX_nil);
        luaX_push(L, dlerror());
      }
    }

    void impl_dlsym(lua_State* L) {
      const char* name = luaL_checkstring(L, 2);
      dlerror(); // clear error
      if (void* result = ::dlsym(check_handle(L, 1)->get(), name)) {
        new_symbol(L, result);
      } else {
        if (const char* message = dlerror()) {
          luaX_push(L, luaX_nil);
          luaX_push(L, message);
        } else {
          new_symbol(L, 0);
        }
      }
    }

    void impl_get(lua_State* L) {
      lua_pushlightuserdata(L, check_handle(L, 1)->get());
    }
  }

  void new_handle(lua_State* L, void* ptr) {
    luaX_new<handle>(L, ptr);
    luaX_set_metatable(L, "dromozoa.dyld.handle");
  }

  void initialize_handle(lua_State* L) {
    lua_newtable(L);
    {
      luaL_newmetatable(L, "dromozoa.dyld.handle");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      lua_pop(L, 1);

      luaX_set_field(L, -1, "dlclose", impl_dlclose);
      luaX_set_field(L, -1, "dlsym", impl_dlsym);
      luaX_set_field(L, -1, "get", impl_get);
    }
    luaX_set_field(L, -2, "handle");
  }
}
