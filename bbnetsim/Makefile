CXX      := g++
CXXFLAGS := -std=c++20 -I$(NS3_INCLUDE_DIR)
LDFLAGS  := -L$(NS3_LIB_DIR)

TARGET   := bbsim
OBJS     := main.o

.PHONY: check_env check_dirs all clean

all: check_env check_dirs bin/$(TARGET)

bin/$(TARGET): $(patsubst %,build/%,$(OBJS))

check_env:
ifndef NS3_INCLUDE_DIR
	$(error NS3_INCLUDE_DIR must be defined)
endif

ifndef NS3_LIB_DIR
	$(error NS3_LIB_DIR must be defined)
endif

check_dirs:
	@if[ ! -d "build" ]; then mkdir bin; fi
	@if[ ! -d "bin" ]; then mkdir bin; fi

clean:
	@rm -rf build/* bin/*
