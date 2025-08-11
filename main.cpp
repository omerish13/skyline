
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"

const int TAIL_LENGTH = 40;
const double PI = 3.14;

const int NUM_WAVES = 10;
double waveY[NUM_WAVES];

typedef struct
{
	double x, y;
} POINT2D;


typedef struct
{
	POINT2D position;
	int size;
} STAR;

const int NUM_STARS = 100;

STAR stars[NUM_STARS];
double offset = 0;


void init()
{
	int i,j;
	glClearColor(0.0,0.0,0.3,0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // set the coordinates system



	srand(time(0));

	for (i = 0;i < NUM_STARS;i++)
	{
		stars[i].position.x = -1 + 2 * ((rand() % 1000) / 1000.0); // random value in [-1,1)
		stars[i].position.y = -0.2 + 2 * ((rand() % 1000) / 1000.0); // random value in [-0.2,1)
		stars[i].size = 1 + rand() % 3;

	}

    double waterTop    = -0.2;
    double waterBottom = -1.0;
    double spacing     = (waterTop - waterBottom) / NUM_WAVES;
    for (int i = 0; i < NUM_WAVES; ++i) {
        waveY[i] = waterBottom + i * spacing;
    }

}

double ReflectY(double originalX, double originalY) {
    double waterLevel = -0.2;
    double mirrored   = waterLevel - (originalY - waterLevel);
    // Add a wave effect to the reflection
    double wave = 0.01 * sin(10 * PI * originalX + offset)
                  + 0.005 * sin(5 * PI * originalX - 2 * offset);
    return mirrored + wave;
}


void DrawStars()
{
	int i;

	glColor3d(1, 1, 0); // yellow

	for (i = 0;i < NUM_STARS;i++)
	{
		glPointSize(stars[i].size);
		glBegin(GL_POINTS);
		glVertex2d(stars[i].position.x, stars[i].position.y);
		glEnd();
	}
}

void DrawSky()
{
	glBegin(GL_POLYGON);
	glColor3d(0.1, 0.0, 0.3);
	glVertex2d(-1, 1); // left-top
	glVertex2d(1, 1); // right-top
	glColor3d(0.9, 0.7, 0.4);
	glVertex2d(1, -0.2); // right-water level
	glVertex2d(-1, -0.2); // left-water level
	glEnd();

	glBegin(GL_POLYGON);
	glColor3d(0.8, 0.5, 0.3);
	glVertex2d(1, -0.2); // right-water level
	glVertex2d(-1, -0.2); // left-water level
	glColor3d(0.4, 0.3, 0.5);
	glVertex2d(-1, -1); // left-top
	glVertex2d(1, -1); // right-top
	glEnd();

	DrawStars();
}

void DrawFunctionGraph()
{
	double x, y;

	// x and y axes
	glColor3d(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2d(-1, 0);
	glVertex2d(1, 0);
	glVertex2d(0,-1);
	glVertex2d(0,1);
	glEnd();

	// draw graph of function
	glBegin(GL_LINE_STRIP);
	for (x = -1;x <= 1;x += 0.01)
	{
//		y =fabs( 0.5*sin(x*2*PI )*cos(4*x*PI+ offset));
		y = 0.6 * cos(0.4 * PI * x) + 0.1*sin(x * 20 * PI )+ 0.1 * sin(x * 5 * PI);
		glVertex2d(x, y);
	}
	glEnd();

	// reflections
/*
	glColor3d(0.4, 0.6, 1);
	glBegin(GL_LINE_STRIP);
	for (x = -1;x <= 1;x += 0.01)
	{
		y = -fabs(0.5 * sin(x * 2 * PI) * cos(4 * x * PI+offset));
		glVertex2d(x, y);
	}
	glEnd();
	*/



}

void DrawPointedBuilding(double x, double y, double w){
    glBegin(GL_POLYGON);
    glColor3d(0.2, 0.2, 0.2);
    glVertex2d(x, y);
    glVertex2d(x + w / 2, y);
    glColor3d(0.9 * fabs(x), y - 0.4 * fabs(x), y);
    glVertex2d(x + w, -0.2);
    glVertex2d(x, -0.2);
    glEnd();

    // shininess
    glColor3d(0.9, 0.9, 0.8);
    glBegin(GL_POLYGON);
    glVertex2d(x + w / 2, y);
    glVertex2d(x + w /2 + 0.01, y);
    glVertex2d(x + w + 0.01, -0.2);
    glVertex2d(x + w, -0.2);
    glEnd();
    int maxStores = 20;
    int maxWindowsPerStore = 10;
    double yTop    = y;
    double yBottom = -0.2;
    glColor3d(1,1,0);
    glPointSize(1);

    for(int store = 1; store <= maxStores; ++store)
    {
        // yLevel is interpolated between yBottom and yTop
        double tVert  = static_cast<double>(store) / (maxStores + 1.0);
        double yLevel = yBottom + tVert * (yTop - yBottom);
        double ratio  = (yLevel - yTop) / (yBottom - yTop);
        double widthAtLevel = (1.0 - ratio)*(w/2.0) + ratio*w;

        int windowsThisStore = static_cast<int>(floor(maxWindowsPerStore * (widthAtLevel / w)));
        if (windowsThisStore < 1) continue;

        double xLeft  = x;
        double xRight = x + widthAtLevel;
        double dx     = (xRight - xLeft) / (windowsThisStore + 1);

        for(int col = 1; col <= windowsThisStore; ++col) {
            double windX = xLeft + col * dx;
            glBegin(GL_POINTS);
            glVertex2d(windX, yLevel);
            glEnd();
        }
    }
}


void DrawTriangleEndBuilding(double x, double y, double w)
{
    // body
    glBegin(GL_POLYGON);
    glColor3d(0.2, 0.2, 0.2);
    glVertex2d(x + w / 2.0, y);          // apex
    glColor3d(0.9 * fabs(x), y - 0.4 * fabs(x), y);
    glVertex2d(x + w, -0.2);             // base right
    glVertex2d(x, -0.2);                 // base left
    glEnd();

    // shininess on right edge
    glColor3d(0.9, 0.9, 0.8);
    glBegin(GL_POLYGON);
    glVertex2d(x + w / 2.0, y);
    glVertex2d(x + w + 0.01, -0.2);
    glVertex2d(x + w, -0.2);
    glEnd();

    // windows: place them inside the triangular silhouette
    int maxStores = 20;
    int maxWindowsPerStore = 10;
    double yTop    = y;
    double yBottom = -0.2;
    double tVert,yLevel, ratio, widthAt;
    double xLeft, dx, wx, xCenter;
    glColor3d(1, 1, 0);
    glPointSize(1);
    for (int store = 1; store <= maxStores; ++store) {
        tVert  = static_cast<double>(store) / (maxStores + 1.0);
        yLevel = yBottom + tVert * (yTop - yBottom);
        ratio  = (yTop - yLevel) / (yTop - yBottom);
        widthAt = ratio * w;           // width shrinks to zero at the apex
        if (widthAt < 0.02) continue;         // skip very narrow floors
        int windowsThisStore = static_cast<int>(floor(maxWindowsPerStore * (widthAt / w)));
        if (windowsThisStore < 1) continue;
        xCenter = x + w / 2.0;         // centre of the triangle
        xLeft   = xCenter - widthAt / 2.0;
        dx      = widthAt / (windowsThisStore + 1);
        for (int col = 1; col <= windowsThisStore; ++col) {
            wx = xLeft + col * dx;
            glBegin(GL_POINTS);
            glVertex2d(wx, yLevel);
            glEnd();
        }
    }
}


void DrawSquareBuilding(double x, double y, double w)
{
    glBegin(GL_POLYGON);
    glColor3d(0.2, 0.2, 0.2);
    glVertex2d(x, y);
    glVertex2d(x + w, y);
    glColor3d(0.9 * fabs(x), y - 0.4 * fabs(x), y);
    glVertex2d(x + w, -0.2);
    glVertex2d(x, -0.2);
    glEnd();

    // shininess
    glColor3d(0.9, 0.9, 0.8);
    glBegin(GL_POLYGON);
    glVertex2d(x + w, y);
    glVertex2d(x + w + 0.01, y);
    glVertex2d(x + w + 0.01, -0.2);
    glVertex2d(x + w, -0.2);
    glEnd();

    // windows
    int num_stores = 20;
    int num_windows_per_store = 10;
    double dh, dv; // horizontal and vertical spacings
    dh = w / num_windows_per_store;
    dv = (y + 0.2) / num_stores;
    int store, column;
    glColor3d(1, 1, 0);
    glPointSize(1);
    for (store = 2; store < num_stores - 2; store++)
        for (column = 0; column < num_windows_per_store - 1; column++)
        {
            glBegin(GL_POINTS);
            glVertex2d(x + 0.01 + column * dh, -0.2 + store * dv);
            glEnd();
        }
}


void DrawHalfCircleWaves() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4d(0.8, 0.8, 1.0, 0.2);    // subtle highlight colour
    const int SEGMENTS = 80;
    const double amplitude = 0.05;    // height of each arc

    for (int w = 0; w < NUM_WAVES; ++w) {
        double baseY = waveY[w];
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= SEGMENTS; ++i) {
            double x = -1.0 + 2.0 * i / SEGMENTS;             // x ∈ [-1,1]
            // Compute the half‑circle’s vertical displacement: yOffset = A·√(1–x²)
            double yOffset = amplitude * sqrt(fmax(0.0, 1.0 - x * x));
            // Invert the offset so the arc bulges downward into the water
            double y = baseY + yOffset;
            if (y <= -0.2)                                     // only draw in water
                glVertex2d(x, y);
        }
        glEnd();
    }
}




void DrawBuildings()
{
	double x, y,w;
	w = 0.12;

	// far buildings
	for (x = -1;x <= 1;x += 0.05)
	{
		//		y =fabs( 0.5*sin(x*2*PI )*cos(4*x*PI+ offset));
		y = 0.6 * cos(0.4 * PI * x) + 0.15 * sin(x * 20 * PI) + 0.2 * sin(x * 5 * PI);
        if (x < -0.7 || x > 0.7)
        {
            DrawTriangleEndBuilding(x, y, w);

        }
        else if (x < -0.5 || x > 0.5)
        {
            DrawPointedBuilding(x, y, w);
        }
        else
        {
            DrawSquareBuilding(x, y, w);
        }

        x += w;
	}

	// near buildings
	w = 0.15;
	for (x = -1;x <= 1;x += 0.05)
	{
		//		y =fabs( 0.5*sin(x*2*PI )*cos(4*x*PI+ offset));
		y = 0.2 * cos(0.4 * PI * x) + 0.1 * sin(x * 13 * PI) + 0.1 * sin(x * 5 * PI);

        if (x < -0.7 || x > 0.7)
        {
            DrawSquareBuilding(x, y, w);
        }
        else if (x < -0.5 || x > 0.5)
        {
            DrawTriangleEndBuilding(x, y, w);
        }
        else
        {
            DrawPointedBuilding(x, y, w);
        }


		x += w;
	

		// frame
/*		glColor3d(0, 0, 0);

		glBegin(GL_LINE_LOOP);
		glVertex2d(x, y);
		glVertex2d(x + w, y);
		glVertex2d(x + w, -0.2);
		glVertex2d(x, -0.2);
		x += w;
		glEnd();
		*/
	}

}

void DrawSceneWithReflection() {
    // draw the normal skyline
    DrawBuildings();

    // draw reflection: invert the y‑axis and translate to the water level
    glPushMatrix();
    glTranslated(0.0, -0.21, 0.0);
    glScaled(1.0, -0.5, 1.0);
    glTranslated(0.0, 0.2, 0.0);
    DrawBuildings();   // draw mirrored skyline
    glPopMatrix();

    // darken the reflection region
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.4f); // 40 % opaque black
    glBegin(GL_QUADS);
    glVertex2d(-1.0, -1.0);
    glVertex2d( 1.0, -1.0);
    glVertex2d( 1.0, -0.2);
    glVertex2d(-1.0, -0.2);
    glEnd();
}




void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawSky();
	//DrawFunctionGraph();
    DrawSceneWithReflection();
    DrawHalfCircleWaves();         // draw the water waves



	glutSwapBuffers(); // show all
}

void idle() 
{
	int index;
	int i,j;

	offset += 0.02;

	// sparking
	
	if (rand() % 10 == 0)
	{
		index = rand() % NUM_STARS;
		stars[index].size = 1 + rand() % 3;
	}

    for (int w = 0; w < NUM_WAVES; ++w) {
        waveY[w] += 0.0005;            // upward speed
        if (waveY[w] > -0.2)           // recycle at the waterline
            waveY[w] = -1.0;
    }


    glutPostRedisplay(); // indirect call to display
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // defines visual memory
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("City skyline");

	// 
	glutDisplayFunc(display); // this is a pointer to refresh window function
	glutIdleFunc(idle); // this is a pointer to idle function that runs in background all the time

	init();

	glutMainLoop();

    return 0;
}