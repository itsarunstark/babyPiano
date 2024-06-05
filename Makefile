CC=gcc
LIB=./lib
BIN=./

all:	$(BIN)/piano
	echo "Build finished."

$(BIN)/piano:	$(LIB)/piano.o $(LIB)/window.o $(LIB)/audio.o $(LIB)/tools.o $(LIB)/key.o $(LIB)/font.o
	$(CC) $(FLAGS) $(LIB)/*.o -o $@ -lSDL2 -lSDL2_ttf -lm

$(LIB)/piano.o:	piano.c
	if [ ! -d $(LIB) ];then mkdir $(LIB);fi
	$(CC) $(FLAGS) -c $? -o $@

$(LIB)/window.o:	window.c
	$(CC) $(FLAGS) -c $? -o $@

$(LIB)/audio.o:	audio.c
	$(CC) $(FLAGS) -c $? -o $@

$(LIB)/tools.o:	tools.c
	$(CC) $(FLAGS) -c $? -o $@

$(LIB)/font.o:	font.c
	$(CC) $(FLAGS) -c $? -o $@

$(LIB)/key.o:	key.c
	$(CC) $(FLAGS) -c $? -o $@
