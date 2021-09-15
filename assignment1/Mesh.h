#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <map>
#include <functional>
#include <algorithm>
#include <cmath> 

using namespace std;

struct Color
{
	float r, g, b;

	Color(float r, float g, float b) : r(r), g(g), b(b) {};
};

struct Vertex
{
	float* coords, * normals; //3d coordinates etc
	int idx; //who am i; verts[idx]
	Color* color;

	vector< int > vertList; //adj vvertices;
	vector< float > adjVertLengthList;
	vector< int > triList; 
	vector< int > edgeList; 
	
	Vertex(int i, float* c) : idx(i), coords(c) {};
};

struct Edge
{
	int idx; //edges[idx]
	int v1i, v2i; //endpnts
	float length;
	Edge(int id, int v1, int v2) : idx(id), v1i(v1), v2i(v2) { };

	void computeLength(Vertex* v1, Vertex* v2)
	{
		length = sqrt(pow(v1->coords[0] - v2->coords[0], 2) + pow(v1->coords[1] - v2->coords[1], 2) + pow(v1->coords[2] - v2->coords[2], 2));
	}

};

struct Triangle
{
	int idx; //tris[idx]
	int v1i, v2i, v3i;
	Triangle(int id, int v1, int v2, int v3) : idx(id), v1i(v1), v2i(v2), v3i(v3) {};
};

class Mesh
{
private:
	void addTriangle(int v1, int v2, int v3);
	void addEdge(int v1, int v2);
	void addVertex(float x, float y, float z);
	bool makeVertsNeighbor(int v1i, int v2i);
public:
	vector< Vertex* > verts;
	vector< Triangle* > tris;
	vector< Edge* > edges;

	Mesh() {} ;
	void createCube(float side);
	void loadOff(char* name);
	float getLengthOfEdgeByAdjacentVertexId(int start_vertex_id, int adjacent_vertex_id);
	int getEdgeId(int v1, int v2);
	void fillAdjVertLengthList();

};
