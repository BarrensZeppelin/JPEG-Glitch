EXE=glitch
SOURCES=main.cpp Glitcher.cpp
OBJS=$(SOURCES:.cpp=.o)

$(EXE): $(OBJS)
	$(CXX) $^ -o $(EXE) -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm $(OBJS) $(EXE)

