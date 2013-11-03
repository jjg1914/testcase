# Copyright (C) 2012 John J. Glynn
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Source file root directory
SRC ?= src
# Header file root directory
INCLUDE ?= include
# Default build directory
BIN ?= bin

# Project name.
PROJECT = TestCase++

# Executable to build
EXECUTABLES = test

# Packages to build with. Requires pkg-config identifiers
PACKAGES =

# Files that are part of the project, but are not compiled
EXTRA = .gitignore

# Other libaries
LDLIBS =

# assembler
AS = as
# scanner generator
LEX = flex
# parser generator
YACC = bison
# C compiler
CC = gcc
# C++ compiler
CXX = g++
# linker
LD = g++

# assembler flags
ASFLAGS = 
# c preproccessor flags
CPPFLAGS = -I $(CURDIR)/$(INCLUDE) -std=c++11
# lex flags
LFLAGS = 
# yacc flags
YFLAGS = -d -y
# c flags
CFLAGS = -g -Wall -Wextra -Werror
# c++ flags
CXXFLAGS = $(CFLAGS)
# linking flags
LDFLAGS = -pthread

# END OPTIONS

# assembly source files
ASSOURCE = $(subst $(SRC)/,,$(shell find $(SRC) -name "[^.]*.s"))
ASOUTPUT = $(subst .s,.o,$(ASSOURCE))
# yacc source files
YSOURCE = $(subst $(SRC)/,,$(shell find $(SRC) -name "[^.]*.y"))
YOUTPUT = $(subst .y,.c,$(YSOURCE))
# lex source files
LSOURCE = $(subst $(SRC)/,,$(shell find $(SRC) -name "[^.]*.l"))
LOUTPUT = $(subst .l,.c,$(LSOURCE))
# c source files
CSOURCE = $(subst $(SRC)/,,$(shell find $(SRC) -name "[^.]*.c"))
CHEADER = $(subst $(INCLUDE)/,,$(shell find $(INCLUDE) -name "[^.]*.h"))
COUTPUT = $(subst .c,.o,$(CSOURCE))\
		  $(subst .y,.o,$(YSOURCE))\
		  $(subst .l,.o,$(LSOURCE))
# c++ source files
CXXSOURCE = $(subst $(SRC)/,,$(shell find $(SRC) -name "[^.]*.cpp"))
CXXHEADER = $(subst $(INCLUDE)/,,$(shell find $(INCLUDE) -name "[^.]*.hpp"))
CXXOUTPUT = $(subst .cpp,.o,$(CXXSOURCE))

export SRCDIR = $(CURDIR)/$(SRC)
export INCLUDEDIR = $(CURDIR)/$(INCLUDE)

export PACKAGES
export PROJECT

export AS
export LEX
export YACC
export CC
export CXX
export LD

export ASFLAGS
export CPPFLAGS
export LFLAGS
export YFLAGS
export LFLAGS
export CFLAGS
export CXXFLAGS
export LDFLAGS

export LDLIBS

export ASSOURCE
export ASOUTPUT
export YSOURCE
export YOUTPUT
export YOBJECT
export LSOURCE
export LOUTPUT
export LOBJECT
export CSOURCE
export COUTPUT
export CXXSOURCE
export CXXOUTPUT

all : $(EXECUTABLES)

$(EXECUTABLES): yacc lex
	$(MAKE) $@ -C $(BIN) "EXECUTABLE=$@"

compile: as c cxx

as:
	$(MAKE) as -C $(BIN)

c: yacc lex
	$(MAKE) c -C $(BIN)

cxx: 
	@echo $(CXXSOURCE)
	$(MAKE) cxx -C $(BIN)

yacc :
	$(MAKE) yacc -C $(BIN)

lex :
	$(MAKE) lex -C $(BIN)

dist :
	tar --transform 's,^,$(PROJECT)/,' -czf $(PROJECT).tar.gz\
		Makefile $(BIN)/Makefile\
		$(EXTRA)\
		$(addprefix $(SRC)/,$(CSOURCE))\
		$(addprefix $(SRC)/,$(YSOURCE))\
		$(addprefix $(SRC)/,$(LSOURCE))\
		$(addprefix $(SRC)/,$(CXXSOURCE))\
		$(addprefix $(INCLUDE)/,$(CHEADER))\
		$(addprefix $(INCLUDE)/,$(CXXHEADER))

clean :
	$(MAKE) clean -C $(BIN)
