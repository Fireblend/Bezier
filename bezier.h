#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

//Display callback para la ventana a dibujar.
void drawingMachine ();

//Algoritmos auxiliares de Bresenham
int analizaLinea(int x0,int y0,int x1,int y1);
void Bresenham(int x0, int y0, int x1, int y1, int plot);
void Bresenham_aux(int X0, int Y0, int X1, int Y1,int id, int plot);

void init();
