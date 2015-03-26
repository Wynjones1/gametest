#pragma once
#include "mesh.h"
#include "linemesh.h"

template<uint8_t MAXDEPTH, uint8_t MINFACES>
class KDNode
{
public:
	bool leaf;
	uint8_t axis;
	LineCube *cube;
	union
	{
		struct
		{
			KDNode<MAXDEPTH, MINFACES> *left;
			KDNode<MAXDEPTH, MINFACES> *right;
		};
		std::vector<glm::u16vec3> *faces;
	};

	KDNode (std::vector<glm::vec3>    &vertices,
			std::vector<glm::u16vec3> &faces_in,
			glm::vec3 &min, glm::vec3 &max, uint8_t last_axis = 0, uint8_t depth = 0)
			: leaf(false)
			, left(NULL)
			, right(NULL)
			, axis((last_axis + 1) % 3)
			, cube(NULL)
	{
		cube = new LineCube(min, max);
		std::vector<glm::u16vec3> left_faces, right_faces;
		auto middle = (min[axis] + max[axis]) / 2;
		for(auto f : faces_in)
		{
			for(auto i = 0; i < 3; i++)
			{
				auto v = vertices[f[i]][axis];
				bool found = false;

				if(min[axis] <= v && v < middle)
				{
					left_faces.push_back(f);
					found = true;
				}
				if(middle <= v && v < max[axis])
				{
					right_faces.push_back(f);
					found = true;
				}
				if(found) break;
			}
		}
		if( depth == MAXDEPTH                     ||
			faces_in.size() < MINFACES            ||
			(left_faces.size()  == faces_in.size() && right_faces.size() == faces_in.size()))
		{
			leaf = true;
			faces = new std::vector<glm::u16vec3>(faces_in);
		}
		else
		{
			auto left_max   = max;
			auto right_min  = min;
			left_max[axis]  = middle;
			right_min[axis] = middle;
			left  = new KDNode<MAXDEPTH, MINFACES>(vertices, left_faces, min, left_max, axis, depth + 1);
			right = new KDNode<MAXDEPTH, MINFACES>(vertices, right_faces, right_min, max, axis, depth + 1);
		}
	}

	void Draw(Program &prog)
	{
		if(leaf)
		{
			cube->Draw(prog);
		}
		else
		{
			left->Draw(prog);
			right->Draw(prog);
		}
	}
};

template<uint8_t MAXDEPTH = 10, uint8_t MINFACES = 4>
class KDMesh : public Mesh
{
public:
	KDMesh(const char *filename, const char *texture_name)
		: Mesh(filename, texture_name)
	{
		glm::vec3 min = glm::vec3(std::numeric_limits<float>().max());
		glm::vec3 max = glm::vec3(std::numeric_limits<float>().lowest());
		for(auto &v : vertices)
		{
			for(auto i = 0; i < 3; i++)
			{
				min[i] = std::min(min[i], v[i]);
				max[i] = std::max(max[i], v[i]);
			}
		}

		tree = new KDNode<MAXDEPTH, MINFACES>(vertices, faces, min, max);
	}

	void Draw(Program &program)
	{
		Mesh::Draw(program);
	}

	KDNode<MAXDEPTH, MINFACES> *tree;
};

