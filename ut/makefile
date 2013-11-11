# ------------------------------------------------------------------------------
# Makefile for unit tests
# ------------------------------------------------------------------------------

# Compiler:
CC=g++

# Flags:
CFLAGS=-c -Wall -g

# Libraries
LIBS=-lglfw3 -lopengl32 -lglew32 -lgdi32

# Executive prefix
EXEPREFIX=run_

# SW source path (relative)
SRC_PATH=../sw/

all: ut_mem_pool ut_playground ut_timer

# ------------------------------------------------------------------------------
# ut_mem_pool
# ------------------------------------------------------------------------------
UT_MEM_POOL_OBJS = mem_pool.o ut.o timer.o ut_mem_pool.o
ut_mem_pool: $(UT_MEM_POOL_OBJS)
	$(CC) -o $(EXEPREFIX)$@ $(UT_MEM_POOL_OBJS)

ut_mem_pool.o: ut_mem_pool.cpp
	$(CC) $(CFLAGS) ut_mem_pool.cpp -I$(SRC_PATH)

# ------------------------------------------------------------------------------
# ut_playground
# ------------------------------------------------------------------------------
UT_PLAYGROUND_OBJS = 	mem_pool.o \
			ut.o \
			timer.o \
			gl_renderable.o \
			gl_renderer.o \
			ut_playground.o
ut_playground: $(UT_PLAYGROUND_OBJS)
	$(CC) -o $(EXEPREFIX)$@ $(UT_PLAYGROUND_OBJS) $(LIBS)

ut_playground.o: ut_playground.cpp
	$(CC) $(CFLAGS) ut_playground.cpp -I$(SRC_PATH)

# ------------------------------------------------------------------------------
# ut_timer
# ------------------------------------------------------------------------------
UT_TIMER_OBJS = ut.o timer.o ut_timer.o
ut_timer: $(UT_TIMER_OBJS)
	$(CC) -o $(EXEPREFIX)$@ $(UT_TIMER_OBJS)

ut_timer.o: ut_timer.cpp
	$(CC) $(CFLAGS) ut_timer.cpp -I$(SRC_PATH)


# ------------------------------------------------------------------------------
# Compile SW files
# ------------------------------------------------------------------------------
# When required, compiles file.cpp sources into file.o objects
# $< refers to the right side of the : below
%.o: $(SRC_PATH)%.cpp
	$(CC) $(CFLAGS) $^

# Cleanup
clean:
	rm *.o *~ *.exe
