// Laberinto.cpp: define el punto de entrada de la aplicación de consola.
//
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <iostream>
#include "ImageLoader.h"
#include <math.h>
#define M_PI 3.14159265358979323846
using namespace std;

GLfloat xi = -8.5;
GLfloat yi = -8;
int px = 7;
int py = 0;

int ganador = 0;

double rx = 0;
double ry = -14;
double rz = 10;

double pxx = 0;
double pyy = -8;
double pz = 4;

double upx = 0;
double upy = 1;
double upz = 0;

/////////////////////////

GLint escala = 1;

GLdouble anguloY = 0;
GLdouble anguloX = 0;
GLdouble anguloZ = 0;

void muro(int x, int y);
void mono(int x, int y);
void ganar();
void emoprint(int x, int y);

GLuint _text1;
GLuint _text2;
GLuint _text3;
GLuint _text4;
GLuint _text5;
GLuint _text6;



GLuint loadTexture(Image* image) {
	GLuint idtextura;
	glGenTextures(1, &idtextura);
	glBindTexture(GL_TEXTURE_2D, idtextura);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return idtextura;
}


void initRendering() {
	Image* lado1 = loadBMP("1.bmp");
	_text1 = loadTexture(lado1);
	delete lado1;

	Image* lado2 = loadBMP("2.bmp");
	_text2 = loadTexture(lado2);
	delete lado2;

	Image* lado3 = loadBMP("3.bmp");
	_text3 = loadTexture(lado3);
	delete lado3;

	Image* lado4 = loadBMP("4.bmp");
	_text4 = loadTexture(lado4);
	delete lado4;

	Image* lado5 = loadBMP("5.bmp");
	_text5 = loadTexture(lado5);
	delete lado5;

	Image* lado6 = loadBMP("6.bmp");
	_text6 = loadTexture(lado6);
	delete lado6;
}

void cargarTextura(GLuint _textura) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//Matriz
int mapa[17][22] = {
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,4 },
	{ 1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1 },
	{ 1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1 },
	{ 1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1 },
	{ 3,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1 },
	{ 1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1 },
	{ 1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1 },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };

int emoji[15][15] = {
	{ 0,0,0,0,0,1,1,1,1,1,0,0,0,0,0 },
	{ 0,0,0,1,1,2,2,2,2,1,1,1,0,0,0 },
	{ 0,0,1,2,2,2,2,1,1,1,2,2,1,0,0 },
	{ 0,1,2,2,2,2,1,3,3,1,2,2,2,1,0 },
	{ 0,1,2,2,2,2,1,1,3,1,2,2,2,1,0 },
	{ 1,2,2,1,2,2,1,1,1,1,2,2,2,2,1 },
	{ 1,2,2,1,2,2,2,1,1,1,2,2,2,2,1 },
	{ 1,2,2,1,2,2,2,2,1,1,2,2,2,2,1 },
	{ 1,2,2,1,2,2,2,1,1,1,2,2,2,2,1 },
	{ 1,2,2,1,2,2,1,3,3,1,2,2,2,2,1 },
	{ 0,1,2,2,1,2,1,1,3,1,2,2,2,1,0 },
	{ 0,1,2,2,2,2,1,1,1,1,2,2,2,1,0 },
	{ 0,0,1,2,2,2,2,1,1,1,2,2,1,0,0 },
	{ 0,0,0,1,1,2,2,2,2,1,1,1,0,0,0 },
	{ 0,0,0,0,0,1,1,1,1,1,0,0,0,0,0 }
};


void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
}



void ArrowKey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if (ganador == 0)
		{
			if (mapa[px + 1][py] == 0)
			{
				mapa[px][py] = 0;
				mapa[px + 1][py] = 2;
				px = px + 1;
				//
				rx = rx + 0.6;
				pxx = pxx + 0.6;
				//
			}
		}

		break;

	case GLUT_KEY_LEFT:
		if (ganador == 0)
		{
			if (mapa[px - 1][py] == 0)
			{
				mapa[px][py] = 0;
				mapa[px - 1][py] = 2;
				px = px - 1;
				//
				rx = rx - 0.6;
				pxx = pxx - 0.6;
				//
			}
		}
		break;

	case GLUT_KEY_UP:
		if (ganador == 0)
		{

			if (py<16)
			{
				if (mapa[px][py + 1] == 0)
				{
					mapa[px][py] = 0;
					mapa[px][py + 1] = 2;
					py = py + 1;
					//
					ry = ry + 0.6;
					pyy = pyy + 0.6;
					//
				}
				if (mapa[px][py + 1] == 4)
				{
					mapa[px][py] = 0;
					mapa[px][py + 1] = 2;
					py = py + 1;
					ganar();
				}
			}
			if (py == 16)
			{
				px = px;
				py = py;
			}
		}

		break;

	case GLUT_KEY_DOWN:
		if (ganador == 0)
		{
			if (py>0)
			{
				if (mapa[px][py - 1] == 0)
				{
					mapa[px][py] = 0;
					mapa[px][py - 1] = 2;
					py = py - 1;
					//
					ry = ry - 0.6;
					pyy = pyy - 0.6;
					//
				}
			}
		}

		break;
	case GLUT_KEY_HOME:
		if (xi > -10)
			xi -= 0.5;
		break;
	case GLUT_KEY_PAGE_UP:
		if (yi < 10)
			yi += 0.5;
		break;

	case GLUT_KEY_PAGE_DOWN:
		if (yi > -10)
			yi -= 0.5;
		break;

	case GLUT_KEY_END:
		if (xi < 10)
			xi += 0.5;
		break;
	case GLUT_KEY_F2:
		anguloZ += 5;
		break;
	case GLUT_KEY_F3:
		anguloZ -= 5;
		break;
	case GLUT_KEY_F4:
		anguloY += 5;
		break;
	case GLUT_KEY_F5:
		anguloY -= 5;
		break;
	case GLUT_KEY_F6:
		if (escala <= 2)
			escala = 2;
		escala = escala - 1;
		break;
	case GLUT_KEY_F7:
		if (escala > 5)
			escala = 5;
		escala = escala + 1;
		break;
	case GLUT_KEY_F8:
		anguloX += 5;
		break;
	case GLUT_KEY_F9:
		anguloX -= 5;
		break;




	default:
		break;
	}

	glutPostRedisplay();
}

void ganar()
{
	rx = 0;
	ry = -8.2;
	rz = 16.8;
	pxx = 0;
	pyy = -5.8;
	pz = 10.8;
	ganador = 1;
}

void emoprint(int x, int y)
{
	cout << "jola" << endl;
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(-10 + rx, 9 + ry, -10 + rz, pxx, pyy, pz, 0, 1, 0);
	gluLookAt(rx, ry, rz, pxx, pyy, pz, upx, upy, upz);
	//gluLookAt(0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(xi, yi, -3);
	glScaled(escala, escala, 1.0);
	//gluPerspective(40.0, 1.0, 1.0, 30.0);

	glRotated(anguloX, 1.0, 0.0, 0.0);
	glRotated(anguloY, 0.0, 1.0, 0.0);
	glRotated(anguloZ, 0.0, 0.0, 1.0);

	mapa[px][py] = 2;

	//Suelo
	cargarTextura(_text1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(0, 0, 0);
	glTexCoord2f(1, 0.0);		glVertex3f(0, 17, 0);
	glTexCoord2f(1.0, 1.0);		glVertex3f(17, 17, 0);
	glTexCoord2f(0.0, 1.0);		glVertex3f(17, 0, 0);
	glEnd();

	for (int i = 0; i<17; i++)
	{
		for (int j = 0; j<22; j++)
		{
			if (mapa[i][j] == 1)
			{
				muro(i, j);
			}
			if (mapa[i][j] == 2)
			{
				mono(i, j);
			}
		}
	}
	glPopMatrix(); // Cierra la matriz
	glFlush();
	glutSwapBuffers();
}

void mono(int x, int y) {
	//Frontal Y
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex3f(x + 0.2, y + 0.2, 0.8);
	glVertex3f(x + 0.8, y + 0.2, 0.8);
	glVertex3f(x + 0.8, y + 0.8, 0.8);
	glVertex3f(x + 0.2, y + 0.8, 0.8);
	glEnd();

	//lateral izquierdo
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex3f(x + 0.2, y + 0.2, 0.0);
	glVertex3f(x + 0.2, y + 0.2, 0.8);
	glVertex3f(x + 0.2, y + 0.8, 0.8);
	glVertex3f(x + 0.2, y + 0.8, 0.0);
	glEnd();

	//Lateral derecha
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex3f(x + 0.8, y + 0.2, 0.0);
	glVertex3f(x + 0.8, y + 0.2, 0.8);
	glVertex3f(x + 0.8, y + 0.8, 0.8);
	glVertex3f(x + 0.8, y + 0.8, 0.0);
	glEnd();

	//Lateral abajo y
	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.4, 1);
	glVertex3f(x + 0.8, y + 0.2, 0.0);
	glVertex3f(x + 0.8, y + 0.2, 0.8);
	glVertex3f(x + 0.2, y + 0.2, 0.8);
	glVertex3f(x + 0.2, y + 0.2, 0.0);
	glEnd();

	//Lateral Arriba y
	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.4, 1);
	glVertex3f(x + 0.8, y + 0.8, 0.0);
	glVertex3f(x + 0.8, y + 0.8, 0.8);
	glVertex3f(x + 0.2, y + 0.8, 0.8);
	glVertex3f(x + 0.2, y + 0.8, 0.0);
	glEnd();
}
void muro(int x, int y) {
	//Frontal Y
	cargarTextura(_text6);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(x, y, 1);
	glTexCoord2f(1.0, 0.0);		glVertex3f(x + 1, y, 1);
	glTexCoord2f(1.0, 1.0);		glVertex3f(x + 1, y + 1, 1);
	glTexCoord2f(0.0, 1.0);		glVertex3f(x, y + 1, 1);
	glEnd();

	//lateral izquierdo
	cargarTextura(_text3);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(x, y, 0.0);
	glTexCoord2f(1.0, 0.0);		glVertex3f(x, y, 1.0);
	glTexCoord2f(1.0, 1.0);		glVertex3f(x, y + 1, 1.0);
	glTexCoord2f(0.0, 1.0);		glVertex3f(x, y + 1, 0.0);
	glEnd();

	//Lateral derecha
	cargarTextura(_text3);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(x + 1, y, 0.0);
	glTexCoord2f(1.0, 0.0);		glVertex3f(x + 1, y, 1.0);
	glTexCoord2f(1.0, 1.0);		glVertex3f(x + 1, y + 1, 1.0);
	glTexCoord2f(0.0, 1.0);		glVertex3f(x + 1, y + 1, 0.0);
	glEnd();

	//Lateral abajo y
	cargarTextura(_text3);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(x + 1, y, 0.0);
	glTexCoord2f(1.0, 0.0);		glVertex3f(x + 1, y, 1.0);
	glTexCoord2f(1.0, 1.0);		glVertex3f(x, y, 1.0);
	glTexCoord2f(0.0, 1.0);		glVertex3f(x, y, 0.0);
	glEnd();

	//Lateral Arriba y
	cargarTextura(_text3);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(x + 1, y + 1, 0.0);
	glTexCoord2f(1.0, 0.0);		glVertex3f(x + 1, y + 1, 1.0);
	glTexCoord2f(1.0, 1.0);		glVertex3f(x, y + 1, 1.0);
	glTexCoord2f(0.0, 1.0);		glVertex3f(x, y + 1, 0.0);
	glEnd();


}

void reshape(int w, int h) {
	//glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//Inicializa la matriz de proyeccion
	gluPerspective(60, (GLfloat)h / (GLfloat)w, 1, 300);
	//glOrtho(-10.0, 10.0, -10.00, 10.0, 0.1, 20.0); // WorkSpace
	glMatrixMode(GL_MODELVIEW); // cambiamos la matrix :D
	glLoadIdentity();
}
void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case 'd':
		rx = rx + 0.2;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'a':
		rx = rx - 0.2;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'l':
		pxx = pxx + 0.2;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'j':
		pxx = pxx - 0.2;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'w':
		ry = ry + 0.2;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 's':
		ry = ry - 0.2;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'i':
		pyy = pyy + 0.2;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'k':
		pyy = pyy - 0.2;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'c':
		rz = rz + 1;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'z':
		rz = rz - 1;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'm':
		pz = pz + 1;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'b':
		pz = pz - 1;
		cout << "eye X: " << rx << endl;
		cout << "eye Y: " << ry << endl;
		cout << "eye Z: " << rz << endl;
		cout << "at X: " << pxx << endl;
		cout << "at Y: " << pyy << endl;
		cout << "at Z: " << pz << endl;
		break;
	case 'g':
		rx = 0;
		ry = -14;
		rz = 10;

		pxx = 0;
		pyy = -8;
		pz = 4;

		break;

	case 49:
		ry = ry + 0.2;
		pyy = pyy + 0.2;
		break;
	case 50:
		ry = ry - 0.2;
		pyy = pyy - 0.2;
		break;

	case 51:
		rx = rx + 0.2;
		pxx = pxx + 0.2;
		break;
	case 52:
		rx = rx - 0.2;
		pxx = pxx - 0.2;
		break;
	case 53:
		rz = rz + 0.2;
		pz = pz + 0.2;
		break;
	case 54:
		rz = rz - 0.2;
		pz = pz - 0.2;
		break;

	case 'f':
		upx = upx - 1;
		break;
	case 'h':
		upx = upx + 1;
		break;

	case 'v':
		upy = upy - 1;
		break;
	case 'n':
		upy = upy + 1;
		break;
	case 't':
		upz = upz - 1;
		break;
	case 'y':
		upz = upz + 1;
		break;
	case 'q':
		rx = rx + 90;
		break;
	case 'e':
		rx = rx - 90;
		break;
	}







}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(650, 650);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Laberinto");
	glEnable(GL_DEPTH_TEST);
	init();
	initRendering();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(ArrowKey);
	glutMainLoop();
	return 0;
}

