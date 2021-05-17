CC=clang
CXX=clang++

CFLAGS= 
LIBS= 

project=pred

all: ${project} test_project

test_project: ${project}.c Makefile.test
	mkdir -p test
	make -C ./ -f Makefile.test

${porject}: ${project}.c 
	${CC} ${CFLAGS} ${porject}.c -o ${project} ${LIBS}

.PHONY: clean 

clean: 
	rm -rf ${project}
	make -C ./ -f Makefile.test clean
