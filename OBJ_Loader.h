#ifndef  __OBJ_LOADER_H__
#define  __OBJ_LOADER_H__

#pragma once



//see  https://github.com/Bly7/OBJ-Loader
//For OBJ format see https://en.wikipedia.org/wiki/Wavefront_.obj_file

// OBJ_Loader.h - A Single Header OBJ Model Loader



// Iostream - STD I/O Library
#include <iostream>

// Vector - STD Vector/Array Library
#include <vector>

// String - STD String Library
#include <string>

// fStream - STD File I/O Library
#include <fstream>

// Math.h - STD math Library
#include <math.h>

// Print progress to console while loading (large models)
#define OBJL_CONSOLE_OUTPUT

// Namespace: OBJL
//
// Description: The namespace that holds eveyrthing that
//	is needed and used for the OBJ Model Loader
namespace objl
{
	// Structure: Vector2
	//
	// Description: A 2D Vector that Holds Positional Data
	struct Vector2
	{
		// Default Constructor
		Vector2()
		{
			X = 0.0f;
			Y = 0.0f;
		}
		// Variable Set Constructor
		Vector2(float X_, float Y_)
		{
			X = X_;
			Y = Y_;
		}
		// Bool Equals Operator Overload
		bool operator==(const Vector2& other) const
		{
			return (this->X == other.X && this->Y == other.Y);
		}
		// Bool Not Equals Operator Overload
		bool operator!=(const Vector2& other) const
		{
			return !(this->X == other.X && this->Y == other.Y);
		}
		// Addition Operator Overload
		Vector2 operator+(const Vector2& right) const
		{
			return Vector2(this->X + right.X, this->Y + right.Y);
		}
		// Subtraction Operator Overload
		Vector2 operator-(const Vector2& right) const
		{
			return Vector2(this->X - right.X, this->Y - right.Y);
		}
		// Float Multiplication Operator Overload
		Vector2 operator*(const float& other) const
		{
			return Vector2(this->X *other, this->Y * other);
		}

		// Positional Variables
		float X;
		float Y;
	};

	// Structure: Vector3
	//
	// Description: A 3D Vector that Holds Positional Data
	struct Vector3
	{
		// Default Constructor
		Vector3()
		{
			X = 0.0f;
			Y = 0.0f;
			Z = 0.0f;
		}
		// Variable Set Constructor
		Vector3(float X_, float Y_, float Z_)
		{
			X = X_;
			Y = Y_;
			Z = Z_;
		}
		// Bool Equals Operator Overload
		bool operator==(const Vector3& other) const
		{
			return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);
		}
		// Bool Not Equals Operator Overload
		bool operator!=(const Vector3& other) const
		{
			return !(this->X == other.X && this->Y == other.Y && this->Z == other.Z);
		}
		// Addition Operator Overload
		Vector3 operator+(const Vector3& right) const
		{
			return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
		}
		// Subtraction Operator Overload
		Vector3 operator-(const Vector3& right) const
		{
			return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
		}
		// Float Multiplication Operator Overload
		Vector3 operator*(const float& other) const
		{
			return Vector3(this->X * other, this->Y * other, this->Z * other);
		}
		// Float Division Operator Overload
		Vector3 operator/(const float& other) const
		{
			return Vector3(this->X / other, this->Y / other, this->Z / other);
		}

		// Positional Variables
		float X;
		float Y;
		float Z;
	};

	// Structure: Vertex
	//
	// Description: Model Vertex object that holds
	//	a Position, Normal, and Texture Coordinate
	struct Vertex
	{
		// Position Vector
		Vector3 Position;

		// Normal Vector
		Vector3 Normal;

		// Texture Coordinate Vector
		Vector2 TextureCoordinate;
	};

	struct Material
	{
		Material()
		{
			name;
			Ns = 0.0f;
			Ni = 0.0f;
			d = 0.0f;
			illum = 0;
		}

		// Material Name
		std::string name;
		// Ambient Color
		Vector3 Ka;
		// Diffuse Color
		Vector3 Kd;
		// Specular Color
		Vector3 Ks;
		// Specular Exponent
		float Ns;

		// Optical Density
		float Ni;

		// Dissolve,  transparent. (0~1.0, 1.0 for no transparent  fully opaque?) some times called "Tr"
		float d;

		// Illumination
		int illum;
		//{
		//		0. Color on and Ambient off
		//		1. Color on and Ambient on
		//		2. Highlight on
		//		3. Reflection on and Ray trace on
		//		4. Transparency: Glass on, Reflection : Ray trace on
		//		5. Reflection : Fresnel on and Ray trace on
		//		6. Transparency : Refraction on, Reflection : Fresnel off and Ray trace on
		//		7. Transparency : Refraction on, Reflection : Fresnel on and Ray trace on
		//		8. Reflection on and Ray trace off
		//		9. Transparency : Glass on, Reflection : Ray trace off
		//		10. Casts shadows onto invisible surfaces
		//}


		// Ambient Texture Map
		std::string map_Ka;
		// Diffuse Texture Map
		std::string map_Kd;
		// Specular Texture Map
		std::string map_Ks;
		// Specular Hightlight Map
		std::string map_Ns;
		// Alpha Texture Map
		std::string map_d;
		// Bump Map
		std::string map_bump;
	};

	// Structure: Mesh
	//
	// Description: A Simple Mesh Object that holds
	//	a name, a vertex list, and an index list
	struct Mesh
	{
	public:
		// Mesh Name
		std::string MeshName;
		// Vertex List
		std::vector<Vertex> Vertices;
		// Index List
		std::vector<unsigned int> Indices;

		// Material
		Material MeshMaterial;




	public:
		// Default Constructor
		Mesh(){
			MeshName.clear();
			Vertices.clear();
			Indices.clear();
		}

		// Variable Set Constructor
		Mesh(const std::vector<Vertex>& _Vertices, const std::vector<unsigned int>& _Indices){

			Vertices = _Vertices;
			Indices = _Indices;
		}
	};

	// Namespace: Math
	//
	// Description: The namespace that holds all of the math
	//	functions need for OBJL
	namespace math
	{
		// Vector3 Cross Product
		Vector3 CrossV3(const Vector3 a, const Vector3 b)
		{
			return Vector3(a.Y * b.Z - a.Z * b.Y,
				a.Z * b.X - a.X * b.Z,
				a.X * b.Y - a.Y * b.X);
		}

		// Vector3 Magnitude Calculation
		float MagnitudeV3(const Vector3 in)
		{
			return (sqrtf(powf(in.X, 2) + powf(in.Y, 2) + powf(in.Z, 2)));
		}

		// Vector3 DotProduct
		float DotV3(const Vector3 a, const Vector3 b)
		{
			return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
		}

		// Angle between 2 Vector3 Objects
		float AngleBetweenV3(const Vector3 a, const Vector3 b)
		{
			float angle = DotV3(a, b);
			angle /= (MagnitudeV3(a) * MagnitudeV3(b));
			return angle = acosf(angle);
		}

		// Projection Calculation of a onto b
		Vector3 ProjV3(const Vector3 a, const Vector3 b)
		{
			Vector3 bn = b / MagnitudeV3(b);
			return bn * DotV3(a, bn);
		}
	}

	// Namespace: Algorithm
	//
	// Description: The namespace that holds all of the
	// Algorithms needed for OBJL
	namespace algorithm
	{
		// Vector3 Multiplication Opertor Overload
		Vector3 operator*(const float& left, const Vector3& right)
		{
			return Vector3(right.X * left, right.Y * left, right.Z * left);
		}

		// A test to see if P1 is on the same side as P2 of a line segment ab
		bool SameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b)
		{
			Vector3 cp1 = math::CrossV3(b - a, p1 - a);
			Vector3 cp2 = math::CrossV3(b - a, p2 - a);

			if (math::DotV3(cp1, cp2) >= 0)
				return true;
			else
				return false;
		}

		// Generate a cross produect normal for a triangle
		Vector3 GenTriNormal(Vector3 t1, Vector3 t2, Vector3 t3)
		{
			Vector3 u = t2 - t1;
			Vector3 v = t3 - t1;

			Vector3 normal = math::CrossV3(u, v);

			return normal;
		}

		// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
		bool inTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3)
		{
			// Test to see if it is within an infinite prism that the triangle outlines.
			bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3)
				&& SameSide(point, tri3, tri1, tri2);

			// If it isn't it will never be on the triangle
			if (!within_tri_prisim)
				return false;

			// Calulate Triangle's Normal
			Vector3 n = GenTriNormal(tri1, tri2, tri3);

			// Project the point onto this normal
			Vector3 proj = math::ProjV3(point, n);

			// If the distance from the triangle to the point is 0
			//	it lies on the triangle
			if (math::MagnitudeV3(proj) == 0)
				return true;
			else
				return false;
		}

		// Split a String into a string array at a given token
		inline void split(const std::string &in,
			std::vector<std::string> &out,
			std::string token)
		{
			out.clear();

			std::string temp;

			for (int i = 0; i < int(in.size()); i++)
			{
				std::string test = in.substr(i, token.size());

				if (test == token)
				{
					if (!temp.empty())
					{
						out.push_back(temp);
						temp.clear();
						i += (int)token.size() - 1;
					}
					else
					{
						out.push_back("");
					}
				}
				else if (i + token.size() >= in.size())
				{
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				}
				else
				{
					temp += in[i];
				}
			}
		}

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string &in)
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t space_start = in.find_first_of(" \t", token_start);
			size_t tail_start = in.find_first_not_of(" \t", space_start);
			size_t tail_end = in.find_last_not_of(" \t");
			if (tail_start != std::string::npos && tail_end != std::string::npos)
			{
				return in.substr(tail_start, tail_end - tail_start + 1);
			}
			else if (tail_start != std::string::npos)
			{
				return in.substr(tail_start);
			}
			return "";
		}

		// Get first token of string
		inline std::string firstToken(const std::string &in)
		{
			if (!in.empty())
			{
				size_t token_start = in.find_first_not_of(" \t");
				size_t token_end = in.find_first_of(" \t", token_start);
				if (token_start != std::string::npos && token_end != std::string::npos)
				{
					return in.substr(token_start, token_end - token_start);
				}
				else if (token_start != std::string::npos)
				{
					return in.substr(token_start);
				}
			}
			return "";
		}

		// Get element at given index position
		template <class T>
		inline const T & getElement(const std::vector<T> &elements, std::string &index)
		{
			int idx = std::stoi(index);
			if (idx < 0)
				idx = int(elements.size()) + idx;
			else
				idx--;
			return elements[idx];
		}
	}

	// Class: Loader
	//
	// Description: The OBJ Model Loader
	class Loader
	{
	public:
		// Loaded Mesh Objects
		std::vector<Mesh> LoadedMeshes;
		// Loaded Vertex Objects
		std::vector<Vertex> LoadedVertices;
		// Loaded Index Positions
		std::vector<unsigned int> LoadedIndices;
		// Loaded Material Objects
		std::vector<Material> LoadedMaterials;

		//max_x, min_x, 
		//max_y, min_y, 
		//max_z, min_z, 
		std::array<Vertex, 6> BoundaryVertex;
		std::array<uint32_t, 6> BoundaryVertex_VID;
		Vector3 InitScaleVec;
		Vector3 InitRotateVec;
		Vector3 InitTransVec;

	public:
		// Default Constructor

		Loader(){
			BoundaryVertex[0].Position.X = -std::numeric_limits<float>::infinity();
			BoundaryVertex[1].Position.X = +std::numeric_limits<float>::infinity();

			BoundaryVertex[2].Position.Y = -std::numeric_limits<float>::infinity();
			BoundaryVertex[3].Position.Y = +std::numeric_limits<float>::infinity();

			BoundaryVertex[4].Position.Z = -std::numeric_limits<float>::infinity();
			BoundaryVertex[5].Position.Z = +std::numeric_limits<float>::infinity();
		}

		~Loader()
		{
			LoadedMeshes.clear();
		}

	public:
		// Load a file into the loader
		//
		// If file is loaded return true
		//
		// If the file is unable to be found
		// or unable to be loaded return false
		bool LoadFile(std::string Path)
		{
			// If the file is not an .obj file return false
			if (Path.substr(Path.size() - 4, 4) != ".obj") {
				assert(0 && "Error: unknown OBJ file Type.");
				return false;
			}
			else {

				std::ifstream file(Path);
				if (!file.is_open()) {
					assert(0 && "Error: fail to open OBJ file.");
					return false;
				}
				else{

					LoadedMeshes.clear();
					LoadedVertices.clear();
					LoadedIndices.clear();

					std::vector<Vector3> Positions;
					std::vector<Vector2> TCoords;
					std::vector<Vector3> Normals;

					std::vector<Vertex> Vertices;
					std::vector<unsigned int> Indices;

					std::vector<std::string> MeshMatNames;

					bool listening = false;
					std::string meshname;

					Mesh tempMesh;

#ifdef OBJL_CONSOLE_OUTPUT
					const unsigned int outputEveryNth = 1000;
					unsigned int outputIndicator = outputEveryNth;
					uint32_t line_idx = 0;
#endif

					std::string curline;
					while (std::getline(file, curline)){

#ifdef OBJL_CONSOLE_OUTPUT
						line_idx++;
						if ((line_idx % 1000) == 0) {
							//std::cout << std::endl << "OBJ_loader: line " << line_idx << " : " << curline << std::endl;
							std::cout << std::endl << "OBJ_loader: line " << line_idx << std::endl;
						}


						if ((outputIndicator = ((outputIndicator + 1) % outputEveryNth)) == 1){
							if (!meshname.empty()){
								std::cout
									<< "\r- " << meshname
									<< "\t| vertices > " << Positions.size()
									<< "\t| texcoords > " << TCoords.size()
									<< "\t| normals > " << Normals.size()
									<< "\t| triangles > " << (Vertices.size() / 3)
									<< (!MeshMatNames.empty() ? "\t| material: " + MeshMatNames.back() : "");
							}
						}
#endif

						// Generate a Mesh Object or Prepare for an object to be created
						if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g" || curline[0] == 'g'){
							if (!listening){
								listening = true;

								if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g")
								{
									meshname = algorithm::tail(curline);
								}
								else
								{
									meshname = "unnamed";
								}
							}
							else
							{
								// Generate the mesh to put into the array

								if (!Indices.empty() && !Vertices.empty()){
									// Create Mesh
									tempMesh = Mesh(Vertices, Indices);
									tempMesh.MeshName = meshname;

									// Insert Mesh
									LoadedMeshes.push_back(tempMesh);

									// Cleanup
									Vertices.clear();
									Indices.clear();
									meshname.clear();

									meshname = algorithm::tail(curline);
								}
								else{
									if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g"){
										meshname = algorithm::tail(curline);
									}
									else{
										meshname = "unnamed";
									}
								}
							}
#ifdef OBJL_CONSOLE_OUTPUT
							std::cout << std::endl;
							outputIndicator = 0;
#endif
						}

						// Generate a Vertex Position
						if (algorithm::firstToken(curline) == "v"){
							std::vector<std::string> spos;
							Vector3 vpos;
							algorithm::split(algorithm::tail(curline), spos, " ");

							vpos.X = std::stof(spos[0]);
							vpos.Y = std::stof(spos[1]);
							vpos.Z = std::stof(spos[2]);

							Positions.push_back(vpos);
						}


						// Generate a Vertex Texture Coordinate
						if (algorithm::firstToken(curline) == "vt"){
							std::vector<std::string> stex;
							Vector2 vtex;
							algorithm::split(algorithm::tail(curline), stex, " ");

							vtex.X = std::stof(stex[0]);
							vtex.Y = std::stof(stex[1]);

							TCoords.push_back(vtex);
						}


						// Generate a Vertex Normal;
						if (algorithm::firstToken(curline) == "vn"){
							std::vector<std::string> snor;
							Vector3 vnor;
							algorithm::split(algorithm::tail(curline), snor, " ");

							vnor.X = std::stof(snor[0]);
							vnor.Y = std::stof(snor[1]);
							vnor.Z = std::stof(snor[2]);

							Normals.push_back(vnor);
						}

						// Generate a Parameter space vertices;
						if (algorithm::firstToken(curline) == "vp") {
							assert(0 && "error: vp currently un-supported.");
						}


						// Generate a Face (vertices & indices)
						if (algorithm::firstToken(curline) == "f"){
							// Generate the vertices
							std::vector<Vertex> vVerts;
							GenVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, curline);

							// Add Vertices
							for (int i = 0; i < int(vVerts.size()); i++)
							{
								Vertices.push_back(vVerts[i]);

								LoadedVertices.push_back(vVerts[i]);
								Update_BoundaryValue(vVerts[i], 0);
							}

							std::vector<unsigned int> iIndices;

							VertexTriangluation(iIndices, vVerts);

							// Add Indices
							for (int i = 0; i < int(iIndices.size()); i++){
								unsigned int indnum = (unsigned int)((Vertices.size()) - vVerts.size()) + iIndices[i];
								Indices.push_back(indnum);

								indnum = (unsigned int)((LoadedVertices.size()) - vVerts.size()) + iIndices[i];
								LoadedIndices.push_back(indnum);
							}

						}


						// Get Mesh Material Name
						if (algorithm::firstToken(curline) == "usemtl"){
							MeshMatNames.push_back(algorithm::tail(curline));

							// Create new Mesh, if Material changes within a group
							if (!Indices.empty() && !Vertices.empty()){
								// Create Mesh
								tempMesh = Mesh(Vertices, Indices);
								tempMesh.MeshName = meshname;
								int i = 2;
								while (1) {
									tempMesh.MeshName = meshname + "_" + std::to_string(i);

									for (auto &m : LoadedMeshes)
										if (m.MeshName == tempMesh.MeshName)
											continue;
									break;
								}

								// Insert Mesh
								LoadedMeshes.push_back(tempMesh);

								// Cleanup
								Vertices.clear();
								Indices.clear();
							}

#ifdef OBJL_CONSOLE_OUTPUT
							outputIndicator = 0;
#endif
						}



						// Load Materials
						if (algorithm::firstToken(curline) == "mtllib"){
							// Generate LoadedMaterial

							// Generate a path to the material file
							std::vector<std::string> temp;
#ifdef __GNUC__
							algorithm::split(Path, temp, "/");
#endif

#ifdef   _MSC_VER
							algorithm::split(Path, temp, "\\");
#endif





							std::string pathtomat = "";

							if (temp.size() != 1)
							{
								for (int i = 0; i < temp.size() - 1; i++)
								{
#ifdef __GNUC__
									pathtomat += temp[i] + "/";
#endif

#ifdef   _MSC_VER
									pathtomat += temp[i] + "\\";
#endif
								}
							}


							

#ifdef OBJL_CONSOLE_OUTPUT
							std::cout << std::endl << "- find materials in: " << pathtomat << std::endl;
#endif
							
							std::string mtl_file_name= algorithm::tail(curline);
							//
							//pathtomat+= algorithm::tail(curline);
							// Load Materials 
							// LoadMaterials(pathtomat);
							//

							LoadMaterials(pathtomat, mtl_file_name);
						}

					}
					file.close();

#ifdef OBJL_CONSOLE_OUTPUT
					std::cout << std::endl;
#endif

					// Deal with last mesh
					if (!Indices.empty() && !Vertices.empty()){
						// Create Mesh
						tempMesh = Mesh(Vertices, Indices);
						tempMesh.MeshName = meshname;

						// Insert Mesh
						LoadedMeshes.push_back(tempMesh);
					}

					


					// Set Materials for each Mesh
					for (int i = 0; i < MeshMatNames.size(); i++){
						std::string matname = MeshMatNames[i];

						// Find corresponding material name in loaded materials
						// when found copy material variables into mesh material
						for (int j = 0; j < LoadedMaterials.size(); j++){
							if (LoadedMaterials[j].name == matname){
								LoadedMeshes[i].MeshMaterial = LoadedMaterials[j];
								break;
							}
						}
					}

					if (LoadedMeshes.empty() && LoadedVertices.empty() && LoadedIndices.empty()){

						cout << "ERROR: Fail to load Mesh!" << endl;
						assert(0 && "ERROR: Fail to load Mesh!");

						return false;
					}

					else{
						std::cout <<std::endl<<std::endl << "Meshes in the OBJ file, Init Arguments:" << std::endl <<std::endl;

						float max_x = BoundaryVertex[0].Position.X;
						float min_x = BoundaryVertex[1].Position.X;

						float max_y = BoundaryVertex[2].Position.Y;
						float min_y = BoundaryVertex[3].Position.Y;

						float max_z = BoundaryVertex[4].Position.Z;
						float min_z = BoundaryVertex[5].Position.Z;


						float x_length = (max_x - min_x);
						float y_length = (max_y - min_y);
						float z_length = (max_z - min_z);


						assert((max_x >= min_x) && "error, the max value must greater than min value");
						assert((max_y >= min_y) && "error, the max value must greater than min value");
						assert((max_z >= min_z) && "error, the max value must greater than min value");

						assert((x_length >= 0) && "error: the mesh length on X dimension must greater than 0.0f.");
						assert((y_length >= 0) && "error: the mesh length on Y dimension must greater than 0.0f.");
						assert((z_length >= 0) && "error: the mesh length on Z dimension must greater than 0.0f.");


						float mid_x = (x_length / 2) + min_x;
						float mid_y = (y_length / 2) + min_y;
						float mid_z = (z_length / 2) + min_z;

						cout << "\t max_x: " << max_x << endl;
						cout << "\t min_x: " << min_x << endl;

						cout << "\t max_y: " << max_y << endl;
						cout << "\t min_y: " << min_y << endl;

						cout << "\t max_z: " << max_z << endl;
						cout << "\t min_z: " << min_z << endl;


						//step 1. get init scale matrix
						cout << "\t length on 3 dimension: " << "(" << x_length << ", " << y_length << ", " << z_length << ")" << std::endl;
						float max_len = max(max(x_length, y_length), z_length);
						float scale_arg = (1.0 / max_len);  //we force the max_demension to 1.0.
						InitScaleVec = Vector3(scale_arg, scale_arg, scale_arg);
						cout << "\t init_scale_vec: (" << InitScaleVec.X << "," << InitScaleVec.Y << "," << InitScaleVec.Z << ")" << std::endl;


						//step 2. get init rotate matrix,
						//actually currently do not do do init rotate.
						InitRotateVec = Vector3(0.0f, 0.0f, 0.0f);
						std::cout << "\t init_rotate_vec: (" << InitRotateVec.X << "," << InitRotateVec.Y << "," << InitRotateVec.Z << ")" << std::endl;


						//step 3. get init shift matrix
						std::cout << "\t mesh mid_pos : " << " \t(" << mid_x << ", " << mid_y << ", " << mid_z << ")" << std::endl;
						InitTransVec = Vector3(0.0f - mid_x, 0.0f - mid_y, 0.0f - mid_z);
						std::cout << "\t move mesh mid_pos to origin(0, 0, 0), init_trans_vec: (" << InitTransVec.X << "," << InitTransVec.Y << "," << InitTransVec.Z << ")" << std::endl;

						std::cout << std::endl << std::endl;

						return true;
					}
				}
			}

			




		}


		void Update_BoundaryValue(const Vertex& t_vertex, uint32_t t_vid){



			//Updata max_x 
			if (t_vertex.Position.X > BoundaryVertex[0].Position.X) {
				BoundaryVertex[0] = t_vertex;
				BoundaryVertex_VID[0] = t_vid;
			}

			//Updata min_x 
			if (t_vertex.Position.X < BoundaryVertex[1].Position.X) {
				BoundaryVertex[1] = t_vertex;
				BoundaryVertex_VID[1] = t_vid;
			}

			//Updata max_y 
			if (t_vertex.Position.Y > BoundaryVertex[2].Position.Y) {
				BoundaryVertex[2] = t_vertex;
				BoundaryVertex_VID[2] = t_vid;
			}

			//Updata min_y
			if (t_vertex.Position.Y < BoundaryVertex[3].Position.Y) {
				BoundaryVertex[3] = t_vertex;
				BoundaryVertex_VID[3] = t_vid;
			}

			//Updata max_z 
			if (t_vertex.Position.Z > BoundaryVertex[4].Position.Z) {
				BoundaryVertex[4] = t_vertex;
				BoundaryVertex_VID[4] = t_vid;
			}

			//Updata min_z 
			if (t_vertex.Position.Z < BoundaryVertex[5].Position.Z) {
				BoundaryVertex[5] = t_vertex;
				BoundaryVertex_VID[5] = t_vid;
			}

		}



	private:
		// Generate vertices from a list of positions, 
		//	tcoords, normals and a face line
		void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
			const std::vector<Vector3>& iPositions,
			const std::vector<Vector2>& iTCoords,
			const std::vector<Vector3>& iNormals,
			std::string icurline)
		{
			std::vector<std::string> sface, svert;
			Vertex vVert;
			algorithm::split(algorithm::tail(icurline), sface, " ");

			//assert((sface.size()==3) && "Error: currently only Support Triangle.");




			bool noNormal = false;

			// For every given vertex do this
			for (int i = 0; i < int(sface.size()); i++)
			{
				// See What type the vertex is.
				int vtype;

				algorithm::split(sface[i], svert, "/");

				// Check for just position - v1
				if (svert.size() == 1)
				{
					// Only position
					vtype = 1;
				}

				// Check for position & texture - v1/vt1
				if (svert.size() == 2)
				{
					// Position & Texture
					vtype = 2;
				}

				// Check for Position, Texture and Normal - v1/vt1/vn1
				// or if Position and Normal - v1//vn1
				if (svert.size() == 3)
				{
					if (svert[1] == "")
					{
						// Position & Normal
						vtype = 3;
					}
					else
					{
						// Position, Texture, and Normal
						vtype = 4;
					}
				}

				// Calculate and store the vertex
				switch (vtype)
				{
				case 1: // P
				{
					vVert.Position = algorithm::getElement(iPositions, svert[0]);
					vVert.TextureCoordinate = Vector2(0, 0);
					noNormal = true;
					oVerts.push_back(vVert);
				}
				break;

				case 2: // P/T
				{
					vVert.Position = algorithm::getElement(iPositions, svert[0]);
					vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
					noNormal = true;
					oVerts.push_back(vVert);
				}
				break;

				case 3: // P//N
				{
					vVert.Position = algorithm::getElement(iPositions, svert[0]);
					vVert.TextureCoordinate = Vector2(0, 0);
					vVert.Normal = algorithm::getElement(iNormals, svert[2]);
					oVerts.push_back(vVert);
				}
				break;
				case 4: // P/T/N
				{
					vVert.Position = algorithm::getElement(iPositions, svert[0]);
					vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
					vVert.Normal = algorithm::getElement(iNormals, svert[2]);
					oVerts.push_back(vVert);
				}
				break;
				default:
				{
					assert(0 && "error: Unknown vetex type.");
				}
				break;
				}
			}

			// take care of missing normals
			// these may not be truly acurate but it is the 
			// best they get for not compiling a mesh with normals	
			if (noNormal)
			{
				Vector3 A = oVerts[0].Position - oVerts[1].Position;
				Vector3 B = oVerts[2].Position - oVerts[1].Position;

				Vector3 normal = math::CrossV3(A, B);

				for (int i = 0; i < int(oVerts.size()); i++)
				{
					oVerts[i].Normal = normal;
				}
			}
		}

		// Triangulate a list of vertices into a face by printing
		//	inducies corresponding with triangles within it
		void VertexTriangluation(std::vector<unsigned int>& oIndices,
			const std::vector<Vertex>& iVerts)
		{
			// If there are 2 or less verts,
			// no triangle can be created,
			// so exit
			if (iVerts.size() < 3)
			{
				return;
			}
			// If it is a triangle no need to calculate it
			if (iVerts.size() == 3)
			{
				oIndices.push_back(0);
				oIndices.push_back(1);
				oIndices.push_back(2);
				return;
			}

			// Create a list of vertices
			std::vector<Vertex> tVerts = iVerts;

			while (true)
			{
				// For every vertex
				for (int i = 0; i < int(tVerts.size()); i++)
				{
					// pPrev = the previous vertex in the list
					Vertex pPrev;
					if (i == 0)
					{
						pPrev = tVerts[tVerts.size() - 1];
					}
					else
					{
						pPrev = tVerts[i - 1];
					}

					// pCur = the current vertex;
					Vertex pCur = tVerts[i];

					// pNext = the next vertex in the list
					Vertex pNext;
					if (i == tVerts.size() - 1)
					{
						pNext = tVerts[0];
					}
					else
					{
						pNext = tVerts[i + 1];
					}

					// Check to see if there are only 3 verts left
					// if so this is the last triangle
					if (tVerts.size() == 3)
					{
						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(tVerts.size()); j++)
						{
							if (iVerts[j].Position == pCur.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pPrev.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pNext.Position)
								oIndices.push_back(j);
						}

						tVerts.clear();
						break;
					}
					if (tVerts.size() == 4)
					{
						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(iVerts.size()); j++)
						{
							if (iVerts[j].Position == pCur.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pPrev.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pNext.Position)
								oIndices.push_back(j);
						}

						Vector3 tempVec;
						for (int j = 0; j < int(tVerts.size()); j++)
						{
							if (tVerts[j].Position != pCur.Position
								&& tVerts[j].Position != pPrev.Position
								&& tVerts[j].Position != pNext.Position)
							{
								tempVec = tVerts[j].Position;
								break;
							}
						}

						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(iVerts.size()); j++)
						{
							if (iVerts[j].Position == pPrev.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pNext.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == tempVec)
								oIndices.push_back(j);
						}

						tVerts.clear();
						break;
					}

					// If Vertex is not an interior vertex
					float angle = math::AngleBetweenV3(pPrev.Position - pCur.Position, pNext.Position - pCur.Position) * (180 / 3.14159265359);
					if (angle <= 0 && angle >= 180)
						continue;

					// If any vertices are within this triangle
					bool inTri = false;
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (algorithm::inTriangle(iVerts[j].Position, pPrev.Position, pCur.Position, pNext.Position)
							&& iVerts[j].Position != pPrev.Position
							&& iVerts[j].Position != pCur.Position
							&& iVerts[j].Position != pNext.Position)
						{
							inTri = true;
							break;
						}
					}
					if (inTri)
						continue;

					// Create a triangle from pCur, pPrev, pNext
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (iVerts[j].Position == pCur.Position)
							oIndices.push_back(j);
						if (iVerts[j].Position == pPrev.Position)
							oIndices.push_back(j);
						if (iVerts[j].Position == pNext.Position)
							oIndices.push_back(j);
					}

					// Delete pCur from the list
					for (int j = 0; j < int(tVerts.size()); j++)
					{
						if (tVerts[j].Position == pCur.Position)
						{
							tVerts.erase(tVerts.begin() + j);
							break;
						}
					}

					// reset i to the start
					// -1 since loop will add 1 to it
					i = -1;
				}

				// if no triangles were created
				if (oIndices.size() == 0)
					break;

				// if no more vertices
				if (tVerts.size() == 0)
					break;
			}
		}

		// Load Materials from .mtl file
		bool LoadMaterials(const std::string& path, const std::string& file_name)
		{
			std::string full_name = path + file_name;

			// If the file is not a material file return false
			if (full_name.substr(full_name.size() - 4, full_name.size()) != ".mtl"){
				assert(0 && "error: invalid Material file type!");
				return false;
			}
			else
			{
				std::ifstream file(full_name);

				// If the file is not found return false
				if (!file.is_open()) {
					assert(0 && "error: fail to open Material file!");
					return false;
				}
				else
				{
					//FIXME: the path to texture may need refine.

					Material tempMaterial;

					bool listening = false;

					// Go through each line looking for material variables
					std::string curline;
					while (std::getline(file, curline))
					{
						// new material and material name
						if (algorithm::firstToken(curline) == "newmtl")
						{
							if (!listening)
							{
								listening = true;

								if (curline.size() > 7)
								{
									tempMaterial.name = algorithm::tail(curline);
								}
								else
								{
									tempMaterial.name = "none";
								}
							}
							else
							{
								// Generate the material

								// Push Back loaded Material
								LoadedMaterials.push_back(tempMaterial);

								// Clear Loaded Material
								tempMaterial = Material();

								if (curline.size() > 7)
								{
									tempMaterial.name = algorithm::tail(curline);
								}
								else
								{
									tempMaterial.name = "none";
								}
							}
						}
						// Ambient Color
						if (algorithm::firstToken(curline) == "Ka")
						{
							std::vector<std::string> temp;
							algorithm::split(algorithm::tail(curline), temp, " ");

							if (temp.size() != 3)
								continue;

							tempMaterial.Ka.X = std::stof(temp[0]);
							tempMaterial.Ka.Y = std::stof(temp[1]);
							tempMaterial.Ka.Z = std::stof(temp[2]);
						}
						// Diffuse Color
						if (algorithm::firstToken(curline) == "Kd")
						{
							std::vector<std::string> temp;
							algorithm::split(algorithm::tail(curline), temp, " ");

							if (temp.size() != 3)
								continue;

							tempMaterial.Kd.X = std::stof(temp[0]);
							tempMaterial.Kd.Y = std::stof(temp[1]);
							tempMaterial.Kd.Z = std::stof(temp[2]);
						}
						// Specular Color
						if (algorithm::firstToken(curline) == "Ks")
						{
							std::vector<std::string> temp;
							algorithm::split(algorithm::tail(curline), temp, " ");

							if (temp.size() != 3)
								continue;

							tempMaterial.Ks.X = std::stof(temp[0]);
							tempMaterial.Ks.Y = std::stof(temp[1]);
							tempMaterial.Ks.Z = std::stof(temp[2]);
						}
						// Specular Exponent
						if (algorithm::firstToken(curline) == "Ns")
						{
							tempMaterial.Ns = std::stof(algorithm::tail(curline));
						}
						// Optical Density
						if (algorithm::firstToken(curline) == "Ni")
						{
							tempMaterial.Ni = std::stof(algorithm::tail(curline));
						}

						// Dissolve
						if (algorithm::firstToken(curline) == "d"){
							tempMaterial.d = std::stof(algorithm::tail(curline));
						}

						if (algorithm::firstToken(curline) == "Tr"){
							tempMaterial.d = (1.0f - std::stof(algorithm::tail(curline)));
						}

						// Illumination
						if (algorithm::firstToken(curline) == "illum")
						{
							tempMaterial.illum = std::stoi(algorithm::tail(curline));
						}
						// Ambient Texture Map
						if (algorithm::firstToken(curline) == "map_Ka")
						{
							tempMaterial.map_Ka = path + algorithm::tail(curline);

							{
								std::string _file_name_ = tempMaterial.map_Ka;
								std::ifstream _t_file_(_file_name_);
								// If the file is not found, report error.
								if (!_t_file_.is_open()) {
									std::cout << "error: fail to open Material file, please check file path:" << _file_name_ << std::endl;
									assert(0 && "error: fail to open Material file, please check file path!");
								}
								_t_file_.close();
							}
							
						}
						// Diffuse Texture Map
						if (algorithm::firstToken(curline) == "map_Kd")
						{
							tempMaterial.map_Kd = path + algorithm::tail(curline);

							{
								std::string _file_name_ = tempMaterial.map_Kd;
								std::ifstream _t_file_(_file_name_);
								// If the file is not found, report error.
								if (!_t_file_.is_open()) {
									std::cout << "error: fail to open Material file, please check file path:" << _file_name_ << std::endl;
									assert(0 && "error: fail to open Material file, please check file path!");
								}
								_t_file_.close();
							}

						}
						// Specular Texture Map
						if (algorithm::firstToken(curline) == "map_Ks")
						{
							tempMaterial.map_Ks = path + algorithm::tail(curline);

							{
								std::string _file_name_ = tempMaterial.map_Ks;
								std::ifstream _t_file_(_file_name_);
								// If the file is not found, report error.
								if (!_t_file_.is_open()) {
									std::cout << "error: fail to open Material file, please check file path:" << _file_name_ << std::endl;
									assert(0 && "error: fail to open Material file, please check file path!");
								}
								_t_file_.close();
							}

						}
						// Specular Hightlight Map
						if (algorithm::firstToken(curline) == "map_Ns")
						{
							tempMaterial.map_Ns = path + algorithm::tail(curline);

							{
								std::string _file_name_ = tempMaterial.map_Ns;
								std::ifstream _t_file_(_file_name_);
								// If the file is not found, report error.
								if (!_t_file_.is_open()) {
									std::cout << "error: fail to open Material file, please check file path:" << _file_name_ << std::endl;
									assert(0 && "error: fail to open Material file, please check file path!");
								}
								_t_file_.close();
							}

						}
						// Alpha Texture Map
						if (algorithm::firstToken(curline) == "map_d")
						{
							tempMaterial.map_d = path + algorithm::tail(curline);

							{
								std::string _file_name_ = tempMaterial.map_d;
								std::ifstream _t_file_(_file_name_);
								// If the file is not found, report error.
								if (!_t_file_.is_open()) {
									std::cout << "error: fail to open Material file, please check file path:" << _file_name_ << std::endl;
									assert(0 && "error: fail to open Material file, please check file path!");
								}
								_t_file_.close();
							}

						}
						// Bump Map
						if (algorithm::firstToken(curline) == "map_Bump" || algorithm::firstToken(curline) == "map_bump" || algorithm::firstToken(curline) == "bump")
						{
							tempMaterial.map_bump = path + algorithm::tail(curline);

							{
								std::string _file_name_ = tempMaterial.map_bump;
								std::ifstream _t_file_(_file_name_);
								// If the file is not found, report error.
								if (!_t_file_.is_open()) {
									std::cout << "error: fail to open Material file, please check file path:" << _file_name_ << std::endl;
									assert(0 && "error: fail to open Material file, please check file path!");
								}
								_t_file_.close();
							}
						}
					}

					// Deal with last material
					// Push Back loaded Material
					LoadedMaterials.push_back(tempMaterial);
				}

				// Test to see if anything was loaded

				if (LoadedMaterials.empty()) {
#ifdef OBJL_CONSOLE_OUTPUT
					std::cout << std::endl << "WARNNING: Load zero material, may be the Materials is empty. " << std::endl;
					std::cout << std::endl << "WARNNING: Load zero material, may be the Materials is empty. " << std::endl;
					std::cout << std::endl << "WARNNING: Load zero material, may be the Materials is empty. " << std::endl;
#endif
					// If not return false
					return false;
				}
				else {

#ifdef OBJL_CONSOLE_OUTPUT
					std::cout << std::endl << "Material: "<< path <<" load success."<< std::endl;
#endif
					// If so return true
					return true;
				}
			}
		}

	};
}

#endif // ! __OBJ_LOADER_H__
