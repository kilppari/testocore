# ------------------------------------------------------------------------------
# Makefile for unit tests
# ------------------------------------------------------------------------------

# Compiler:
CC=g++

# Flags:
CFLAGS=-c -Wall -g

# Binary name
FILENAME=run_tc

ut_run: ut_mem_pool

# ------------------------------------------------------------------------------
# UT targets
# ------------------------------------------------------------------------------
ut_mem_pool: mem_pool.o ut.o timer.o ut_mem_pool.o
	$(CC) -o $(FILENAME) mem_pool.o ut.o timer.o ut_mem_pool.o

# ------------------------------------------------------------------------------
# Compile test scripts
# ------------------------------------------------------------------------------
ut_mem_pool.o: ut_mem_pool.cpp
	$(CC) $(CFLAGS) ut_mem_pool.cpp -I../sw

# ------------------------------------------------------------------------------
# Compile SW files
# ------------------------------------------------------------------------------
ut.o: ../sw/ut.cpp
	$(CC) $(CFLAGS) ../sw/ut.cpp

mem_pool.o: ../sw/mem_pool.cpp
	$(CC) $(CFLAGS) ../sw/mem_pool.cpp

timer.o: ../sw/timer.cpp
	$(CC) $(CFLAGS) ../sw/timer.cpp

# Cleanup
clean:
	rm *.o
