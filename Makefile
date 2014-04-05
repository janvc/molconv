# Copyright 2014 Jan von Cosel
#
# This file is part of molconv.
#
# molconv if free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# molconv is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have recieved a copy of the GNU General Public License
# along with molconv. If not, see <http://www.gnu.org/licenses/>.
#
############################################################################
#

# variable definitions:
CXX = g++
MKDIR_P = mkdir -p

SRC_DIR = source
OBJ_DIR = objects
BIN_DIR = bin
DIRS = $(OBJ_DIR) $(BIN_DIR)

CFLAGS = -Wall -Wextra -g --pedantic-errors -O0 -std=c++0x
LINKFLAGS = -lboost_system -lboost_filesystem -lboost_program_options
INCLUDES = -I/usr/include/eigen3

VPATH = $(SRC_DIR): \
        $(OBJ_DIR)

MOLCONV = $(addprefix $(BIN_DIR)/,molconv)

MOLCONV_OBJS = $(addprefix $(OBJ_DIR)/,\
                 exceptions.o      \
                 atom.o            \
                 molecule.o        \
                 utilities.o       \
                 molconv.o         \
                 )


# build dependencies:
all: $(DIRS) $(MOLCONV)

$(MOLCONV): $(MOLCONV_OBJS)
	$(CXX) -o $@ $^ $(LINKFLAGS)

$(OBJ_DIR)/molconv.o: $(addprefix $(OBJ_DIR)/,\
                        molecule.o      \
                        utilities.o     \
                        )

$(OBJ_DIR)/exceptions.o: $(addprefix $(SRC_DIR)/,\
                           exceptions.h   \
                           exceptions.cpp \
                           )

$(OBJ_DIR)/molecule.o: $(addprefix $(SRC_DIR)/,\
                         molecule.h        \
                         atom_properties.h \
                         utilities.h       \
                         )

$(OBJ_DIR)/atom.o: $(addprefix $(SRC_DIR)/,\
                     atom.h \
                     )

$(OBJ_DIR)/utilities.o: $(addprefix $(SRC_DIR)/,\
                          utilities.h       \
                          atom_properties.h \
                          )

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(INCLUDES) $< -o $@


$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

$(BIN_DIR):
	$(MKDIR_P) $(BIN_DIR)

clean:
	rm -r $(OBJ_DIR) $(BIN_DIR)

