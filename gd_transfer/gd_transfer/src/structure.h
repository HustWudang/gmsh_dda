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
	int i_type;
	int i_dimension;
	int i_reserve;
	int i_NodeNo;
	int i_PointNo;
};

class CGmshLine
{
public:
	int i_ElementNo;
	int i_type;
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
	int i_type;
	int i_dimension;
	int i_reserve;
	int i_GlobalSurfaceNo;
	int i_PointNo_0;
	int i_PointNo_1;
	int i_PointNo_2;
};

class GmshDDA
{
public:
	int i_NumPoint;
	int i_NumElement;
	int i_NumNode;
	int i_NumLine;
	int i_NumTriangle;
	vector<CGmshPoint> lv_GmshPoint;
	vector<CGmshNode> lv_GmshNode;
	vector<CGmshLine> lv_GmshLine;
	vector<CGmshTriangle> lv_GmshTriangle;
	void fReadGmsh_msh(string str_0);
	void fWriteDDABlock_json(string str_0);
	void fWriteDDABlock_vtp(string str_0);
};




