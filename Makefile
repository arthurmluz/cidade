# Makefile para Linux e macOS

PROG = main
FONTES = app/Ponto.cpp app/Cor.cpp app/Poligono.cpp app/Temporizador.cpp app/ListaDeCoresRGB.cpp app/Instancia.cpp  util/movimentos.cpp app/Modelo.cpp util/ImageClass.cpp util/TextureClass.cpp main.cpp 

OBJETOS = $(FONTES:.cpp=.o)
CPPFLAGS = -g -O3 -DGL_SILENCE_DEPRECATION # -Wall -g  # Todas as warnings, infos de debug

UNAME = `uname`

all: $(TARGET)
	-@make $(UNAME)

Darwin: $(OBJETOS)
#	g++ $(OBJETOS) -O3 -Wno-deprecated -framework OpenGL -framework Cocoa -framework GLUT -lm -o $(PROG)
	g++ $(OBJETOS) -O3 -framework OpenGL -framework Cocoa -framework GLUT -lm -lSOIL -o $(PROG)

Linux: $(OBJETOS)
	g++ $(OBJETOS) -O3 -lGL -lGLU -lglut -lm -lSOIL  -o $(PROG)

clean:
	-@ rm -f $(OBJETOS) $(PROG)
