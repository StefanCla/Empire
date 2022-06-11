#include "Headers/Models/Geometry.h"

ModelData Geometry::GetModel(GeometryType type)
{
	switch (type)
	{
	case GeometryType::Cube:
		return GetCubeModel();
		break;
	case GeometryType::Point:
		return GetPointModel();
		break;
	case GeometryType::Sphere:
		return GetSphereModel(1.0f, SPHERE_SECTORS, SPHERE_STACKS);
		break;
	case GeometryType::Circle:
		return GetCircleModel(CIRCLE_TRIANGLES);
		break;
	default:
		assert(false && "Invalid Geometry type provided in GetModel() of Geometry\n");
	}

	return GetCubeModel();
}

std::vector<float> CubeVertices =
{
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.33f, 0.5f,
	0.5f, 0.5f,	-0.5f, 0.0f, 0.0f, -1.0f, 0.33f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

	-0.5f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f, 0.33f, 0.5f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.67f, 0.5f,
	0.5f, 0.5f,	0.5f, 0.0f, 0.0f, 1.0f, 0.67f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.33f, 1.0f,

	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f,
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.67f, 1.0f,
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.67f, 0.5f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.33f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.33f, 0.5f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.33f, 0.5f,
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.67f, 0.5f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.67f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.33f, 0.0f,

	-0.5f, 0.5f, -0.5f,	0.0f, 1.0f, 0.0f, 0.67f, 0.5f,
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,
	0.5f, 0.5f,	0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.67f, 0.0f
};

std::vector<unsigned int> CubeIndices = {
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	10, 11, 8,

	12, 13, 14,
	14, 15, 12,

	16, 17, 18,
	18, 19, 16,

	20, 21, 22,
	22, 23, 20
};

ModelData Geometry::GetCubeModel()
{
	ModelData data;
	data.Vertices = CubeVertices;
	data.Indices = CubeIndices;

	return data;
}

ModelData Geometry::GetPointModel()
{
	ModelData data;
	data.Vertices = std::vector<float>({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
	data.Indices = std::vector<unsigned int>({ 0 });

	return data;
}

ModelData Geometry::GetSphereModel(float radius, unsigned int sectors, unsigned int stacks)
{
	ModelData data;

	float x = 0.0f, y = 0.0f, z = 0.0f, xy = 0.0f;
	float nx = 0.0f, ny = 0.0f, nz = 0.0f, lengthInv = 1.0f / radius;
	float u = 0.0f, v = 0.0f;

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / stacks;
	float sectorAngle = 0.0f, stackAngle = 0.0f;

	for (unsigned int i = 0; i <= stacks; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);

		for (unsigned int j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;
			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);

			data.Vertices.push_back(x);
			data.Vertices.push_back(y);
			data.Vertices.push_back(z);
			
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			data.Vertices.push_back(nx);
			data.Vertices.push_back(ny);
			data.Vertices.push_back(nz);
			
			u = (float)j / sectors;
			v = (float)i / stacks;

			data.Vertices.push_back(u);
			data.Vertices.push_back(v);
		}
	}

	int k1 = 0, k2 = 0;

	for (unsigned int i = 0; i < stacks; ++i)
	{
		k1 = i * (sectors + 1);
		k2 = k1 + sectors + 1;

		for (unsigned int j = 0; j < sectors; ++j)
		{
			if (i != 0)
			{
				data.Indices.push_back(k1);
				data.Indices.push_back(k2);
				data.Indices.push_back(k1 + 1);
			}
			if (i != (stacks - 1))
			{
				data.Indices.push_back(k1 + 1);
				data.Indices.push_back(k2);
				data.Indices.push_back(k2 + 1);
			}

			++k1;
			++k2;
		}
	}

	return data;
}

ModelData Geometry::GetCircleModel(unsigned int sides)
{
	ModelData data;
	unsigned int vertexCount = sides + 2;

	for (int i = 0; i < 8; i++)
		data.Vertices.push_back(0.0f);

	for (int i = 0; i < vertexCount; i++)
	{
		data.Vertices.push_back(cos(i * (2.0f * PI) / sides));
		data.Vertices.push_back(sin(i * (2.0f * PI) / sides));
		data.Vertices.push_back(0.0f);

		for (int j = 0; j < 5; j++)
			data.Vertices.push_back(0.0f);
	}

	data.Indices = std::vector<unsigned int>({ 0 });
	return data;
}
