#pragma once

#define SPRITE_SIZE 1000

typedef struct sprite
{
	int Xposition;
	int Yposition;
	int position[SPRITE_SIZE][2];
	char printchar[SPRITE_SIZE];

}sprite;

//initialise for skullenemy
void Text_Init(sprite* s, char* path);

//creates a skullenemy
sprite Text_CreateSprite();

//Renders enemy
void Text_RenderEnemy(sprite *s);

// Set enemy position
void Text_Moveenemy(sprite *s, int x, int y);

//Read from text file and store in array
void _Readandstoretext(sprite *s, const char *path);

