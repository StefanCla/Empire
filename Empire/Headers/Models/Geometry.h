#pragma once
#include "Headers/Precomp.h"
#include "Headers/Models/Model.h"
#include <iostream>

enum class GeometryType
{
	Cube, Point, Sphere, Circle
};

class Geometry
{
public:
	static ModelData GetModel(GeometryType type);

private:
	static ModelData GetCubeModel();
	static ModelData GetPointModel();
	static ModelData GetSphereModel(float radius, unsigned int sectors, unsigned int stacks);
	static ModelData GetCircleModel(unsigned int sides);
};
