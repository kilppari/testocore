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
SRC_PATH=../sw

# Path for binaries
BIN_PATH=bin

all: ut_mem_pool ut_playground ut_timer ut_gl_renderer

_SW_OBJS =	mem_pool.o \
		ut.o \
		timer.o \
		gl_renderable.o \
		gl_renderer.o \

SW_OBJS = $(patsubst %,$(BIN_PATH)/%,$(_SW_OBJS))

# ------------------------------------------------------------------------------
# TARGETS
# ------------------------------------------------------------------------------

## 1. ut_playground
UT_PLAYGROUND_OBJS = $(SW_OBJS) $(BIN_PATH)/ut_playground.o
ut_playground: $(UT_PLAYGROUND_OBJS)
	$(CC) -o $(BIN_PATH)/$(EXEPREFIX)$@ $(UT_PLAYGROUND_OBJS) $(LIBS)

## 2. ut_mem_pool
UT_MEM_POOL_OBJS = bin/mem_pool.o bin/timer.o bin/ut.o bin/ut_mem_pool.o
ut_mem_pool: $(UT_MEM_POOL_OBJS)
	$(CC) -o $(BIN_PATH)/$(EXEPREFIX)$@ $(UT_MEM_POOL_OBJS)

## 3. ut_timer
UT_TIMER_OBJS = bin/timer.o bin/ut.o bin/ut_timer.o
ut_timer: $(UT_TIMER_OBJS)
	$(CC) -o $(BIN_PATH)/$(EXEPREFIX)$@ $(UT_TIMER_OBJS)

## 4. ut_gl_renderer
UT_GL_RENDERER_OBJS = $(SW_OBJS) $(BIN_PATH)/ut_gl_renderer.o
ut_gl_renderer: $(UT_GL_RENDERER_OBJS)
	$(CC) -o $(BIN_PATH)/$(EXEPREFIX)$@ $(UT_GL_RENDERER_OBJS) $(LIBS)

## 5. ut_process
UT_PROCESS_OBJS = bin/ut.o bin/ut_process.o
ut_process: $(UT_PROCESS_OBJS)
	$(CC) -o $(BIN_PATH)/$(EXEPREFIX)$@ $(UT_PROCESS_OBJS)

# ------------------------------------------------------------------------------
# Compile SW and UT files
# ------------------------------------------------------------------------------
# When required, compiles file.cpp sources into file.o objects
# $< refers to the right side of the : below
$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_PATH)/%.o: %.cpp
	$(CC) $(CFLAGS) $^ -I$(SRC_PATH) -o $@


# Cleanup
clean:
	rm bin/*.o bin/*.exe
