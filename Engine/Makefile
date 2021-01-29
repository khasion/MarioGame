TARGET = game
SRC = Src
INC = Include
BIN = Bin

CC = g++
CXXFLAGS = -std=c++14 -std=gnu++14 -lallegro_font -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_primitives -lallegro_color -lallegro

SOURCE = $(wildcard $(SRC)/*.cpp)
OBJECT = $(patsubst %,$(BIN)/%, $(notdir $(SOURCE:.cpp=.o)))

$(BIN)/$(TARGET) : $(OBJECT)
	$(CC) $(CXXFLAGS) -o $@ $^
$(BIN)/%.o : $(SRC)/%.cpp
	$(CC) -c $< -o $@ -std=c++14
clean:
	rm Bin/*.o
	rm Bin/game
