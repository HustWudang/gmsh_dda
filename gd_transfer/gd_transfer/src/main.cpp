/*
* Main procedure to make the transfer
* from Gmsh file "0.msh"
* to DDA program "0_block.json"
*/

// header file.
#include "structure.h"

// namespace std;
using namespace std;

// main function.
int main()
{
	// // [Screen Reminder] - check 2d or 3d mesh ...
	int MESH_TYPE = 0;
	cout << "------------------------------------------------------\n";
	cout << " Transfer Gmsh file (0.msh)                           \n";
	cout << "    to DDA block file (0_block.json & 0_block.vtp)    \n";
	cout << "------------------------------------------------------\n";
	cout << "[ 1st ] - INPUT Mesh Type (as follows):               \n";
	cout << "------------------------------------------------------\n";
	cout << " >>   2 | 2d triangles                                \n";
	cout << " >>   3 | 3d tetrahedra                               \n";
	cout << "------------------------------------------------------\n";
	cout << " >>   1 | triangle element + fracture network         \n";
	cout << "------------------------------------------------------\n";
	cin >> MESH_TYPE;

	if (MESH_TYPE == 2)
	{
		cout << "[Msg] tranferring 2d trianglular mesh ...\n";
		CGmshNetDDA mGmshDDA_2d;
		mGmshDDA_2d.fRead_BlkPhyAttr_txt("0_BlkPhyAttr.txt");
		//mGmshDDA_2d.fRead_Gmsh_msh_v10("0.msh");
		mGmshDDA_2d.fRead_Gmsh_msh_v40("0.msh");
		mGmshDDA_2d.fWrite_DDABlock_triangle_json("0_block.json");
		mGmshDDA_2d.fWrite_DDABlock_triangle_vtp("0_block.vtp");
	}
	else if (MESH_TYPE == 3)
	{
		cout << "[Msg] tranferring 3d tetrahedral mesh ...\n";
		CGmshNetDDA mGmshDDA_3d;
		mGmshDDA_3d.fRead_BlkPhyAttr_txt("0_BlkPhyAttr.txt");
		mGmshDDA_3d.fRead_Gmsh_msh_v10("0.msh");
		mGmshDDA_3d.fWrite_DDABlock_tetrahedron_json("0_block.json");
		mGmshDDA_3d.fWrite_DDABlock_tetrahedron_vtp("0_block.vtp");
	}
	else if (MESH_TYPE == 1)
	{
		cout << "[Msg] tranferring 2d trianglular mesh ...\n";
		CGmshNetDDA mGmshNetDDA_2d;
		mGmshNetDDA_2d.fRead_BlkPhyAttr_txt("0_BlkPhyAttr.txt");
		//mGmshDDA_2d.fRead_Gmsh_msh_v10("0.msh");
		mGmshNetDDA_2d.fRead_Gmsh_msh_v40("0.msh");
		mGmshNetDDA_2d.fRead_Network2d_JointLine_txt("0_fracture.txt");
		mGmshNetDDA_2d.fTransfer_EstablishNetwork(1e-5);
		mGmshNetDDA_2d.fWrite_DDABlock_triangle_json("0_block.json");
		mGmshNetDDA_2d.fWrite_DDABlock_triangle_vtp("0_block.vtp");
		mGmshNetDDA_2d.fWrite_Network2d_matrix_vtk("0_matrix_network.vtk");
		mGmshNetDDA_2d.fWrite_Network2d_fracture_global_vtk("0_fracture_network_global.vtk");
		mGmshNetDDA_2d.fWrite_Network2d_fracture_local_vtk("0_fracture_network_local.vtk");
	}
	else
	{
		cout << "[Error] - wrong instruction is input from screen !\n";
	}
	
	// [Main function return].
	system("pause");
	return 0;
}
