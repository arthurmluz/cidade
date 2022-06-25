#include "Objetos3D.h"

// *********************************************************************
//   DESCRICAO DO FORMATO ".TRI"
// *********************************************************************

/*
 A primeira linha do arquivo contém o número de triângulos que forma o objeto
 As demais linhas descrevemas os triângulos:
 x1 y1 z1 x2 y2 z2 x3 y3 z3
 onde
 x1 y1 z1 : primeiro vértice do triângulo
 x2 y2 z2 : segundo vértice do triângulo
 x3 y3 z3 : terceiro vértice do triângulo
 Um exemplo com dois triângulos:
 2
 0 0 10 10 0 10 5 10 10
 10 0 10 20 0 10 15 10 10
 
 */
// **********************************************************************
// void LeObjeto(char *Nome)
// **********************************************************************
void Objeto3D::LeObjeto (string Nome)
{
    ifstream arq;
    arq.open(Nome, ios::in);
    if (!arq)
    {
        cout << "Erro na abertura do arquivo " << Nome.c_str() << "." << endl;
        exit(1);
    }
    arq >> nFaces;
    faces = new TTriangle[nFaces];
    float x,y,z;
    char str[9];
    for (int i=0;i<nFaces;i++)
    {
        // Le os trs vŽrtices
        arq >> x >> y >> z; // Vertice 1
        faces[i].P1.Set(x,y,z);
        if(x > maxX) maxX = x;
        if(x < minX) minX = x;

        if(y > maxY) maxY = y;
        if(y < minY) minY = y;

        if(z > maxZ) maxZ = z;
        if(z < minZ) minZ = z;

        arq >> x >> y >> z; // Vertice 2
        faces[i].P2.Set(x,y,z);
        if(x > maxX) maxX = x;
        if(x < minX) minX = x;

        if(y > maxY) maxY = y;
        if(y < minY) minY = y;

        if(z > maxZ) maxZ = z;
        if(z < minZ) minZ = z;

        arq >> x >> y >> z >> str; // Vertice 3
        faces[i].P3.Set(x,y,z);
        sscanf(str, "0x%02x%02x%02x", &faces[i].cor.r, &faces[i].cor.g, &faces[i].cor.b);

        if(x > maxX) maxX = x;
        if(x < minX) minX = x;

        if(y > maxY) maxY = y;
        if(y < minY) minY = y;

        if(z > maxZ) maxZ = z;
        if(z < minZ) minZ = z;

    }
    cout << "Arquivo " << Nome.c_str() << " Lido com Sucesso." << endl;
}

// **********************************************************************
// void ExibeObjeto (TTriangle **Objeto)
// **********************************************************************

// Rotina que faz um produto vetorial
void ProdVetorial (TPoint v1, TPoint v2, TPoint &vresult)
    {
        vresult.X = v1.Y * v2.Z - (v1.Z * v2.Y);
        vresult.Y = v1.Z * v2.X - (v1.X * v2.Z);
        vresult.Z = v1.X * v2.Y - (v1.Y * v2.X);
    }

// Esta rotina tem como funcao calcular um vetor unitario
void VetUnitario(TPoint &vet)
    {
        float modulo;

        modulo = sqrt (vet.X * vet.X + vet.Y * vet.Y + vet.Z * vet.Z);

        if (modulo == 0.0) return;

        vet.X /= modulo;
        vet.Y /= modulo;
        vet.Z /= modulo;
    }
void CalculaNormal(TPoint &vet, TPoint p1, TPoint p2, TPoint p3){
    TPoint vetor1, vetor2;
    vetor1.Set(p2.X-p1.X, p2.Y-p1.Y, p2.Z-p1.Z);
    vetor2.Set(p3.X-p1.X, p3.Y-p1.Y, p3.Z-p1.Z);
    VetUnitario(vetor1);
    VetUnitario(vetor2);
    ProdVetorial(vetor1, vetor2, vet);

}

void Objeto3D::ExibeObjeto ()
{
    for(int i =0;i < nFaces; i++){
        TPoint normal;
        CalculaNormal(normal, faces[i].P1, faces[i].P2, faces[i].P3);

        glBegin(GL_TRIANGLES);
            glColor3f(faces[i].cor.r/255.0, faces[i].cor.g/255.0, faces[i].cor.b/255.0);
            glNormal3f(normal.X, normal.Y, normal.Z);
            glVertex3f(faces[i].P1.X, faces[i].P1.Y, faces[i].P1.Z);
            glVertex3f(faces[i].P2.X, faces[i].P2.Y, faces[i].P2.Z);
            glVertex3f(faces[i].P3.X, faces[i].P3.Y, faces[i].P3.Z);
        glEnd();
    }
    
}
