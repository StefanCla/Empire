#pragma once
//Standard
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <string>
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//Define
#define SCREENWIDTH 1024
#define SCREENHEIGHT 768
#define PI 3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798f 
#define PLANETNUMBER 8192 //Made it a define for debugging 8192
#define SHIPNUM_SV 4 //Num of SV ships of 1 empire
#define SHIPNUM_DV 4 //Num of DV ships of 1 empire
#define SHIPNUM_CV 20 //Num of CV ships of 1 empire
#define SHIP_SV_COST 5000
#define SHIP_DV_COST 10000
#define SHIP_CV_COST 20000
#define PLANET_BASE_CREDIT_GEN 25
#define PLANET_MEMBERSHIP_FEE_WEIGHT 0.1f
#define PLANET_CREDITS_TECH_LEVEL_WEIGHT 0.25f
#define PLANET_BASE_SOI_RADIUS 3
#define PLANET_SOI_TECH_LEVEL_WEIGHT 0.25f
#define PLANET_TECH_LEVEL_INCREASE_COST 250
#define MAXMOVEMENT 10.0f //Max distance the ships will be able to move each turn
#define BULLET_DEBUG_DRAWING false

//Linux
#ifdef LINUX
#define SPHERE_SECTORS 12
#define SPHERE_STACKS 6
#define CIRCLE_TRIANGLES 16
#define IMGUI_IMPL_OPENGL_ES3
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define TRUE 1
#define FALSE 0
//Might remove these since we dont need them everywhere
#include <GLES3/gl3.h>
#elif WINDOWS
#define SPHERE_SECTORS 16
#define SPHERE_STACKS 8
#define CIRCLE_TRIANGLES 32
#define IMGUI_IMPL_OPENGL_4
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#define TRUE 1
#define FALSE 0
#include <glad/glad.h>
#endif