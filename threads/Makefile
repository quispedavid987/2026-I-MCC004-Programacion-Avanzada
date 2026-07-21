# Variables
CXX = g++
CXXFLAGS = -std=c++11 -O3 -pthread
TARGET = demoThreads
SRC = demoThreads.cpp

# Regla por defecto: compila el programa
all: $(TARGET)

# Regla para construir el ejecutable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Regla para compilar y ejecutar inmediatamente
run: $(TARGET)
	./$(TARGET)

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET)