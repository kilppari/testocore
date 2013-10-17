# Makefile for unit tests

# Compiler:
CC=g++

# Flags:
CFLAGS=-c -Wall

ut_run: ut_mem_pool

# Create UTs
ut_mem_pool: ut_mem_pool.o mem_pool.o
	$(CC) -o run_ut ut_mem_pool.o mem_pool.o -lrt

# Compile UT files
ut_mem_pool.o: ut_mem_pool.cpp
	$(CC) $(CFLAGS) ut_mem_pool.cpp -I../sw

# Compile SW files
mem_pool.o: ../sw/mem_pool.cpp
	$(CC) $(CFLAGS) ../sw/mem_pool.cpp

# Cleanup
clean:
	rm -rf *o run_ut