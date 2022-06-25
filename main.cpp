// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Programa b�sico para criar aplicacoes 3D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <tuple>

using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "app/Temporizador.h"
#include "app/ListaDeCoresRGB.h"
#include "app/Ponto.h"
#include "app/Modelo.h"

#include "util/movimentos.h"
#include "util/TextureClass.h"
#include "util/Objetos3D.h"

#define WIDTH 900
#define HEIGHT 900
#define GAS_SPAWN 30
#define TERRAIN_VIEW 200
#define BUILDING_RATE 5
#define BUILDING_HEIGHT 10

#define NOME_MAPA "tabuleiro_1280"
int SPEED {1};
int MAX_SPEED {5};

int TABULEIRO_X {20};
int TABULEIRO_Z {20};

Temporizador T;
double AccumDeltaT=0;

GLfloat AspectRatio, angulo=0;
float rotacao = 0;

// Controle do modo de projecao
// 0: Projecao Paralela Ortografica; 1: Projecao Perspectiva
// A funcao "PosicUser" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'p'
int ModoDeProjecao = 1;

// Controle do modo de projecao
// 0: Wireframe; 1: Faces preenchidas
// A funcao "Init" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'e'
int ModoDeExibicao = 1;

// posicao inicial do observador
int obsX = 0, obsY = 15, obsZ = 30;

Modelo tabuleiro, disparador; 
Instancia jogador;

int camera = 0, qtdGasolina = 100;
vector<Modelo> gasolina;

double nFrames=0;
double TempoTotal=0;
Ponto CantoEsquerdo = {-static_cast<float>(TABULEIRO_X),-1,-static_cast<float>(TABULEIRO_Z)};

Objeto3D *Objetos = new Objeto3D[1];
//TEXTURAS
// Lista de nomes das texturas
string nomeTexturas[] = {
    "CROSS.png",
    "DL.png",
    "DLR.png",
    "DR.png",
    "LR.png",
    "None.png",
    "UD.png",
    "UDL.png",
    "UDR.png",
    "UL.png",
    "ULR.png",
    "UR.png",
    };

int idTexturaRua[LAST_IMG];  // vetor com os identificadores das texturas


// **********************************************************************
// void CarregaTexturas()
// **********************************************************************
void initTexture()
{
    for(int i=0;i<LAST_IMG;i++){
        string filename = "assets/"+nomeTexturas[i];
        idTexturaRua[i] = LoadTexture(filename.c_str());
    }
}
// **********************************************************************
//  void init(void)
//        Inicializa os parametros globais de OpenGL
// **********************************************************************
void desenhaDisparador(int num);
void criaInstancias(){
    int lin, col;
    disparador.obtemLimites(lin, col);
    jogador.posicao = Ponto(TABULEIRO_X/2, -0.7, TABULEIRO_Z/2);
    jogador.rotacao = 0;
    jogador.modelo = desenhaDisparador;
    jogador.escala = Ponto(0.02, 0, 0.02);
    if( lin > col ) 
        jogador.raio = lin/2 * jogador.escala.x;
        //jogador.raio = (max.x/5) * jogador.escala.x;//TAM_MAPA/100; 
    else
        jogador.raio = col/2 * jogador.escala.x;
        //jogador.raio = (max.y/5) * jogador.escala.x;//TAM_MAPA/100;  
}
void init(void)
{
    glClearColor(0.0f, 0.0f, 0.5, 1.0f); // Fundo de tela preto

    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE );
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    initTexture();

    Objetos[0].LeObjeto(static_cast<string>("assets/teste.txt"));

    string nome = static_cast<string>("assets/")+NOME_MAPA+static_cast<string>(".txt");
    tabuleiro.LeMapa(nome.c_str());

    TABULEIRO_Z = tabuleiro.getLinhas();
    TABULEIRO_X = tabuleiro.getColunas();

    int maior = TABULEIRO_X > TABULEIRO_Z ? TABULEIRO_X : TABULEIRO_Z;
    
    disparador.LeObjetoNave("assets/nave.txt");
    criaInstancias();
    obsX=0;obsY=6;obsZ=-4;

    //CantoEsquerdo = {-static_cast<float>(TABULEIRO_X),-1, -static_cast<float>(TABULEIRO_Z)};

    CantoEsquerdo = {0,-1, 0};

    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    if (ModoDeExibicao) // Faces Preenchidas??
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+= 1;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        TempoTotal = 0;
        nFrames = 0;
    }
}


// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma c�lula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e est� sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int corBorda, int corDentro)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, idTexturaRua[corDentro]);

    if(corDentro == None || corDentro > LAST_IMG){
        defineCor(MediumForestGreen);
       glBindTexture (GL_TEXTURE_2D, idTexturaRua[None]);
    }
    else defineCor(Gray);// desenha QUAD preenchido
    glPushMatrix();
        glBegin ( GL_QUADS );
            glNormal3f(0,1,0);

            glTexCoord2f(0.0, 0.0f);
            glVertex3f(-0.5f,  0.0f, -0.5f);

            if(NOME_MAPA == "Mapa1")
                glTexCoord2f(0.0f, 1.0f);
            else
                glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-0.5f,  0.0f,  0.5f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f( 0.5f,  0.0f,  0.5f);

            if(NOME_MAPA == "Mapa1")
                glTexCoord2f(1.0f, 0.0f);
            else
                glTexCoord2f(0.0f, 1.0f);
            glVertex3f( 0.5f,  0.0f, -0.5f);
        glEnd();
    glPopMatrix();

//    defineCor(corBorda);
//    glBegin ( GL_LINE_STRIP );
//        glNormal3f(0,1,0);
//        glVertex3f(-0.5f,  0.0f, -0.5f);
//        glVertex3f(-0.5f,  0.0f,  0.5f);
//        glVertex3f( 0.5f,  0.0f,  0.5f);
//        glVertex3f( 0.5f,  0.0f, -0.5f);
//    glEnd();
}

// **********************************************************************
void desenhaDisparador(int num){
    int lin, col;
    disparador.obtemLimites(lin, col);
    glPushMatrix();
        glTranslatef(-(col/2), 0, -(lin/2));
        disparador.desenhaVerticesColoridas();
    glPopMatrix();
}

void criaGasolina(){

}
// **********************************************************************
void desenhaGasolina(){
    if( gasolina.size() < GAS_SPAWN ){
        criaGasolina();
    }
}
// **********************************************************************
//
//
// **********************************************************************
void DesenhaPredio(int altura, int cor){
    defineCor(cor);

    glBegin ( GL_QUADS );
        // Front Face
        glNormal3f(0,0,1);
        glVertex3f(-0.3, 0,  0.3);
        glVertex3f( 0.3, 0,  0.3);
        glVertex3f( 0.3,  altura,  0.3);
        glVertex3f(-0.3,  altura,  0.3);
        // Back Face
        glNormal3f(0,0,-1);
        glVertex3f(-0.3, 0, -0.3);
        glVertex3f(-0.3,  altura, -0.3);
        glVertex3f( 0.3,  altura, -0.3);
        glVertex3f( 0.3, 0, -0.3);
        // Top Face
        glNormal3f(0,1,0);
        glVertex3f(-0.3,  altura, -0.3);
        glVertex3f(-0.3,  altura,  0.3);
        glVertex3f( 0.3,  altura,  0.3);
        glVertex3f( 0.3,  altura, -0.3);
        // Bottom Face
        glNormal3f(0,-1,0);
        glVertex3f(-0.3, 0, -0.3);
        glVertex3f( 0.3, 0, -0.3);
        glVertex3f( 0.3, 0,  0.3);
        glVertex3f(-0.3, 0,  0.3);
        // Right face
        glNormal3f(1,0,0);
        glVertex3f( 0.3, 0, -0.3);
        glVertex3f( 0.3,  altura, -0.3);
        glVertex3f( 0.3,  altura,  0.3);
        glVertex3f( 0.3, 0,  0.3);
        // Left Face
        glNormal3f(-1,0,0);
        glVertex3f(-0.3, 0, -0.3);
        glVertex3f(-0.3, 0,  0.3);
        glVertex3f(-0.3,  altura,  0.3);
        glVertex3f(-0.3,  altura, -0.3);
    glEnd();
}

void DesenhaPiso()
{
    srand(101);
    glPushMatrix();
    glTranslated(CantoEsquerdo.x/2, CantoEsquerdo.y, CantoEsquerdo.z/2);

    for(int x=0; x<TABULEIRO_X;x++) {
        glPushMatrix();
        for(int z=0; z<TABULEIRO_Z;z++) {
            float chancePredio = rand()%BUILDING_RATE;
            float altura = rand()%BUILDING_HEIGHT+1;
            float cor = rand()%LAST_COLOR;
            if(x < jogador.posicao.x-TERRAIN_VIEW || x > jogador.posicao.x+TERRAIN_VIEW) {
                continue;
            }
            if(z < jogador.posicao.z-TERRAIN_VIEW || z > jogador.posicao.z+TERRAIN_VIEW) {
                continue;
            }

            bool predio = false;
            int ladrilho = tabuleiro.getLadrilho(x, z);
            DesenhaLadrilho(White, ladrilho);
            if( ladrilho == None && chancePredio == 0 ) DesenhaPredio(altura, cor);
            glTranslated(0, 0, 1);
        }
        glPopMatrix();
        glTranslated(1, 0, 0);
    }
    glPopMatrix();
}

void andaJogador(){
    double modulo = jogador.dir.modulo();
    if(modulo == 0) return;
    jogador.dir = (jogador.dir / modulo) * SPEED;

    Ponto verificaParedes = jogador.posicao + jogador.dir;
    verificaParedes.x = round(verificaParedes.x);
    verificaParedes.z = round(verificaParedes.z);
    if( !(verificaParedes.x < 0 || verificaParedes.x > TABULEIRO_X-1 )){
        if( !(verificaParedes.z < 0 || verificaParedes.z > TABULEIRO_Z-1 )){
            Ponto testeProximo = jogador.posicao + (jogador.dir / (modulo * SPEED));
            testeProximo.x = round(testeProximo.x); testeProximo.z = round(testeProximo.z);
            int piso = tabuleiro.getLadrilho(static_cast<int>(testeProximo.x), static_cast<int>(testeProximo.z));
            if(piso == None)
                goto here;
            jogador.posicao = verificaParedes;
        }
        else goto here;
    }
    else{
        here:
        double modulo = jogador.dir.modulo();
        if(modulo > 1){
            jogador.dir.versor();
            SPEED = 1;
        }
        else jogador.dir = Ponto(0,0,0);
    }

}
void animaJogador(){
    andaJogador();
    if(jogador.dir.x == 0 && jogador.dir.z == 0)
        jogador.rotacao = rotacao;

    jogador.desenha(0);
}

// **********************************************************************
//  void DefineLuz(void)
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4 } ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {TABULEIRO_X/2.0f, 3.0f, TABULEIRO_Z/2.0f };  // Posi��o da Luz
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

 glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de ilumina��o
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz n�mero Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentra��oo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

}
// **********************************************************************

// **********************************************************************
//  void PosicUser()
// **********************************************************************
void PosicUser()
{

    // Define os par�metros da proje��o Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define o volume de visualiza��o sempre a partir da posicao do
    // observador
    if (ModoDeProjecao == 0)
        glOrtho(-10, 10, -10, 10, 0, 7); // Projecao paralela Orthografica
    else gluPerspective(90,AspectRatio,0.01,100); // Projecao perspectiva

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float cameraX, cameraZ;
    float alvoX = jogador.posicao.x;
    float alvoZ = jogador.posicao.z;
    switch(static_cast<int>(rotacao)){
        case 0: //reto
            cameraX = jogador.posicao.x;
            cameraZ = jogador.posicao.z-2;
            alvoZ +=3;   
            break;
        case 270: //esquerda
            alvoX +=3; 
            cameraZ =jogador.posicao.z;
            cameraX =jogador.posicao.x-2;
            break;
        case 90: //direita
            alvoX -=3;  
            cameraZ =jogador.posicao.z;
            cameraX =jogador.posicao.x+2;
            break;
        case 180: //traseira
            alvoZ -=3;
            cameraZ =jogador.posicao.z+2;
            cameraX =jogador.posicao.x;
            break;
    }

    // padrão
    if (camera == 0){
        gluLookAt(cameraX, jogador.posicao.y+3, cameraZ,   // Posi��o do Observador
              alvoX,jogador.posicao.y,alvoZ,     // Posi��o do Alvo
              0.0f,1.0f,0.0f);
    }else{
        if(camera == 1){
            gluLookAt(obsX, obsY, obsZ,   // Posi��o do Observador
                  jogador.posicao.x, jogador.posicao.y, jogador.posicao.z,     // Posi��o do Alvo
                  0.0f,1.0f,0.0f);
        }
        else{
            gluLookAt(jogador.posicao.x, jogador.posicao.y, jogador.posicao.z,   // Posi��o do Observador
                  alvoX, jogador.posicao.y, alvoZ,     // Posi��o do Alvo
                  0.0f,1.0f,0.0f);
        }
    }

}
// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

	// Evita divis�o por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a rela��o entre largura e altura para evitar distor��o na imagem.
    // Veja fun��o "PosicUser".
	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Seta a viewport para ocupar toda a janela
    glViewport(0, 0, w, h);
    //cout << "Largura" << w << endl;

	PosicUser();

}

// **********************************************************************
//  void display( void )
// **********************************************************************
float PosicaoZ = -30;
void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glLoadIdentity();
	DefineLuz();

	PosicUser();

	glMatrixMode(GL_MODELVIEW);

   // DesenhaPiso();
    glPushMatrix();
        glTranslatef(TABULEIRO_X/2, 0, TABULEIRO_Z/2);
        Objetos[0].ExibeObjeto();
    glPopMatrix();
    animaJogador();
    desenhaGasolina();

	glutSwapBuffers();
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
    Ponto novo;
	switch ( key )	{
        case 27:        // Termina o programa qdo
            exit ( 0 );   // a tecla ESC for pressionada
            break;
        case 'p':
            ModoDeProjecao = !ModoDeProjecao;
            glutPostRedisplay();
            break;
        case 'e':
            ModoDeExibicao = !ModoDeExibicao;
            init();
            glutPostRedisplay();
            break;
        case 'v':
            camera= camera >= 2 ? 0 : camera+1;
            obsX = jogador.posicao.x;
            obsY = jogador.posicao.y+10;
            obsZ = jogador.posicao.z-2;

            break;

        case '7':
            obsX+=1;
            break;
        case '4':
            obsX-=1;
            break;
        case '8':
            obsY+=1;
            break;
        case '5':
            obsY-=1;
            break;
        case '9':
            obsZ+=1;
            break;
        case '6':
            obsZ-=1;
            break;
        case ' ':
            if(jogador.dir.x == 0 && jogador.dir.z == 0)
                andaFrente(jogador, rotacao, tabuleiro);
            else {
                jogador.dir.x = 0;
                jogador.dir.z = 0;
                SPEED = 1;
            }
            break;
        default:
            break;
    }

    printf("%d %d %d\n", obsX, obsY, obsZ);


}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys ){
        case GLUT_KEY_UP:     
            SPEED++;
            if(SPEED >= MAX_SPEED) SPEED = MAX_SPEED;
            printf("speed = %d\n", SPEED);
            break;
        case GLUT_KEY_LEFT:
            rotacao -= 90;
            if(rotacao <= 0){
               rotacao = 270; 
            }
            printf("%f\n", jogador.rotacao);
            break;
        case GLUT_KEY_RIGHT:
            rotacao += 90;
            if(rotacao >= 360){
               rotacao = 0; 
            }
            printf("%f\n", jogador.rotacao);
            break;
        case GLUT_KEY_DOWN:
            SPEED--;
            if(SPEED <= 0) SPEED = 1;
            printf("speed = %d\n", SPEED);
            break;
        default:
            break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
	glutInit            ( &argc, argv );
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( WIDTH, HEIGHT );
	glutCreateWindow    ( "Computacao Grafica - Cidade 3D" );

	init ();

	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );
	return 0;
}



