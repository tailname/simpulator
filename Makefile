# Путь к установке SystemC
SYSTEMC_HOME ?= /home/user/lib/systemc/install
SYSTEMC_LIB ?= $(SYSTEMC_HOME)/lib-linux64

CXX = g++
CXXFLAGS = -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_LIB) -I./src -std=c++14 -Wl,-rpath=$(SYSTEMC_LIB)/lib-linux64
LDFLAGS = -lsystemc -lm
#LD ?= /user/bin/ld
# Целевой исполняемый файл
TARGET = simulation

# Объектные файлыs
OBJECTS = main.o memory.o TestBench.o processor.o

# Правило по умолчанию
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Правила для объектных файлов


memory.o: src/memory.cpp src/memory.h src/common.h
	$(CXX) $(CXXFLAGS) -c src/memory.cpp

TestBench.o: src/TestBench.cpp src/TestBench.h src/common.h
	$(CXX) $(CXXFLAGS) -c src/TestBench.cpp

processor.o: src/processor.cpp src/processor.h src/common.h src/memory.h
	$(CXX) $(CXXFLAGS) -c src/processor.cpp

main.o: src/main.cpp src/common.h src/processor.h src/TestBench.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp
# Очистка
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
