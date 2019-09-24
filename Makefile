CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0`

OBJ=main.c

app: $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $(OBJ) -g -o app

%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBS) -g $< -o $@

clear: $(OBJ)
	rm $(OBJ)
