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
CC = g++
MKDIR_P = mkdir -p

SRC_DIR = source
OBJ_DIR = objects
BIN_DIR = bin
DIRS = $(OBJ_DIR) $(BIN_DIR)

CFLAGS = -Wall -Wextra -g --pedantic-errors -O0

VPATH = $(SRC_DIR): \
        $(OBJ_DIR)

MOLCONV = $(addprefix $(BIN_DIR)/,molconv)

MOLCONV_OBJS = $(addprefix $(OBJ_DIR)/,\
                 atom_properties.o \
                 molecule.o        \
                 utilities.o       \
                 configuration.o   \
                 molconv.o         \
                 )


# build dependencies:
all: $(DIRS) $(MOLCONV)

$(MOLCONV): $(MOLCONV_OBJS)
	$(CC) -o $@ $^

$(OBJ_DIR)/molconv.o: $(addprefix $(OBJ_DIR)/,\
                        molecule.o      \
                        utilities.o     \
                        configuration.o \
                        )

$(OBJ_DIR)/molecule.o: $(addprefix $(SRC_DIR)/,\
                         molecule.h   \
                         )

$(OBJ_DIR)/utilities.o: $(addprefix $(SRC_DIR)/,\
                          utilities.h   \
                          )

$(OBJ_DIR)/configuration.o: $(addprefix $(SRC_DIR)/,\
                              configuration.h   \
                              )

$(OBJ_DIR)/atom_properties.o: $(addprefix $(SRC_DIR)/,\
                                atom_properties.h \
                                )

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@


$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

$(BIN_DIR):
	$(MKDIR_P) $(BIN_DIR)

clean:
	rm -r $(OBJ_DIR) $(BIN_DIR)

