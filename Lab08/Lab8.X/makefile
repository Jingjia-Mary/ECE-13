all: rpg

rpg: Player.c Game.c Lab08_main.c
	gcc -Wall Player.c Game.c Lab08_main.c -o rpg

Player.o: Player.c Player.h UNIXBOARD.h
    	gcc -Wall -c Player.c

Game.o: Game.c Game.h Player.h UNIXBOARD.h
    	gcc -Wall -c Game.c

rpg.o: Lab08_main.c UNIXBOARD.h
    	gcc -Wall -c Lab08_main.c -o rpg.o

clean:
    	rm *.o rpg
    	rm -f rpg