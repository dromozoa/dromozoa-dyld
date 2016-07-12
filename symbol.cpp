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

#include "common.hpp"

namespace dromozoa {
  namespace {
    class symbol {
    public:
      explicit symbol(void* ptr) : ptr_(ptr) {}

      void* get() const {
        return ptr_;
      }

    private:
      void* ptr_;
    };

    symbol* check_symbol(lua_State* L, int arg) {
      return luaX_check_udata<symbol>(L, arg, "dromozoa.dyld.symbol");
    }

    void impl_get(lua_State* L) {
      lua_pushlightuserdata(L, check_symbol(L, 1)->get());
    }

    void impl_is_null(lua_State* L) {
      luaX_push(L, check_symbol(L, 1)->get() == 0);
    }
  }

  void new_symbol(lua_State* L, void* ptr) {
    luaX_new<symbol>(L, ptr);
    luaX_set_metatable(L, "dromozoa.dyld.symbol");
  }

  void initialize_symbol(lua_State* L) {
    lua_newtable(L);
    {
      luaL_newmetatable(L, "dromozoa.dyld.symbol");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      lua_pop(L, 1);

      luaX_set_field(L, -1, "get", impl_get);
      luaX_set_field(L, -1, "is_null", impl_is_null);
    }
    luaX_set_field(L, -2, "symbol");
  }
}
