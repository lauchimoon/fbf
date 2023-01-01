CC = g++
SRC = $(wildcard *.cc)
LDLIBS = -L../raylib/src/ -lraylib -lGL -lm -ldl -lpthread
INCLUDE = -I../raylib/src/ -I../raygui/src/
OUT = fbf

default:
	$(CC) $(SRC) $(LDLIBS) $(INCLUDE) -o $(OUT)

run: default
	./$(OUT)
	rm $(OUT)
