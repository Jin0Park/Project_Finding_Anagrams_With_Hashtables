all: run

run: Hashtable.c Hashtable.h Main.c
	gcc -g Main.c -o main

test: test_suite

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR_A = ${HOME}/src/googletest-release-1.8.0/googletest
GOOGLE_TEST_LIB = ${HOME}/lib/gtest/libgtest.a
GOOGLE_TEST_INCLUDE = ${HOME}/src/googletest-release-1.8.0/googletest/include/

GTEST_DOWNLOAD = ${HOME}/src/googletest-release-1.8.0/

# Where to find user code.
USER_DIR = .

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR_A)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR_A)/include/gtest/*.h \
                $(GTEST_DIR_A)/include/gtest/internal/*.h

# Builds gtest.a

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR_A)/src/*.cc $(GTEST_DIR_A)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.

release-1.8.0.tar.gz : 
	@echo -e "\033[1;93m ----------------"
	@echo NEED GTEST ZIP
	@echo -e "----------------\033[0m"

${GTEST_DIR_A}/src/gtest-all.cc: release-1.8.0.tar.gz
	@echo -e "\033[1;93m==========================="
	@echo Extracting gtest source
	@echo -e "===========================\033[0m"
	mkdir -p ${HOME}/src
	cp release-1.8.0.tar.gz ${HOME}/src/
	tar -xzmf ${HOME}/src/release-1.8.0.tar.gz -C ${HOME}/src/

gtest_main.o : $(GTEST_DIR)/src/gtest_main.cc
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

$(GOOGLE_TEST_LIB): ${GTEST_DIR_A}/src/gtest-all.cc
	@echo -e "\033[1;93m==========================="
	@echo Building the GTest library
	@echo -e "===========================\033[0m"
	g++ -isystem ${GTEST_DIR_A}/include -I${GTEST_DIR_A} \
		-pthread -c ${GTEST_DIR_A}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	mkdir -p ~/lib/gtest/
	mv libgtest.a ~/lib/gtest/

lib: HashTable.c Assert007.c Assert007.h
	@echo -e "\033[1;93m==========================="
	@echo Building Example
	@echo -e "===========================\033[0m"
	echo Compiling the Example implementation
	gcc -g HashTable.c Assert007.c

## Puts it all together. 
test_suite: $(GOOGLE_TEST_LIB) test_hashtable.o LinkedList.o Hashtable.o Assert007.o
	g++ -o test_suite test_hashtable.o LinkedList.o Hashtable.o Assert007.o \
		 -L${HOME}/lib/gtest -lgtest -lpthread
	@echo -e "\033[38;5;198m===================================="
	@echo Run tests by running ./test_suite
	@echo -e "====================================\033[0m"

### This builds the hashtable library. Change the filenames accordingly. 
Hashtable.o: Hashtable.c Hashtable.h LinkedList.o
	gcc -c -Wall -g Hashtable.c -o Hashtable.o


## Compiles your hashtable tests.
test_hashtable.o : test_hashtable.cc
	g++ -c -Wall -I $(GOOGLE_TEST_INCLUDE) test_hashtable.cc \
		-o test_hashtable.o

Assert007.o: Assert007.c Assert007.h
	@echo -e "\033[1;93m======================================="
	@echo Building Assert007.o...
	@echo -e "====================================\033[0m"
	gcc -c -Wall -g Assert007.c -o Assert007.o

clean: 
	rm -rf *~ main
