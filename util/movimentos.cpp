#include "movimentos.h"

void andaFrente(Instancia &jogador){
//    printf("y = %f\n", jogador.dir.y);
    jogador.dir.y += 1 ;
    float alfa = (jogador.rotacao * M_PI)/180.0f;
    //printf("angulo>: %f\n", alfa);
    float xr = (-sin(alfa) * jogador.dir.y);
    float yr = cos(alfa) * jogador.dir.y;
//    float xr = jogador.dir.x * cos(alfa) - jogador.dir.y * sin(alfa);
//    float yr = jogador.dir.x * sin(alfa) - jogador.dir.y * cos(alfa);
    jogador.dir = Ponto(xr, 0,yr);
//    printf("(%f, %f)\n", jogador.dir.x, jogador.dir.y);
}


Ponto andaFrenteCamera(float rotacao){
    float alfa = (rotacao * M_PI)/180.0f;
    float xr = (-sin(alfa) * 1);
    float zr = -cos(alfa) * 1;
    return Ponto(xr, 0, zr);
}

