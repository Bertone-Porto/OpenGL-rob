#define _CRT_SECURE_NO_WARNINGS
#define PI 3.141592654


#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>
#include "RgbImage.h"


using namespace std;
// Variáveis globais
float aberturaPincaEsq = 20.0f, aberturaPincaDir = 20.0f;
float rotacaoTronco = 0.0f;
float posicaoRobo = 0.0f;
float rotacaoCabeca = 0.0f;
float inclinacaoCabeca = 0.0f;
float rotacaoCabecaVertical = 0.0f;

float robotPositionX = 0.0f;
float robotPositionY = 0.0f;
float robotDirection = 0.0f;



const char* filenameRugged = "./texture_rugged_metal.bmp";
const char* filenameRusted = "./texture_rusted_metal.bmp";
const char* filenameGolden = "./texture_golden_metal.bmp";
const char* filenameGlass = "./texture_glass.bmp";
const char* filenameTexMetal1 = "./metalTexture1.bmp";


float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float robotPositionZ = 0.0f;  


GLuint textureId;
GLUquadric* quad;

GLuint _textureIdMetal1;
GLuint _textureIdRugged; //The id of texture
GLuint _textureIdRusted; //The id of texture
GLuint _textureIdGolden; //The id of texture
GLuint _textureIdGlass; //The id of texture
GLuint _textureIdSphere;
GLuint _textureIdCylinder;



GLUquadric* quadRugged;
GLUquadric* quadRusted;
GLUquadric* quadGolden;
GLUquadric* quadGlass;
GLUquadric* quadSphere;
GLUquadric* quadCylinder;


bool modoPerspectiva = true;
bool textureOn = true;

float scaleFactor = 10.0;

float diameterCylinder = 0.5 * scaleFactor;
float diameterSphere = 0.4  * scaleFactor;
float sizeArm = 4.5  * scaleFactor;
float sizeForearm = 3.0  * scaleFactor;
float sizeHand = 2.0 * scaleFactor;
float sizeClampPart = 1.0 * scaleFactor;
float diameterBase = 2.0 * scaleFactor;
float heightBase = 0.5 * scaleFactor;

float eyeDistance = 20.0;
float eyeX;
float eyeY;
float eyeZ;
float viewAngleX = 0.0;
float viewAngleZ = 15.0;

//
float angleArmEsq = 0.0f;
float angleArmDir = 0.0f;

float angleFlexArmEsq = 0.0f; 
float angleFlexArmDir = 0.0f; 
//

float angleArm = 70.0;
float angleForearm = 70.0;
float angleHand = 0.0;
float angleClampZ = 0.0;
float angleClampY = 0.0;

GLfloat angle, fAspect, rot_x, rot_y;
int fundo = 0;

float rotY = 0.0f;
float rotX = 0.0f;

float posX = 0.0f; // Posição no eixo X
float posZ = 0.0f; // Posição no eixo Z

//float rotacaoCabeca = 0.0f;

float rotacaoPernas = 0.0f;   // Posição no eixo X
//float anguloEmRadios = (rotacaoPernas * PI) / 180.0f;


float lastMouseX = 400.0f; 
float lastMouseY = 300.0f; 
float cameraDistance = 900.0f; 
float cameraAzimuth = 0.0f;   
float cameraElevation = 20.0f; 





float my_cos(float angle) {
    float result = 1.0f;
    float term = 1.0f;
    int n = 1;
    
    while (n < 10) {
        term *= (-angle * angle) / ((2 * n - 1) * (2 * n));
        result += term;
        n++;
    }
    return result;
}

float my_sin(float angle) {
    float result = angle;
    float term = angle;
    int n = 1;

    while (n < 10) {
        term *= (-angle * angle) / ((2 * n) * (2 * n + 1));
        result += term;
        n++;
    }
    return result;
}

// Callback para teclas especiais
void GerenciaSetas(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            rotacaoCabecaVertical += 5.0f; // Inclina a cabeça para cima
            if (rotacaoCabecaVertical > 15.0f) // Limita o movimento
                rotacaoCabecaVertical = 15.0f;
            break;
        case GLUT_KEY_DOWN:
            rotacaoCabecaVertical -= 5.0f; // Inclina a cabeça para baixo
            if (rotacaoCabecaVertical < -15.0f) // Limita o movimento
                rotacaoCabecaVertical = -15.0f;
            break;
        case GLUT_KEY_LEFT:
            rotacaoCabeca -= 5.0f; // Gira cabeça para esquerda
            break;
        case GLUT_KEY_RIGHT:
            rotacaoCabeca += 5.0f; // Gira cabeça para direita
            break;
    }
    glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {  // Zoom-in
            cameraDistance -= 30.0f;  // Reduza a distância para aproximar
            if (cameraDistance < 50.0f) cameraDistance = 50.0f;  // Limite mínimo
        } else if (button == GLUT_RIGHT_BUTTON) {  // Zoom-out
            cameraDistance += 30.0f;  // Aumente a distância para afastar
            if (cameraDistance > 5000.0f) cameraDistance = 5000.0f;  // Limite máximo
        }
        glutPostRedisplay();  // Atualiza a tela para aplicar o zoom
    }
}


GLuint LoadTexture(const char* filename) {

	GLuint textureId;

	RgbImage theTexMap(filename); //Image with texture

	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);	//Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,	//Always GL_TEXTURE_2D
		0,						//0 for now
		GL_RGB,					//Format OpenGL uses for image
		theTexMap.GetNumCols(),	//Width 
		theTexMap.GetNumRows(),	//Height
		0,						//The border of the image
		GL_RGB,					//GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,		//GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		theTexMap.ImageData());	//The actual pixel data
	return textureId; //Returns the id of the texture
}

// Inicializa texturas
void InicializaRendering() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

	quadRugged = gluNewQuadric();
	_textureIdRugged = LoadTexture(filenameRugged);

	quadRusted = gluNewQuadric();
	_textureIdRusted = LoadTexture(filenameRusted);

	quadGolden = gluNewQuadric();
	_textureIdGolden = LoadTexture(filenameGolden);

	quadGlass = gluNewQuadric();
	_textureIdGlass = LoadTexture(filenameGlass);
		
	
	//
	quadSphere = gluNewQuadric();
	quadCylinder = gluNewQuadric();
	_textureIdMetal1 = LoadTexture(filenameTexMetal1);
	_textureIdCylinder = _textureIdMetal1;
	_textureIdSphere = _textureIdMetal1;
}

// Inicializa parâmetros de iluminação
void InicializaLuz(void)
{
	GLfloat luzAmbiente0[4] = { 0.6,0.6,0.6,1.0 };
	GLfloat luzDifusa0[4] = { 0.9,0.9,0.9,1.0 };
	GLfloat posicaoLuz0[4] = { 0.0, 3000.0, 0.0, 0.0 };

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente0);

	// Define os parâmetros das fontes de luz
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa0);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz0);

	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita as fontes de luz
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
	//glFrontFace(GL_CW);
	// Habilita blending para usar alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	angle = 45;
	rot_x = 0;
	rot_y = 0;
}


// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	// Especifica a projeção perspectiva
	gluPerspective(angle, fAspect, 0.1, 5000);
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	// Especifica posição do observador e do alvo
	gluLookAt(0, 800, 800, 0, 0, 0, 0, 1, 0);
}

//garra
void drawCylinder(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght*30.0);
}

//garra
void drawCone(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght*30.0);
}

//garra
void drawDisk(float diameterInner, float diameterOuter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}

//garra
void drawSphere(float diameter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdSphere);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameter, 40.0, 40.0);
}


// Função para desenhar um paralelepípedo (tronco)
void drawParallelepiped(float width, float height, float depth, GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);

    //face frontal
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, depth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, depth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, depth);

    //face traseira
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, -depth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, -depth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -depth);

    //face superior
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, height, -depth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, -depth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, depth);

    //face inferior
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, -depth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, -depth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, -height, depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, -height, depth);

    //face lateral esquerda
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, -depth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height, depth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -depth);

    //face lateral direita
    glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height, -depth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, depth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -depth);

    glEnd();
}

//desenhar esteira (pé)
void drawMovingTrack(float width, float height, float depth, GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPushMatrix();
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    
    //frente
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, depth + 0.2f);
    drawParallelepiped(width, height, 0.2f, texture);
    glPopMatrix();

    //traseira
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -depth - 0.2f);
    drawParallelepiped(width, height, 0.2f, texture);
    glPopMatrix();

    //superior
    glPushMatrix();
    glTranslatef(0.0f, height + 0.2f, 0.0f);
    drawParallelepiped(width, 0.2f, depth + 0.4f, texture);
    glPopMatrix();

    //inferior
    glPushMatrix();
    glTranslatef(0.0f, -height - 0.2f, 0.0f);
    drawParallelepiped(width, 0.2f, depth + 0.4f, texture);
    glPopMatrix();
    
    glPopMatrix();
}





// função para desenhar um cubo (cabeça)
void DesenhaCubo(GLuint _textureId, float lenghtX, float lenghtY, float height)
{
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);			// Face posterior
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, -height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, -height);
	glEnd();
	glBegin(GL_QUADS);			// Face frontal
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, height);
	glEnd();
	glBegin(GL_QUADS);			// Face lateral esquerda
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, -height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
	glEnd();
	glBegin(GL_QUADS);			// Face lateral direita
	glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, -height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, lenghtY, height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
	glEnd();
	glBegin(GL_QUADS);			// Face superior
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, lenghtY, height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, lenghtY, height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
	glEnd();
	glBegin(GL_QUADS);			// Face inferior
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, -lenghtY, -height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, -lenghtY, -height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
	glEnd();
}

// Função que desenha um bastão com uma luz no final
void DesenhaBastao(float diam_start, float diam_end, float lenght, float radius, int color)
{
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, _textureIdGolden);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluQuadricTexture(quadGolden, TRUE);
	gluQuadricTexture(quadGolden, GL_TRUE);

	gluCylinder(quadGolden, diam_start, diam_end, lenght, 72, 72);

	glTranslatef(0.0f, 0.0f, lenght+radius);
	switch (color)
	{
	case 1:
		glColor4f(1.0f, 0.0f, 0.0f, 0.7f);
		break;
	case 2:
		glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
		break;
	case 3:
		glColor4f(0.0f, 0.0f, 1.0f, 0.7f);
		break;
	case 4:
		glColor4f(1.0f, 1.0f, 0.0f, 0.7f);
		break;
	}
	glBindTexture(GL_TEXTURE_2D, _textureIdGlass);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluQuadricTexture(quadGlass, TRUE);
	gluQuadricTexture(quadGlass, GL_TRUE);

	//gluSphere(quadGlass, radius, 72, 72);
	gluSphere(quadGlass, radius, 72, 72);
}

// Função que desenha parte da coroa
void DesenhaParteCoroa(float base, float top, float length, float height)
{
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, _textureIdGolden);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-base, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-top, length, height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(top, length, height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(base, 0, 0);
	glEnd();
}


//braço esquerdo
void desenhaBracoEsq() {
    glPushMatrix();
	glRotatef(rotacaoTronco, 0.0, 1.0, 0.0);
    // Posicionar o braço esquerdo no tronco
    glTranslatef(-25.0,-20.0, 0.0);
    glRotatef(angleFlexArmEsq, 1.0f, 0.0f, 0.0f);
    // Desenhar a base do braço
    /*glColor3f(1.0f, 1.0f, 1.0f); // Cor branca
    drawCylinder(diameterBase, heightBase); // Cilindro base
    glTranslatef(0.0f, 0.0f, heightBase); // Mover para o topo da base
    drawDisk(diameterCylinder, diameterBase); // Disco para conectar base e braço*/

    // Desenhar o braço
    
    glRotatef(angleArm, 0.0f, 0.0f, 1.0f); // Rotacionar o braço em torno de Z
    drawCylinder(diameterCylinder, sizeArm); // Cilindro do braço
    
    //
    glRotatef(angleArmEsq, 0.0f, 0.0f, 1.0f); // Rotacionar o braço esquerdo
	//

    // Desenhar o antebraço
    glTranslatef(0.0f, 0.0f, sizeArm + diameterSphere / 5); // Mover para o final do braço
    glRotatef(angleForearm, 0.0f, 1.0f, 0.0f); // Rotacionar o antebraço
    //

    //
    drawSphere(diameterSphere); // Esfera no cotovelo
    glTranslatef(0.0f, 0.0f, diameterSphere / 5); // Mover para o início do antebraço
    drawCylinder(diameterCylinder, sizeForearm); // Cilindro do antebraço

    // Desenhar a garra
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, sizeForearm + diameterSphere / 5); // Mover para o final do antebraço
    glRotatef(angleClampZ, 0.0f, 0.0f, 1.0f); // Rotação da garra
    drawSphere(diameterSphere); // Esfera na base da garra
    glTranslatef(0.0f, 0.0f, diameterSphere / 2); // Mover para a extremidade da garra


    // Parte superior da garra
    glPushMatrix();
    glRotatef(angleClampY + 60, 0.0f, 1.0f, 0.0f);
    drawCylinder(diameterCylinder / 3, sizeClampPart); // Cilindro superior da garra
    glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
    drawSphere(diameterSphere / 3); // Esfera na ponta
    glTranslatef(0.0f, 0.0f, diameterSphere / 15);
    glRotatef(-60, 0.0f, 1.0f, 0.0f);
    drawCone(diameterCylinder / 3, sizeClampPart); // Cone na extremidade
    glPopMatrix();

    // Parte inferior da garra
    glPushMatrix();
    glRotatef(-angleClampY - 60, 0.0f, 1.0f, 0.0f);
    drawCylinder(diameterCylinder / 3, sizeClampPart); // Cilindro inferior da garra
    glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
    drawSphere(diameterSphere / 3); // Esfera na ponta
    glTranslatef(0.0f, 0.0f, diameterSphere / 15);
    glRotatef(60, 0.0f, 1.0f, 0.0f);
    drawCone(diameterCylinder / 3, sizeClampPart); // Cone na extremidade
    glPopMatrix();

    glPopMatrix();
}

//braço direito
void desenhaBracoDir() {
    glPushMatrix();
	glRotatef(rotacaoTronco, 0.0, 1.0, 0.0);
    // Posicionar o braço direito no tronco
    glTranslatef(25.0, -20.0, 0.0);
    glRotatef(angleFlexArmDir, 1.0f, 0.0f, 0.0f);
    
    // Desenhar a base do braço
    /*glColor3f(1.0f, 1.0f, 1.0f); // Cor branca
    drawCylinder(diameterBase, heightBase); // Cilindro base
    glTranslatef(0.0f, 0.0f, heightBase); // Mover para o topo da base
    drawDisk(diameterCylinder, diameterBase); // Disco para conectar base e braço*/

    // Desenhar o braço
    glColor3f(1.0f, 1.0f, 1.0f); 
    glRotatef(angleArm, 0.0f, 0.0f, 1.0f); // Rotacionar o braço em torno de Z
    gluQuadricTexture(quad, GL_TRUE);
    drawCylinder(diameterCylinder, sizeArm); // Cilindro do braço
    
    //
    glRotatef(angleArmDir, 0.0f, 0.0f, 1.0f); // Rotacionar o braço direito
	//

    // Desenhar o antebraço
    glTranslatef(0.0f, 0.0f, sizeArm + diameterSphere / 5); // Mover para o final do braço
    glRotatef(angleForearm, 0.0f, 1.0f, 0.0f); // Rotacionar o antebraço
    //

    //
    drawSphere(diameterSphere); // Esfera no cotovelo
    glTranslatef(0.0f, 0.0f, diameterSphere / 5); // Mover para o início do antebraço
    drawCylinder(diameterCylinder, sizeForearm); // Cilindro do antebraço

    // Desenhar a garra
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, sizeForearm + diameterSphere / 5); // Mover para o final do antebraço
    glRotatef(angleClampZ, 0.0f, 0.0f, 1.0f); // Rotação da garra
    drawSphere(diameterSphere); // Esfera na base da garra
    glTranslatef(0.0f, 0.0f, diameterSphere / 2); // Mover para a extremidade da garra

	glColor3f(1.0f, 0.0f, 0.0f);

    // Parte superior da garra
    glPushMatrix();
    glRotatef(angleClampY + 60, 0.0f, 1.0f, 0.0f);
    drawCylinder(diameterCylinder / 3, sizeClampPart); // Cilindro superior da garra
    glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
    drawSphere(diameterSphere / 3); // Esfera na ponta
    glTranslatef(0.0f, 0.0f, diameterSphere / 15);
    glRotatef(-60, 0.0f, 1.0f, 0.0f);
    drawCone(diameterCylinder / 3, sizeClampPart); // Cone na extremidade
    glPopMatrix();

    // Parte inferior da garra
    glPushMatrix();
    glRotatef(-angleClampY - 60, 0.0f, 1.0f, 0.0f);
    drawCylinder(diameterCylinder / 3, sizeClampPart); // Cilindro inferior da garra
    glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
    drawSphere(diameterSphere / 3); // Esfera na ponta
    glTranslatef(0.0f, 0.0f, diameterSphere / 15);
    glRotatef(60, 0.0f, 1.0f, 0.0f);
    drawCone(diameterCylinder / 3, sizeClampPart); // Cone na extremidade
    glPopMatrix();


    
    glPopMatrix();
}


void desenhaTronco() {

    glPushMatrix();
	
	glRotatef(rotacaoTronco, 0.0, 1.0, 0.0);
	//tronco
	glPushMatrix();
	glTranslatef(0.0f, -60.0f, 0.0f); 
	glRotatef(rotY, 0.0, 1.0, 0.0); 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdMetal1);
	glColor3f(1.0, 1.0, 1.0); 

	//tronco principal
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(quadGlass, 30.0, 20.0, 60.0, 72, 72);
	glPopMatrix();

	//botão 
	glPushMatrix();
	glTranslatef(0.0f, 25.0f, 26.5f); 
	glColor3f(0.0f, 0.0f, 0.0f); 
	glRotatef(0.0, 1.0, 0.0, 0.0); 
	gluDisk(quadGlass, 0.0, 2.0, 36, 1); 
	glPopMatrix();

	//botão 
	glPushMatrix();
	glTranslatef(0.0f, 30.0f, 25.5f); 
	glColor3f(0.0f, 0.0f, 0.0f); 
	glRotatef(0.0, 1.0, 0.0, 0.0); 
	gluDisk(quadGlass, 0.0, 2.0, 36, 1); 
	glPopMatrix();

	//botão 
	glPushMatrix();
	glTranslatef(0.0f, 35.0f, 25.0f); 
	glColor3f(0.0f, 0.0f, 0.0f); 
	glRotatef(0.0, 1.0, 0.0, 0.0); 
	gluDisk(quadGlass, 0.0, 2.0, 36, 1); 
	glPopMatrix();
	
	//botão 
	glPushMatrix();
	glTranslatef(10.0f, 35.0f, 23.0f); 
	glColor3f(1.0f, 1.0f, 0.0f); 
	glRotatef(0.0, 1.0, 0.0, 0.0); 
	gluDisk(quadGlass, 0.0, 2.0, 36, 1); 
	glPopMatrix();
	
	//botão
	glPushMatrix();
	glTranslatef(-10.0f, 35.0f, 23.0f); 
	glColor3f(1.0f, 1.0f, 0.0f); 
	glRotatef(0.0, 1.0, 0.0, 0.0); 
	gluDisk(quadGlass, 0.0, 2.0, 36, 1); 
	glPopMatrix();
	glPopMatrix(); //fecha o tronco


    
    //cintura
    glPushMatrix();
    glTranslatef(0.0f, -80.0f, 0.0f); 
    glRotatef(rotY, 0.0, 1.0, 0.0); 
    glBindTexture(GL_TEXTURE_2D, _textureIdMetal1);
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(quadGlass, 25.0, 30.0, 20.0, 72, 72);
    glPopMatrix();
    glPopMatrix();
    

    //tronco-pescoço 
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f); 
    glRotatef(rotY, 0.0, 1.0, 0.0); 
    glBindTexture(GL_TEXTURE_2D, _textureIdGlass);
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(quadGlass, 15.0, 15.0, 10.0, 72, 72);
    glPopMatrix();
    
    
    
    //tampa-pescoço
    glPushMatrix();
    glTranslatef(0.0f, 10.0f, 0.0f);  
    glRotatef(90.0, 1.0, 0.0, 0.0);    
    gluDisk(quadGlass, 0.0, 15.0, 72, 1); 
    glPopMatrix();
    
    //tampa-tronco
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);  
    glRotatef(90.0, 1.0, 0.0, 0.0);    
    gluDisk(quadGlass, 0.0, 20.0, 72, 1); 
    glPopMatrix();
    
    
    //tampa-manga esquerda
    glPushMatrix();
    glTranslatef(-30.0f, -20.0f, 0.0f);  
    glRotatef(0.0, 1.0, 0.0, 0.0);    
    glBindTexture(GL_TEXTURE_2D, _textureIdMetal1);
    gluDisk(quadGlass, 0.0, 11.0, 72, 1); 
    glPopMatrix();
    
    //tampa-manga direita
    glPushMatrix();
    glTranslatef(30.0f, -20.0f, 0.0f);  
    glRotatef(0.0, 1.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, _textureIdMetal1);    
    gluDisk(quadGlass, 0.0, 11.0, 72, 1); 
    glPopMatrix();
    
    
    //manga esquerda
    glPushMatrix();
    glTranslatef(-30.0, -20.0, 0.0); 
    glRotatef(0.0, 0.0, 1.0, 0.0); 
    glBindTexture(GL_TEXTURE_2D, _textureIdMetal1);
    gluCylinder(quadGlass, 10.0, 15.0, 30.0, 36, 36); 
    glPopMatrix();

    //manga direita
    glPushMatrix();
    glTranslatef(30.0, -20.0, 0.0); 
    glRotatef(0.0, 0.0, 1.0, 0.0); 
    glBindTexture(GL_TEXTURE_2D, _textureIdMetal1);
    gluCylinder(quadGlass, 10.0, 15.0, 30.0, 36, 36); 
    glPopMatrix();
    
    glPopMatrix(); // Fecha o tronco
    glPopMatrix();
}



//desenhar a cabeça
void desenhaCabeca() {
	glPushMatrix();
	// Aplica rotações (pan)
	//glRotatef(rot_x, 1, 0, 0);
	//glRotatef(rot_y, 0, 1, 0);
	glRotatef(rotacaoCabecaVertical, 1, 0, 0); 
	glRotatef(rotacaoCabeca, 0, 1, 0);

	// Desenha pescoço (cone)
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, _textureIdRugged);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluQuadricTexture(quadRugged, TRUE);
	gluQuadricTexture(quadGolden, GL_TRUE);

	gluCylinder(quadRugged, 10.0f, 7.5f, 21.5f, 72, 72);
	glTranslatef(0.0f, 0.0f, 21.5f);

	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, _textureIdRusted);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluQuadricTexture(quadRusted, TRUE);
	gluQuadricTexture(quadGolden, GL_TRUE);

	gluCylinder(quadRusted, 7.5f, 2.0f, 17.0f, 72, 72);
	glPopMatrix();
	glTranslatef(0.0f, 37.0f, 0.0f);

	// Desenha haste para os olhos (paralelepipedo)
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	DesenhaCubo(_textureIdGolden, 30.0f, 3.0f, 1.5f);

	// Desenha olhos (esferas)
	glColor4f(1.0f, 1.0f, 0.3f, 0.8f);
	glPushMatrix();
	glTranslatef(-25.0, 0, 2.0);
	glBindTexture(GL_TEXTURE_2D, _textureIdGlass);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluQuadricTexture(quadGlass, TRUE);
	gluQuadricTexture(quadGlass, GL_TRUE);

	gluSphere(quadGlass, 3.0f, 72, 72);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(25.0, 0, 2.0);
	glBindTexture(GL_TEXTURE_2D, _textureIdGlass);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluQuadricTexture(quadGlass, TRUE);
	gluQuadricTexture(quadGlass, GL_TRUE);

	gluSphere(quadGlass, 3.0f, 72, 72);
	glPopMatrix();

	// Desenha bastões (cilindros e esferas)
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, -7.0f, 0.0f);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(3.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 25.0f, 1.5, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, -7.0f, 0.0f);
	glRotatef(75.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-3.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 25.0f, 1.5, 4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, -7.0f, 0.0f);
	glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(3.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 25.0f, 1.5, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, -7.0f, 0.0f);
	glRotatef(165.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-3.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 25.0f, 1.5, 3);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, -7.0f, 0.0f);
	glRotatef(210.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(3.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 25.0f, 1.5, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, -7.0f, 0.0f);
	glRotatef(255.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-3.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 25.0f, 1.5, 4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, -7.0f, 0.0f);
	glRotatef(300.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(3.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 25.0f, 1.5, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, -7.0f, 0.0f);
	glRotatef(345.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-3.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 25.0f, 1.5, 3);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(15.0f, 2.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 5.0f, 1.5, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-15.0f, 2.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	DesenhaBastao(1.0f, 0.5f, 5.0f, 1.5, 4);
	glPopMatrix();

	// Desenha coroa
	glPushMatrix();
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, _textureIdRusted);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTranslatef(0.0f, 0.0f, 3.0f);
	gluCylinder(quadRusted, 0.1f, 10.0f, 2.0f, 72, 72);
	glTranslatef(0.0f, 0.0f, 2.0f);
	glBindTexture(GL_TEXTURE_2D, _textureIdGolden);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluQuadricTexture(quadGolden, TRUE);
	gluQuadricTexture(quadGolden, GL_TRUE);

	gluDisk(quadGolden, 0.0f, 10.0f, 72, 72);
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glPopMatrix();

	// Desenha as partes da coroa
	glPushMatrix();
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 5.0f, 9.0f);
	DesenhaParteCoroa(2.0f, 5.0f, 6.0f, 5.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(75.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 5.0f, 9.0f);
	DesenhaParteCoroa(2.0f, 5.0f, 6.0f, 5.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 5.0f, 9.0f);
	DesenhaParteCoroa(2.0f, 5.0f, 6.0f, 5.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(165.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 5.0f, 9.0f);
	DesenhaParteCoroa(2.0f, 5.0f, 6.0f, 5.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(210.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 5.0f, 9.0f);
	DesenhaParteCoroa(2.0f, 5.0f, 6.0f, 5.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(255.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 5.0f, 9.0f);
	DesenhaParteCoroa(2.0f, 5.0f, 6.0f, 5.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(300.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 5.0f, 9.0f);
	DesenhaParteCoroa(2.0f, 5.0f, 6.0f, 5.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(345.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 5.0f, 9.0f);
	DesenhaParteCoroa(2.0f, 5.0f, 6.0f, 5.0f);
	glPopMatrix();

	// Desenha cabeça (elipsoide transparente)
	glEnable(GL_CULL_FACE);
	glColor4f(0.4f, 0.4f, 0.4f, 0.5f);
	glPushMatrix();
	glScalef(1.0f, 0.4f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, _textureIdGlass);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluQuadricTexture(quadGlass, TRUE);
	gluQuadricTexture(quadGlass, GL_TRUE);

	gluSphere(quadGlass, 40.0f, 72, 72);
	glDisable(GL_CULL_FACE);
	glPopMatrix();
	glPopMatrix();
}

//pernas 
void desenhaPernas() {
    glPushMatrix();
    glRotatef(rotacaoPernas, 0.0, 1.0, 0.0); 
    	
    //cintura
    glPushMatrix();
    glTranslatef(0.0f, -100.0f, 0.0f); 
    glBindTexture(GL_TEXTURE_2D, _textureIdMetal1);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(quadGlass, 35.0, 30.0, 30.0, 72, 72);
    glPopMatrix();
    
    //tampa cintura-perna
    glPushMatrix();
    glTranslatef(0.0f, 25.0f, 0.0f);  
    glRotatef(90.0, 1.0, 0.0, 0.0);    
    gluDisk(quadGlass, 0.0, 30.0, 72, 1); 
    glPopMatrix();
    
    //tampa perna 1
    glPushMatrix();
    glTranslatef(-22.0f, 0.0f, 0.0f);  
    glRotatef(90.0, 1.0, 0.0, 0.0);    
    gluDisk(quadGlass, 0.0, 15.0, 72, 1); 
    glPopMatrix();
    
    
    //tampa perna 2
    glPushMatrix();
    glTranslatef(22.0f, 0.0f, 0.0f);  
    glRotatef(90.0, 1.0, 0.0, 0.0);    
    gluDisk(quadGlass, 0.0, 15.0, 72, 1); 
    glPopMatrix();
    
    
    
    //botão do lado direito
    glPushMatrix();
    glTranslatef(19.0f, 20.0f, 25.0f); 
    glRotatef(0.0, 0.0, 1.0, 0.0);   
    glColor3f(0.0, 0.0, 0.0);        
    gluDisk(quadGlass, 0.0, 5.0, 36, 1); 
    glPopMatrix();

    //botão do lado esquerdo
    glPushMatrix();
    glTranslatef(-19.0f, 20.0f, 25.0f); 
    glRotatef(0.0, 0.0, 1.0, 0.0);  
    glColor3f(0.0, 0.0, 0.0);        
    gluDisk(quadGlass, 0.0, 5.0, 36, 1); 
    glPopMatrix();
    
    glPopMatrix();

    //perna e pé 1
    glPushMatrix();
    glTranslatef(22.0f, -140.0f, 0.0f); 
    glBindTexture(GL_TEXTURE_2D, _textureIdRugged);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(quadGlass, 15.0, 15.0, 40.0, 30, 30); 
    glPopMatrix();
    glPopMatrix();

    //pé 1
    glPushMatrix();
    glTranslatef(22.0, -160.0f, 0.0f); 
    drawParallelepiped(20.0f, 20.0f, 20.0f, _textureIdMetal1); 
    drawMovingTrack(21.0f, 11.65f, 22.0f, _textureIdRugged); 
    glPopMatrix();

    //perna e pé 2
    glPushMatrix();
    glTranslatef(-22.0f, -140.0f, 0.0f); 
    glBindTexture(GL_TEXTURE_2D, _textureIdRugged);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(quadGlass, 15.0, 15.0, 40.0, 30, 30); 
    glPopMatrix();
    glPopMatrix();

    //pé 2
    glPushMatrix();
    glTranslatef(-22.0, -160.0f, 0.0f); 
    drawParallelepiped(20.0f, 20.0f, 20.0f, _textureIdMetal1); 
    drawMovingTrack(21.0f, 11.65f, 22.0f, _textureIdRugged); 
    glPopMatrix();

    glPopMatrix(); //fecha geral das pernas
}

//desenhar o chão
void desenhaPiso(float tamanho, int numQuadrados) {
    float metade = tamanho / 2.0f;         
    float tamQuadrado = tamanho / numQuadrados; 

    glPushMatrix();
    glTranslatef(0.0f, -181.0f, 0.0f); 
    glBegin(GL_QUADS);

    for (int i = 0; i < numQuadrados; ++i) {
        for (int j = 0; j < numQuadrados; ++j) {
            if ((i + j) % 2 == 0)
                glColor3f(0.55f, 0.27f, 0.07f); 
            else
                glColor3f(0.55f, 0.37f, 0.07f); 

            float x0 = -metade + i * tamQuadrado;
            float z0 = -metade + j * tamQuadrado;
            float x1 = x0 + tamQuadrado;
            float z1 = z0 + tamQuadrado;

            glVertex3f(x0, 0.0f, z0);
            glVertex3f(x1, 0.0f, z0);
            glVertex3f(x1, 0.0f, z1);
            glVertex3f(x0, 0.0f, z1);
        }
    }

    glEnd();
    glPopMatrix();
}

//verificar os limites do movimento
bool dentroDoPiso(float x, float z, float tamanhoPiso) {
    float limite = tamanhoPiso / 2.0f;  
    return (x >= -limite && x <= limite && z >= -limite && z <= limite);
}


void GerenciaMouseMotion(int x, int y) {
    float deltaX = x - lastMouseX; 
    float deltaY = y - lastMouseY; 


    cameraAzimuth += deltaX * 0.05f;   
   // cameraElevation += deltaY * 0.05f;


    if (cameraAzimuth > 360.0f) cameraAzimuth -= 360.0f;
    if (cameraAzimuth < 0.0f) cameraAzimuth += 360.0f;

   // if (cameraElevation > 89.0f) cameraElevation = 89.0f;
   // if (cameraElevation < -89.0f) cameraElevation = -89.0f;


    lastMouseX = x;
   // lastMouseY = y;

    glutPostRedisplay(); 
}




void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    //converte azimute e elevação para radianos
    float azimuthRad = cameraAzimuth * PI / 180.0f;
    float elevationRad = cameraElevation * PI / 180.0f;

    //calcula posição da câmera em coordenadas esféricas
    float camX = cameraDistance * my_cos(elevationRad) * my_sin(azimuthRad);
    float camY = cameraDistance * my_sin(elevationRad);
    float camZ = cameraDistance * my_cos(elevationRad) * my_cos(azimuthRad);

    //configura a visão
    gluLookAt(camX, camY, camZ, 
              0.0, 0.0, 0.0, 
              0.0, 1.0, 0.0); 

    desenhaPiso(5000.0f, 40); 

    glPushMatrix();
    glTranslatef(robotPositionX, 0.0, robotPositionZ);

    desenhaTronco();
    desenhaCabeca();
    desenhaPernas();
    desenhaBracoEsq();
    desenhaBracoDir();

    glPopMatrix();

    glutSwapBuffers();
}





// Função para ajustar a janela
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45.0, (float)w / h, 1.0, 5000.0);
    if (modoPerspectiva) {
        gluPerspective(45.0, (float)w/h, 1.0, 5000.0);
    } else {
        glOrtho(-200, 200, -200 * h / w, 200 * h / w, 1.0, 5000.0);
    }
    glMatrixMode(GL_MODELVIEW);
}



//controle do teclado
void teclado(unsigned char key, int x, int y) {    

	float anguloEmRadios = (rotacaoPernas * PI) / 180.0f; 
	
    switch (key) {
		case 'w': 
        {
            //calcular a nova posição desejada
            float novaPosX = robotPositionX + 2.9f * my_sin(anguloEmRadios);
            float novaPosZ = robotPositionZ + 2.9f * my_cos(anguloEmRadios);

            //verificar se a nova posição está dentro do piso
            if (dentroDoPiso(novaPosX, novaPosZ, 2000.0f)) {
                robotPositionX = novaPosX;
                robotPositionZ = novaPosZ;
            }
        }
        break;

   		case 's': //move para trás na direção oposta ao que o robô está apontando
        {
            //calcular a nova posição desejada
            float novaPosX = robotPositionX - 2.9f * my_sin(anguloEmRadios);
            float novaPosZ = robotPositionZ - 2.9f * my_cos(anguloEmRadios);

            //verificar se a nova posição está dentro do piso
            if (dentroDoPiso(novaPosX, novaPosZ, 2000.0f)) {
                robotPositionX = novaPosX;
                robotPositionZ = novaPosZ;
            }
        }
        break;
        case 'a': rotacaoTronco -= 1.0f; break; //gira o tronco para esquerda
        case 'd': rotacaoTronco += 1.0f; break; //gira o tronco para direita
        case 'q': rotacaoPernas -= 1.0f; break; //gira o tronco para direita
        case 'e': rotacaoPernas += 1.0f; break; //gira o tronco para direita

        
        case 'x': //Increase view angle z axis
			if (viewAngleZ < 180) viewAngleZ += 3;
			glutPostRedisplay();
			break;
		case 'z': //Decrease view angle z axis
			if (viewAngleZ > 0) viewAngleZ -= 3;
			glutPostRedisplay();
			break;
		case 'c': //Decrease view angle x axis
			if (viewAngleX > 0) viewAngleX -= 3;
			glutPostRedisplay();
			break;
		case 'v': //Increase view angle x axis
			if (viewAngleX < 180) viewAngleX += 3;
			glutPostRedisplay();
			break;
		case '5': //Increase clamp angle y axis
			if (angleClampY < 60) angleClampY += 3;
			glutPostRedisplay();
			break;
		case '6': //Decrease clamp angle y axis
			if (angleClampY > 0) angleClampY -= 3;
			glutPostRedisplay();
			break;
		    case 27: exit(0); // Tecla ESC para sair*/
		
		//
		case '1': // Aumenta o ângulo do braço esquerdo
			angleArmEsq += 3;
			if (angleArmEsq >= 360) angleArmEsq = 0;
			glutPostRedisplay();
			break;
		case '2': // Diminui o ângulo do braço esquerdo
			angleArmEsq -= 3;
			if (angleArmEsq <= 0) angleArmEsq = 360;
			glutPostRedisplay();
			break;
		case '3': // Aumenta o ângulo do braço direito
			angleArmDir += 3;
			if (angleArmDir >= 360) angleArmDir = 0;
			glutPostRedisplay();
			break;
		case '4': // Diminui o ângulo do braço direito
			angleArmDir -= 3;
			if (angleArmDir <= 0) angleArmDir = 360;
			glutPostRedisplay();
			break;
		case '7': // Flexionar braço esquerdo para frente
            if (angleFlexArmEsq < 20) angleFlexArmEsq += 3;
            glutPostRedisplay();
            break;
        case '8': // Flexionar braço esquerdo para trás
            if (angleFlexArmEsq > -20) angleFlexArmEsq -= 3;
            glutPostRedisplay();
            break;
        case '9': // Flexionar braço direito para frente
            if (angleFlexArmDir < 20) angleFlexArmDir += 3;
            glutPostRedisplay();
            break;
        case '0': // Flexionar braço direito para trás
            if (angleFlexArmDir > -18) angleFlexArmDir -= 3;
            glutPostRedisplay();
            break;
		case 'p': // Alternar entre perspectiva e ortogonal
            modoPerspectiva = !modoPerspectiva;
            reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            break;

    }
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Robo - Perdidos no Espaco (1965)");

    glEnable(GL_DEPTH_TEST);

    // Registra as funções de callback
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutMouseFunc(GerenciaMouse);
    glutSpecialFunc(GerenciaSetas);
    glutMotionFunc(GerenciaMouseMotion);       
    glutPassiveMotionFunc(GerenciaMouseMotion); 

    // Inicializa iluminação e renderização
    InicializaLuz();
    InicializaRendering();

    //inicializa as variáveis para o controle de câmera
    lastMouseX = 400.0f; 
    lastMouseY = 100.0f; 

    glutMainLoop(); 
    return 0;
}


