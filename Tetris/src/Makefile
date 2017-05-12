Tetris: main.o Graphics.o AI.o Board.o Engine.o HUD.o MainState.o MenuState.o Tetromino.o
	g++ $^ -o $@ -framework SDL2 -framework SDL2_ttf

%.o : %.cpp
	g++ $< -o $@ -std=c++11 -c

clean:
	$(RM) Tetris *.o
