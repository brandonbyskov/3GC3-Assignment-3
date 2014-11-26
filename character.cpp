																										   /*	Particle Simulation
*	Assignment 2 - CS 3GC3 McMaster University
*	By Eric Amshukov - 1133146
*	November Friday 7th, 2014
*
*	Description:
*		This program populates particle objects from midair which are subsequently bounded by frictional 
*	and gravitational laws of physics. Incidently, it demonstrates the natural consequences of colliding objects.
*	In addition, OpenGL lighting is utilised to illuminate the perception of depth in the objects.
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <GL/glew.h>
#include <freeglut.h>
//#include <GL/GLU.h>
//#include <GL/wglew.h>

using namespace std;

/* Simulation States - paused, is populating particles, camera is on particle */
bool gPaused;

/* Movement states */
bool movingForward, movingBackward, movingLeft, movingRight; 

/* Total particle count, Camera degree count */

/* Global angle theta */
float gTheta[3];

float currentMouse[] = {0, 0, 0};

/* Global origin */
float gOrigin[3];

/* Recent Particle Position/Direction */

/* Global Camera Position */
float gCamPos[3];

/* Global physics */
float gravity;
float friction;

/* Light values */
float qaAmbientLight[] = {0.2, 0.2, 0.2, 1.0};
float qaDiffuseLight[] = {0.8, 0.8, 0.8, 1.0};
float qaSpecularLight[] = {1.0, 1.0, 1.0, 1.0};

float lightPosition[] = {0.5, 0.5, 0.0, 1.0};


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
		float col[3];
		float pos[3];
		float dir[3];
		float rot[3];
		float speed[3];
		float size;

		public:
			Player(float* _o, float _size)
			{
				size = _size;

				
				for (int i = 0; i < 3; i++)
				{
					pos[i] = _o[i];
					rot[i] = 1.0;
					dir[i] = 1.0;
					speed[i] = 0.0;
				}
				col[0] = 1.0; 
				col[1] = 1.0;
				col[2] = 0.0;
			}

			void changeDirection()
			{

			}

			void move()
			{
				if (movingForward)
				{
					speed[0] = 0.1;
	   				pos[0] += 1.0 * speed[0];
				}

				if (movingBackward)
				{
					speed[0] = -0.1;
	   				pos[0] += 1.0 * speed[0];
				}

				if (movingRight)
				{
					speed[2] = 0.1;
	   				pos[2] += 1.0 * speed[2];
				}

				if (movingLeft)
				{
					speed[2] = -0.1;
	   				pos[2] += 1.0 * speed[2];
				}

				if (!movingBackward && !movingForward)
				{
					pos[0] += speed[0];
					if (speed[0] < -0.0001 || speed[0] > 0.0001)
					{
						speed[0] *= 0.9;
					}
					else
					{
						speed[0] = 0;
					}
				}
				if (!movingRight && !movingLeft)
				{
					pos[2] += speed[2];
					if (speed[2] < -0.0001 || speed[2] > 0.0001)
					{
						speed[2] *= 0.9;
					}
					else
					{
						speed[2] = 0;
					}
				}
			}

			float * getPos()
			{
				return pos;
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

/* Particle class - Design for a particle which initially rotates and moves arbitrarily */
class Particle
{
	/* Particle states */
	bool hasBounced;		
	bool isFalling;

	/* Age of particle (Incremented with each step of the animation) */
	int age;
	float rSpeed;

	/* Physics characteristics */
	float pos[3];	 //Position
	float dir[3];	 //Direction
	float rot[3];	 //Rotation
	float normal[3]; //Normal vector

	/* Random values generated for the direction, rotation and speed */
	float dRandFactor[3];
	float rRandFactor[3];

	float colour[3];		//Colour of the particle
	float oSpeed[3];		//Original particle speed
	float speed[3];			//Particle speed

 	float white[3];			//Colour white


	
	/* Sets the Normal vector of two coplanar vectors */
	float * getNormal(float* v1[3], float* v2[3])
	{
		/*Cross product*/
		normal[0] = (*v1[1]) * (*v2[2]) - (*v1[2]) * (*v2[1]);
		normal[1] = (*v1[2]) * (*v2[0]) - (*v1[0]) * (*v2[2]);
		normal[1] = (*v1[0]) * (*v2[1]) - (*v1[1]) * (*v2[0]);

		/*Magnitude of the vector*/
		float m = sqrt(normal[0]*normal[0] +
					   normal[1]*normal[1] + 
					   normal[2]*normal[2]);

		/*Normalize the normal vector by dividing each component by magnitude*/
		for (int i = 0; i < 3; i++)
		{
			normal[i] /= m;
		}
		
		return normal;
	}

	public:
		/* Constructor - Initializes a Particle with float position coordinates */
		Particle(float* _pos)
		{
			hasBounced = false;
			isFalling = true;
			age = 0;	//Initialize the particle's life

			rSpeed = 1;
		
			dRandFactor[0] = (float)((rand() % 100)/3);
			dRandFactor[1] = (float)((rand() % 100));
			dRandFactor[2] = (float)((rand() % 100)/3);

			for (int i = 0; i < 3; i++)
			{
				rRandFactor[i] = (float)((rand() % 360));
				speed[i] = 0.001f;		   //The higher the speed, the faster
				oSpeed[i] = speed[i];
				pos[i] = _pos[i];		
				rot[i] = rRandFactor[i];
				colour[i] = (float)(rand() % 100)/100;			//randomize particle colour
				white[i] = 1.0;
			}

			dir[0] = 1.0f + dRandFactor[0];
			dir[1] = -1.0f - dRandFactor[1];
			dir[2] = 1.0f + dRandFactor[2];
		}

		/* Increments the age of the particle by one animation step */
		void incrementAge()
		{
			age++;
		}

		void setBounceState(bool q)
		{
			hasBounced = q;
		}

		/* Modifies the direction coordinates of a particle */
		void modDirection(float x, float y, float z)
		{
			dir[0] = x;
			dir[1] = y;
			dir[2] = z;
		}

		/* Modifies the position of a particle */
		void modPosition()
		{
			/* Accelerate by a factor of gravity */
			if (isFalling)
			{
				speed[1] *= gravity*0.95;
			}

			/* Decelerate by a factor of gravity */
			else
			{
				/* Stop decelerating once the speed is below an extremely low amount */
				if (speed[1] > 0.0005)
				{
					speed[1] /= gravity;
				}

				/* Change the direction once the particle has reached its max height */
				else
				{
					dir[1] = -dir[1];
					isFalling = true;
				}
			}

			/* If a particle bounces off the ground, flip 'y' direction */
			if (hasBounced)
			{
				dir[1] = -dir[1];
				hasBounced = false;
				isFalling = false;
			
				/* Reduce the speed of a particle when it bounces by a factor of 'friction' */
				for (int i = 0; i < 3; i++)
				{
					speed[i] *= friction;
				}
			}

			/* Finally, after modifying the direction and speed accordingly,
				set the new position coordinates */
			pos[0] += dir[0] * speed[0];
			pos[1] += dir[1] * speed[1];
			pos[2] += dir[2] * speed[2];

		}

		/* Increments the angle of a particle */
		void rotate()
		{
			for (int i = 0; i < 3; i++)
			{
				
				/* Reset angle back to 0 after a full cycle */
				if (rot[i] >= 360)
				{
					rot[i] = 0;
				}

				/* Reduce rotation speed by a factor of friction */
				rot[i] += rSpeed * friction;
			}
		}


		int getAge()
		{
			return age;
		}

		/* Returns whether the particle is falling or not */
		bool getFallingState()
		{
			return isFalling;
		}
		
		/* Returns whether the particle has bounced or not */
		bool getBounceState()
		{
			return hasBounced;
		}

		/* Returns the position coordinates of a particle */
		float * getPosition()
		{
			return pos;
		}
		
		float * getDirection()
		{
			return dir;
		}

		/* Draws a particle in a hierarchical fashion */
		void draw()
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, colour);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);
			glMaterialfv(GL_FRONT, GL_SPECULAR, colour);
			glMaterialfv(GL_FRONT, GL_SHININESS, white);
		
			glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2]);
			glRotatef(rot[0], rot[0], rot[1], rot[2]);
			glColor3fv(colour);
			glutSolidCube(0.5);
			glPopMatrix();
		}
};

/*Create a particle and platform vector list, respectively */
vector<Particle> particle;
vector<Platform> platform;



/* Set the global origin of the simulation */

void printInstructions()
{
	printf("Particle Simulator Commands\n");
	printf("___________________________\n");
	printf("Pause:                               'P'\n");
	printf("Rotate Horizontally:                 LEFT/RIGHT ARROW KEY\n");
	printf("Rotate Vertically:                   UP/DOWN ARROW KEY\n");
	printf("Toggle the population of particles:  SPACE BAR\n");
	printf("Toggle friction:                     'F' \n");
	printf("Toggle Particle Camera Mode:         'K'\n");
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
	/* Initialize states */
	gPaused = false;
	movingForward = false;
	movingBackward = false;
	movingLeft = false;
	movingRight = false; 

	
	/* Initialize total particle counter */

	/* Initialize max particle life */

	/* Initialze physics */
	gravity = 1.28f;
	friction = 1.0;

	/* Initialize random generator function */
	srand (time(NULL));

	/* Initialize global origin, theta and camera position */
	setOrigin(0.0, 0.0, 0.0);
	setTheta(1.0, 1.0, 1.0);
	setCamPosition(15.0, 15.0, 15.0);

	/* Enable lighting and create a light0 with ambience, diffusion and specularity */
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
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
}

/* Checks whether a particle has collided with a platform */

/* Takes user's special key input */
void specialKeyboard(int key, int x, int y)
{
	/* arrow key presses move the camera */
	switch(key)
	{
		/* Rotate left, right, up and down, respectively */
		case GLUT_KEY_LEFT:
			gTheta[0] -= 0.1;
			gTheta[2] -= 0.1;
			break;

		case GLUT_KEY_RIGHT:
			gTheta[0] += 0.1;
			gTheta[2] += 0.1;
			break;

		case GLUT_KEY_UP:
			gTheta[1] += 0.1;
			//gTheta[2] += 0.1;
			break;

		case GLUT_KEY_DOWN:
			gTheta[1] -= 0.1;
			break;
	}
}

/* Takes user's keyboard input */

void keyboardUp(unsigned char key, int x, int y)
{
	/* Character Movement Release*/
	if (key == 'w' || key == 'W')
	{
		movingForward = false;
	}
	if (key == 's' || key == 'S')
	{
		movingBackward = false;
	}

	if (key == 'd' || key == 'D')
	{
		movingRight = false;
	}
	if (key == 'a' || key == 'A')
	{
		movingLeft = false;
	}
}
void keyboard(unsigned char key, int x, int y)
{
	/* Enable/Disable global fricton */
	if (key == 'f' || key == 'F')
	{
		if (friction == 1.0)
		{
			friction = 0.7;
		}
		else
		{
			friction = 1.0;
		}		
	}

	/* Freezes the simulation */
	if (key == 'p' || key == 'P')
	{
		gPaused = !gPaused;
	}

	/* Character Movement */
	if (key == 'w' || key == 'W')
	{
		movingBackward = false;
		movingForward = true;
	}
	if (key == 's' || key == 'S')
	{
		movingBackward = true;
		movingForward = false;
	}

	if (key == 'd' || key == 'D')
	{
		movingLeft = false;
		movingRight = true;
	}
	if (key == 'a' || key == 'A')
	{
		movingLeft = true;
		movingRight = false;
	}

}

void passiveMouse(int x, int y)
{
	if (x - currentMouse[0] < 0)
	{
		
		gTheta[2] -= 0.1;
	}
	if (x - currentMouse[0] > 0)
	{
		
		gTheta[2] += 0.1;
	}

	if (y - currentMouse[1] < 0)
	{
		gTheta[1] -= 0.1;
	}
	if (y - currentMouse[1] > 0)
	{
		gTheta[1] += 0.1;
	}

	currentMouse[0] = x;
	currentMouse[1] = y;
}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	gCamPos[0] = player1.getPos()[0] - 10;
	gCamPos[1] = player1.getPos()[1] + 10;
	gCamPos[2] = player1.getPos()[2] - 2;

	/* Adjust camera position and center based off of trigonometric rotation */
	gluLookAt(gCamPos[0]*sin(gTheta[0]), 
		gCamPos[1]*cos(gTheta[1]), 
		gCamPos[2]*-cos(gTheta[2]),
		player1.getPos()[0],
		player1.getPos()[1],
		player1.getPos()[2],
		0,1,0);

	glColor3f(0.2,0.2,0.2);


	platform.front().drawPlatform();

	glColor3f(0.0, 0.0, 1.0);

	player1.draw();
	player1.move();


	
	/* Swap front buffer with back buffer */
	glutSwapBuffers();
}

/* Idle call back function which is run everytime nothing else is called back */
void idle()
{
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
	glutSpecialFunc(specialKeyboard);

	glutPassiveMotionFunc(passiveMouse);
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutIdleFunc(idle);

	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}