#Copyright (c) 2015 Benjamin Pylko

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.

AT = @
INCDIR = -I$(HOME)/wpilib/cpp/current/include -Isrc/
CPPFLAGS += -g -Wall -W $(INCDIR) -std=c++14 -fPIC
LFLAGS = -L$(HOME)/wpilib/cpp/current/lib -lwpi
CXX = arm-frc-linux-gnueabi-g++
TEAM = 3181
RMCOMMAND = rm -f
DEPLOYTARGET = roboRIO-$(TEAM).local

SOURCES = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h)
OBJECTS = $(patsubst src/%.cpp,src/%.o,$(wildcard src/*.cpp))

all: $(OBJECTS)
	test -d bin/ || mkdir -p bin/
	$(CXX) $(CPPFLAGS) $(OBJECTS) $(LFLAGS) -o bin/FRCUserProgram
