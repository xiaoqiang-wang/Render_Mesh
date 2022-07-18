#ifndef __MESH_H__
#define __MESH_H__
#pragma once



#include <iostream>
#include <time.h>       /* time */
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <vector>
#include <assert.h>
#include <windows.h>

using namespace std;

//SOIL
#include <SOIL/SOIL.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


//GLM
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;


#include "Common.h"

#include "Texture.h"

#include "Material.h"







std::string Gen_simple_FS(glm::uvec3 color_rgb);






#define OGL

#ifdef OGL

#define RIGHT_X_BOUNDARY 1.0    //right X bounary of normalize space.
#define LEFT_X_BOUNDARY -1.0    //left X bounary of normalize space.

#define UPPER_Y_BOUNDARY 1.0   //upper Y bounary of normalize space.
#define DOWN_Y_BOUNDARY -1.0   //down Y bounary of normalize space.

#define FAR_Z_BOUNDARY 1.0     //FAR Z bounary of normalize space.
#define NEAR_Z_BOUNDARY -1.0   //NEAR Z bounary of normalize space.


#else
#define RIGHT_BOUNDARY 1.0
#define LEFT_BOUNDARY  0.0
#define UPPER_BOUNDARY 1.0
#define DOWN_BOUNDARY  0.0
#endif


class Triangle {
public:
	glm::vec4 position[3];
	Triangle(glm::vec4 position_0, glm::vec4 position_1, glm::vec4 position_2) {
		position[0] = position_0;
		position[1] = position_1;
		position[2] = position_2;
	};
	uint32_t is_out_of_screen();
};


class Point {
public:
	glm::vec4 position[1];
	Point(glm::vec4 position_0) {
		position[0] = position_0;
	};
	uint32_t is_out_of_screen();
};






class Attributes
{
public:
	std::string name;
	uint32_t start_idx = 0; //attri start line index.
	uint32_t dimension = 0; //vec1? vec2? vec3? vec4?
	bool    is_position = false; //whether the attributes is position.
	std::vector<glm::vec4> attr;

	Attributes() {};
	~Attributes() {};
private:
};




class Mesh_config
{
public:


	Mesh_config() {};
	~Mesh_config() {};
	bool to_draw_mesh_vertex;
	bool to_draw_BoundaryVertex;
	bool to_draw_BoundaryVertex_connect;

	bool to_draw_mesh_line;
	bool to_draw_mesh_surface;

	bool to_draw_boundingbox_vertex;
	bool to_draw_boundingbox_line;
	bool to_draw_boundingbox_surface;


	glm::uvec3 mesh_vertex_color            = glm::uvec3(0.0f, 0.0f, 0.0f);// black vertex.
	glm::uvec3 BoundaryVertex         = glm::uvec3(1.0f, 0.0f, 0.0f);//red boundary vertex 
	glm::uvec3 BoundaryVertex_connect = glm::uvec3(43, 243, 66);//green connect

	glm::uvec3 mesh_line                    = glm::uvec3(0.0f, 0.0f, 0.0f); //black line.
	glm::uvec3 mesh_surface                 = glm::uvec3(1.0f, 0.0f, 0.0f);//red surface.

	glm::uvec3 boundingbox_vertex           = glm::uvec3(241, 104, 87);//red boundingbox vertex 
	glm::uvec3 boundingbox_line             = glm::uvec3(0.0f, 1.0f, 0.0f);//green boundingbox line.
	glm::uvec3 boundingbox_surface          = glm::uvec3(1.0f, 0.0f, 0.0f);//red boundingbox surface.

	bool is_need_to_draw[8] = { 0 };//the 8 configure.
	GLuint program[8] = { 0 };//the 8 program.	

private:
};






class GPUProgram{
public:
	GPUProgram();
	~GPUProgram();
	GLuint getId();
	//void attachShader(Shader& _shader);
	void use();

private:
	GLuint id;
};



//Fixme: TODO furture use  those type.
struct VertexAttribute{
	std::string name;
	GLint size;  //Must be 1, 2, 3, 4.
	GLenum type; //
	//GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by glVertexAttribPointer and glVertexAttribIPointer.
	//GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted by glVertexAttribPointer.
	//GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only token accepted by the type parameter for that function.
	GLboolean normalized;
};

struct Uniform{
	std::string name;
	GLint size;  //Must be 1, 2, 3, 4.
	GLenum type; //
};


struct Texture{
	std::string name;
	GLint size;  //Must be 1, 2, 3, 4.
	GLenum type; //
};


//a RenderMode means
// 1. mesh general Tringle surface Mode
// 2. mesh Line
// 3. mesh Vertices
class Render_Mode_1
{
public:
	bool enable    = false;
	GLuint Program = 0;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;

	GLuint vertex_count = 0;
	GLuint index_count  = 0;

	//surface mode,
	//
	// line mode 
	//
	// vertex mode
	//
	// 透明 mesh
	GLuint Textures[32] = {0};

public:
	Render_Mode_1() {};
	~Render_Mode_1() {};
	void Draw() {

		std::vector<std::string> Attributes_name;
		std::vector<std::string> Uniforms_name;
		std::vector<std::string> Textures_name;

		//BIND Program.
		VERIFY_GL(glValidateProgram(Program));
		VERIFY_GL(glUseProgram(Program));

		// VERIFY_GL(glBindBufferRange(GL_UNIFORM_BUFFER, 1, 1132, 256, 128));

		
		// glEnable(GL_CULL_FACE);
		// glEnable(GL_DEPTH_TEST);

		VERIFY_GL(glBindVertexArray(VAO));
		for (uint32_t i = 0; i < Textures_name.size(); i++) {
			
			std::string t_sampler_name = Textures_name[i];
			GLuint t_sampler_location  = Query_Sampler_Location(Program, t_sampler_name);

			//Enable_Attribute_Location(Program, t_texture_name);
		}


		VERIFY_GL(glBindVertexArray(VAO));
		for (uint32_t i = 0; i < Attributes_name.size();i++){
			std::string t_attr_name = Attributes_name[i];
			Enable_Attribute_Location(Program, t_attr_name);
		}
		
		/////////////////////////////////////////////////////////
		{
			//VERIFY_GL(glDrawArrays(GL_TRIANGLES, 0, vertex_count));//FIXME: todo
			//VERIFY_GL(glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0));
			VERIFY_GL(glDrawElements(GL_LINES, index_count, GL_UNSIGNED_INT, 0));

			// float default_point_size = 10.5f;
			// VERIFY_GL(glEnable(GL_PROGRAM_POINT_SIZE));
			// VERIFY_GL(glPointSize(default_point_size));

			//VERIFY_GL(glDrawElements(GL_POINTS, mesh_vertex_count, GL_UNSIGNED_INT, 0));
			//VERIFY_GL(glDrawElements(GL_POINTS, 36, GL_UNSIGNED_INT, 0));
		}

		Reset_GL_Context();
	};
};

// Structure: Vertex
//
// Description: Model Vertex object that holds
//	a Position, Normal, and Texture Coordinate
struct Vertex
{
	// Position Vector
	glm::vec3 Position;

	// Normal Vector
	glm::vec3 Normal;

	// Texture Coordinate Vector
	glm::vec2 TextureCoordinate;

	//Fixme: TODO we may need to consider the tagnent
	//const std::vector<glm::vec3>& _Tangent ,
};

class Mesh {
private:
	// Vertex List
	std::vector<Vertex> Vertices;
	
	// Index List
	std::vector<uint32_t> Indices;

	// Material
	Material material;

private:
	//the 6 bounding vertex on mesh. which own (max_x, min_x), (max_y, min_y), (max_z, min_z)
	std::array<Vertex, 6> BoundaryVertex;

	//VID which own max_x, min_x, max_y, min_y, max_z, min_z
	std::array<uint32_t, 6> BoundaryVertex_VID;

	//OK the bounding box to save the mesh.
	std::array<glm::vec3, 8> BoundingBox;

	//
	GLfloat boundingSphere = 0;

private:
	//************* a mesh have those data *************
	// 1.attributes.
	// 2.Programs
	// 2.uniforms;
	// 3.textures
	//

	//********* a mesh can draw*************
	// 1. surface  vertex count.
	// 2. line.
	// 3. vertex 
private:	
	glm::mat4 ModelMatrix = glm::mat4(1.0);

	
	GLenum Render_Mode = GL_TRIANGLES;
	// GL_TRIANGLES;
	// GL_POINTS;
	// GL_LINES;

	GLuint Triangle_Mode_Program     = 0;
	GLuint Triangle_Mode_VAO         = 0;


	GLuint Line_Mode_Surface_Program = 0;
	GLuint Line_Mode_Line_Program    = 0;
	GLuint Line_Mode_VAO             = 0;

	GLuint Point_Mode_Surface_Program = 0;
	GLuint Point_Mode_Line_Program    = 0;
	GLuint Point_Mode_Point_Program   = 0;
	GLuint Point_Mode_VAO             = 0;





	GLuint ambientTextureId  = 0;
	GLuint diffuseTextureId  = 0;
	GLuint normalTextureId   = 0;
	GLuint specularTextureId = 0;


	
	//Render_Mode_1 Triangle, Line, Point;

	//FIXME: todo
	//Render_Mode_1 viewBounaryVertx_mode, BounaryVertx_connect, BoundingBoxVertex, BoundingBox_connect, BoundingBox_surface;


	GLuint Gen_Triangle_Mode_VAO(const std::vector<string> &attribute_list);

	GLuint gen_Triangle_Mode_data();
	GLuint gen_Line_Mode_data();
	GLuint gen_Point_Mode_data();


private:
	void Updata_BoundaryVertex(const Vertex &t_vertex, const GLuint t_vid);


public:
	GLuint Progam = 0;//attribute, uniforms/UBOs, texture, //FBO.
	

	GLuint Gen_VAO(
		const std::vector<Vertex>& t_Vertices,
		const std::vector<Vertex>& t_Indices,
		const std::vector<string>& attribute_list);

	std::string name;
	//static std::vector<class Mesh *> all_mesh;


	Mesh_config config;

	

	static uint32_t global_mesh_instance_count;
	uint32_t mesh_id = 0;  //openGL

	//Generally: we DO NOT consider a mesh is a rigid body.
	//
	bool is_rigid   = false;
	bool is_visible = false;

//private:
	bool to_get_local_vertex = true;
	uint32_t  local_max_x_vid = -1;
	uint32_t  local_min_x_vid = -1;
	uint32_t  local_max_y_vid = -1;
	uint32_t  local_min_y_vid = -1;
	uint32_t  local_max_z_vid = -1;
	uint32_t  local_min_z_vid = -1;

	
	bool is_out_side_normal_space = true; //is mesh out of normalize space [-1,1]?
	bool where_the_mesh[6] = { 
		false, //mesh'x greather than x=1.0
		false, //mesh'x less than x=-1.0

		false, //mesh'y greather than y=1.0
		false, //mesh'y less than y=-1.0

		false, //mesh'z greather than z=1.0
		false  //mesh'z less than z=-1.0
	};
	


	//how many vertices on the mesh.
	uint32_t mesh_vertex_count = 0;

	

	GLuint textures[16] = { 0 };


private:	
	//the buffer only save mesh position.
	GLfloat *mesh_position_buffer     = nullptr;
	GLfloat *mesh_normal_buffer       = nullptr;
	GLfloat *mesh_textureCoord_buffer = nullptr;


public:


	static const uint32_t MAX_NUM_OF_ATTRIBUTTES = 16;    // OpenGL spec
	uint32_t num_of_attributes              = 0;                       // how many attributes a mesh have.
	uint32_t num_of_attributes_components   = 0;                       // how many attributes component a mesh have.

	std::vector<std::string> mesh_candidate_in_position_name = { "in_pos","in_Pos", "in_position","in_ATTRIBUTE0", "gl_Position" };
	//std::vector<Attributes> mesh_attributes;

	Attributes mesh_attributes[MAX_NUM_OF_ATTRIBUTTES];


	//float max_value[3] = { 0.0f, 0.0f, 0.0f };
	//float min_value[3] = { 0.0f, 0.0f, 0.0f };

	
	glm::vec3 trans_vec  = glm::vec3(0.0f); //init shift vec
	glm::vec3 scale_vec  = glm::vec3(1.0f); //init scale vec
    glm::vec3 rotate_vec = glm::vec3(0.0f); //init roate vec in degree
  
	//the init_model_matrix, used to froce the mesh to screen centry.
	glm::mat4 init_model_matrix = glm::mat4(1.0f);






private:
	std::vector<glm::vec3> BoundaryVertex_local;

	//the bounding 8 box vertex
	std::array<glm::vec4, 8>  mesh_boundingbox_vertex;
	std::array<glm::vec4, 8>  mesh_boundingbox_vertex_local;

	//PROGRAMS
	GLuint mesh_vertex_Program                  = 0;
	GLuint mesh_line_Program                    = 0;
	
	GLuint BoundaryVertex_Program               = 0;
	GLuint BoundaryVertex_connect_Program       = 0;

	GLuint mesh_boundingBox_vertex_Program      = 0;
	GLuint mesh_boundingBox_line_Program        = 0;
	GLuint mesh_boundingBox_surface_Program     = 0;






	//for triangle surfaces
	//GLuint mesh_surface_VA0                     = 0;
	//GLuint mesh_line_VA0                        = 0;

	//for GL_TRIANGLES mode.
	GLuint mesh_vertex_VA0                      = 0;
	GLuint mesh_triangle_line_VA0               = 0;

	//for GL_POINTS
	GLuint mesh_point_VAO                       = 0;


	GLuint boundary_vertex_VAO                  = 0; //the 6 boundary vertex on the mesh.
	GLuint boundingbox_vertex_VA0               = 0; //the 8 boundingbox vertex on the mesh.
	GLuint boundingbox_surface_VA0              = 0;
	GLuint boundingbox_line_VA0                 = 0;



	//return vertices count.
	GLuint Read_Mesh_Vertex(const std::string &file_name);
	

	GLuint Gen_Mesh_Vertex_VAO();
	GLuint Gen_Mesh_Triangle_Line_VAO();

	GLuint Gen_Boundary_Vertex_VAO();
	
	void Gen_Init_Model_Matrix(
		bool need_init_model_matrix,
		bool use_global_init,
		const glm::vec3& t_Init_Scale,
		const glm::vec3& t_Init_Rotate,
		const glm::vec3& t_Init_Trans
	);


	void Gen_BoundingBox_Vertex();
	GLuint Gen_BoundingBox_Vertex_VAO();
	GLuint Gen_BoundingBox_Surface_VAO();
	GLuint Gen_BoundingBox_Line_VAO();




	/////////////////////////

#ifdef show_local
	GLuint boundary_vertex_VAO_local = 0;//the 6 boundary vertex on the mesh.

	GLuint boundingbox_vertex_VAO_local = 0; //the 8 boundingbox vertex on the mesh.
	GLuint boundingbox_surface_VAO_local = 0;
	GLuint boundingbox_line_VAO_local = 0;


	GLuint Gen_Boundary_Vertex_VAO_local();
	void Gen_BoundingBox_Vertex_local();
	GLuint Gen_BoundingBox_Vertex_VAO_local();
	GLuint Gen_BoundingBox_Surface_VAO_local();
	GLuint Gen_BoundingBox_Line_VAO_local();
#endif
	///////////////////////
public:
	Mesh(std::string mesh_input_data, 
		std::string vs_src, 
		std::string fs_src,
		std::string line_vs,
		std::string line_fs,
		GLenum primitive_type,
		bool is_word_space) {
		//input_file = mesh_input_data;
		//vs_str = vs_src;
		//fs_str = fs_src;
		//
		//Triangle_Mode_Program = CreateProgram(vs_str,fs_str);
		//
		//assert(is_word_space==true && "error: need to be word space!");
		//Read_VS_output(input_file, vertex_count, &vertex_buffer);
		//
		////Read_Mesh_in_local_space(input_file, vertex_count, &vertex_buffer);
		//
		//
		//VAO = CreateVAO(vertex_count, vertex_buffer, Triangle_Mode_Program);
		//
		//
		//Mesh_to_Line(vertex_count, vertex_buffer, point_count, &point_vb_buffer_ptr);
		//line_vs_str = line_vs;
		//line_fs_str = line_fs;
		//
		//mesh_line_Program = CreateProgram(line_vs_str, line_fs_str);
		//VAO_LINE = CreateVAO(point_count, point_vb_buffer_ptr, mesh_line_Program);


		//cout <<" create mesh success." << endl;
	};

	
	Mesh(
		const std::vector<Vertex>& _Vertices,
		const std::vector<unsigned int>& _Indices,
		const Material& _material,

		const glm::vec3& t_Init_Scale,
		const glm::vec3& t_Init_Rotate,
		const glm::vec3& t_Init_Trans
	);





	Mesh(std::string mesh_input_file_name,
		std::string surface_vs_src,
		std::string surface_fs_src,

		glm::uvec3 vertex_color,
		glm::uvec3 line_color,
		glm::uvec3 boundary_vertex_color,
		glm::uvec3 boundary_vertex_connect_color,

		glm::uvec3 boundingbox_vertex_color,
		glm::uvec3 boundingbox_line_color,
		glm::uvec3 boundingbox_surface_color,


		glm::vec3 init_trans_arg,
		glm::vec3 init_scale_arg,
		glm::uvec3 init_rotate_arg,
		bool need_init_model_matrix,
		GLuint *tex_arg = nullptr
	);

	//void Draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model);

	void Draw(glm::mat4 projection,
		glm::mat4 view,
		glm::mat4 model,
		bool to_draw_mesh_vertex,
		bool to_draw_BoundaryVertex,
		bool to_draw_BoundaryVertex_connect,

		bool to_draw_mesh_line,
		bool to_draw_mesh_surface,

		bool to_draw_boundingbox_vertex,
		bool to_draw_boundingbox_line,
		bool to_draw_boundingbox_surface);

	void Draw_1(glm::mat4 projection,
		glm::mat4 view,
		glm::mat4 model);


};










//for render a normal mesh.
GLuint Gen_Mesh_Surface_VAO();
//for render a mesh's Vertex
GLuint Gen_Mesh_Vertex_VAO();
//for render a mesh's Line
GLuint Gen_Mesh_Line_VAO();


//for draw boundary Vertex
GLuint Gen_BoundaryVertex_VAO();
GLuint Gen_BoundaryVertex_Line_VAO();

GLuint Gen_BoundingBox_Surface_VAO();
GLuint Gen_BoundingBox_Vertex_VAO();
GLuint Gen_BoundingBox_Line_VAO();






uint32_t Load_Model_from_OBJ(const std::string& model_path, std::vector<Mesh *> &mesh_list);

#endif
