all: decode

decode: c6d6_decode.o DataDef.o
	g++ -o $@ $^ `root-config --libs`
	
%.o : %.cpp
	g++ -o $@ -c $^ `root-config --cflags`

clean :
	rm -f *.o decode
