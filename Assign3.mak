# Linux (default)
EXE = 3gc3_t1
LDFLAGS = -lGL -lGLU -lglut
CFLAGS=-g -Wall
CC=g++

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
    EXE = 3gc3_t1.exe
        LDFLAGS = -lfreeglut -lglu32 -lopengl32
endif

# OS X
OS := $(shell uname)
ifeq ($(OS), Darwin)
        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

a3: character.cpp
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm ./*.o ./*.exe ./t1