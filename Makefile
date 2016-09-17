.SUFFIXES:	.cpp .h
.PHONY:		clean

create:
	-rm *.h.gch
	g++ *.h *.cpp

clean:
	-rm *.h.gch
