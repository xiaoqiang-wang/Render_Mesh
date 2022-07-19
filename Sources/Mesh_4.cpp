//for render a normal mesh.
GLuint Mesh::Gen_Triangle_Mode_VAO(const std::vector<string> &attribute_list)
{

	uint32_t mesh_vertex_count = Vertices.size();

	/////////////////////////////////////
	//vec4 pos + vec4 normal + vec2 uv.
	const uint32_t attribute_component_count = 10;
	uint32_t attribute_buffer_size = attribute_component_count * mesh_vertex_count;
	GLfloat *t_attribute_buffer = new GLfloat[attribute_buffer_size]();//Initial buffer to zero.

	//
	// memory layout:
	// addr   0:   vec4 pos_0,    vec4 pos_1,    vec4 pos_2,    ..., vec4 pos_n,
	// addr 4*n:   vec4 normal_0, vec4 normal_1, vec4 normal_2, ..., vec4 normal_n,
	// addr 8*n:   vec4 uv_0,     vec4 uv_1,     vec4 uv_2,     ..., vec4 uv_n,
	//

	for (size_t t_vid = 0; t_vid < mesh_vertex_count; t_vid++) {	
		uint32_t t_position_index = (t_vid * 4);//position have 4 compnent
		t_attribute_buffer[t_position_index + 0] = Vertices[t_vid].Position.x;
		t_attribute_buffer[t_position_index + 1] = Vertices[t_vid].Position.y;
		t_attribute_buffer[t_position_index + 2] = Vertices[t_vid].Position.z;
		t_attribute_buffer[t_position_index + 3] = 1.0;// force the w to 1.0

		uint32_t t_normal_index = (4 * mesh_vertex_count) + (t_vid * 4);
		t_attribute_buffer[t_normal_index + 0] = Vertices[t_vid].Normal.x;
		t_attribute_buffer[t_normal_index + 1] = Vertices[t_vid].Normal.y;
		t_attribute_buffer[t_normal_index + 2] = Vertices[t_vid].Normal.z;
		t_attribute_buffer[t_normal_index + 3] = 1.0;//force the w to 1.0

		uint32_t t_uv_index = (8 * mesh_vertex_count) + (t_vid * 2);
		t_attribute_buffer[t_uv_index + 0] = Vertices[t_vid].TextureCoordinate.x;
		t_attribute_buffer[t_uv_index + 1] = Vertices[t_vid].TextureCoordinate.y;
	}



	uint32_t mesh_index_count = Indices.size();
	GLuint* t_index_buffer = new GLuint[mesh_index_count];

	for (size_t t_id = 0; t_id < mesh_index_count; t_id++) {
		t_index_buffer[t_id] = Indices[t_id];
	}

	


	//gen VAO.
	GLuint  VAO = 0;
	{
		VERIFY_GL(glGenVertexArrays(1, &VAO));
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		VERIFY_GL(glBindVertexArray(VAO));

		//GEN VBO
		{
			GLuint VBO = 0;
			VERIFY_GL(glGenBuffers(1, &VBO));
			VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));

			//VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*attribute_vec_size* mesh_vertex_count, mesh_position_buffer, GL_STATIC_DRAW));

			//total 9 component
			//copy data to the VBO.
			VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * attribute_component_count * mesh_vertex_count, t_attribute_buffer, GL_STATIC_DRAW));
		}

		
		for (size_t i = 0; i < attribute_list.size(); i++){

		}

		{
			GLint pos_location = -1;
			VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_Position"));
			if (pos_location == -1) {
				std::cout << "error: Fail to query attribute localtion!" << std::endl;
				assert(false && "error: Fail to query attribute localtion!");
			}
			else {
				VERIFY_GL(glEnableVertexAttribArray(pos_location));
				VERIFY_GL(glVertexAttribPointer(pos_location, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0)));
				VERIFY_GL(glDisableVertexAttribArray(pos_location));
			}


//#if USE_NORMAL_UV
#if 1
			GLint normal_location = -1;
			VERIFY_GL_RET(normal_location, glGetAttribLocation(Triangle_Mode_Program, "in_Normal"));
			if (normal_location == -1) {
				std::cout << "error: Fail to query attribute localtion!" << std::endl;
				assert(false && "error: Fail to query attribute localtion!");
			}
			else {
				VERIFY_GL(glEnableVertexAttribArray(normal_location));
				VERIFY_GL(glVertexAttribPointer(normal_location, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(GLfloat) * 4 * mesh_vertex_count)));
				VERIFY_GL(glDisableVertexAttribArray(normal_location));
			}

			GLint uv_location = -1;//texture coordinate.
			VERIFY_GL_RET(uv_location, glGetAttribLocation(Triangle_Mode_Program, "in_UV"));
			if (uv_location == -1) {
				std::cout << "error: Fail to query attribute localtion!" << std::endl;
				assert(false && "error: Fail to query attribute localtion!");
			}
			else {
				VERIFY_GL(glEnableVertexAttribArray(uv_location));
				VERIFY_GL(glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(GLfloat) * 8 * mesh_vertex_count)));
				VERIFY_GL(glDisableVertexAttribArray(uv_location));
			}
#endif

			VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
		}

		//GEN EBO
		{
			GLuint EBO = 0;
			VERIFY_GL(glGenBuffers(1, &EBO));
			VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			VERIFY_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mesh_index_count, t_index_buffer, GL_STATIC_DRAW));
			VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

			//BIND EBO to the VAO
			VERIFY_GL(glVertexArrayElementBuffer(VAO, EBO));
		}

		// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
		VERIFY_GL(glBindVertexArray(0));
	}

	delete[] t_attribute_buffer;

	return VAO;
}


GLuint Mesh::gen_Triangle_Mode_data(){

#if 1
	std::string VS_src = Read_file_to_str("default.vert");
	std::string FS_src = Read_file_to_str("default.frag");
#else
	std::string VS_src = Read_file_to_str("vert.txt");
	std::string FS_src = Read_file_to_str("frag.txt");
#endif

#if 0
	{

		std::string vs_src =
			" #version 330 core     	                                                        \n"
			" // = object.matrixWorld                                                           \n"
			" uniform mat4 modelMatrix;                                                         \n"
			"                                                                                   \n"
			//" // = camera.matrixWorldInverse * object.matrixWorld                               \n"
			//" uniform mat4 modelViewMatrix;                                                     \n"
			"                                                                                   \n"
			" // = camera.projectionMatrix                                                      \n"
			" uniform mat4 projectionMatrix;                                                    \n"
			"                                                                                   \n"
			" // = camera.matrixWorldInverse                                                    \n"
			" uniform mat4 viewMatrix;                                                          \n"
			"                                                                                   \n"
			" // = inverse transpose of modelViewMatrix                                         \n"
			" uniform mat3 normalMatrix;                                                        \n"
			"                                                                                   \n"
			" // = camera position in world space                                               \n"
			" uniform vec3 cameraPosition;                                                      \n"
			"                                                                                   \n"
			"                                                                                   \n"
			"                                                                                   \n"
			"                                                                                   \n"
			"                                                                                   \n"
			" // default vertex attributes provided by Geometry and BufferGeometry              \n"
			"                                                                                   \n"
			" layout(location = 0) in highp vec4 in_position;                                   \n"
			//" layout(location = 1) in highp vec4 in_normal;                                     \n"
			//" layout(location = 2) in highp vec2 in_texcoord;                                   \n"
			"                                                                                   \n"
			" uniform highp mat4 mvp;                                                           \n"
			"                                                                                   \n"
			"                                                                                   \n"
			" out highp vec3 Normal;                                                            \n"
			" out highp vec2 Texcoord;                                                          \n"
			"                                                                                   \n"
			" void main(){                                                                      \n"
			" 	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position);  \n"
			" 	                                                                                \n"
			//" 	Normal    = normalMatrix*vec3(in_normal.xyz);                                   \n"
			//" 	Texcoord  = in_texcoord;                                                        \n"
			" }                                                                                 \n";



		std::string fs_src =

			"#version 330 core                                     \n"
			"                                                      \n"
			"in vec2 TexCoord;                                     \n"
			"uniform vec4 global_time;                             \n"
			"out vec4 color;                                       \n"
			"// texture samplers                                   \n"
			"uniform sampler2D texture1;                           \n"
			"uniform sampler2D texture2;                           \n"
			"                                                      \n"
			"void main()                                           \n"
			"{                                                     \n"
			//"	vec4 color1 =  vec4(1.0, 1.0,1.0,1.0);             \n"
			//"	vec4 color2 = texture(texture1, TexCoord);         \n"
			"	color =  vec4(1.0, 0.0,0.0,0.0);                   \n"
			//"	color =  vec4(global_time.x);                      \n"
			//"	color = (texture(texture1, TexCoord)*0.8);         \n"
			"}                                                     \n";
	}
#endif

	Triangle_Mode_Program = CreateProgram(VS_src, FS_src);


	std::vector<string> attribute_list = {"in_Position", "in_Normal","in_UV"};
	Triangle_Mode_VAO = Gen_Triangle_Mode_VAO(attribute_list);


	return Triangle_Mode_VAO;

	//TODO
	//textures ...
}

GLuint Mesh::gen_Line_Mode_data() {


	std::string VS_src = Read_file_to_str("default.vert");
	std::string FS_src = Read_file_to_str("gray_color.frag");

	GLuint Gray_Surface_Program = CreateProgram(VS_src, FS_src);


	std::string VS_src1 = Read_file_to_str("default.vert");
	std::string FS_src1 = Read_file_to_str("black_color.frag");
	GLuint Black_Line_Program = CreateProgram(VS_src1, FS_src1);

	return 0;
}


GLuint Mesh::gen_Point_Mode_data() {

	return 0;
}



Mesh::Mesh(
	const std::vector<Vertex>& _Vertices,
	const std::vector<unsigned int>& _Indices,
	const Material& _material,

	const glm::vec3& t_Init_Scale,
	const glm::vec3& t_Init_Rotate,
	const glm::vec3& t_Init_Trans
) {

	for (uint32_t t_vid = 0; t_vid < _Vertices.size(); t_vid++){
		Vertex t_vertex = _Vertices[t_vid];
		Vertices.push_back(t_vertex);

		Updata_BoundaryVertex(t_vertex, t_vid);
	}

	Indices = _Indices;


	uint32_t texture_count = 0;
	uint32_t texture_ID = 0;
	material = _material;
	// if (!material.ambientTexName.empty()) {
	// 	ambientTextureId = gen_simple_2D_texture(material.ambientTexName);
	// 	texture_ID = ambientTextureId;
	// 	texture_count++;
	// }

	if (!material.diffuseTexName.empty()) {
		diffuseTextureId = gen_simple_2D_texture(material.diffuseTexName);
		texture_ID = diffuseTextureId;
		texture_count++;
	}

	// if (!material.specularTexName.empty()) {
	// 	specularTextureId = gen_simple_2D_texture(material.specularTexName);
	// 	texture_ID = specularTextureId;
	// 
	// 	texture_count++;
	// }

    // if(!material.specularHightlightTexName.empty()) {
	// 	texture_ID = gen_simple_2D_texture(material.specularHightlightTexName);
	// 	texture_count++;
	// }

	// if (!material.AlphaTexName.empty()) {
	// 	texture_ID = gen_simple_2D_texture(material.AlphaTexName);
	// 	texture_count++;
	// }

	// if (!material.BumpMapTexName.empty()) {
	// 	texture_ID = gen_simple_2D_texture(material.BumpMapTexName);
	// 	texture_count++;
	// }

	// if (!material.normalTexName.empty()) {
	// 	normalTextureId = gen_simple_2D_texture(material.normalTexName);
	// 	texture_ID      = normalTextureId;
	// 	texture_count++;
	// }
	// 
	// if (texture_count>1){
	// 	cout<<"Error: multi_texture currenlty un-supported : "<< texture_count <<endl;
	// 	//assert(0 && "Error: multi-texture un-supported.");
	// }
	
	diffuseTextureId = texture_ID;

	//OK TO init the model matrix
	Gen_Init_Model_Matrix(
		true, 
		true,
		t_Init_Scale,
		t_Init_Rotate,
		t_Init_Trans
	);





	// see the page for shader demos.
	//  http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
	// 
	// GLSL demo see below code
	//  https://github.com/rlk/obj/tree/master/etc
	//  
	//  https://github.com/monguri/sdlgl3-wavefront
	//  https://community.khronos.org/t/glsl-shading-with-obj-file/76918


	// Uniforms
	//
	//
	//
	//  https://threejs.org/docs/#api/en/renderers/webgl/WebGLProgram.cacheKey
	//
	//
	// vertex_shader?
	// fragment_shader?
	// Light informaion， material information

	// Light information
	//
	// Material_info
	// How to gen VS, FS.
	//
	// texture
	// Mesh.
	// bounding box
	// meterial
	//
	//


	//std::string vs_src0 = Read_file_to_str("default.vert");
	//std::string fs_str0 = Read_file_to_str("default.frag");
	//
	//std::string vs_src = Read_file_to_str("vert.txt");
	//std::string fs_src = Read_file_to_str("frag.txt");


	
#if 0
	{

		std::string vs_src =
			" #version 330 core     	                                                        \n"
			" // = object.matrixWorld                                                           \n"
			" uniform mat4 modelMatrix;                                                         \n"
			"                                                                                   \n"
			//" // = camera.matrixWorldInverse * object.matrixWorld                               \n"
			//" uniform mat4 modelViewMatrix;                                                     \n"
			"                                                                                   \n"
			" // = camera.projectionMatrix                                                      \n"
			" uniform mat4 projectionMatrix;                                                    \n"
			"                                                                                   \n"
			" // = camera.matrixWorldInverse                                                    \n"
			" uniform mat4 viewMatrix;                                                          \n"
			"                                                                                   \n"
			" // = inverse transpose of modelViewMatrix                                         \n"
			" uniform mat3 normalMatrix;                                                        \n"
			"                                                                                   \n"
			" // = camera position in world space                                               \n"
			" uniform vec3 cameraPosition;                                                      \n"
			"                                                                                   \n"
			"                                                                                   \n"
			"                                                                                   \n"
			"                                                                                   \n"
			"                                                                                   \n"
			" // default vertex attributes provided by Geometry and BufferGeometry              \n"
			"                                                                                   \n"
			" layout(location = 0) in highp vec4 in_position;                                   \n"
			//" layout(location = 1) in highp vec4 in_normal;                                     \n"
			//" layout(location = 2) in highp vec2 in_texcoord;                                   \n"
			"                                                                                   \n"
			" uniform highp mat4 mvp;                                                           \n"
			"                                                                                   \n"
			"                                                                                   \n"
			" out highp vec3 Normal;                                                            \n"
			" out highp vec2 Texcoord;                                                          \n"
			"                                                                                   \n"
			" void main(){                                                                      \n"
			" 	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position);  \n"
			" 	                                                                                \n"
			//" 	Normal    = normalMatrix*vec3(in_normal.xyz);                                   \n"
			//" 	Texcoord  = in_texcoord;                                                        \n"
			" }                                                                                 \n";



		std::string fs_src =

			"#version 330 core                                     \n"
			"                                                      \n"
			"in vec2 TexCoord;                                     \n"
			"uniform vec4 global_time;                             \n"
			"out vec4 color;                                       \n"
			"// texture samplers                                   \n"
			"uniform sampler2D texture1;                           \n"
			"uniform sampler2D texture2;                           \n"
			"                                                      \n"
			"void main()                                           \n"
			"{                                                     \n"
			//"	vec4 color1 =  vec4(1.0, 1.0,1.0,1.0);             \n"
			//"	vec4 color2 = texture(texture1, TexCoord);         \n"
			"	color =  vec4(1.0, 0.0,0.0,0.0);                   \n"
			//"	color =  vec4(global_time.x);                      \n"
			//"	color = (texture(texture1, TexCoord)*0.8);         \n"
			"}                                                     \n";
	}
#endif


	gen_Triangle_Mode_data();

	gen_Line_Mode_data();
	gen_Point_Mode_data();
}


 
//  
//  Mesh::Mesh(
//  	const std::string &name_arg,
//  	GLuint vetex_count,
//  	GLuint Program,
//  	GLuint VAO,
//  
//  	const std::array<Vertex, 6> &BoundaryVertex,
//  	const std::array<uint32_t, 6>  &BoundaryVertex_VID
//   ){
//  	 VERIFY_GL(glGetError());
//  	 cout << "\t creating mesh: "<< name_arg <<"... " <<  std::endl;
//  	 //cout << "\t create mesh success. vertex_count : " << obj_Mesh.Vertices.size() << endl << endl;
//  	 name = name_arg;
//  
//  	 mesh_vertex_count = vetex_count;
//  	 Triangle_Mode_Program = Program;
//  
//  
//  	 mesh_triangle_line_VA0 = VAO;
//  
//  
//  	 // init the 6 mesh boundary vertex
//  	 BoundaryVertex     = BoundaryVertex;
//  	 BoundaryVertex_VID = BoundaryVertex_VID;
//  
//  	 //
//  	 //
//  	 // used to generate mesh init_Model_Matrix
//  	 //
//  	 //
//  	 Gen_Init_Model_Matrix(true);
//  
//  
//  	 if (!material.ambientTexName.empty()){
//  		 ambientTextureId = gen_simple_2D_texture(material.ambientTexName);
//  	 }
//  
//  	 if (!material.diffuseTexName.empty()) {
//  		 diffuseTextureId = gen_simple_2D_texture(material.diffuseTexName);
//  	 }
//  
//  	 if (!material.specularTexName.empty()) {
//  		 specularTextureId = gen_simple_2D_texture(material.specularTexName);
//  	 }
//  
//  	 if (!material.normalTexName.empty()) {
//  		 normalTextureId = gen_simple_2D_texture(material.normalTexName);
//  	 }
//  };
//  








//for render a normal mesh.
GLuint Gen_Mesh_Surface_VAO(const objl::Mesh &t_Mesh){


	std::string mesh_name    = t_Mesh.MeshName;
	GLuint mesh_vertex_count = t_Mesh.Vertices.size();

	/////////////////////////////////////
	//vec4 pos + vec4 normal + vec2 uv.
	const uint32_t attribute_component_count = 10;
	uint32_t attribute_buffer_size = attribute_component_count * mesh_vertex_count;
	GLfloat *t_attribute_buffer = new GLfloat[attribute_buffer_size]();//Initial buffer to zero.




	//
	// memory layout:
	// addr   0:   vec4 pos_0,    vec4 pos_1,    vec4 pos_2,    ..., vec4 pos_n,
	// addr 4*n:   vec4 normal_0, vec4 normal_1, vec4 normal_2, ..., vec4 normal_n,
	// addr 8*n:   vec4 uv_0,     vec4 uv_1,     vec4 uv_2,     ..., vec4 uv_n,
	//

	for (size_t t_vid = 0; t_vid < mesh_vertex_count; t_vid++) {
#if 1		
		uint32_t t_position_index = (t_vid * 4);//position have 4 compnent
		t_attribute_buffer[t_position_index + 0] = t_Mesh.Vertices[t_vid].Position.X;
		t_attribute_buffer[t_position_index + 1] = t_Mesh.Vertices[t_vid].Position.Y;
		t_attribute_buffer[t_position_index + 2] = t_Mesh.Vertices[t_vid].Position.Z;
		t_attribute_buffer[t_position_index + 3] = 1.0;// force the w to 1.0

		uint32_t t_normal_index = (4 * mesh_vertex_count) + (t_vid * 4);
		t_attribute_buffer[t_normal_index + 0] = t_Mesh.Vertices[t_vid].Normal.X;
		t_attribute_buffer[t_normal_index + 1] = t_Mesh.Vertices[t_vid].Normal.Y;
		t_attribute_buffer[t_normal_index + 2] = t_Mesh.Vertices[t_vid].Normal.Z;
		t_attribute_buffer[t_normal_index + 3] = 1.0;//force the w to 1.0

		uint32_t t_uv_index = (8 * mesh_vertex_count) + (t_vid * 2);
		t_attribute_buffer[t_uv_index + 0] = t_Mesh.Vertices[t_vid].TextureCoordinate.X;
		t_attribute_buffer[t_uv_index + 1] = t_Mesh.Vertices[t_vid].TextureCoordinate.Y;

#else
		//used code just keep it here.
		t_attribute_buffer[(t_vid * 4) + 0] = t_Mesh.Vertices[t_vid].Position.X;
		t_attribute_buffer[(t_vid * 4) + 1] = t_Mesh.Vertices[t_vid].Position.Y;
		t_attribute_buffer[(t_vid * 4) + 2] = t_Mesh.Vertices[t_vid].Position.Z;
		t_attribute_buffer[(t_vid * 4) + 3] = 1.0;// force the w to 1.0

		//t_attribute_buffer[(t_vid * 4) + 4] = t_Mesh.Vertices[t_vid].Normal.X;
		//t_attribute_buffer[(t_vid * 4) + 5] = t_Mesh.Vertices[t_vid].Normal.Y;
		//t_attribute_buffer[(t_vid * 4) + 6] = t_Mesh.Vertices[t_vid].Normal.Z;
		//t_attribute_buffer[(t_vid * 4) + 7] = 1.0;//force the w to 1.0
		//							
		//t_attribute_buffer[(t_vid * 4) + 8] = t_Mesh.Vertices[t_vid].TextureCoordinate.X;
		//t_attribute_buffer[(t_vid * 4) + 9] = t_Mesh.Vertices[t_vid].TextureCoordinate.Y;
#endif
	}



	uint32_t mesh_index_count = t_Mesh.Indices.size();
	GLuint* t_index_buffer    = new GLuint[mesh_index_count];

	for (size_t t_id = 0; t_id < mesh_index_count; t_id++){
		t_index_buffer[t_id] = t_Mesh.Indices[t_id];
	}

	GLuint Triangle_Mode_Program = 0;


	//gen VAO.
	GLuint  VAO = 0;
	{
		VERIFY_GL(glGenVertexArrays(1, &VAO));
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		VERIFY_GL(glBindVertexArray(VAO));

		//GEN VBO
		{
			GLuint VBO = 0;
			VERIFY_GL(glGenBuffers(1, &VBO));
			VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));

			//VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*attribute_vec_size* mesh_vertex_count, mesh_position_buffer, GL_STATIC_DRAW));

			//total 9 component
			//copy data to the VBO.
			VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * attribute_component_count * mesh_vertex_count, t_attribute_buffer, GL_STATIC_DRAW));
		}


		{
			GLint pos_location = -1;
			VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_position"));
			if (pos_location == -1) {
				std::cout << "error: Fail to query attribute localtion!" << std::endl;
				assert(false && "error: Fail to query attribute localtion!");
			}
			else {
				VERIFY_GL(glEnableVertexAttribArray(pos_location));
				VERIFY_GL(glVertexAttribPointer(pos_location, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0)));
				VERIFY_GL(glDisableVertexAttribArray(pos_location));
			}


#if USE_NORMAL_UV
			GLint normal_location = -1;
			VERIFY_GL_RET(normal_location, glGetAttribLocation(Triangle_Mode_Program, "in_normal"));
			if (normal_location == -1) {
				std::cout << "error: Fail to query attribute localtion!" << std::endl;
				assert(false && "error: Fail to query attribute localtion!");
			}
			else {
				VERIFY_GL(glEnableVertexAttribArray(normal_location));
				VERIFY_GL(glVertexAttribPointer(normal_location, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(GLfloat) * 4 * mesh_vertex_count)));
				VERIFY_GL(glDisableVertexAttribArray(normal_location));
			}

			GLint uv_location = -1;//texture coordinate.
			VERIFY_GL_RET(uv_location, glGetAttribLocation(Triangle_Mode_Program, "in_uv"));
			if (uv_location == -1) {
				std::cout << "error: Fail to query attribute localtion!" << std::endl;
				assert(false && "error: Fail to query attribute localtion!");
			}
			else {
				VERIFY_GL(glEnableVertexAttribArray(uv_location));
				VERIFY_GL(glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(GLfloat) * 8 * mesh_vertex_count)));
				VERIFY_GL(glDisableVertexAttribArray(uv_location));
			}
#endif

			VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
		}

		//GEN EBO
		{
			GLuint EBO = 0;
			VERIFY_GL(glGenBuffers(1, &EBO));
			VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			VERIFY_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mesh_index_count, t_index_buffer, GL_STATIC_DRAW));
			VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

			//BIND EBO to the VAO
			VERIFY_GL(glVertexArrayElementBuffer(VAO, EBO));
		}

		// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
		VERIFY_GL(glBindVertexArray(0));
	}

	delete[] t_attribute_buffer;

	return VAO;
}



//return mesh count.
uint32_t Load_Model_from_OBJ(const std::string& model_path, std::vector<Mesh *> &mesh_list) {

	//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\cube\\cube.obj";
	//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\09_Crytek_Sponza\\sponza.obj";
	//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\13_Erato_broken\\erato-1.obj";

	objl::Loader loader;
	std::cout << model_path << std::endl;
	bool load_res = loader.LoadFile(model_path);
	assert((load_res == true) && "error: Fail to load \" .obj \" file.");


	glm::vec3 t_Init_Scale  = glm::vec3(loader.InitScaleVec.X,  loader.InitScaleVec.Y, loader.InitScaleVec.Z);
	glm::vec3 t_Init_Rotate = glm::vec3(loader.InitRotateVec.X, loader.InitRotateVec.Y,loader.InitRotateVec.Z);
	glm::vec3 t_Init_Trans  = glm::vec3(loader.InitTransVec.X,  loader.InitTransVec.Y, loader.InitTransVec.Z);

	//uint32_t total_vertices_count = 0;
	for (uint32_t mesh_idx = 0; mesh_idx < loader.LoadedMeshes.size(); mesh_idx++) {

		//to constructor a new Mesh
		objl::Mesh &t_Objl_Mesh = loader.LoadedMeshes[mesh_idx];

		std::vector<glm::vec3> t_Position;
		std::vector<glm::vec3> t_Normal;
		std::vector<glm::vec2> t_UV;


		
		

		std::vector<Vertex>  t_Vertices;
		//copy OBJ file data
		for (size_t i = 0; i < t_Objl_Mesh.Vertices.size(); i++){
			Vertex t_Vertex;
			t_Vertex.Position          = glm::vec3(t_Objl_Mesh.Vertices[i].Position.X,          t_Objl_Mesh.Vertices[i].Position.Y, t_Objl_Mesh.Vertices[i].Position.Z);
			t_Vertex.Normal            = glm::vec3(t_Objl_Mesh.Vertices[i].Normal.X,            t_Objl_Mesh.Vertices[i].Normal.Y,   t_Objl_Mesh.Vertices[i].Normal.Z);
			t_Vertex.TextureCoordinate = glm::vec2(t_Objl_Mesh.Vertices[i].TextureCoordinate.X, t_Objl_Mesh.Vertices[i].TextureCoordinate.Y);

			t_Vertices.push_back(t_Vertex);
		}

		std::vector<uint32_t> t_Indices;
		for (size_t i = 0; i < t_Objl_Mesh.Indices.size(); i++) {
			t_Indices.push_back(t_Objl_Mesh.Indices[i]);
		}

		Material t_material;
		{
			t_material.name = t_Objl_Mesh.MeshMaterial.name;
			t_material.ambient  = glm::vec3(t_Objl_Mesh.MeshMaterial.Ka.X, t_Objl_Mesh.MeshMaterial.Ka.Y, t_Objl_Mesh.MeshMaterial.Ka.Z);
			t_material.diffuse  = glm::vec3(t_Objl_Mesh.MeshMaterial.Kd.X, t_Objl_Mesh.MeshMaterial.Kd.Y, t_Objl_Mesh.MeshMaterial.Kd.Z);
			t_material.specular = glm::vec3(t_Objl_Mesh.MeshMaterial.Ks.X, t_Objl_Mesh.MeshMaterial.Ks.Y, t_Objl_Mesh.MeshMaterial.Ks.Z);

			//FIXME: TODO
			//t_material.transmittance = glm::vec3(t_Objl_Mesh.MeshMaterial.d, t_Objl_Mesh.MeshMaterial.d, t_Objl_Mesh.MeshMaterial.d);
			//t_material.emission = glm::vec3(t_Objl_Mesh.MeshMaterial.Ns, t_Objl_Mesh.MeshMaterial.Ns, t_Objl_Mesh.MeshMaterial.Ka);

			t_material.shininess = 0.0f;

			t_material.ior = t_Objl_Mesh.MeshMaterial.Ni;


			t_material.illum = t_Objl_Mesh.MeshMaterial.illum;

			t_material.ambientTexName            = t_Objl_Mesh.MeshMaterial.map_Ka;
			t_material.diffuseTexName            = t_Objl_Mesh.MeshMaterial.map_Kd;
			t_material.specularTexName           = t_Objl_Mesh.MeshMaterial.map_Ks;
			t_material.specularHightlightTexName = t_Objl_Mesh.MeshMaterial.map_Ns;
			t_material.AlphaTexName              = t_Objl_Mesh.MeshMaterial.map_d;

			t_material.BumpMapTexName            = t_Objl_Mesh.MeshMaterial.map_bump;
			t_material.normalTexName             = "";
		}

		Mesh * t_mesh = new Mesh(
			t_Vertices, 
			t_Indices, 
			t_material,
			t_Init_Scale,
			t_Init_Rotate,
			t_Init_Trans
		);
		//OK save the mesh to the list
		mesh_list.push_back(t_mesh);
	}

	return mesh_list.size();
}

