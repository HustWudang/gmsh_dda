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
	cin >> MESH_TYPE;

	if (MESH_TYPE == 2)
	{
		GmshDDA mGmshDDA_2d;
		mGmshDDA_2d.fReadBlkPhyAttr_txt("0_BlkPhyAttr.txt");
		//mGmshDDA_2d.fReadGmsh_msh("0.msh");
		mGmshDDA_2d.fReadGmshNew_msh("0.msh");
		mGmshDDA_2d.fWriteDDABlock_triangle_json("0_block.json");
		mGmshDDA_2d.fWriteDDABlock_triangle_vtp("0_block.vtp");
	}
	else if (MESH_TYPE == 3)
	{
		GmshDDA mGmshDDA_3d;
		mGmshDDA_3d.fReadBlkPhyAttr_txt("0_BlkPhyAttr.txt");
		mGmshDDA_3d.fReadGmsh_msh("0.msh");
		mGmshDDA_3d.fWriteDDABlock_tetrahedron_json("0_block.json");
		mGmshDDA_3d.fWriteDDABlock_tetrahedron_vtp("0_block.vtp");
	}
	else
	{
		cout << "[Error] - wrong instruction is input from screen !\n";
	}
	
	// [Main function return].
	system("pause");
	return 0;
}
