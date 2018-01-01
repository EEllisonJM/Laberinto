#include "stdafx.h"
#include <GL/glut.h>
#include "ImageLoader.h"
#include <math.h>
#define M_PI 3.14159265358979323846
using namespace std;
//-------------
GLfloat auxX, ax = 0.5;
GLfloat auxY, ay = 9.5;
//----------------
double mouseX, mouseY, oldMouseX, oldMouseY, angle;
/*Posicion Jugador Inicial*/
int px = 0;
int py = 9;
/*glLookAt*/
double rx = 1.5, ry = 9.5, rz = .7;//Specifies the position of the eye point.
double pxx = 2.5, pyy = 9.55, pzz = 0.7;//Specifies the position of the reference point.
GLfloat av = 0;
GLint escala = 1;
/*Texturas*/
GLuint _text1;
GLuint _text2;
GLuint _text3;
GLuint _text4;
GLuint _text5;
GLuint _text6;

/*Métodos*/
void graficarMuro(int x, int y);
void graficarJugador(int x, int y);

/*Cargar imagen de textura a usar*/
GLuint loadTexture(Image* image) {
	GLuint idtextura;
	glGenTextures(1, &idtextura);
	glBindTexture(GL_TEXTURE_2D, idtextura);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return idtextura;
}
/*Inicializar renderizado [Generar una imagen desde un modelo]*/
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
/*Cargar textura*/
void cargarTextura(GLuint _textura) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
/* Diseño del laberinto [mapa].
0 => Camino[Abarca todo el espacio del laberinto]
1 => Muro
3 => Entrada
4 => Salida
*/
int mapa[17][22] = {
	{ 1,1,1,1,1,1,1,1,1,3/*(0,9)*/,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1 },
	{ 1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1 },
	{ 1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1 },
	{ 1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1 },
	{ 1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1 },
	{ 1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1 },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,1 } };
/*Makes 3D drawing work when something is in front of something else*/
void init(void) {
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
}
/*Suelo[Todo el espacio del laberinto] - Pasto*/
void graficarSuelo() {
	cargarTextura(_text1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(0, 0, 0);
	glTexCoord2f(1, 0.0);		glVertex3f(0, 17, 0);
	glTexCoord2f(1.0, 1.0);		glVertex3f(17, 17, 0);
	glTexCoord2f(0.0, 1.0);		glVertex3f(17, 0, 0);
	glEnd();
}
/*Graficar al jugador*/
void graficarJugador(int x, int y) {
	//Frontal Y
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex3f(x + 0.2, y + 0.2, 0.4);
	glVertex3f(x + 0.4, y + 0.2, 0.4);
	glVertex3f(x + 0.4, y + 0.4, 0.4);
	glVertex3f(x + 0.2, y + 0.4, 0.4);
	glEnd();

	//lateral izquierdo
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex3f(x + 0.2, y + 0.2, 0.0);
	glVertex3f(x + 0.2, y + 0.2, 0.4);
	glVertex3f(x + 0.2, y + 0.4, 0.4);
	glVertex3f(x + 0.2, y + 0.4, 0.0);
	glEnd();

	//Lateral derecha
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex3f(x + 0.4, y + 0.2, 0.0);
	glVertex3f(x + 0.4, y + 0.2, 0.4);
	glVertex3f(x + 0.4, y + 0.4, 0.4);
	glVertex3f(x + 0.4, y + 0.4, 0.0);
	glEnd();

	//Lateral abajo y
	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.4, 1);
	glVertex3f(x + 0.4, y + 0.2, 0.0);
	glVertex3f(x + 0.4, y + 0.2, 0.4);
	glVertex3f(x + 0.2, y + 0.2, 0.4);
	glVertex3f(x + 0.2, y + 0.2, 0.0);
	glEnd();

	//Lateral Arriba y
	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.4, 1);
	glVertex3f(x + 0.4, y + 0.4, 0.0);
	glVertex3f(x + 0.4, y + 0.4, 0.4);
	glVertex3f(x + 0.2, y + 0.4, 0.4);
	glVertex3f(x + 0.2, y + 0.4, 0.0);
	glEnd();
}
/*Graficar el muro*/
void graficarMuro(int x, int y) {
	//Lateral Abajo y [Techo]
	cargarTextura(_text6);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(x, y, 1);
	glTexCoord2f(1.0, 0.0);		glVertex3f(x + 1, y, 1);
	glTexCoord2f(1.0, 1.0);		glVertex3f(x + 1, y + 1, 1);
	glTexCoord2f(0.0, 1.0);		glVertex3f(x, y + 1, 1);
	glEnd();
	//Lateral izquierdo
	cargarTextura(_text3);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(x, y, 0.0);
	glTexCoord2f(1.0, 0.0);		glVertex3f(x, y, 1.0);
	glTexCoord2f(1.0, 1.0);		glVertex3f(x, y + 1, 1.0);
	glTexCoord2f(0.0, 1.0);		glVertex3f(x, y + 1, 0.0);
	glEnd();
	//Lateral derecho
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

	//Lateral Arriba y [Ayotea]
	cargarTextura(_text3);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);		glVertex3f(x + 1, y + 1, 0.0);
	glTexCoord2f(1.0, 0.0);		glVertex3f(x + 1, y + 1, 1.0);
	glTexCoord2f(1.0, 1.0);		glVertex3f(x, y + 1, 1.0);
	glTexCoord2f(0.0, 1.0);		glVertex3f(x, y + 1, 0.0);
	glEnd();
}
/*Creacion de la escena*/
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//Inicializa la matriz de proyeccion
	gluPerspective(50, (GLfloat)h / (GLfloat)w, .01, 25.0);//(fovy, aspect, near, far)
	glMatrixMode(GL_MODELVIEW); // cambiamos la matrix :D
	glLoadIdentity();
	//gluLookAt(0, 10, 0, 0, 2, 5, 0, 1, 0);
}
/*Movimientos del  [Jugador] con las teclas de direccion*/
void ArrowKey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_RIGHT://Derecha
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(//Transgraficar una vista
		rx, ry, rz,//Posicion de la camara
		pxx, pyy, pzz,//[Lo que queremos ver]
		0, 0, 1);//Orientacion de la camara

	glPushMatrix();
	//--------------
	glTranslatef(auxX, auxY, 0);
	glScaled(escala, escala, 1.0);

	/*glRotated(anguloX, 1.0, 0.0, 0.0);
	glRotated(anguloY, 0.0, 1.0, 0.0);
	glRotated(anguloZ, 0.0, 0.0, 1.0);*/
	//--------------
	mapa[px][py] = 2;

	graficarSuelo();//Pasto

	for (int i = 0; i<17; i++)
	{
		for (int j = 0; j<22; j++)
		{
			if (mapa[i][j] == 1)
			{//1 => Muro
				graficarMuro(i, j);//(x,y)
			}
			if (mapa[i][j] == 2)
			{
				graficarJugador(i, j);
			}
		}
	}
	glPopMatrix(); // Cierra la matriz
	glFlush();
	glutSwapBuffers();
}
int aux, aux2 = 0;

void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'x'://Adelante
		ax = rx + .3 * sin(av);
		ay = ry - .3 * cos(av);
		aux = (int)ax;
		aux2 = (int)ay;
		if (mapa[aux][aux2] == 0) {
			rx = ax;
			ry = ay;
			/*Objeto a Mirar*/
			pxx = rx + .3 * sin(av);
			pyy = ry - .3 * cos(av);
		}
		break;
	case 'n'://Izquierda
			 /*Mover objeto a Mirar*/
		av = av + (M_PI / 36);
		pxx = rx + .3 * sin(av);
		pyy = ry - .3 * cos(av);
		break;
	case 'm'://Derecha
		av = av - (M_PI / 36);
		pxx = rx + .3 * sin(av);
		pyy = ry - .3 * cos(av);
		break;
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);/*Inicializar el modo de visualización.*/
	glutInitWindowSize(650, 670);/*Inicializar el tamaño de la ventana*/
	glutInitWindowPosition(340, 5);/*Inicializar la posición    de    la    ventana    en    el    escritorio*/
	glutCreateWindow("Laberinto");/*Crear ventana con Titulo*/
								  //glClearColor(.5, 1, .2, 0);/*Color de fondo*/
	glEnable(GL_DEPTH_TEST);/*Activar el test del buffer de profundidad: */

	init();
	initRendering();
	glutDisplayFunc(display);/* Definir el callback para redibujar la ventana.*/
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(ArrowKey);
	glutMainLoop();/* bucle de eventos */
	return 0;
}