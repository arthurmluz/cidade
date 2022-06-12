//
//  Poligono.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//
#include <iostream>
#include <fstream>
using namespace std;

#include "Modelo.h"
#include "Cor.h"

Modelo::Modelo()
{
    
}

int Modelo::getColunas(){
    return col;
}

int Modelo::getLinhas(){
    return lin;
}

int Modelo::getCores(){
    return cores;
}

void Modelo::obtemLimites(int &linhas, int &colunas){
   linhas = lin;
   colunas = col; 
}

void DesenhaRetangulo()
{
    glBegin(GL_QUADS);
        glVertex3d(-0.5, 0, -0.5);
        glVertex3d(-0.5, 0, +0.5);
        glVertex3d(+0.5, 0, +0.5);
        glVertex3d(+0.5, 0,-0.5);
    glEnd();
}

int Modelo::getLadrilho(int x, int z){
    return matriz[x][z];
}

void Modelo::desenhaVerticesColoridas()
{
    glPushMatrix();
    for(int y = 0; y < lin; y++){
        for(int x = 0; x < col; x++){
            int numCor = matriz[x][y];
            Cor a = ListaCores[numCor];
            if(numCor == 0) {
                glTranslatef(1, 0, 0);
                continue;
            }
            glColor3f(a.r/255.0, a.g/255.0, a.b/255.0);
            DesenhaRetangulo();
            glTranslatef(1, 0, 0);
        }
        glTranslatef(-col, 0, 1);
    }
    glPopMatrix();
}

void Modelo::LeObjeto(const char *nome){
    ifstream input;            // ofstream arq;
    input.open(nome, ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    //int nLinha = 0;

    std::string a;
    getline(input, a); // #CORES

    unsigned int qtdCores;
    input >> qtdCores;

    Cor cores[qtdCores];
    for(int i = 0; i < qtdCores; i++){
        int tmp, r, g, b;

        // le as cores
        input >> tmp >> cores[i].r >> cores[i].g >> cores[i].b >> a;
        //cout << "R: " << cores[i].r << " G: " << cores[i].g << " B: " << cores[i].b << endl;
    }
    for(int i = 0; i < qtdCores; i++){
        ListaCores[i] = cores[i];
    }

    // linha vazia
    input >> a;

    input >> lin >> col;
    //cout << "lin: " << lin << " col: " << col << endl;
    int tmp;
    cout << endl;
    for(int y = lin-1; y >= 0; y--){
        for(int x = 0; x < col; x++){
            input >> tmp;
            matriz[x][y] = tmp;
        }
    }

    cout << "Modelo lido com sucesso!" << endl;

}

void Modelo::LeObjetoNave(const char *nome){
    ifstream input;            // ofstream arq;
    input.open(nome, ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    //int nLinha = 0;

    std::string a;
    getline(input, a); // #CORES

    unsigned int qtdCores;
    input >> qtdCores;

    Cor cores[qtdCores];
    for(int i = 0; i < qtdCores; i++){
        int tmp, r, g, b;

        // le as cores
        input >> tmp >> cores[i].r >> cores[i].g >> cores[i].b >> a;
        //cout << "R: " << cores[i].r << " G: " << cores[i].g << " B: " << cores[i].b << endl;
    }
    for(int i = 0; i < qtdCores; i++){
        ListaCores[i] = cores[i];
    }

    // linha vazia
    input >> a;

    input >> lin >> col;
    //cout << "lin: " << lin << " col: " << col << endl;
    int tmp;
    for(int y = lin-1; y >= 0; y--){
        for(int x = 0; x < col; x++){
            input >> tmp;
            matriz[x][y] = tmp-1;
        }
    }

    cout << "Modelo lido com sucesso!" << endl;

}