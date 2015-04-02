#ifndef COMBINED_H
#define COMBINED_H


#include "Board.h"
#include <time.h>
#include "AI.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <math.h>
#include <cstdio>
#include <cstring>
#include <pthread.h>

#define NUM_PARTICLES    1000          /* Number of particles  */
#define NUM_DEBRIS       70            /* Number of debris     */

struct Faces
{
	Points p1,p2,p3;
}
;

struct particleData
{
  float   position[3];
  float   speed[3];
  float   color[3];
};

struct debrisData
{
  float   position[3];
  float   speed[3];
  float   orientation[3];        /* Rotation angles around x, y, and z axes */
  float   orientationSpeed[3];
  float   color[3];
  float   scale[3];
};

struct Expl
{
	particleData 	particles[NUM_PARTICLES];
	debrisData 		debris[NUM_DEBRIS];   
	int         	fuel = 0;                /* "fuel" of the explosion */
	float 			angle = 0.0;             /* camera rotation angle */
};


struct GamePlay
{
	Board PlayerBoard;
	int PlayerId;
	clock_t LastBulletTime;
};

std::vector<Faces> alien;
std::vector<Faces> ship;
std::vector<Faces> missile;
std::vector<Faces> bullet;
std::vector<Points> Stars;
std::vector<Expl> Explosions;

GamePlay newg;

bool Keys[9];

std::vector<Faces> loadOBJ(char * path);

void ProcessKeys();
void specialKeys( int key, int x, int y );
void specialKeysUp( int key, int x, int y ) ;
void handleKeypress(unsigned char key, int x, int y);
void handleKeypressUp(unsigned char key, int x, int y);
Expl newExplosion (float x, float y, float z);

#endif