#include "movimentos.h"

void andaFrente(Instancia &jogador, float rotacao, Modelo &tabuleiro){
    jogador.dir.y += 1 ;
    float alfa = (rotacao * M_PI)/180.0f;
    float xr = (-sin(alfa) * jogador.dir.y);
    float yr = cos(alfa) * jogador.dir.y;
    jogador.dir = Ponto(xr, 0,yr);
    int novoX = jogador.posicao.x+jogador.dir.x;
    int novoZ = jogador.posicao.z+jogador.dir.z;
    if(!(tabuleiro.getLadrilho(novoX,novoZ) == 5))
        jogador.dir = jogador.dir * 1;
    printf("a = %d, %d, %d\n", tabuleiro.getLadrilho(novoX,novoZ), novoX, novoZ);

}


Ponto andaFrenteCamera(float rotacao){
    float alfa = (rotacao * M_PI)/180.0f;
    float xr = (-sin(alfa) * 1);
    float zr = -cos(alfa) * 1;
    return Ponto(xr, 0, zr);
}

