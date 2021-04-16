CXX = clang++
CXXFLAGS = -std=c++17 -pthread -fsanitize=address,undefined -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -Wuninitialized -g
LD_FLAGS = -std=c++17 -pthread -fsanitize=address,undefined -g

src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

mvs: $(obj)
	$(CXX) -o $@ $^ $(LD_FLAGS)

clear:
	rm $(obj) mvs