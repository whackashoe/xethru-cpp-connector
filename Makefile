CXX=g++
CPPFLAGS=-std=c++11 -Wall -Wextra -pedantic -O0
CFLAGS=
LDFLAGS=-lserial
EXAMPLE_DIR=examples
SRC_DIR=include
RM=rm -f


all: presence_example respiration_example 
	@echo "complete"

presence_example: $(EXAMPLE_DIR)/presence_example.o
	$(CXX) $(CPPFLAGS) $(EXAMPLE_DIR)/presence_example.o -o $(EXAMPLE_DIR)/presence_example $(LDFLAGS)
	@echo "presence_example built"

respiration_example: $(EXAMPLE_DIR)/respiration_example.o
	$(CXX) $(CPPFLAGS) $(EXAMPLE_DIR)/respiration_example.o -o $(EXAMPLE_DIR)/respiration_example $(LDFLAGS)
	@echo "respiration_example built"

$(EXAMPLE_DIR)/presence_example.o: $(EXAMPLE_DIR)/presence_example.cpp $(SRC_DIR)/*.hpp
	$(CXX) $(CPPFLAGS) -c $(EXAMPLE_DIR)/presence_example.cpp -o $(EXAMPLE_DIR)/presence_example.o

$(EXAMPLE_DIR)/respiration_example.o: $(EXAMPLE_DIR)/respiration_example.cpp $(SRC_DIR)/*.hpp
	$(CXX) $(CPPFLAGS) -c $(EXAMPLE_DIR)/respiration_example.cpp -o $(EXAMPLE_DIR)/respiration_example.o

clean:
	$(RM) $(EXAMPLE_DIR)/*.o $(EXAMPLE_DIR)/presence_example $(EXAMPLE_DIR)/respiration_example
	@echo "cleaned"
