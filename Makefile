EXE:= ./bin/a.out
SRC_DIR:= ./src
INC_DIR:= ./inc ./inc/cppjieba1
SRCS:= $(wildcard $(SRC_DIR)/*.cc)
CXXFLAGS:= -std=c++11

$(EXE):$(SRCS)
	clang++ -g -o $@ $^ $(addprefix -I,$(INC_DIR)) $(CXXFLAGS)

clean:
	rm ./bin/a.out
