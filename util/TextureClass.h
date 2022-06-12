// *****************************************************************************************
//  TextureClass.h
// *****************************************************************************************

#include "ImageClass.h"
GLuint LoadTexture (const char *nomeTex);

// Estruturas para armazenar as texturas de uma cidade

// Constantes para indexar as texturas 
enum PISOS {
    CROSS,
    DL,
    DLR,
    DR,
    LR,
    None,
    UD,
    UDL,
    UDR,
    UL,
    ULR,
    UR,
    LAST_IMG
};

