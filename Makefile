FNAME    =result
LFLAGS   = -o
CFLAGS   = -c -Wall -std=c++17
LIB_PATH = ../../../dx_library/Trie/

$(FNAME): $(FNAME).o
	@g++ $(LFLAGS) $(FNAME) main.o
	@./$(FNAME)

$(FNAME).o:
	@g++ $(CFLAGS)  main.cpp

auxilary_files:


clean:
	@rm -rf $(FNAME) *.o *.gv *.ps