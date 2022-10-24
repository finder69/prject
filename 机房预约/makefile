INC = ./include
SRC = ./src
OBJ = ./obj
BIN = ./bin

SOURCE = $(wildcard $(SRC)/*.cpp)
OBJECT = $(patsubst %.cpp, ${OBJ}/%.o, $(notdir $(SOURCE)))

TARGET = out
BIN_TARGET = $(BIN)/$(TARGET)

CC = g++
CXXFLAGS = -g  -I${INC}

${BIN_TARGET}:${OBJECT}
	$(CC) -o $@ $(OBJECT)

${OBJ}/%.o:${SRC}/%.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

.PHONY:clean
clean:
	find $(OBJ) -name *.o -exec rm -rf {} \;
	rm -rf${BIN—TARGET}

