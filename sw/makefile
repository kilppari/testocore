# ------------------------------------------------------------------------------
# Temporary makefile
# ------------------------------------------------------------------------------

# Compiler:
CC=g++

# Flags:
CFLAGS=-c -Wall -g

# Binary name
FILENAME=run

# Libraries
LIBS=-lglfw3 -lopengl32 -lglew32 -lgdi32

run: temp

# ------------------------------------------------------------------------------
# UT targets
# ------------------------------------------------------------------------------
temp: mem_pool.o gl_renderable.o gl_renderer.o temp.o
	$(CC) -o $(FILENAME) mem_pool.o gl_renderable.o gl_renderer.o temp.o $(LIBS)


# ------------------------------------------------------------------------------
# Compile SW files
# ------------------------------------------------------------------------------
temp.o: temp.cpp
	$(CC) $(CFLAGS) temp.cpp

gl_renderable.o: gl_renderable.cpp
	$(CC) $(CFLAGS) gl_renderable.cpp

gl_renderer.o: gl_renderer.cpp
	$(CC) $(CFLAGS) gl_renderer.cpp

mem_pool.o: mem_pool.cpp
	$(CC) $(CFLAGS) mem_pool.cpp

# Cleanup
clean:
	rm *.o *~
