#define TINYOBJLOADER_IMPLEMENTATION
#include "Headers/Models/Model.h"
#include "Headers/Renderer/VertexArray.h"
#include "Headers/Renderer/VertexBuffer.h"
#include "Headers/Renderer/IndexBuffer.h"
#include "Headers/Renderer/Shader.h"
#include "Headers/Textures/Texture.h"
#include "Headers/tiny_obj_loader.h"
#include <iostream>

Model::Model(const std::string& filePath)
{
	LoadFromOBJ(filePath);

	if (glGetError() != GL_NO_ERROR)
		printf("Failed to initialize model from file\n");
}

Model::Model(std::vector<float> vertices, std::vector<unsigned int> indices)
{
	m_ModelData.Vertices = vertices;
	m_ModelData.Indices = indices;

	if (glGetError() != GL_NO_ERROR)
		printf("Failed to initialize model from model data\n");
}

Model::~Model()
{
}

void Model::Bind()
{
	m_Shader->Bind();
	m_VAO->Bind();
	
	for (const std::shared_ptr<Texture>& texture : m_Textures)
		texture->Bind();
}

void Model::Unbind()
{
	m_Shader->Unbind();
	m_VAO->Unbind();

	for (const std::shared_ptr<Texture>& texture : m_Textures)
		texture->Unbind();
}

void Model::AddTexture(std::shared_ptr<Texture> texture)
{
	m_Textures.push_back(texture);
}

void Model::LoadFromOBJ(const std::string& filePath)
{
	//Example based on: https://github.com/tinyobjloader/tinyobjloader

	tinyobj::ObjReaderConfig readerConfig;
#ifdef LINUX
	readerConfig.mtl_search_path = "../../../Empire/Resources/Models/"; //Path to material files
#elif WINDOWS
	readerConfig.mtl_search_path = "../Empire/Resources/Models/"; //Path to material files
#endif

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(filePath, readerConfig))
	{
		if (!reader.Error().empty())
			std::cerr << "TinyObjReader: " << reader.Error();
		exit(1);
	}

	if (!reader.Warning().empty())
		std::cout << "TinyObjReader: " << reader.Warning();

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	//Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			for (size_t v = 0; v < fv; v++)
			{
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
				m_ModelData.Vertices.push_back(vx);
				m_ModelData.Vertices.push_back(vy);
				m_ModelData.Vertices.push_back(vz);

				if (idx.normal_index >= 0)
				{
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
					m_ModelData.Vertices.push_back(nx);
					m_ModelData.Vertices.push_back(ny);
					m_ModelData.Vertices.push_back(nz);
				}

				if (idx.texcoord_index >= 0)
				{
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
					m_ModelData.Vertices.push_back(tx);
					m_ModelData.Vertices.push_back(ty);
				}

				m_ModelData.Indices.push_back(index_offset + v);
			}

			index_offset += fv;
			shapes[s].mesh.material_ids[f];
		}
	}
}

void Model::InitGLBuffers()
{
	m_Shader->Bind();
	m_VAO = std::make_shared<VertexArray>();
	m_VAO->Bind();

	m_VBO = std::make_shared<VertexBuffer>(&m_ModelData.Vertices[0], m_ModelData.Vertices.size() * sizeof(float));
	m_VBO->SetLayout({
		{ ShaderDataType::Float3, "in_position" },
		{ ShaderDataType::Float3, "in_normal" },
		{ ShaderDataType::Float2, "in_texCoord" }
	});

	m_IBO = std::make_shared<IndexBuffer>(&m_ModelData.Indices[0], m_ModelData.Indices.size());

	m_VAO->AddVertexBuffer(VertexBufferType::ModelVB, m_Shader->GetRendererID(), m_VBO);
	m_VAO->SetIndexBuffer(m_IBO);
}
