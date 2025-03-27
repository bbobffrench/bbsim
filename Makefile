CXX      := g++
CXXFLAGS := -std=c++20 -I$(NS3_INCLUDE_DIR) -I./include
LDFLAGS  := -L$(NS3_LIB_DIR) -lns3-dev-core-default -lns3-dev-network-default           \
            -lns3-dev-wifi-default -lns3-dev-mobility-default -lns3-dev-netanim-default \

TARGET   := bbsim
OBJS     := mesh-network.o buffalo-byte.o bbsim.o main.o

.PHONY: check_env check_dirs all clean

all: check_env check_dirs bin/$(TARGET)

bin/$(TARGET): $(patsubst %,build/%,$(OBJS))
	@echo Linking...
	@$(CXX) $^ -o $@ $(LDFLAGS)
	@echo Done.

build/mesh-network.o: src/mesh-network.cc include/mesh-network.h
	@echo Building $@...
	@$(CXX) $(CXXFLAGS) -g -c $< -o $@

build/buffalo-byte.o: src/buffalo-byte.cc include/buffalo-byte.h include/mesh-network.h
	@echo Building $@...
	@$(CXX) $(CXXFLAGS) -g -c $< -o $@

build/bbsim.o: src/bbsim.cc include/bbsim.h include/buffalo-byte.h include/mesh-network.h
	@echo Building $@...
	@$(CXX) $(CXXFLAGS) -g -c $< -o $@

build/main.o: src/main.cc include/bbsim.h 
	@echo Building $@...
	@$(CXX) $(CXXFLAGS) -g -c $< -o $@

check_env:
ifndef NS3_INCLUDE_DIR
	$(error NS3_INCLUDE_DIR must be defined)
endif

ifndef NS3_LIB_DIR
	$(error NS3_LIB_DIR must be defined)
endif

check_dirs:
	@if [ ! -d "build" ]; then mkdir build; fi
	@if [ ! -d "bin" ]; then mkdir bin; fi

clean:
	@rm -rf build/ bin/
