/*	Terrain Simulation
*	Assignment 3 - CS 3GC3 McMaster University
*	By Eric Amshukov 1133146 and Brandon Byskov 1068517
*	November Friday 27th, 2014
*/

#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <list>
#include <vector>
#include <GL/glew.h>
#include <freeglut.h>

//possibly unnecessary:
//#include <GL/wglew.h>

//brandon's includes:
//#  include <GL/gl.h>
//#  include <GL/glu.h>
//#  include <GL/freeglut.h>


using namespace std;

/* Movement states */
bool hasVerticalCollision;

/* Global angle theta */
float gTheta[3];

/* Current mouse position */
float currentMouse[] = {0, 0, 0};

/* Global origin */
float gOrigin[3];

/* Camera */
float cameraSpeed;
float gCamPos[3];

/* Global physics */
float gravity;
float friction;

/* Light values */
float qaAmbientLight[] = {0.2, 0.2, 0.2, 1.0};
float qaDiffuseLight[] = {0.8, 0.8, 0.8, 1.0};
float qaSpecularLight[] = {1.0, 1.0, 1.0, 1.0};
float lightPosition[] = {0.5, 0.5, 0.0, 1.0};


/* Colours */
float blue[] = {0, 0, 1};
float red[] = {1, 0, 0};
float green[] = {0, 1, 0};
float orange[] = {1, 0.5, 0};
float purple[] = {1.0, 0.0, 0.8};
float white[] = {1.0, 1.0, 1.0};
float black[] = {0.0, 0.0, 0.0};
float grey[] = {0.4, 0.4, 0.4};
float skin[] = {0.96, 0.80, 0.69};
float wood[] = {0.52, 0.37, 0.26};

class Player
{
		bool hasVerticalCollision;
		bool hasHorizontalCollision;
		bool movingForward, movingBackward, movingLeft, movingRight, falling; 
		float pos[3];		//Position
		float dir[3];		//Direction of travel
		float rot[3];		//Orientation
		float velocity[3];		//velocity of movement
		float size;			//Size of character model

		public:
			Player(float* _o, float _size)
			{
				hasHorizontalCollision = false;
				movingForward = false;
				movingBackward = false;
				movingLeft = false;
				movingRight = false;
				falling = true;
				size = _size;
				
				for (int i = 0; i < 3; i++)
				{
					pos[i] = _o[i];
					rot[i] = 1.0;
					dir[i] = 1.0;
					velocity[i] = 0.0;		//Player is initially not moving
				}
			}

			/* Increments a player's position appropriate to the direction they are moving in */
			void move()
			{
				if (falling)
				{
					velocity[1] += gravity;
					pos[1] -= 1.0 * velocity[1];
				}
				else
				{
					velocity[1] = 0;
				}

				/* Moves player FORWARDS at a set speed */
				if (movingForward)
				{
					velocity[0] = 0.1;		
	   				pos[0] += 1.0 * velocity[0];

						if(pos[1] <= 0)
						{
							//t->getHeight()[i][j] - 17
							//p.setY(t->getHeight()[i][j] - 17);
						}
				}

				/* Moves player BACKWARDS at a set speed */
				if (movingBackward)
				{
					velocity[0] = -0.1;
	   				pos[0] += 1.0 * velocity[0];
				}

				/* Strafes player to the RIGHT at a set speed */
				if (movingRight)
				{
					velocity[2] = 0.1;
	   				pos[2] += 1.0 * velocity[2];
				}

				/* Strafes player to the LEFT at a set speed */
				if (movingLeft)
				{
					velocity[2] = -0.1;
	   				pos[2] += 1.0 * velocity[2];
				}

				/* DECELERATES velocity of player when they are not moving forward/backward */
				if (!movingBackward && !movingForward)
				{
					pos[0] += velocity[0];
					if (velocity[0] < -0.0001 || velocity[0] > 0.0001)	//limit
					{
						velocity[0] *= friction;
					}
					else
					{
						velocity[0] = 0;
					}
				}

				/* DECELERATES velocity of player when they are not moving side to side */
				if (!movingRight && !movingLeft)
				{
					pos[2] += velocity[2];
					if (velocity[2] < -0.0001 || velocity[2] > 0.0001) //limit
					{
						velocity[2] *= friction;
					}
					else
					{
						velocity[2] = 0;
					}
				}
			}

			/* Returns the position coordinates of a player */
			float * getPos()
			{
				return pos;
			}

			/* Returns X coordinate of player */
			float getX()
			{
				return pos[0];
			}

			/* Returns Y coordinate of player */
			float getY()
			{
				return pos[1];
			}

			/* Returns Z coordinate of player */
			float getZ()
			{
				return pos[2];
			}

			float getSize()
			{
				return size;
			}

			void collidesVertically(float y)
			{
				falling = false;
				pos[1] = y;
			}

			void setY(float _y)
			{
				//pos[1] = _y;
			}

			void setMovement(int v)
			{
				switch (v)
				{
					case 0:
						movingBackward = true;
						break;
					case 1:
						movingForward = true;
						break;
					case 2:
						movingLeft = true;
						break;
					case 3:
						movingRight = true;
						break;
					default:
						break;
				}
			}

			void cancelMovement(int v)
			{
				switch (v)
				{
					case 0:
						movingBackward = false;
						break;
					case 1:
						movingForward = false;
						break;
					case 2:
						movingLeft = false;
						break;
					case 3:
						movingRight = false;
						break;
					default:
						break;
				}
			}

			void draw()
			{		
				//Body
				glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
				glMaterialfv(GL_FRONT, GL_SPECULAR, blue);
				glMaterialfv(GL_FRONT, GL_SHININESS, white);

				glPushMatrix();
				glTranslatef(pos[0]+2, pos[1]+2, pos[2]+2);
				glRotatef(-90,1,0,0);
				glRotatef(-90,0,0,1);
				glutSolidCylinder(size, size*2, 10, 10);

					//Head
					glMaterialfv(GL_FRONT, GL_AMBIENT, skin);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, skin);
					glMaterialfv(GL_FRONT, GL_SPECULAR, skin);
					glMaterialfv(GL_FRONT, GL_SHININESS, skin);
				
					glPushMatrix();
					glTranslatef(0.0, 0.0, size*2);
					glutSolidSphere(size, 10, 10);

						//HatBase
						glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
						glMaterialfv(GL_FRONT, GL_SPECULAR, black);
						glMaterialfv(GL_FRONT, GL_SHININESS, white);
				
						glPushMatrix();
						glTranslatef(0.0, 0.0, size * 0.8);
						glutSolidCone(size*2, size*0.8, 10, 10);

							//HatTop
							glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
							glMaterialfv(GL_FRONT, GL_SPECULAR, black);
							glMaterialfv(GL_FRONT, GL_SHININESS, white);
				
							glPushMatrix();
							//glTranslatef(0.0, 0.0, size);
							glRotatef(30, 1.0, 0.0, 0.0);
							glutSolidCone(size, size*3, 10, 10);

							glPopMatrix();
						glPopMatrix();

						//Left Eye
						glMaterialfv(GL_FRONT, GL_AMBIENT, black);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
						glMaterialfv(GL_FRONT, GL_SPECULAR, black);
						glMaterialfv(GL_FRONT, GL_SHININESS, white);

						glPushMatrix();
						glTranslatef(size*0.3, size*0.7, size*0.5);
						glutSolidSphere(size*0.2, 10, 10);
						glPopMatrix();

						//Right Eye
						glPushMatrix();
						glTranslatef(-size*0.3, size*0.8, size*0.5);
						glutSolidSphere(size*0.2, 10, 10);
						glPopMatrix();

						//Beard
						glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);
						glMaterialfv(GL_FRONT, GL_SPECULAR, grey);
						glMaterialfv(GL_FRONT, GL_SHININESS, grey);

						glPushMatrix();
						glTranslatef(0.0, size*0.6, -size*0.2);
						glRotatef(-150, 1.0, 0.0, 0.0);
						glutSolidCone(size/2, size*2, 10, 10);
						glPopMatrix();

					glPopMatrix();	//Head

					//Arms
					glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
					glMaterialfv(GL_FRONT, GL_SPECULAR, blue);
					glMaterialfv(GL_FRONT, GL_SHININESS, white);

					//right
					glPushMatrix();
					glTranslatef(1.5*size, -0.8*size , 0.0);
					glRotatef(-40, 1.0, 1.0, 0.0);
					glutSolidCylinder(size/2, size*2, 10, 10);
					glPopMatrix();

					//left
					glPushMatrix();
					glTranslatef(-1.5*size, 0.8*size, 0.0);
					glRotatef(40, 1.0, 1.0, 0.0);
					glutSolidCylinder(size/2, size*2, 10, 10);

						//staffBase
						glMaterialfv(GL_FRONT, GL_AMBIENT, wood);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, wood);
						glMaterialfv(GL_FRONT, GL_SPECULAR, wood);
						glMaterialfv(GL_FRONT, GL_SHININESS, black);

						glPushMatrix();
						glTranslatef(0.0, 0.0, -0.5*size);
						glRotatef(-60, 1.0, 1.0, 0.0);
						glutSolidCylinder(size/6, size*4, 10, 10);

							//staffTop
							glMaterialfv(GL_FRONT, GL_AMBIENT, red);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
							glMaterialfv(GL_FRONT, GL_SPECULAR, red);
							glMaterialfv(GL_FRONT, GL_SHININESS, black);

							glPushMatrix();
							glTranslatef(0.0, 0.0, size*4);
							glRotatef(0.0, 1.0, 1.0, 0.0);
							glutSolidSphere(size/3, size*4, 10);
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();

					//Bottom
					glMaterialfv(GL_FRONT, GL_AMBIENT, black);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
					glMaterialfv(GL_FRONT, GL_SPECULAR, blue);
					glMaterialfv(GL_FRONT, GL_SHININESS, black);

					glPushMatrix();
					glTranslatef(0.0, 0.0, -1.0);
					glutSolidCone(size*1.5, size*4, 10, 10);
					glPopMatrix();

				glPopMatrix();
			}
};

/* Platform class - Design for a 6 sided rectangular prism */
class Platform
{
	float origin[3];

	/* Depth, width and height, respectively */
	float d;
	float w;
	float h;

	/* Two toned colour of platform */
	float colour[3];
	float colour2[3];

	/* cube - takes an array of 8 vertices, and draws 6 faces
	 *  with drawPolygon, making up a box
	 */
	void cube(float v[8][3])
	{
		//side
		glColor3f(1.0,1.0,0.8);
		drawPolygon(0, 3, 2, 1, v);

		//side
		drawPolygon(1, 0, 4, 5, v);

		//top
		glColor3f(1.0,1.0,1.0);
		drawPolygon(5, 1, 2, 6, v);

		glColor3f(1.0,1.0,0.8);
		drawPolygon(2, 3, 7, 6, v);
		drawPolygon(6, 5, 4, 7, v);
		drawPolygon(4, 0, 3, 7, v);
	}

	/* Draws a side of the cube */
	void drawPolygon(int a, int b, int c, int d, float v[8][3])
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, colour2);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, colour2);
		glMaterialfv(GL_FRONT, GL_SPECULAR, colour2);
		glMaterialfv(GL_FRONT, GL_SHININESS, colour);

		glNormal3f(0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex3fv(v[a]);
			glVertex3fv(v[b]);
			glVertex3fv(v[c]);
			glVertex3fv(v[d]);
		glEnd();
	}

	public:
		/* Constructor - Initializes a platform with specified by the origin coordinates, depth, width and height, respectively*/
		Platform(float* o, float _d, float _w, float _h)
		{
			d = _d;
			w = _w;
			h = _h;

			for (int i = 0; i < 3; i++)
			{
				origin[i] = o[i];
				colour[i] = 1.0;
			}
			colour2[0] = 1.0; 
			colour2[1] = 1.0;
			colour2[2] = 0.8;
		}

		/* Draws the platform using a center origin */
		void drawPlatform()
		{
			float vertices[8][3] = { {origin[0]-w/2, origin[1]-h/2, origin[2]+d/2},
									 {origin[0]-w/2, origin[1]+h/2, origin[2]+d/2},
									 {origin[0]+w/2, origin[1]+h/2, origin[2]+d/2},
									 {origin[0]+w/2, origin[1]-h/2, origin[2]+d/2}, 
									 {origin[0]-w/2, origin[1]-h/2, origin[2]-d/2}, 
									 {origin[0]-w/2, origin[1]+h/2, origin[2]-d/2}, 
									 {origin[0]+w/2, origin[1]+h/2, origin[2]-d/2},
									 {origin[0]+w/2, origin[1]-h/2, origin[2]-d/2} };

			cube(vertices);
		}

		/* Returns the origin coordinates of the platform */
		float * getOrigin()
		{
			return origin;
		}

		/* Returns the depth of the platform */
		float getDepth()
		{
			return d; 
		}
		
		/* Returns the height of the platform */
		float getHeight()
		{
			return h;
		}

		/* Returns the width of the platform */
		float getWidth()
		{
			return w;
		}

};

vector<Platform> platform;


class Terrain {

	int** heightmap;
	int size;
public:

	Terrain(int terrain_size, int fault_iterations) {
		size = terrain_size;

		heightmap = new int*[size];
		for(int i = 0; i < size; i++) {
			heightmap[i] = new int[size];
		}

		for(int i = 0;i<size;i++) {
			for(int j = 0;j<size;j++) {
				heightmap[i][j] = 0;
			}
		}
		this->build(fault_iterations);
	};

	//faults the terrain once
	void fault() {
		float v = rand();
		float a = sin(v);
		float b = cos(v);
		float d = sqrt(2*size*size);
		// rand() / RAND_MAX gives a random number between 0 and 1.
		// therefore c will be a random number between -d/2 and d/2
		float c = ((double)rand() / RAND_MAX) * d - d/2;

		//int displacement = (((double)rand() / RAND_MAX)>.5)?1:-1;


		for(int i = 0;i<size;i++) {
			for(int j = 0;j<size;j++) {
				if (a*i + b*j - c > 0) {
					(heightmap[i][j]) += 1;
				}
				else {
					(heightmap[i][j]) -= 1;
				}
			}
		}
		
	}

	//builds a terrain by fualting it several times
	void build(int iterations) {
		if (iterations > 0) {
			for (int i = 1; i<=iterations;i++) {
				this->fault();
			}

			//for debugging, print array
			for(int i = 0;i<size;i++) {
				for(int j = 0;j<size;j++) {
					cout << heightmap[i][j];
					cout << ", ";
				}
				cout<< '\n';
			}
		}
	}

	int ** getHeight()
	{
		return heightmap;
	}

	int getSize()
	{
		return size;
	}

	int getX(float _x)
	{
		
	}


	//displays the terrain
	void display() {
		for(int i = 0;i<size-1;i++) {
			for(int j = 0;j<size-1;j++) {
				glBegin(GL_POLYGON);
					//if ((heightmap[i][j])>0)
					//{
						glColor3ub(0,255,0);
					//}
					//else
					//{
						//glColor3ub(0,0,(-heightmap[i][j]));
					//}

					glVertex3f(i-5, heightmap[i][j]-15, j-5);
					glVertex3f(i+1-5, heightmap[i+1][j]-15, j-5);
					glVertex3f(i+1-5, heightmap[i+1][j+1]-15, j+1-5);
					glVertex3f(i-5, heightmap[i][j+1]-15, j+1-5);
				glEnd();
			}
		}
	}

};

Terrain *terrain;

/* Set the global origin of the simulation */

void printInstructions()
{
	cout << "Particle Simulator Commands\n";
	cout << "___________________________\n";
	cout << "Pause:                               'P'\n";
	cout << "Rotate Horizontally:                 LEFT/RIGHT ARROW KEY\n";
	cout << "Rotate Vertically:                   UP/DOWN ARROW KEY\n";
	cout << "Toggle the population of particles:  SPACE BAR\n";
	cout << "Toggle friction:                     'F' \n";
	cout << "Toggle Particle Camera Mode:         'K'\n";
}
void setOrigin(float x, float y, float z)
{
	gOrigin[0] = x;
	gOrigin[1] = y;
	gOrigin[2] = z;
}

/* Set the global angle theta */
void setTheta(float x, float y, float z)
{
	gTheta[0] = x;
	gTheta[1] = y;
	gTheta[2] = z;
}

/* Set the global position of the Camera */
void setCamPosition(float x, float y, float z)
{
	gCamPos[0] = x;
	gCamPos[1] = y;
	gCamPos[2] = z;
}

/* Create a particle at particular origin coordinates */

/* Creates a platform at the particular origin coordinates with a specified width and heigth, respectively */
Platform createPlatform(float* o, float d, float w, float h)
{
	float pOrigin[3];
	for (int i = 0; i < 3; i++)
	{
		pOrigin[i] = o[i];
	}

	Platform p(pOrigin, d, w, h);
	return p;
}

Player createPlayer(float* o, float m)
{
	float pOrigin[3];
	for (int i = 0; i < 3; i++)
	{
		pOrigin[i] = o[i];
	}

	Player p(pOrigin, m);
	return p;
}

Player player1 = createPlayer(gOrigin, 0.5);

/* Initializes all the variables for the Particle Simulation */
void init()
{
	/* Disable Mouse */
	glutSetCursor(GLUT_CURSOR_NONE);

	/* Initialze physics */
	gravity = 0.001f;
	friction = 0.9;

	/* Initialize global origin, theta and camera position */
	setOrigin(0.0, 0.0, 0.0);
	setTheta(1.0, 1.0, 1.0);
	setCamPosition(15.0, 15.0, 15.0);
	cameraSpeed = 0.08;

	/* Enable lighting and create a light0 with ambience, diffusion and specularity */
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	
	/* Initialize OpenGL colour, Depth Test, Projection mode */
	glColor3f(1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);

	/* Set a perspective */
	gluPerspective(70, 1, 1, 100);

	/* Print Instructions */
	printInstructions();

	/* Create a platform */
	platform.push_back(createPlatform(gOrigin, 15, 15, 1));
	terrain = new Terrain(50,100);
}

/* Takes user's keyboard input when they release a key */
void keyboardUp(unsigned char key, int x, int y)
{
	/* Character Movement Release*/
	if (key == 'w' || key == 'W')
	{
		player1.cancelMovement(1);
	}
	if (key == 's' || key == 'S')
	{
		player1.cancelMovement(0);
	}

	if (key == 'd' || key == 'D')
	{
		player1.cancelMovement(3);
	}
	if (key == 'a' || key == 'A')
	{
		player1.cancelMovement(2);
	}
}
void keyboard(unsigned char key, int x, int y)
{
	/* Enable/Disable global fricton */
	if (key == 'f' || key == 'F')
	{
		if (friction == 1.0)
		{
			friction = 0.9;
		}
		else
		{
			friction = 1.0;
		}		
	}

	/* Quit the simulation */
	if (key =='q' || key == 'Q')
	{
		exit(1);
	}

	/* Character Movement */
	if (key == 'w' || key == 'W')
	{
		player1.setMovement(1);
	}
	if (key == 's' || key == 'S')
	{
		player1.setMovement(0);
	}

	if (key == 'd' || key == 'D')
	{
		player1.setMovement(3);
	}
	if (key == 'a' || key == 'A')
	{
		player1.setMovement(2);
	}

}

void passiveMouse(int x, int y)
{
	/* Rotate camera angle left if mouse moves left */
	if (x - currentMouse[0] < 0)
	{
		gTheta[2] -= cameraSpeed;
		gTheta[0] -= cameraSpeed;
	}

	/* Rotate camera angle Right if mouse moves right */
	if (x - currentMouse[0] > 0)
	{
		gTheta[2] += cameraSpeed;
		gTheta[0] += cameraSpeed;
	}
	
	/* Rotate camera Down if mouse moves down */
	if (y - currentMouse[1] < 0 && gTheta[1] > -0.5)
	{
		gTheta[1] -= cameraSpeed;
	}

	/* Rotate camera Up if mouse moves up */
	if (y - currentMouse[1] > 0 && gTheta[1] < 1.0)
	{
		gTheta[1] += cameraSpeed;
	}

	/* Set current mouse to current position */
	currentMouse[0] = x;
	currentMouse[1] = y;
											  
	/* Move cursor back to position if it goes out of screen */
	if (currentMouse[0] > glutGet(GLUT_WINDOW_WIDTH) - 400)
	{
		glutWarpPointer(200, currentMouse[1]);
		currentMouse[0]--;
	}
	if (currentMouse[0] < 200)
	{
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) - 400, currentMouse[1]);
		currentMouse[0]++;
	}

	if (currentMouse[1] > glutGet(GLUT_WINDOW_HEIGHT) - 200)
	{
		glutWarpPointer(currentMouse[0], 100);
		currentMouse[1]--;
	}

	if (currentMouse[1] < 100)
	{
		glutWarpPointer(currentMouse[0], glutGet(GLUT_WINDOW_HEIGHT) - 200);
		currentMouse[1]++;
	}
}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	gCamPos[0] = player1.getPos()[0] - 10*sin(gTheta[0]);
	gCamPos[1] = player1.getPos()[1] + 10*sin(gTheta[1]);
	gCamPos[2] = player1.getPos()[2] - 10*-cos(gTheta[2]);
										
	/* Adjust camera position and center based off of trigonometric rotation */
	gluLookAt(gCamPos[0], 
		gCamPos[1], 
		gCamPos[2],
		player1.getPos()[0],
		player1.getPos()[1],
		player1.getPos()[2],
		0,1,0);

	glColor3f(0.2,0.2,0.2);


	//platform.front().drawPlatform();
	terrain->display();

	glColor3f(0.0, 0.0, 1.0);

	/* Draw Player1 onto the screen */
	player1.draw();
	player1.move();

	/* Swap front buffer with back buffer */
	glutSwapBuffers();
}

void checkCollision(Terrain *t, Player *p)
{
	float pX = p->getX();
	float pY = p->getY();
	float pZ = p->getZ();
	float pSize = p->getSize();

	for (int i = 0; i < t->getSize(); i++)
	{
		for (int j = 0; j < t->getSize(); j++)
		{
			float tX = t->getSize();
			float tZ = t->getSize();

			if ((pX >= i - 4 && pX < i - 6) && 
				(pZ >= j - 4 && pZ < j - 6) ||
				pY <= t->getHeight()[i][j]-15)
			{
				p->collidesVertically(t->getHeight()[i][j]-15);
			}
		}
	}
}

/* Idle call back function which is run everytime nothing else is called back */
void idle()
{
	checkCollision(terrain, &player1);

	/* Call back display function */
	glutPostRedisplay();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Particle System");	//creates the window

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);

	glutPassiveMotionFunc(passiveMouse);
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutIdleFunc(idle);

	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}