/*
||==========================================================||
|| 		Instituto Tecnologico de Costa Rica	    ||
||       Ingeniera en Computacion - Computer Graphics       ||
||                         Proyecto 0                       ||
||                                                          ||
|| Sergio Morales                                           ||
||==========================================================||
*/

#include "bezier.h"

int pts[108][2];
int cont = 0;


void addVertex(int x, int y){
	pts[cont][0] = x;
	pts[cont][1] = y;
	cont++;
}

/*Main :)*/
int main(int argc, char** argv) {
	
		/*Inicializa Glut y llama a la funcion de dibujo de las lineas*/	
		glutInit(&argc, argv);
		glutInitWindowSize(900, 500); 
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
		glutCreateWindow("~Firma Sergio Morales. Favor no usar con fines poco legales.~"); 

		init();
		
		glClear(GL_COLOR_BUFFER_BIT);  /*Clears buffer to preset values - buffer currently enabled for color writing */
		gluOrtho2D(-0.5, 900-0.5, -0.5, 500-0.5); /*Defines a 2D orthographic projection matrix */
		glutDisplayFunc(drawingMachine); /*Sets the display callback for the  current window */
		glutMainLoop();
	    
	return 0;
}

//Determina en que octante se encuentra la linea de entrada
int analizaLinea(int x0,int y0,int x1,int y1){
	
	int y = y1 - y0;
	int x = x1 - x0;
	
	if(y > 0){
		if(y > x)return 2; 	//Segundo Octante
		else return 1; 		//Primer Octante
	}
	else{
		if(abs(y) > x)return 7;  //Setimo Octante
		else return 8;			 //Octavo Octante :P
	}
}

void Bresenham_aux(int X0, int Y0, int X1, int Y1,int id, int plot){

int Delta_A, Delta_B, d, Xp, Yp;
Xp = X0; Yp = Y0;				//Pixel de inicio

/*La variable id determina a que cuadrante pertenece la linea.
Para cada cuadrante hay un valor distinto de "d" y de "Delta", segun la
direccion que deba pintar.*/			 

glVertex2i(X0, Y0);
glVertex2i(X1, Y1);
         	
/*Cuadrante 1*/
	if(id==1){	
    	Delta_A = 2*(Y1 - Y0);					//Delta_E
    	Delta_B = 2*((Y1 - Y0)-(X1 - X0));		//Delta_NE
    	d = 2*(Y1 - Y0)-(X1 - X0); 				//Valor inicial de d
    	
    	/*Se pinta desde X0 hasta X1*/
    	while (Xp < X1){		
        	
        	if (d < 0){
			/*Pintar E*/
				Xp++;
				d = d + Delta_A;
         	}
         	else{ 
         	/*Pintar NE*/
            	Xp++;
            	Yp++;
            	d = d + Delta_B;
         	}
         	glVertex2i(Xp, Yp);
		}
	}
	

	

/*Cuadrante 2 */ 
	else if(id==2){
		Delta_A =  -2*(X1 - X0);				//Delta_N
		Delta_B = 2*((Y1 - Y0) - (X1 - X0));	//Delta_NE
		d = (Y1 - Y0) - 2*(X1 - X0);			//Valor inicial de d
		
		/*Se pinta desde Y0 hasta Y1*/
		while (Yp < Y1){
			if (d > 0){
				/*Pintar N*/
				Yp++;
				d = d + Delta_A;
			}
			else{
				/*Pintar NE*/
				Yp++;
				Xp++;
				d = d + Delta_B;
			}
			glVertex2i(Xp, Yp);
		}	
	}
	
/*Cuadrante 7*/
	else if(id==7){
		Delta_A =  2*(X1-X0);				//Delta_S
		Delta_B = 2*((Y1-Y0)+(X1-X0));			//Delta_SE
		d = (Y1 - Y0) + 2*(X1 - X0);		//Valor inicial de d
		
		/*Se pinta desde Y0 hasta Y1*/
		while (Yp > Y1){
			if (d < 0){ 
				/*Pintar S*/
				Yp--;
				d = d + Delta_A;
			}
			else{
				/*Pintar SE*/
				Xp++;
				Yp--;
				d = d + Delta_B;
			}
			glVertex2i(Xp, Yp);;
		}
	}
	else{
/*Cuadrante 8*/
		Delta_A =  2*(Y1 - Y0);					//Delta_E
		Delta_B = 2*((Y1-Y0)+(X1-X0));			//Delta_SE
		d = 2*((Y1 - Y0) + (X1 - X0));		//Valor inicial de d
		
		/*Se pinta desde X0 hasta X1*/
		while (Xp < X1){
			if (d > 0){
				/*Pintar E*/
				Xp++;
				d = d + Delta_A;
			}
			else{
				/*Pintar SE*/
				Xp++;
				Yp--;
				d = d + Delta_B;
			}
			glVertex2i(Xp, Yp);
		}
	
	}
}

//Llamada principal del algoritmo de Bresenham 
void Bresenham(int x0, int y0, int x1, int y1, int plot){	
	/*Si se encuentra en el cuadrante 2 o 3 se invierten los puntos*/
	if((x0>x1 && y0>y1) || (x0>x1 && y0<y1))
		Bresenham_aux(x1,y1,x0,y0,analizaLinea(x1,y1,x0,y0), plot);
	else
	/*Sino se llama al algoritmo con los puntos de entrada*/
		Bresenham_aux(x0,y0,x1,y1,analizaLinea(x0,y0,x1,y1), plot);	
}


double expTo(double n,int exp){
	int i;
	double res = n;
	
	for(i=0; i<exp-1; i++){
		res = res*n;
	}
	return res;
}

void bezier(int p0[],int p1[],int p2[],int p3[]){
	
	double t;
	int newVertex[2];
	int x = p0[0];
	int y = p0[1];

	for(t=0.0;t<1;t=(t+0.0001)){
		newVertex[0] = (p0[0]* expTo((1-t),3))+((3*p1[0])*t*expTo((1-t),2))+(3*p2[0]*(1-t)*expTo(t,2))+(p3[0]*expTo(t,3));
		newVertex[1] = (p0[1]* expTo((1-t),3))+((3*p1[1])*t*expTo((1-t),2))+(3*p2[1]*(1-t)*expTo(t,2))+(p3[1]*expTo(t,3));
		Bresenham(x,y, newVertex[0],newVertex[1],0);
		x=newVertex[0];
		y=newVertex[1];
	}
}

/*Draw! :D*/
void drawingMachine() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin (GL_POINTS);

	int i, j;
	
	glColor3f (1, 1, 1);
	for(i = 0; i <= 900; i++){
		for(j = 0; j <= 500; j++){
			glVertex2i(i, j);
		}	
	}
	
	glColor3f (0.866, 0.447, 0.941);
	for(i = 0; i < 108; i += 4)
		bezier(pts[i],pts[i+1],pts[i+2],pts[i+3]);
		
	glEnd();
	glFlush();
}
	
void init(){
	
		addVertex(370, 350); addVertex(130, 400); addVertex(130, 300); addVertex(270, 250);
		addVertex(270, 250); addVertex(340, 230); addVertex(340, 150); addVertex(90, 120);
		addVertex(90, 120);addVertex(10, 130);addVertex(10, 150);addVertex(850, 140);
		addVertex(140, 120);addVertex(430, 590);addVertex(210, 320);addVertex(270, 240);
		addVertex(270, 240);addVertex(560, 690);addVertex(210, 320);addVertex(370, 120);
		addVertex(440, 400);addVertex(230, 370);addVertex(210, 260);addVertex(400, 300);
		addVertex(400, 300);addVertex(30, 270);addVertex(140, 30);addVertex(840, 200);
		addVertex(400, 300);addVertex(30, 270);addVertex(140, 30);addVertex(840, 200);	
		//e
		addVertex(280, 140);addVertex(380, 190);addVertex(280, 190);addVertex(310, 140);
		//r
		addVertex(330, 140);addVertex(345, 160);addVertex(345, 160);addVertex(340, 170);	
		addVertex(355, 140);addVertex(352, 155);addVertex(352, 155);addVertex(365, 170);
		addVertex(340, 170);addVertex(352, 165);addVertex(352, 165);addVertex(365, 170);
		//g
		addVertex(370, 165);addVertex(380, 95);addVertex(430, 195);addVertex(370, 165);	
		addVertex(390, 140);addVertex(410, 80);addVertex(310, 80);addVertex(420, 155);
		
		//i
		addVertex(405, 140);addVertex(455, 178);addVertex(405, 178);addVertex(430, 140);
		
		//o
		addVertex(440, 167);addVertex(450, 97);addVertex(500, 197);addVertex(440, 167);	
		
		//o
		addVertex(340, 165);addVertex(350, 95);addVertex(400, 195);addVertex(340, 165);	
		//r
		addVertex(370, 140);addVertex(385, 160);addVertex(385, 160);addVertex(380, 170);	
		addVertex(395, 140);addVertex(392, 155);addVertex(392, 155);addVertex(405, 170);
		addVertex(380, 170);addVertex(392, 165);addVertex(392, 165);addVertex(405, 170);
		//a
		addVertex(421, 168);addVertex(396, 145);addVertex(427, 133);addVertex(424, 168);
		addVertex(424, 168); addVertex(419, 140); addVertex(424, 140); addVertex(436, 145);
		//l
		addVertex(425, 140);addVertex(520, 390);addVertex(455, 390);addVertex(435, 140);
		//e
		addVertex(430, 140);addVertex(510, 185);addVertex(410, 185);addVertex(460, 140);
		//s
		addVertex(460, 140);addVertex(472, 150);addVertex(472, 150);addVertex(472, 170);
		addVertex(472, 170);addVertex(492, 140);addVertex(492, 140);addVertex(470, 145);
		addVertex(470, 145);addVertex(492, 145);addVertex(492, 145);addVertex(497, 140);
	}
