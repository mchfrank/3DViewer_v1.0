SRCS        := s21_3DV.c
TEST_SRCS	:= test.c
OBJS        := $(SRCS:.c=.o)
TEST_OBJS	:= $(TEST_SRCS:.c=.o)

CC				:= gcc
CFLAGS			:= -Wall -Werror -Wextra
PROFILE_FLAGS 	:= -fprofile-arcs -ftest-coverage
TST_LIBS		:= -lcheck -lm -lpthread
COV_LIBS		:= -lgcov -coverage


all: clean uninstall install open


install:
	make clean
	mkdir build
	cd 3DV && qmake6 && make && make clean && rm Makefile && cd ../ && mv 3DV/3DV.app build

uninstall:
	rm -rf build*

test: clean
	$(CC) $(CFLAGS) $(SRCS) $(TEST_SRCS) $(PROFILE_FLAGS) $(TST_LIBS) -o test
	./test

gcov_report: test
	lcov -t "3DV" -o test.info -c -d .
	genhtml -o report test.info
	open ./report/index.html

clean:
	rm -rf *.o *.a
	rm -rf *.gcno *gcda *.gcov *.info
	rm -rf test ./report && rm -rf .qmake.stash && rm -rf .qmake.stash

style:
	cp ../materials/linters/.clang-format .
	clang-format -n *.h *.c
	rm .clang-format

autostyle:
	cp ../materials/linters/.clang-format .
	clang-format -i *.h *.c
	rm .clang-format

dist: install
	rm -rf Dist_3DV/
	mkdir Dist_3DV/
	mkdir Dist_3DV/src
	mv ./build/3DV.app Dist_3DV/src/
	tar cvzf Dist_3DV.tgz Dist_3DV/
	rm -rf Dist_3DV/
	rmdir build

open:
	open ./build/3DV.app/Contents/MacOS/3DV


