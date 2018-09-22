#pragma once

// lib header.
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>

// namespace std;
using namespace std;

// Gmsh basic elements.
class CGmshPoint
{
public:
	int i_No;
	double x, y, z;
};

class CGmshNode
{
public:
	int i_ElementNo;
	int i_type; // 15
	int i_dimension;
	int i_reserve;
	int i_NodeNo;
	int i_PointNo;
};

class CGmshLine
{
public:
	int i_ElementNo;
	int i_type; // 1
	int i_dimension;
	int i_reserve;
	int i_GlobalLineNo;
	int i_PointNo_0;
	int i_PointNo_1;
};

class CGmshTriangle
{
public:
	int i_ElementNo;
	int i_type; // 2
	int i_dimension;
	int i_reserve;
	int i_GlobalSurfaceNo;
	int i_PointNo_0;
	int i_PointNo_1;
	int i_PointNo_2;
};

class CGmshTetrahedron
{
public:
	int i_ElementNo;
	int i_type; // 4
	int i_dimension;
	int i_reserve;
	int i_GlobalVolumeNo;
	int i_PointNo_0;
	int i_PointNo_1;
	int i_PointNo_2;
	int i_PointNo_3;
};

class BlockPhysicalAttribute
{
public:
	double d_density;
	double d_ElasticModulus;
	double d_PoissonRatio;
	double d_TensionStrength;
	double d_cohesion;
	double d_FrictionAngle;
};

class GmshDDA
{
public:
	int i_NumPoint;
	int i_NumElement;
	int i_NumNode;
	int i_NumLine;
	int i_NumTriangle;
	int i_NumTetrahedron;
	vector<CGmshPoint> lv_GmshPoint;
	vector<CGmshNode> lv_GmshNode;
	vector<CGmshLine> lv_GmshLine;
	vector<CGmshTriangle> lv_GmshTriangle;
	vector<CGmshTetrahedron> lv_GmshTetrahedron;
	BlockPhysicalAttribute m_BlkPhyAttr;
	void fReadBlkPhyAttr_txt(string str_0);
	void fReadGmsh_msh(string str_0);
	void fWriteDDABlock_triangle_json(string str_0);
	void fWriteDDABlock_triangle_vtp(string str_0);
	void fWriteDDABlock_tetrahedron_json(string str_0);
	void fWriteDDABlock_tetrahedron_vtp(string str_0);
};





