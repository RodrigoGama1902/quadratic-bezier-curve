TARGET=a.exe
CXX=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
SFML_STATIC_LIB_DIR= C:\SFML-2.5.1\lib
SFML_INCLUDE_DIR= C:\SFML-2.5.1\include
SFML_STATIC_LIBS=-lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -mwindows
CXXFLAGS=$(DEBUG) $(OPT) $(WARN) -I$(SFML_INCLUDE_DIR) -DSFML_STATIC
LDFLAGS=-L$(SFML_STATIC_LIB_DIR) $(SFML_STATIC_LIBS)
OBJS= main.o


all: $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@rm *.o
	@./$(TARGET) 2>/dev/null

clear:
	@rm *.o
	@rm $(TARGET)

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp -o main.o

