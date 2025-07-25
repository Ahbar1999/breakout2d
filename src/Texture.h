#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D
{
public:
	unsigned int id;
	unsigned int Height, Width;	
	
	unsigned int Internal_Format; //format of texture object
	unsigned int Image_Format; //format of image object

	//Wrapping modes
	unsigned int Wrap_S;
	unsigned int Wrap_T;
	//Filtering modes
	unsigned int Filter_Min;
	unsigned int Filter_Max;

	//Constructor 
	Texture2D();

	//Generates a 2D texture from an image 
	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	//Binds the texture object
	void Bind() const;
};


#endif