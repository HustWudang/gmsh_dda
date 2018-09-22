// header.
#include "structure.h"

// implementation of the functions in "structure.h".

void GmshDDA::fReadBlkPhyAttr_txt(string str_0)
{
	ifstream infile;
	infile.open(str_0);
	if (infile.is_open())
	{
		infile >> m_BlkPhyAttr.d_density 
			>> m_BlkPhyAttr.d_ElasticModulus 
			>> m_BlkPhyAttr.d_PoissonRatio 
			>> m_BlkPhyAttr.d_TensionStrength 
			>> m_BlkPhyAttr.d_cohesion 
			>> m_BlkPhyAttr.d_FrictionAngle;
		cout << "read 0_BlkPhyAttr.txt successfully ...\n";
		infile.close();
	}
	else
	{
		cout << "[Error] in opening .msh file !\n";
	}
}

void GmshDDA::fReadGmsh_msh(string str_0)
{
	ifstream infile;
	infile.open(str_0);
	if (infile.is_open())
	{
		string s_line;
		int i_line = 0;
		while (getline(infile, s_line))
		{
			if (*s_line.begin() == '$') continue;
			i_line++;
			stringstream sstream(s_line);
			if (i_line == 1)
			{
				double d_version;
				int i_binary;
				unsigned long ul_size;
				sstream >> d_version >> i_binary >> ul_size;
			}
			else if (i_line == 2)
			{
				sstream >> i_NumPoint;
			}
			else if (i_line > 2 && i_line <= 2 + i_NumPoint)
			{
				CGmshPoint nGmshPoint_t;
				sstream >> nGmshPoint_t.i_No >> nGmshPoint_t.x >> nGmshPoint_t.y >> nGmshPoint_t.z;
				lv_GmshPoint.push_back(nGmshPoint_t);
			}
			else if (i_line == 3 + i_NumPoint)
			{
				sstream >> i_NumElement;
			}
			else if (i_line > 3 + i_NumPoint && i_line <= 3 + i_NumPoint + i_NumElement)
			{
				int i_ElementNo_t, i_ElementType_t;
				sstream >> i_ElementNo_t >> i_ElementType_t;
				if (i_ElementType_t == 15)
				{
					CGmshNode nGmshNode_t;
					nGmshNode_t.i_ElementNo = i_ElementNo_t;
					nGmshNode_t.i_type = i_ElementType_t;
					sstream >> nGmshNode_t.i_dimension >> nGmshNode_t.i_reserve >> nGmshNode_t.i_NodeNo >> nGmshNode_t.i_PointNo;
					lv_GmshNode.push_back(nGmshNode_t);
				}
				else if (i_ElementType_t == 1)
				{
					CGmshLine nGmshLine_t;
					nGmshLine_t.i_ElementNo = i_ElementNo_t;
					nGmshLine_t.i_type = i_ElementType_t;
					sstream >> nGmshLine_t.i_dimension >> nGmshLine_t.i_reserve >> nGmshLine_t.i_GlobalLineNo 
						>> nGmshLine_t.i_PointNo_0 >> nGmshLine_t.i_PointNo_1;
					lv_GmshLine.push_back(nGmshLine_t);
				}
				else if (i_ElementType_t == 2)
				{
					CGmshTriangle nGmshTriangle_t;
					nGmshTriangle_t.i_ElementNo = i_ElementNo_t;
					nGmshTriangle_t.i_type = i_ElementType_t;
					sstream >> nGmshTriangle_t.i_dimension >> nGmshTriangle_t.i_reserve >> nGmshTriangle_t.i_GlobalSurfaceNo 
						>> nGmshTriangle_t.i_PointNo_0 
						>> nGmshTriangle_t.i_PointNo_1 
						>> nGmshTriangle_t.i_PointNo_2;
					lv_GmshTriangle.push_back(nGmshTriangle_t);
				}
				else if (i_ElementType_t == 4)
				{
					CGmshTetrahedron nGmshTetrahedron_t;
					sstream >> nGmshTetrahedron_t.i_dimension >> nGmshTetrahedron_t.i_reserve >> nGmshTetrahedron_t.i_GlobalVolumeNo 
						>> nGmshTetrahedron_t.i_PointNo_0 
						>> nGmshTetrahedron_t.i_PointNo_1 
						>> nGmshTetrahedron_t.i_PointNo_2 
						>> nGmshTetrahedron_t.i_PointNo_3;
					lv_GmshTetrahedron.push_back(nGmshTetrahedron_t);
				}
				else
				{
					cout << "[Error] Additional type should be considered in fReadGmsh_msh() !\n";
				}
			}
		}
		i_NumNode = lv_GmshNode.size();
		i_NumLine = lv_GmshLine.size();
		i_NumTriangle = lv_GmshTriangle.size();
		i_NumTetrahedron = lv_GmshTetrahedron.size();
		cout << "read 0.msh successfully ...\n";
		infile.close();
	}
	else
	{
		cout << "[Error] in opening .msh file !\n";
	}
}

void GmshDDA::fWriteDDABlock_triangle_json(string str_0)
{
	ofstream of;
	of.open(str_0);
	if (of.is_open())
	{
		for (int i_triangle = 0; i_triangle < i_NumTriangle; i_triangle++)
		{
			/////////// ----- BEGIN -----
			of << "{";
			/////////// ----- id -----
			of << "\"no\":" << i_triangle << ",";
			/////////// ----- node number -----
			of << "\"node_num\":" << 3 << ",";
			/// ----- edge number -----
			of << "\"edge_num\":" << 3 << ",";
			/////////// ----- node coordinate list -----
			of << "\"node_list\":";
			of << "[";
			of << "[" << lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_0 - 1].x << "," << lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_0 - 1].y << "],";
			of << "[" << lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_1 - 1].x << "," << lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_1 - 1].y << "],";
			of << "[" << lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_2 - 1].x << "," << lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_2 - 1].y << "]";
			of << "]" << ",";
			////////// ----- physical parameter -----
			of << "\"density\":" << m_BlkPhyAttr.d_density << ",";
			of << "\"elastic_modulus\":" << m_BlkPhyAttr.d_ElasticModulus << ",";
			of << "\"poisson_ratio\":" << m_BlkPhyAttr.d_PoissonRatio << ",";
			of << "\"cohesion\":" << m_BlkPhyAttr.d_cohesion << ",";
			of << "\"friction_angle\":" << m_BlkPhyAttr.d_FrictionAngle << ",";
			////////// ----- other conditions -----
			of << "\"velo\":";
			of << "[";
			for (int ii = 0; ii < 5; ii++)
			{
				of << 0 << ",";
			}
			of << 0;
			of << "]" << ",";
			of << "\"stress\":";
			of << "[";
			for (int ii = 0; ii < 2; ii++)
			{
				of << 0 << ",";
			}
			of << 0;
			of << "]" << ",";
			of << "\"strain\":";
			of << "[";
			for (int ii = 0; ii < 2; ii++)
			{
				of << 0 << ",";
			}
			of << 0;
			of << "]";
			/////////// ----- END -----
			of << "}\n";
		}
		of.close();
		cout << "write otblock.json successfully ...\n";
	}
	else
	{
		cout << "[Error] cannot open otblock.json !\n";
	}
}

void GmshDDA::fWriteDDABlock_triangle_vtp(string str_0)
{
	ofstream of;
	of.open(str_0);
	if (of.is_open())
	{
		////////// Output file head [little endian]
		of << "<?xml version=\"1.0\"?>";
		of << "<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\">";
		of << "<PolyData>\n";
		////////// cycle the block list
		for (int i_triangle = 0; i_triangle < i_NumTriangle; i_triangle++)
		{
			////////// general
			int iNumBlockNode = 3;
			of << "<Piece NumberOfPoints=\"" << iNumBlockNode << "\" NumberOfVerts=\"" << 0 << "\"";
			of << " NumberOfLines=\"" << 0 << "\" NumberOfStrips=\"" << 0 << "\" NumberOfPolys=\"" << 1 << "\">";

			////////// Points - geometry
			of << "<Points>";
			of << "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">";
			of << float(lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_0 - 1].x) << " " << float(lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_0 - 1].y) << " " << 0 << " ";
			of << float(lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_1 - 1].x) << " " << float(lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_1 - 1].y) << " " << 0 << " ";
			of << float(lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_2 - 1].x) << " " << float(lv_GmshPoint[lv_GmshTriangle[i_triangle].i_PointNo_2 - 1].y) << " " << 0;
			of << "</DataArray>";
			of << "</Points>";

			//////////// Point data - block attributes
			of << "<PointData>";
			of << "<DataArray type=\"Float32\" Name=\"Velocity\" NumberOfComponents=\"2\" format=\"ascii\">";
			double ux = 0;
			double uy = 0;
			of << float(ux) << " " << float(uy) << " ";
			of << float(ux) << " " << float(uy) << " ";
			of << float(ux) << " " << float(uy);
			of << "</DataArray>";
			of << "</PointData>";

			////////// Cell data - block attribtutes
			of << "<CellData>";
			of << "<DataArray type=\"Int32\" Name=\"ID\" format=\"ascii\">";
			int id = i_triangle;
			of << id << " ";
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Density\" format=\"ascii\">";
			float dens = m_BlkPhyAttr.d_density;
			of << dens << " ";
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Elastic_Modulus\" format=\"ascii\">";
			float e_m = m_BlkPhyAttr.d_ElasticModulus;
			of << e_m << " ";
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Poisson_Ratio\" format=\"ascii\">";
			float p_r = m_BlkPhyAttr.d_PoissonRatio;
			of << p_r << " ";
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Cohesion\" format=\"ascii\">";
			float coh = m_BlkPhyAttr.d_cohesion;
			of << coh << " ";
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Friction_Angle\" format=\"ascii\">";
			float f_a = m_BlkPhyAttr.d_FrictionAngle;
			of << f_a << " ";
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Stress\" NumberOfComponents=\"3\" format=\"ascii\">";
			float stress_xx = 0;
			float stress_yy = 0;
			float stress_xy = 0;
			of << stress_xx << " " << stress_yy << " " << stress_xy << " ";
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Strain\" NumberOfComponents=\"3\" format=\"ascii\">";
			float strain_xx = 0;
			float strain_yy = 0;
			float strain_xy = 0;
			of << strain_xx << " " << strain_yy << " " << strain_xy << " ";
			of << "</DataArray>";
			of << "</CellData>";

			////////// Polys - connectivity
			of << "<Polys>";
			of << "<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">";
			of << 0 << " " << 1 << " " << 2;
			of << "</DataArray>";

			////////// Polys - offset
			of << "<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">";
			of << 3;
			of << "</DataArray>";

			////////// Polys - end
			of << "</Polys>";

			////////// Piece - end
			of << "</Piece>\n";
		}
		////////// write file ender
		of << "</PolyData>";
		of << "</VTKFile>";
		of.close();
		cout << "write otblock.vtp successfully ...\n";
	}
	else
	{
		cout << "[Error-File] cannot open file otblock.vtp !\n";
	}
}


void GmshDDA::fWriteDDABlock_tetrahedron_json(string str_0)
{
	ofstream of;
	of.open(str_0);
	if (of.is_open())
	{
		for (int i_tet = 0; i_tet < i_NumTriangle; i_tet++)
		{
			/////////// ----- BEGIN -----
			of << "{";
			/////////// ----- id -----
			of << "\"no\":" << i_tet << ",";
			/// ----- node number -----
			of << "\"node_num\":" << 4 << ",";
			/// ----- face number -----
			of << "\"face_num\":" << 4 << ",";
			/////////// ----- node coordinate list -----
			of << "\"node_list\":";
			of << "[";
			of << "[" << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_0 - 1].x << "," << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_0 - 1].y << "," << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_0 - 1].z << "],";
			of << "[" << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_1 - 1].x << "," << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_1 - 1].y << "," << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_1 - 1].z << "],";
			of << "[" << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_2 - 1].x << "," << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_2 - 1].y << "," << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_2 - 1].z << "],";
			of << "[" << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_3 - 1].x << "," << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_3 - 1].y << "," << lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_3 - 1].z << "]";
			of << "]" << ",";
			/////////// ----- face loop list -----
			of << "\"face_loop_list\":";
			of << "[";
			of << "[0,2,1],";
			of << "[0,3,2],";
			of << "[0,1,3],";
			of << "[1,2,3]";
			of << "]" << ",";
			////////// ----- physical parameter -----
			of << "\"density\":" << m_BlkPhyAttr.d_density << ",";
			of << "\"elastic_modulus\":" << m_BlkPhyAttr.d_ElasticModulus << ",";
			of << "\"poisson_ratio\":" << m_BlkPhyAttr.d_PoissonRatio << ",";
			of << "\"cohesion\":" << m_BlkPhyAttr.d_cohesion << ",";
			of << "\"friction_angle\":" << m_BlkPhyAttr.d_FrictionAngle << ",";
			////////// ----- other conditions -----
			of << "\"velo\":";
			of << "[";
			for (int ii = 0; ii < 11; ii++)
			{
				of << 0 << ",";
			}
			of << 0;
			of << "]" << ",";
			of << "\"stress\":";
			of << "[";
			for (int ii = 0; ii < 5; ii++)
			{
				of << 0 << ",";
			}
			of << 0;
			of << "]" << ",";
			of << "\"strain\":";
			of << "[";
			for (int ii = 0; ii < 5; ii++)
			{
				of << 0 << ",";
			}
			of << 0;
			of << "]";
			/////////// ----- END -----
			of << "}\n";
		}
		of.close();
		cout << "write otblock.json successfully ...\n";
	}
	else
	{
		cout << "[Error] cannot open otblock.json !\n";
	}
}


void GmshDDA::fWriteDDABlock_tetrahedron_vtp(string str_0)
{
	fstream of;
	of.open(str_0, fstream::out | fstream::trunc);    // Open file to input data
	if (of.is_open())
	{
		// Output file head
		of << "<?xml version=\"1.0\"?>" << "\n";
		of << "<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\">" << "\n";
		of << "<PolyData>" << "\n";
		// Write pieces (Each Block is One Piece.)
		for (int i_tet = 0; i_tet < i_NumTetrahedron; i_tet++)
		{
			////////// general
			int iNumBlockNode = 4;
			int iNumBlockFace = 4;
			of << "<Piece NumberOfPoints=\"" << iNumBlockNode << "\" NumberOfVerts=\"" << 0 << "\"";
			of << " NumberOfLines=\"" << 0 << "\" NumberOfStrips=\"" << 0 << "\" NumberOfPolys=\"" << iNumBlockFace << "\">";

			////////// Points - geometry
			of << "<Points>";
			of << "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_0 - 1].x) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_0 - 1].y) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_0 - 1].z) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_1 - 1].x) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_1 - 1].y) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_1 - 1].z) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_2 - 1].x) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_2 - 1].y) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_2 - 1].z) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_3 - 1].x) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_3 - 1].y) << " ";
			of << float(lv_GmshPoint[lv_GmshTetrahedron[i_tet].i_PointNo_3 - 1].z) << " ";
			of << "</DataArray>";
			of << "</Points>";

			//////////// Point data - block attributes
			of << "<PointData Scalars=\"Attribute\" Vectors=\"Velocity\" Tensors=\"Strain_Stress\">";
			// ID
			of << "<DataArray type=\"Int32\" Name=\"Block_ID\" format=\"ascii\">";
			int ID = i_tet;
			of << ID << " " << ID << " " << ID << " " << ID;
			of << "</DataArray>";
			// Physical attributes
			of << "<DataArray type=\"Float32\" Name=\"Density\" format=\"ascii\">";
			float dens = m_BlkPhyAttr.d_density;
			of << dens << " " << dens << " " << dens << " " << dens;
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Elastic_Modulus\" format=\"ascii\">";
			float e_m = m_BlkPhyAttr.d_ElasticModulus;
			of << e_m << " " << e_m << " " << e_m << " " << e_m;
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Poisson_Ratio\" format=\"ascii\">";
			float p_r = m_BlkPhyAttr.d_PoissonRatio;
			of << p_r << " " << p_r << " " << p_r << " " << p_r;
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Cohesion\" format=\"ascii\">";
			float coh = m_BlkPhyAttr.d_cohesion;
			of << coh << " " << coh << " " << coh << " " << coh;
			of << "</DataArray>";
			of << "<DataArray type=\"Float32\" Name=\"Friction_Angle\" format=\"ascii\">";
			float f_a = m_BlkPhyAttr.d_FrictionAngle;
			of << f_a << " " << f_a << " " << f_a << " " << f_a;
			of << "</DataArray>";
			// Velocity
			of << "<DataArray type=\"Float32\" Name=\"Velocity\" NumberOfComponents=\"3\" format=\"ascii\">";
			float ux = 0;
			float uy = 0;
			float uz = 0;
			of << ux << " " << uy << " " << uz << " ";
			of << ux << " " << uy << " " << uz << " ";
			of << ux << " " << uy << " " << uz << " ";
			of << ux << " " << uy << " " << uz << " ";
			of << "</DataArray>";
			// Strain
			of << "<DataArray type=\"Float32\" Name=\"Strain\" NumberOfComponents=\"9\" format=\"ascii\">";
			float fxx = 0;
			float fxy = 0;
			float fxz = 0;
			float fyx = 0;
			float fyy = 0;
			float fyz = 0;
			float fzx = 0;
			float fzy = 0;
			float fzz = 0;
			of << fxx << " " << fxy << " " << fxz << " " << fyx << " " << fyy << " " << fyz << " " << fzx << " " << fzy << " " << fzz << " ";
			of << fxx << " " << fxy << " " << fxz << " " << fyx << " " << fyy << " " << fyz << " " << fzx << " " << fzy << " " << fzz << " ";
			of << fxx << " " << fxy << " " << fxz << " " << fyx << " " << fyy << " " << fyz << " " << fzx << " " << fzy << " " << fzz << " ";
			of << fxx << " " << fxy << " " << fxz << " " << fyx << " " << fyy << " " << fyz << " " << fzx << " " << fzy << " " << fzz;
			of << "</DataArray>";
			// Stress
			of << "<DataArray type=\"Float32\" Name=\"Stress\" NumberOfComponents=\"9\" format=\"ascii\">";
			of << fxx << " " << fxy << " " << fxz << " " << fyx << " " << fyy << " " << fyz << " " << fzx << " " << fzy << " " << fzz << " ";
			of << fxx << " " << fxy << " " << fxz << " " << fyx << " " << fyy << " " << fyz << " " << fzx << " " << fzy << " " << fzz << " ";
			of << fxx << " " << fxy << " " << fxz << " " << fyx << " " << fyy << " " << fyz << " " << fzx << " " << fzy << " " << fzz << " ";
			of << fxx << " " << fxy << " " << fxz << " " << fyx << " " << fyy << " " << fyz << " " << fzx << " " << fzy << " " << fzz;
			of << "</DataArray>";
			// PointData - end.
			of << "</PointData>";

			////////// Polys - connectivity
			of << "<Polys>";
			of << "<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">";
			of << "0 2 1 0 3 2 0 1 3 1 2 3";
			of << "</DataArray>";
			////////// Polys - offset
			of << "<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">";
			of << "3 6 9 12";
			of << "</DataArray>";
			////////// Polys - end
			of << "</Polys>";

			////////// Piece - end
			of << "</Piece>\n";

		}
		// write file ender
		of << "</PolyData>" << "\n";
		of << "</VTKFile>";
		of.close();
		cout << "write otblock.vtp successfully ...\n";
	}
	else
	{
		std::cout << "FOutput_PolyhedronVTP() Error : Can not open Files ! \n";
	}
}