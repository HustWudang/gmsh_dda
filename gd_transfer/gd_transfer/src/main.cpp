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
	// [Screen Reminder].
	cout << "-----\n";
	cout << " Transfer Gmsh file (1.msh) to DDA block file (block.json) ...\n";
	cout << "-----\n";
	
	GmshDDA mGmshDDA_1;
	mGmshDDA_1.fReadBlkPhyAttr_txt("0_BlkPhyAttr.txt");
	mGmshDDA_1.fReadGmsh_msh("0.msh");
	mGmshDDA_1.fWriteDDABlock_json("0_block.json");
	mGmshDDA_1.fWriteDDABlock_vtp("0_block.vtp");
	// [Main function return].
	system("pause");
	return 0;
}
