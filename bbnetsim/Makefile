CXX      := g++
CXXFLAGS := -std=c++20 -I$(NS3_INCLUDE_DIR) -I./include
LDFLAGS  := -L$(NS3_LIB_DIR) -lns3-dev-core-default -lns3-dev-network-default  \
            -lns3-dev-wifi-default -lns3-dev-propagation-default               \
			-lns3-dev-mobility-default

TARGET   := bbsim
OBJS     := main.o mesh-network.o imu-data.o

.PHONY: check_env check_dirs all clean

all: check_env check_dirs bin/$(TARGET)

bin/$(TARGET): $(patsubst %,build/%,$(OBJS))
	@echo Linking...
	@$(CXX) $^ -o $@ $(LDFLAGS)
	@echo Done.

build/main.o: src/main.cc
	@echo Building $@...
	@$(CXX) $(CXXFLAGS) -g -c $< -o $@

build/mesh-network.o: src/mesh-network.cc include/mesh-network.h
	@echo Building $@...
	@$(CXX) $(CXXFLAGS) -g -c $< -o $@

build/imu-data.o: src/imu-data.cc include/imu-data.h include/mesh-network.h
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
