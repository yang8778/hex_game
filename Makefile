CC=g++
LD=g++

CXXFLAGS=-std=gnu++0x -g -O3 

TARGET=./test

.PHONY:all
all:$(TARGET)

OBJS= \
	graph.o \
	board.o \
	player.o\
	main.o

$(TARGET):$(OBJS)
	g++ -o $(TARGET) $(LDFLAGS) -o $@ $^ $(LIBS)

%.d:%.cpp
	@$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

sinclude $(OBJS:.o=.d)

.PHONY:clean
clean:
	rm -rf $(OBJS) $(OBJS:.o=.d) #$(TARGET)
