CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -g
VG       = valgrind --leak-check=full --show-leak-kinds=all \
           --error-exitcode=1

TARGET   = dequetest
OBJS     = main.o deque.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# One rule per object, with its real header dependencies.
main.o: main.cpp deque.h stack.h queue.h
	$(CXX) $(CXXFLAGS) -I. -c main.cpp

deque.o: deque.cpp deque.h
	$(CXX) $(CXXFLAGS) -I. -c deque.cpp

public_tests: tests/public_tests.cpp deque.o deque.h stack.h queue.h
	$(CXX) $(CXXFLAGS) -I. -o public_tests tests/public_tests.cpp deque.o

test: $(TARGET)
	./$(TARGET)

check: public_tests
	./public_tests

valgrind: $(TARGET)
	$(VG) ./$(TARGET)

vcheck: public_tests
	$(VG) ./public_tests

clean:
	rm -f $(OBJS) $(TARGET) public_tests

.PHONY: all test check valgrind vcheck clean
