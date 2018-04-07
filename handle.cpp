// Copyright (C) 2016,2018 Tomoyuki Fujimori <moyu@dromozoa.com>
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

      ~handle() {
        if (ptr_) {
          if (dlclose() != 0) {
            DROMOZOA_UNEXPECTED(dlerror());
          }
        }
      }

      int dlclose() {
        void* ptr = ptr_;
        ptr_ = 0;
        return ::dlclose(ptr);
      }

      void* get() const {
        return ptr_;
      }

    private:
      void* ptr_;
      handle(const handle&);
      handle& operator=(const handle&);
    };

    void new_handle_ref(lua_State* L, void* ptr) {
      luaX_new<handle>(L, ptr);
      luaX_set_metatable(L, "dromozoa.dyld.handle_ref");
    }

    void new_handle(lua_State* L, void* ptr) {
      luaX_new<handle>(L, ptr);
      luaX_set_metatable(L, "dromozoa.dyld.handle");
    }

    handle* check_handle(lua_State* L, int arg) {
      return luaX_check_udata<handle>(L, arg, "dromozoa.dyld.handle_ref", "dromozoa.dyld.handle");
    }

    void push_error(lua_State* L) {
      luaX_push(L, luaX_nil);
      if (const char* message = dlerror()) {
        luaX_push(L, message);
      }
    }

    void impl_gc(lua_State* L) {
      check_handle(L, 1)->~handle();
    }

    void impl_dlclose(lua_State* L) {
      if (check_handle(L, 1)->dlclose() == 0) {
        luaX_push_success(L);
      } else {
        push_error(L);
      }
    }

    void impl_dlsym(lua_State* L) {
      const char* name = luaL_checkstring(L, 2);
      // clear error
      dlerror();
      if (void* result = dlsym(check_handle(L, 1)->get(), name)) {
        new_symbol(L, result);
      } else {
        // return nil without error message if handle is NULL
        push_error(L);
      }
    }

    void impl_get(lua_State* L) {
      lua_pushlightuserdata(L, check_handle(L, 1)->get());
    }

    void impl_dlopen(lua_State* L) {
      const char* file = lua_tostring(L, 1);
      int mode = luaX_check_integer<int>(L, 2);
      if (void* result = dlopen(file, mode)) {
        new_handle(L, result);
      } else {
        push_error(L);
      }
    }

    void impl_dlopen_pthread(lua_State* L) {
      luaX_get_field(L, LUA_REGISTRYINDEX, "dromozoa.dyld.pthread");
      bool is_nil = lua_isnil(L, -1);
      lua_pop(L, 1);
      if (is_nil) {
        if (dlsym(RTLD_DEFAULT, "gnu_get_libc_version")) {
          if (void* result = dlopen("libpthread.so.0", RTLD_LAZY | RTLD_GLOBAL)) {
            new_handle(L, result);
            luaX_set_field(L, LUA_REGISTRYINDEX, "dromozoa.dyld.pthread");
            luaX_push_success(L);
          } else {
            push_error(L);
          }
        } else {
          luaX_set_field(L, LUA_REGISTRYINDEX, "dromozoa.dyld.pthread", true);
          luaX_push_success(L);
        }
      } else {
        luaX_push_success(L);
      }
    }

    void impl_dlclose_pthread(lua_State* L) {
      int result = 0;
      luaX_get_field(L, LUA_REGISTRYINDEX, "dromozoa.dyld.pthread");
      if (handle* self = luaX_to_udata<handle>(L, -1, "dromozoa.dyld.handle")) {
        result = self->dlclose();
      }
      lua_pop(L, 1);
      luaX_set_field(L, LUA_REGISTRYINDEX, "dromozoa.dyld.pthread", luaX_nil);
      if (result == 0) {
        luaX_push_success(L);
      } else {
        push_error(L);
      }
    }
  }

  void initialize_handle(lua_State* L) {
    lua_newtable(L);
    {
      luaL_newmetatable(L, "dromozoa.dyld.handle_ref");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      lua_pop(L, 1);

      luaL_newmetatable(L, "dromozoa.dyld.handle");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      luaX_set_field(L, -1, "__gc", impl_gc);
      lua_pop(L, 1);

      luaX_set_field(L, -1, "dlclose", impl_dlclose);
      luaX_set_field(L, -1, "dlsym", impl_dlsym);
      luaX_set_field(L, -1, "get", impl_get);
    }
    luaX_set_field(L, -2, "handle");

    luaX_set_field(L, -1, "dlopen", impl_dlopen);
    luaX_set_field(L, -1, "dlopen_pthread", impl_dlopen_pthread);
    luaX_set_field(L, -1, "dlclose_pthread", impl_dlclose_pthread);

    luaX_set_field(L, -1, "RTLD_LAZY", RTLD_LAZY);
    luaX_set_field(L, -1, "RTLD_NOW", RTLD_NOW);
    luaX_set_field(L, -1, "RTLD_GLOBAL", RTLD_GLOBAL);
    luaX_set_field(L, -1, "RTLD_LOCAL", RTLD_LOCAL);

    new_handle_ref(L, RTLD_DEFAULT);
    luaX_set_field(L, -2, "RTLD_DEFAULT");
    new_handle_ref(L, RTLD_NEXT);
    luaX_set_field(L, -2, "RTLD_NEXT");
  }
}
