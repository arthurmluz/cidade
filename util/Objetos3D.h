#ifndef objetos_hpp
#define objetos_hpp

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "../app/Ponto.h"
#include <vector>

#include <iostream>
#include <cmath>
#include <climits>
#include <ctime>
#include <fstream>



using namespace std;
// *********************************************************************
//   ESTRUTURAS A SEREM USADAS PARA ARMAZENAR UM OBJETO 3D
// *********************************************************************
typedef struct  // Struct para armazenar um ponto
{
    float X,Y,Z;
    void Set(float x, float y, float z)
    {
        X = x;
        Y = y;
        Z = z;
    }
    void Imprime()
    {
        cout << "X: " << X << " Y: " << Y << " Z: " << Z;
    }
} TPoint;

typedef struct{
    int r;
    int g;
    int b;
    void imprime()
    {
        cout << "R: " << r << " G: " << g << " B: " << b << endl;
    }

} RGB;

typedef struct // Struct para armazenar um triângulo
{
    TPoint P1, P2, P3;
    void imprime()
    {
        cout << "P1 ";  P1.Imprime(); cout << endl;
        cout << "P2 ";  P2.Imprime(); cout << endl;
        cout << "P3 ";  P3.Imprime(); cout << endl;
    }
    RGB cor;
} TTriangle;

// Classe para armazenar um objeto 3D
class Objeto3D
{
    TTriangle *faces; // vetor de faces
    unsigned int nFaces; // Variavel que armazena o numero de faces do objeto
public:
    int maxX, maxY, maxZ;
    int minX, minY, minZ;

    Objeto3D()
    {
        nFaces = 0;
        faces = NULL;
        maxX = maxY = maxZ = INT_MIN;
        minX = minY = minZ = INT_MAX;
    }
    unsigned int getNFaces()
    {
        return nFaces;
    }
    void LeObjeto (string Nome); // implementado fora da classe
    void ExibeObjeto(); // implementado fora da classe
};
#endif
