CXX = clang++
CXXFLAGS = -std=c++17 -pthread -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -Wuninitialized -g
LD_FLAGS = -std=c++17 -pthread -g

src = $(wildcard *.cpp)
mains := config_generator.o mvs.o mvs_parallel.o
obj = $(src:.cpp=.o)
libs = $(filter-out $(mains), $(obj))

all: mvs mvs_parallel config_generator

mvs: $(libs) mvs.o
	$(CXX) -o $@ $^ $(LD_FLAGS)

mvs_parallel: $(libs) mvs_parallel.o
	$(CXX) -o $@ $^ $(LD_FLAGS)

config_generator: $(libs) config_generator.o
	$(CXX) -o $@ $^ $(LD_FLAGS)

clear:
	rm $(obj) mvs mvs_parallel config_generator