#!/usr/bin/env vim

CC	:= g++
V	:= @
RM	+= -r
LIB += -Llib -lpthread
OBJ := ./obj/
SRC := ./src/
USER_FLAGS+= -O4 -I$(SRC) -std=c++11

targets := $(wildcard $(SRC)main/*.cpp) $(wildcard $(SRC)main/*.c)
objects := $(wildcard $(SRC)*/*.cpp) $(wildcard $(SRC)*/*.c)
objects := $(filter-out $(targets), $(objects))
objects := $(patsubst %.cpp,%.o,$(objects))
dirctry := $(sort $(dir $(objects)))
dirctry := $(patsubst %/,%,$(dirctry))
objects := $(notdir $(objects))
objects := $(addprefix $(OBJ),$(objects))
targets := $(basename $(notdir $(targets)))
targets := $(addprefix $(OBJ),$(targets))

define make-target
$(OBJ)$1: $(SRC)main/$1.cpp $(objects)
	@echo + cc $$<
	$(V)$(CC) $(USER_FLAGS) -o $$@ $$^ $(LIB)
endef

define make-intermediate 
$(OBJ)%.o: $1/%.cpp
	@echo + cc $$<
	$(V)$(CC) -c $(USER_FLAGS) -o $$@ $$^ 
endef

all:always $(targets)

run:all
	$(V) ./$(OBJ)/main
	$(V) cd alibaba && make test-init-3v1-month test-lcc

$(foreach btar,$(targets),$(eval $(call make-target,$(notdir $(btar)))))
$(foreach bdir,$(dirctry),$(eval $(call make-intermediate,$(bdir))))

.PHONY:clean always
always:
	$(V)mkdir -p $(OBJ)
	$(V)mkdir -p data
	$(V)mkdir -p data/output
clean:
	$(V)$(RM) $(OBJ)
