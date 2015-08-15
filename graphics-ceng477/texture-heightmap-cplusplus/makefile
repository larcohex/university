LIB=-Llib
INC=-Iinc
SRC=hw3.cpp glinit.cpp glentry.cpp utils.cpp
DIRS=lib/ inc/ res/

all:
	g++ $(SRC) $(LIB) $(INC) -o hw3  -lSOIL -lGL -lGLU -lglut  -lpthread
	
dist:
	@tar -czvf texture.tar.gz $(DIRS) $(SRC) Makefile --exclude "*~"
