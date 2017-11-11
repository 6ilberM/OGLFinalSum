#ifndef LIGHT_H
#define LIGHT_H
#pragma once
#include <iostream>
#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"
#include <iostream>

class CLight
{
public:
	CLight(Camera* camera, GLuint program);
	~CLight();

private:

};






#endif // !LIGHT_H
