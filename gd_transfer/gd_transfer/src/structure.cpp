// header.
#include "structure.h"

using namespace std;

// implementation of the functions in "structure.h".

void CGmshNetDDA::fRead_BlkPhyAttr_txt(string str_0)
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

void CGmshNetDDA::fRead_Gmsh_msh_v10(string str_0)
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
					nGmshTriangle_t.ri_EdgeNodeNo[0] = nGmshTriangle_t.i_PointNo_0;
					nGmshTriangle_t.ri_EdgeNodeNo[1] = nGmshTriangle_t.i_PointNo_1;
					nGmshTriangle_t.ri_EdgeNodeNo[2] = nGmshTriangle_t.i_PointNo_2;
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

void CGmshNetDDA::fRead_Gmsh_msh_v40(string str_0)
{
	ifstream infile;
	infile.open(str_0);
	if (infile.is_open())
	{
		bool b_FindMeshFormat = false;
		bool b_FindEntities = false;
		bool b_FindNodes = false;
		bool b_FindElements = false;
		string s_line;
		while (getline(infile, s_line))
		{
			if (*s_line.begin() == '$')
			{
				// Comments - ommitted.
				size_t st_FindComments = s_line.find("$Comments");
				if (st_FindComments != string::npos)
				{
					string s_sectionline;
					while (getline(infile, s_sectionline))
					{
						// check if go to the end of Comments section.
						size_t st_FindEnComments = s_sectionline.find("$EndComments");
						if (st_FindEnComments != string::npos) { break; }
						else { continue; }
					}
				}
				// $MeshFormat.
				if (b_FindMeshFormat != true)
				{
					size_t st_FindMeshFormat = s_line.find("$MeshFormat");
					if (st_FindMeshFormat != string::npos)
					{
						b_FindMeshFormat = true;
						string s_sectionline;
						while (getline(infile, s_sectionline))
						{
							if (*s_sectionline.begin() == '$')
							{
								// check if go to the end of this section.
								size_t st_FindEndMeshFormat = s_sectionline.find("$EndMeshFormat");
								if (st_FindEndMeshFormat != string::npos) { break; }
								else { cout << "[Error] wrong $MeshFormat section in .msh !\n"; }
							}
							else
							{
								stringstream sstream_section(s_sectionline);
								// found out.
								double d_version;
								int i_binary;
								unsigned long ul_size;
								sstream_section >> d_version >> i_binary >> ul_size;
							}
						}
					}
				}
				// $Entities.
				else if (b_FindEntities != true)
				{
					size_t st_FindEntities = s_line.find("$Entities");
					if (st_FindEntities != string::npos)
					{
						b_FindEntities = true;
						int i_sectionline = 0;
						string s_sectionline;
						while (getline(infile, s_sectionline))
						{
							if (*s_sectionline.begin() == '$')
							{
								// check if go to the end of this section.
								size_t st_FindEndEntities = s_sectionline.find("$EndEntities");
								if (st_FindEndEntities != string::npos) { break; }
								else { cout << "[Error] wrong $Entities format in .msh !\n"; }
							}
							else
							{
								i_sectionline++;
								continue;
							}
						}
					}
				}
				// $Nodes.
				else if (b_FindNodes != true)
				{
					size_t st_FindNodes = s_line.find("$Nodes");
					if (st_FindNodes != string::npos)
					{
						b_FindNodes = true;
						int i_sectionline = 0;
						string s_sectionline;
						while (getline(infile, s_sectionline))
						{
							size_t numEntityBlocks, numNodes, minNodeTag, maxNodeTag;
							if (*s_sectionline.begin() == '$')
							{
								// check if go to the end of this section.
								size_t st_FindEndNodes = s_sectionline.find("$EndNodes");
								if (st_FindEndNodes != string::npos) { break; }
								else { cout << "[Error] wrong $Nodes format in .msh !\n"; }
							}
							else
							{
								i_sectionline++;
								stringstream sstream_section(s_sectionline);
								if (i_sectionline == 1)
								{
									sstream_section >> numEntityBlocks >> numNodes >> minNodeTag >> maxNodeTag;
									// total point number.
									i_NumPoint = numNodes;
									int i_PointArrayCounter = 0;
									for (int i_entities = 0; i_entities < numEntityBlocks; i_entities++)
									{
										int i_entitiesline = 0;
										int entityDim, entityTag, parametric;
										size_t numNodesInBlock;
										string s_entitiesline;
										while (getline(infile, s_entitiesline))
										{
											i_entitiesline++;
											stringstream sstream_entities(s_entitiesline);
											if (i_entitiesline == 1)
											{
												// entities head.
												sstream_entities >> entityDim >> entityTag >> parametric >> numNodesInBlock;
											}
											else if (i_entitiesline >= 2 && i_entitiesline <= 2 + numNodesInBlock - 1)
											{
												CGmshPoint nGmshPoint_t;
												sstream_entities >> nGmshPoint_t.i_No;
												lv_GmshPoint.push_back(nGmshPoint_t);
											}
											else if (i_entitiesline >= 2 + numNodesInBlock && i_entitiesline <= 2 + numNodesInBlock + numNodesInBlock - 1)
											{
												int it_PointArrayNo = i_PointArrayCounter + i_entitiesline - numNodesInBlock - 2;
												sstream_entities >> lv_GmshPoint[it_PointArrayNo].x >> lv_GmshPoint[it_PointArrayNo].y >> lv_GmshPoint[it_PointArrayNo].z;
												if (i_entitiesline == 2 + numNodesInBlock + numNodesInBlock - 1)
												{
													i_PointArrayCounter += numNodesInBlock;
													break;
												}
											}
										}
									}
								}
								else { cout << "[Error] wrong cycle in intepreting $Nodes !\n"; }
							}
						}
					}
				}
				// $Elements.
				else if (b_FindElements != true)
				{
					size_t b_FindElements = s_line.find("$Elements");
					if (b_FindElements != string::npos)
					{
						b_FindElements = true;
						int i_sectionline = 0;
						string s_sectionline;
						while (getline(infile, s_sectionline))
						{
							size_t numEntityBlocks, numElements, minElementTag, maxElementTag;
							if (*s_sectionline.begin() == '$')
							{
								// check if go to the end of this section.
								size_t st_FindEndEntities = s_sectionline.find("$EndElements");
								if (st_FindEndEntities != string::npos) { break; }
								else { cout << "[Error] wrong $Elements format in .msh !\n"; }
							}
							else
							{
								i_sectionline++;
								stringstream sstream_section(s_sectionline);
								if (i_sectionline == 1)
								{
									sstream_section >> numEntityBlocks >> numElements >> minElementTag >> maxElementTag;
									// total point number.
									i_NumElement = numElements;
									int i_GmshNodeNoCounter = 0;
									int i_GmshLineNoCounter = 0;
									int i_GmshTriangleNoCounter = 0;
									int i_GmshTetrahedronNoCounter = 0;
									for (int i_entities = 0; i_entities < numEntityBlocks; i_entities++)
									{
										int i_entitiesline = 0;
										int entityDim, entityTag, elementType;
										size_t numElementsInBlock;
										string s_entitiesline;
										while (getline(infile, s_entitiesline))
										{
											i_entitiesline++;
											stringstream sstream_entities(s_entitiesline);
											if (i_entitiesline == 1)
											{
												// entities head.
												sstream_entities >> entityDim >> entityTag >> elementType >> numElementsInBlock;
											}
											else if (i_entitiesline >= 2 && i_entitiesline <= 2 + numElementsInBlock - 1)
											{
												if (elementType == 15)
												{
													i_GmshNodeNoCounter++;
													CGmshNode nGmshNode_t;
													nGmshNode_t.i_ElementNo = i_GmshNodeNoCounter;
													nGmshNode_t.i_type = elementType;
													sstream_entities >> nGmshNode_t.i_NodeNo >> nGmshNode_t.i_PointNo;
													lv_GmshNode.push_back(nGmshNode_t);
												}
												else if (elementType == 1)
												{
													i_GmshLineNoCounter++;
													CGmshLine nGmshLine_t;
													nGmshLine_t.i_ElementNo = i_GmshLineNoCounter;
													nGmshLine_t.i_type = elementType;
													sstream_entities >> nGmshLine_t.i_GlobalLineNo >> nGmshLine_t.i_PointNo_0 >> nGmshLine_t.i_PointNo_1;
													lv_GmshLine.push_back(nGmshLine_t);
												}
												else if (elementType == 2)
												{
													i_GmshTriangleNoCounter++;
													CGmshTriangle nGmshTriangle_t;
													nGmshTriangle_t.i_ElementNo = i_GmshTriangleNoCounter;
													nGmshTriangle_t.i_type = elementType;
													sstream_entities >> nGmshTriangle_t.i_GlobalSurfaceNo
														>> nGmshTriangle_t.i_PointNo_0
														>> nGmshTriangle_t.i_PointNo_1
														>> nGmshTriangle_t.i_PointNo_2;
													nGmshTriangle_t.ri_EdgeNodeNo[0] = nGmshTriangle_t.i_PointNo_0;
													nGmshTriangle_t.ri_EdgeNodeNo[1] = nGmshTriangle_t.i_PointNo_1;
													nGmshTriangle_t.ri_EdgeNodeNo[2] = nGmshTriangle_t.i_PointNo_2;
													lv_GmshTriangle.push_back(nGmshTriangle_t);
												}
												else if (elementType == 4)
												{
													///// !!!!! THIS CASE IS NOT CHECKED !!!!!.
													i_GmshTetrahedronNoCounter++;
													CGmshTetrahedron nGmshTetrahedron_t;
													nGmshTetrahedron_t.i_ElementNo = i_GmshTetrahedronNoCounter;
													sstream_entities >> nGmshTetrahedron_t.i_GlobalVolumeNo
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
												// check the ending.
												if (i_entitiesline == 2 + numElementsInBlock - 1)
												{
													if (elementType == 15) { i_GmshNodeNoCounter += numElementsInBlock; }
													else if (elementType == 1) { i_GmshLineNoCounter += numElementsInBlock; }
													else if (elementType == 2) { i_GmshTriangleNoCounter += numElementsInBlock; }
													else if (elementType == 4) { i_GmshTetrahedronNoCounter += numElementsInBlock; }
													else { cout << "[Error] Additional type should be considered in fReadGmsh_msh() !\n"; }
													break;
												}
											}
										}
									}
								}
								else { cout << "[Error] wrong cycle in intepreting $Nodes !\n"; }
							}
						}
					}
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

void CGmshNetDDA::fRead_Network2d_JointLine_txt(string str_0)
{
	ifstream infile;
	infile.open(str_0);
	if (infile.is_open())
	{
		infile >> i_NumNetJointLine;
		for (int i_line = 0; i_line < i_NumNetJointLine; i_line++)
		{
			CNet_JointLine mtNetJointLine;
			mtNetJointLine.bg_z = 0;
			mtNetJointLine.ed_z = 0;
			infile >> mtNetJointLine.i_No;
			infile >> mtNetJointLine.bg_x >> mtNetJointLine.bg_y;
			infile >> mtNetJointLine.ed_x >> mtNetJointLine.ed_y;
			lv_NetJointLine.push_back(mtNetJointLine);
		}
		cout << "read fracture.txt successfully ...\n";
		infile.close();
	}
	else
	{
		cout << "[Error] in opening fracture.txt file !\n";
	}
}


void CGmshNetDDA::fWrite_DDABlock_triangle_json(string str_0)
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

void CGmshNetDDA::fWrite_DDABlock_triangle_vtp(string str_0)
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


void CGmshNetDDA::fWrite_DDABlock_tetrahedron_json(string str_0)
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


void CGmshNetDDA::fWrite_DDABlock_tetrahedron_vtp(string str_0)
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


void CGmshNetDDA::fWrite_Network2d_matrix_vtk(string str_0)
{
	// Unstructured Grid in (*.vtk) file.
	// Points.
	// Cell list.
	// Cell types.

	fstream fout;
	fout.open(str_0, fstream::out | fstream::trunc);
	if (fout.is_open())
	{
		// always write big endian.
		fout << "# vtk DataFile Version 2.0\n";
		fout << "Matrix Pipe Network Visualization\n";
		fout << "ASCII\n";
		// Write POLYDATA
		fout << "DATASET UNSTRUCTURED_GRID\n";
		
		// POINTS Data Head.
		i_NumPoint = lv_GmshPoint.size();
		fout << "POINTS " << i_NumPoint << " double\n";
		
		// POINTS Data Contents.
		for (vector<CGmshPoint>::iterator it_point = lv_GmshPoint.begin(); it_point != lv_GmshPoint.end(); it_point++)
		{
			fout << it_point->x << "\t" << it_point->y << "\t" << 0 << "\n";
		}

		// CELLS data.
		i_NumTriangle = lv_GmshTriangle.size();
		fout << "CELLS " << i_NumTriangle << " " << (i_NumTriangle * 4) << "\n";
		for (vector<CGmshTriangle>::iterator it_triangle = lv_GmshTriangle.begin(); it_triangle != lv_GmshTriangle.end(); it_triangle++)
		{
			fout << 3 << "\t" << (it_triangle->i_PointNo_0 - 1) << "\t" << (it_triangle->i_PointNo_1 - 1) << "\t" << (it_triangle->i_PointNo_2 - 1) << "\n";
		}
		fout << "CELL_TYPES " << i_NumTriangle << "\n";
		for (vector<CGmshTriangle>::iterator it_triangle = lv_GmshTriangle.begin(); it_triangle != lv_GmshTriangle.end(); it_triangle++)
		{
			fout << 5 << "\n";
		}
		
		// close file
		fout.close();
		// screen print
		std::cout << "[Msg] Output matrix pipe network successfully ... \n";
	}
	else
	{
		std::cout << "[Error - OF] in fWrite_Network2d_matrix_vtk() !\n";
		return;
	}

}


void CGmshNetDDA::fWrite_Network2d_fracture_global_vtk(string str_0)
{
	// Unstructured Grid in (*.vtk) file.
	// Points.
	// Cell list.
	// Cell types.

	fstream fout;
	fout.open(str_0, fstream::out | fstream::trunc);
	if (fout.is_open())
	{
		// always write big endian.
		fout << "# vtk DataFile Version 2.0\n";
		fout << "Global Fracture Pipe Network Visualization\n";
		fout << "ASCII\n";
		// Write POLYDATA
		fout << "DATASET UNSTRUCTURED_GRID\n";

		// POINTS Data Head.
		i_NumNetJointLine = lv_NetJointLine.size();
		fout << "POINTS " << (2 * i_NumNetJointLine) << " double\n";

		// POINTS Data Contents.
		for (vector<CNet_JointLine>::iterator it_netjntline = lv_NetJointLine.begin(); it_netjntline != lv_NetJointLine.end(); it_netjntline++)
		{
			fout << it_netjntline->bg_x << "\t" << it_netjntline->bg_y << "\t" << 0 << "\n";
			fout << it_netjntline->ed_x << "\t" << it_netjntline->ed_y << "\t" << 0 << "\n";
		}

		// CELLS data.
		fout << "CELLS " << i_NumNetJointLine << " " << (i_NumNetJointLine * 3) << "\n";
		for (int i_netjntline = 0; i_netjntline < i_NumNetJointLine; i_netjntline++)
		{
			fout << 2 << "\t" << (2 * i_netjntline) << "\t" << (2 * i_netjntline + 1) << "\n";
		}
		fout << "CELL_TYPES " << i_NumNetJointLine << "\n";
		for (int i_netjntline = 0; i_netjntline < i_NumNetJointLine; i_netjntline++)
		{
			fout << 3 << "\n";
		}

		// close file
		fout.close();
		// screen print
		std::cout << "[Msg] Output matrix pipe network successfully ... \n";
	}
	else
	{
		std::cout << "[Error - OF] in fWrite_Network2d_matrix_vtk() !\n";
		return;
	}
}


void CGmshNetDDA::fWrite_Network2d_fracture_local_vtk(string str_0)
{
	// Unstructured Grid in (*.vtk) file.
	// Points.
	// Cell list.
	// Cell types.

	fstream fout;
	fout.open(str_0, fstream::out | fstream::trunc);
	if (fout.is_open())
	{
		// always write big endian.
		fout << "# vtk DataFile Version 2.0\n";
		fout << "Local Fracture Pipe Network Visualization\n";
		fout << "ASCII\n";
		// Write POLYDATA
		fout << "DATASET UNSTRUCTURED_GRID\n";

		// POINTS Data Head.
		i_NumPoint = lv_GmshPoint.size();
		fout << "POINTS " << i_NumPoint << " double\n";

		// POINTS Data Contents.
		for (vector<CGmshPoint>::iterator it_point = lv_GmshPoint.begin(); it_point != lv_GmshPoint.end(); it_point++)
		{
			fout << it_point->x << "\t" << it_point->y << "\t" << 0 << "\n";
		}

		// CELLS data.
		int i_NumFracLineSection = 0;
		for (vector<CGmshTriangle>::iterator it_triangle = lv_GmshTriangle.begin(); it_triangle != lv_GmshTriangle.end(); it_triangle++)
		{
			for (int i_edge = 0; i_edge < 3; i_edge++)
			{
				int i_edge_next = i_edge + 1;
				if (i_edge_next == 3) { i_edge_next = 0; }
				if (it_triangle->ri_EdgeNodeNo[i_edge] > it_triangle->ri_EdgeNodeNo[i_edge_next]) { continue; }
				if (1 == it_triangle->ri_EdgeFracFlag[i_edge]) { i_NumFracLineSection++; }
			}
		}
		fout << "CELLS " << i_NumFracLineSection << " " << (i_NumFracLineSection * 3) << "\n";
		for (vector<CGmshTriangle>::iterator it_triangle = lv_GmshTriangle.begin(); it_triangle != lv_GmshTriangle.end(); it_triangle++)
		{
			for (int i_edge = 0; i_edge < 3; i_edge++)
			{
				int i_edge_next = i_edge + 1;
				if (i_edge_next == 3) { i_edge_next = 0; }
				if (it_triangle->ri_EdgeNodeNo[i_edge] > it_triangle->ri_EdgeNodeNo[i_edge_next]) { continue; }
				if (1 == it_triangle->ri_EdgeFracFlag[i_edge])
				{
					fout << 2 << "\t" << (it_triangle->ri_EdgeNodeNo[i_edge] - 1) << "\t" << (it_triangle->ri_EdgeNodeNo[i_edge_next] - 1) << "\n";
				}
			}
		}
		fout << "CELL_TYPES " << i_NumFracLineSection << "\n";
		for (int i_section = 0; i_section < i_NumFracLineSection; i_section++)
		{
			fout << 3 << "\n";
		}

		// close file
		fout.close();
		// screen print
		std::cout << "[Msg] Output matrix pipe network successfully ... \n";
	}
	else
	{
		std::cout << "[Error - OF] in fWrite_Network2d_matrix_vtk() !\n";
		return;
	}
}


void CGmshNetDDA::fTransfer_EstablishNetwork(double distol)
{
	// cycle all block.
	// check block & fracture intersection.
	// find the fractue block edge belonging to.

	// node - joint position relationship.
	for (vector<CGmshPoint>::iterator it_point = lv_GmshPoint.begin(); it_point != lv_GmshPoint.end(); it_point++)
	{
		for (vector<CNet_JointLine>::iterator it_netjntline = lv_NetJointLine.begin(); it_netjntline != lv_NetJointLine.end(); it_netjntline++)
		{
			// check node - joint position.
			if (1 == fmath2d_PointInLineJudge(it_point->x,it_point->y, it_netjntline->bg_x, it_netjntline->bg_y, it_netjntline->ed_x, it_netjntline->ed_y,distol))
			{
				it_point->vi_JntFlag.push_back(it_netjntline->i_No);
			}
		}
		it_point->i_NumJnt = it_point->vi_JntFlag.size();
		if (it_point->i_NumJnt > 0) { it_point->i_JntFlag = 1; }
		else { it_point->i_JntFlag = 0; }
	}

	// update triangle edge - joint status according to the node - joint position relationship.
	for (vector<CGmshTriangle>::iterator it_triangle = lv_GmshTriangle.begin(); it_triangle != lv_GmshTriangle.end(); it_triangle++)
	{
		if (1 == lv_GmshPoint[it_triangle->i_PointNo_0 - 1].i_JntFlag)
		{
			it_triangle->ri_NodeFracFlag[0] = 1;
		}
		else { it_triangle->ri_NodeFracFlag[0] = 0; }
		if (1 == lv_GmshPoint[it_triangle->i_PointNo_1 - 1].i_JntFlag)
		{
			it_triangle->ri_NodeFracFlag[1] = 1;
		}
		else { it_triangle->ri_NodeFracFlag[1] = 0; }
		if (1 == lv_GmshPoint[it_triangle->i_PointNo_2 - 1].i_JntFlag)
		{
			it_triangle->ri_NodeFracFlag[2] = 1;
		}
		else { it_triangle->ri_NodeFracFlag[2] = 0; }
	}

	// flag triangle edge belonging to joint.
	for (vector<CGmshTriangle>::iterator it_triangle = lv_GmshTriangle.begin(); it_triangle != lv_GmshTriangle.end(); it_triangle++)
	{
		// three edges.
		if (1 == it_triangle->ri_NodeFracFlag[0] && 1 == it_triangle->ri_NodeFracFlag[1])
		{
			it_triangle->ri_EdgeFracFlag[0] = 1;
		}
		else { it_triangle->ri_EdgeFracFlag[0] = 0; }
		if (1 == it_triangle->ri_NodeFracFlag[1] && 1 == it_triangle->ri_NodeFracFlag[2])
		{
			it_triangle->ri_EdgeFracFlag[1] = 1;
		}
		else { it_triangle->ri_EdgeFracFlag[1] = 0; }
		if (1 == it_triangle->ri_NodeFracFlag[2] && 1 == it_triangle->ri_NodeFracFlag[0])
		{
			it_triangle->ri_EdgeFracFlag[2] = 1;
		}
		else { it_triangle->ri_EdgeFracFlag[2] = 0; }
	}

}

// judge if a point is inside a line.
int fmath2d_PointInLineJudge(double x0, double y0, double x1, double y1, double x2, double y2, double distol)
{
	// |dn| < distol.
	// ratio [0,1].

	double x01 = x1 - x0;
	double y01 = y1 - y0;
	double x02 = x2 - x0;
	double y02 = y2 - y0;
	double x12 = x2 - x1;
	double y12 = y2 - y1;
	double s_2 = fabs(x01 * y02 - x02 * y01);
	double l12 = sqrt(x12 * x12 + y12 * y12);
	double h0 = s_2 / l12;
	double proj = (-x01 * x12 - y01 * y12) / l12;
	if (h0 < distol && proj > -distol && proj < l12 + distol) { return 1; }
	else { return 0; }
}