// pragma once.
#pragma once

// libs included.
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
	int i_JntFlag; //!< [1]jnt; [0]matrix.
	int i_NumJnt;
	vector<int> vi_JntFlag;
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
	int ri_EdgeNodeNo[3]; //!< i_PointNo_0, i_PointNo_1, i_PointNo_2.
	int ri_NodeFracFlag[3];	//!< [1]frac; [0]bond.
	int ri_EdgeFracFlag[3];	//!< [1]frac; [0]bond.
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

class CNet_JointLine
{
public:
	int i_No;
	double bg_x, bg_y, bg_z;
	double ed_x, ed_y, ed_z;
};




class CGmshNetDDA
{
public:
	///// block part /////
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
	///// network part /////
	int i_NumNetJointLine;
	vector<CNet_JointLine> lv_NetJointLine;
	///// interface functions /////
	void fRead_BlkPhyAttr_txt(string str_0);
	void fRead_Gmsh_msh_v10(string str_0);
	void fRead_Gmsh_msh_v40(string str_0);
	void fRead_Network2d_JointLine_txt(string str_0);
	void fWrite_DDABlock_triangle_json(string str_0);
	void fWrite_DDABlock_triangle_vtp(string str_0);
	void fWrite_DDABlock_tetrahedron_json(string str_0);
	void fWrite_DDABlock_tetrahedron_vtp(string str_0);
	void fWrite_Network2d_matrix_vtk(string str_0);
	void fWrite_Network2d_fracture_global_vtk(string str_0);
	void fWrite_Network2d_fracture_local_vtk(string str_0);
	///// data operation functions /////
	void fTransfer_EstablishNetwork(double distol);
};

//! judge if a point is inside a line.
//! \return 1 inside;
//! \return 0 outside.
int fmath2d_PointInLineJudge(double x0, double y0, double x1,double y1, double x2, double y2, double distol);


