# Copyright (C) 2016-2018 Tomoyuki Fujimori <moyu@dromozoa.com>
#
# This file is part of dromozoa-dyld.
#
# dromozoa-dyld is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# dromozoa-dyld is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with dromozoa-dyld.  If not, see <http://www.gnu.org/licenses/>.

CPPFLAGS += -Ibind -I$(LUA_INCDIR)
CXXFLAGS += -Wall -W $(CFLAGS)
LDLIBS += -ldl

OBJS = \
	handle.o \
	module.o \
	symbol.o
TARGET = dyld.so

all: $(TARGET)

check:
	./test.sh

clean:
	rm -f *.o $(TARGET)

dyld.so: $(OBJS)
	$(CXX) $(LDFLAGS) $(LIBFLAG) $^ $(LDLIBS) -o $@

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

install:
	mkdir -p $(LIBDIR)/dromozoa
	cp $(TARGET) $(LIBDIR)/dromozoa
