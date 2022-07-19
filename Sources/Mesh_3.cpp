static glm::vec3 Light_Pos = glm::vec3(3, 5, -5);
static  glm::vec3 Light_Color = glm::vec3(0.95, 0.95, 0.95);
static float LightPower = 40.0f;

static bool increse = true;

void Mesh::Draw_1(glm::mat4 projection,
	glm::mat4 view,
	glm::mat4 model)
{
	//cout<<"Drawing Mesh:"<<input_file<<endl;
	//glm::mat4 mvp = projection * view * model * init_model;
	

	trans_vec  = glm::vec3(0.0f);  //init shift vec
	scale_vec  = glm::vec3(1.0f);//init scale vec
	rotate_vec = glm::vec3(0.0f);  //init roate vec in degree
	
	glm::mat4 modelMatrix = init_model_matrix;
	//glm::mat4 modelMatrix = glm::mat4(1.0);

	
	glm::mat4 viewMatrix = view;
	glm::mat4 projectionMatrix = projection;

	

	glm::mat4 normalMatrix;
	
	bool to_draw_mesh_surface = true;
	if (to_draw_mesh_surface) {

		//for triangles we may need to draw triangles surface and triangle line.
		//if (mesh_mode == GL_TRIANGLES)
		if (true){

			
			VERIFY_GL(glValidateProgram(Triangle_Mode_Program));
			VERIFY_GL(glUseProgram(Triangle_Mode_Program));

			for (uint32_t i = 0; i < 16; i++) {
				if (textures[i] != 0) {
					VERIFY_GL(glActiveTexture(GL_TEXTURE0 + i));
					VERIFY_GL(glBindTexture(GL_TEXTURE_2D, textures[i]));

					//uint32_t sampler_pos = 0;
					//VERIFY_GL_RET(sampler_pos, glGetUniformLocation(Triangle_Mode_Program, "texture1"));
					//VERIFY_GL(glUniform1i(sampler_pos, i));

					std::string texture_name = "texture" + to_string(i);
					GLuint sampler_pos = -1;
					VERIFY_GL_RET(sampler_pos, glGetUniformLocation(Triangle_Mode_Program, texture_name.data()));
					if (sampler_pos != -1) {//-1 for invalid, uniform variable.
						VERIFY_GL(glUniform1i(sampler_pos, i));
					}
				}
			}

			//UNIFORMS
			////////////////////////////////////////////////////////////////////////////////////
			{
				//Set_Uniform_Mat4(Triangle_Mode_Program, "modelMatrix",      modelMatrix);
				//Set_Uniform_Mat4(Triangle_Mode_Program, "viewMatrix",       viewMatrix);
				//Set_Uniform_Mat4(Triangle_Mode_Program, "projectionMatrix", projectionMatrix);


				Set_Uniform_Mat4(Triangle_Mode_Program, "M", modelMatrix);
				Set_Uniform_Mat4(Triangle_Mode_Program, "V", viewMatrix);

				glm::mat4 _mvp = projectionMatrix * viewMatrix*modelMatrix;
				Set_Uniform_Mat4(Triangle_Mode_Program, "MVP", _mvp);

				
			
				if (increse) {
					//Light_Pos = Light_Pos + (glm::vec3(0.5));

					Light_Pos.x = Light_Pos.x + 0.001f;
					LightPower = LightPower + 0.001f;
					if (Light_Pos.x > 10.0f) {
						increse = false;
					}
				}
				else {
					
					//Light_Pos = Light_Pos - (glm::vec3(0.5));
					Light_Pos.x = Light_Pos.x - 0.01f;
					LightPower = LightPower - 0.01f;

					if (Light_Pos.x < -10.0f) {
						increse = true;
					}
				}

				

				
				Set_Uniform_Vec3(Triangle_Mode_Program,   "LightPosition_worldspace", Light_Pos);
				Set_Uniform_Vec3(Triangle_Mode_Program,   "LightColor", Light_Color);
				Set_Uniform_float1(Triangle_Mode_Program, "LightPower", LightPower);
				

				glm::vec3 _temp_vec = glm::vec3(0, 0, 0);
				Set_Uniform_Vec3(Triangle_Mode_Program, "MaterialAmbient", _temp_vec);
				Set_Uniform_Vec3(Triangle_Mode_Program, "MaterialDiffuse", _temp_vec);
				Set_Uniform_Vec3(Triangle_Mode_Program, "MaterialSpecular", material.specular);

				//Set_Uniform_Vec3(Triangle_Mode_Program, "MaterialAmbient", material.ambient);
				//Set_Uniform_Vec3(Triangle_Mode_Program, "MaterialDiffuse",  material.diffuse);
				//Set_Uniform_Vec3(Triangle_Mode_Program, "MaterialSpecular", material.specular);
			}
			



			//ATTRIBUTES
			////////////////////////////////////////////////////////////////////////////////////
			{
				VERIFY_GL(glBindVertexArray(Triangle_Mode_VAO));
				Enable_Attribute_Location(Triangle_Mode_Program, "in_Position");
				Enable_Attribute_Location(Triangle_Mode_Program, "in_Normal");
				Enable_Attribute_Location(Triangle_Mode_Program, "in_UV");
			}

			//Textures
			{
				VERIFY_GL(glActiveTexture(GL_TEXTURE0));
				VERIFY_GL(glBindTexture(GL_TEXTURE_2D, diffuseTextureId));
				Set_Sampler_Texture(Triangle_Mode_Program, "myTextureSampler", 0);
			}


			//TO DRAW THE MESH.
			////////////////////////////////////////////////////////////////////////////////////
			{
				//VERIFY_GL(glDrawArrays(GL_TRIANGLES, 0, Vertices.size()));//FIXME: todo
				//VERIFY_GL(glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0));
				VERIFY_GL(glDrawElements(GL_LINES, Indices.size(), GL_UNSIGNED_INT, 0));

				//VERIFY_GL(glDrawElements(GL_LINES, Vertices.size(), GL_UNSIGNED_INT, 0));

				// float default_point_size = 10.5f;
				// VERIFY_GL(glEnable(GL_PROGRAM_POINT_SIZE));
				// VERIFY_GL(glPointSize(default_point_size));

				
				//VERIFY_GL(glDrawElements(GL_POINTS, mesh_vertex_count, GL_UNSIGNED_INT, 0));
				//VERIFY_GL(glDrawElements(GL_POINTS, 36, GL_UNSIGNED_INT, 0));

				switch (Render_Mode)
				{
				case GL_TRIANGLES:
				{
					VERIFY_GL(glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0));
				}
				break;
				case GL_LINES:
				{

					//disable update color buffer, and only update depth
					{
						VERIFY_GL(glDisable(GL_DEPTH_TEST));
						VERIFY_GL(glDepthMask(GL_TRUE));
						//disable update color buffer, and only update depth
						VERIFY_GL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

						VERIFY_GL(glUseProgram(Line_Mode_Surface_Program));
						VERIFY_GL(glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0));
					}

					//enable update color buffer
					{
						VERIFY_GL(glDisable(GL_DEPTH_TEST));
						VERIFY_GL(glDepthMask(GL_TRUE));

						//enable update color buffer
						VERIFY_GL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

						VERIFY_GL(glUseProgram(Line_Mode_Line_Program));
						VERIFY_GL(glDrawElements(GL_LINES, Indices.size(), GL_UNSIGNED_INT, 0));
					}
					
					
				}
				break;
				case GL_POINTS:
				{

					//disable update color buffer, and only update depth
					{
						VERIFY_GL(glDisable(GL_DEPTH_TEST));
						VERIFY_GL(glDepthMask(GL_TRUE));
						//disable update color buffer, and only update depth
						VERIFY_GL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

						VERIFY_GL(glUseProgram(Point_Mode_Surface_Program));
						VERIFY_GL(glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0));
					}

					//enable update color buffer
					{
						VERIFY_GL(glDisable(GL_DEPTH_TEST));
						VERIFY_GL(glDepthMask(GL_TRUE));

						//enable update color buffer
						VERIFY_GL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

						VERIFY_GL(glUseProgram(Point_Mode_Line_Program));
						VERIFY_GL(glDrawElements(GL_LINES, Indices.size(), GL_UNSIGNED_INT, 0));


						VERIFY_GL(glUseProgram(Point_Mode_Point_Program));
						VERIFY_GL(glDrawElements(GL_POINTS, Indices.size(), GL_UNSIGNED_INT, 0));
					}
				}
				break;
				default:
					assert(0 && "error: un-support render mode.");
					break;
				}
			}


			//remeber to reset GL context.
			Reset_GL_Context();
		}
	}
};





Mesh::Mesh(std::string mesh_input_file_name,
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
	GLuint *tex_arg
) {

	cout << "Creating Mesh:" << mesh_input_file_name << std::endl;
	name = mesh_input_file_name;
	
	mesh_id = global_mesh_instance_count;
	global_mesh_instance_count++;

	//Read_VS_output(input_file, vertex_count, &vertex_buffer);
	//assert(is_local_space==true && "error: need to be local space.");
	//Read_Mesh_in_local_space(input_file, vertex_count, &vertex_buffer, pos_x_name, pos_y_name, pos_z_name);
	//GLuint VAO = Create_IN_MESH_VAO(vertex_count, vertex_buffer, Triangle_Mode_Program);
	//Mesh_to_Line(vertex_count, vertex_buffer, point_count, &point_vb_buffer_ptr);


	//line_VA0 = Create_IN_MESH_VAO(point_count, point_vb_buffer_ptr, mesh_line_Program);
	//mesh_line_VA0 = Create_IN_MESH_VAO(point_count, point_vb_buffer_ptr, mesh_line_Program);

	//why we need to read vertices first?
	


	
	// if (mesh_input_file_name.substr(mesh_input_file_name.size() - 4, 4) != ".obj") {
	// 	// If the file is not an .obj file we use loader to load a obj file.
	// 	assert(0 && "error: un-supported!");
	// }
	// else{
	// 	mesh_vertex_count = Read_Mesh_Vertex(mesh_input_file_name);
	// }

	mesh_vertex_count = Read_Mesh_Vertex(mesh_input_file_name);




	
////////////////////////////////////////////////////////////

	trans_vec = init_trans_arg;
	assert(init_scale_arg.x > 0.0f && "can't scaling below than zero.");
	assert(init_scale_arg.y > 0.0f && "can't scaling below than zero.");
	assert(init_scale_arg.z > 0.0f && "can't scaling below than zero.");
	scale_vec = init_scale_arg;

	rotate_vec = init_rotate_arg;

	// Gen_Init_Model_Matrix(need_init_model_matrix);








	//vs_str = vs_src;
	//fs_str = fs_src;

	/*
	// 
	// basic : input_without_mvp.
	// basic : 3D_with_mvp
	// basic : 4D_with_mvp
	//
	*/
	
	
	/*
	const std::string default_vs_src =
		"#version 330 core                                     \n"
		"layout(location = 0) in highp vec4 in_position;       \n"
		"uniform highp mat4 mvp;                               \n"
		"void main()                                           \n"
		"{                                                     \n"
		"    gl_Position =vec4(in_position.xyzw);              \n"
		"}                                                     \n";
	*/
	//we should draw the mesh from inner to outsider.
	//vertex, boundray_vertex, line, surface, boundingbox_vertex, boundingbox_line, bounding_box_surface.


//
//	bool FOR_GET_VISIBLE_MESH = true;
//	//for render simple VS output.
//	if (FOR_GET_VISIBLE_MESH==true){
//	
//	}
//	




	if (true)
	{
		//uint8_t R = r * 255;
		//uint8_t G = g * 255;
		//uint8_t B = b * 255;
		//uint8_t A = 1.0 * 255;
		
		
		// uint8_t R = rand() % 256;
		// uint8_t G = rand() % 256;
		// uint8_t B = rand() % 256;
		// uint8_t A = 255;


		uint8_t R = 0;
		uint8_t G = 0;
		uint8_t B = 0;
		uint8_t A = 0;

		if (is_out_side_normal_space == false){
			//the mesh inside the normalized [-1,1]^3 space.
			R = 255;
			G = 255;
			B = 255;
			A = 255;

		}
		else {

			if (where_the_mesh[0] == true) {
			}

			if (where_the_mesh[1] == true) {
			}

			if (where_the_mesh[2] == true) {
			}

			if (where_the_mesh[3] == true) {
			}

			if (to_draw_clean_mesh == 1) {
				R = 255;
				G = 255;
				B = 255;
				A = 255;
			}
			else
			{
				//to random generate mesh color.
				if (true) {

					R = rand() % 256;
					G = rand() % 256;
					B = rand() % 256;
					glm::uvec3 color = glm::uvec3(R, G, B);

					static std::vector<glm::uvec3> used_colors;

					while (std::find(used_colors.begin(), used_colors.end(), color) != used_colors.end()) {
						R = rand() % 256;
						G = rand() % 256;
						B = rand() % 256;
					}
					used_colors.push_back(color);
				}
				else
				{
					//
					//float value = ((float)mesh_id / (float)40);
					//float r = 0.0f, g = 0.0f, b = 0.0f;
					//getHeatMapColor(value, &r, &g, &b);
					//

					//
					// R = r * 255;
					// G = g * 255;
					// B = b * 255;
					//

					glm::uvec3 colors[22] = {
						glm::uvec3(142,207,201),
						glm::uvec3(255,190,122),
						glm::uvec3(250,127,111),
						glm::uvec3(130,176,210),
						glm::uvec3(190,184,220),
						glm::uvec3(231,218,210),
						glm::uvec3(153,153,153),

						glm::uvec3(40, 120, 181),
						glm::uvec3(154,201,219),
						glm::uvec3(248,172,140),
						glm::uvec3(200,36,35),
						glm::uvec3(255,136,132),

						glm::uvec3(20, 81, 124),
						glm::uvec3(47, 127, 193),
						glm::uvec3(231,239,250),
						glm::uvec3(150,195,125),
						glm::uvec3(243,210,102),
						glm::uvec3(216, 56, 58),
						glm::uvec3(247, 255, 237),
						glm::uvec3(248,243,249),
						glm::uvec3(196,151,178),
						glm::uvec3(169,184,194)
					};

					static std::vector<uint32_t> used_colors;

					uint32_t index = rand() % (sizeof(colors) / sizeof(glm::uvec3));
					while (std::find(used_colors.begin(), used_colors.end(), index) != used_colors.end()) {
						index = rand() % (sizeof(colors) / sizeof(glm::uvec3));
					}
					used_colors.push_back(index);

					R = colors[index].x;
					G = colors[index].y;
					B = colors[index].z;
				}
			}
		}

		//mesh is inside the normal cube.
		if (is_out_side_normal_space==false){
			Triangle_Mode_Program = CreateProgram(surface_vs_src, surface_fs_src);
		}
		else{
			Triangle_Mode_Program = CreateProgram(surface_vs_src, Gen_simple_FS(glm::uvec3(R, G, B)));
		}
		
		

		mesh_vertex_Program = CreateProgram(surface_vs_src, Gen_simple_FS(vertex_color));

		BoundaryVertex_Program = CreateProgram(surface_vs_src, Gen_simple_FS(boundary_vertex_color));


		BoundaryVertex_connect_Program = CreateProgram(surface_vs_src, Gen_simple_FS(boundary_vertex_connect_color));
		mesh_line_Program = CreateProgram(surface_vs_src, Gen_simple_FS(line_color));


		mesh_boundingBox_vertex_Program = CreateProgram(surface_vs_src, Gen_simple_FS(boundingbox_vertex_color));
		mesh_boundingBox_line_Program = CreateProgram(surface_vs_src, Gen_simple_FS(boundingbox_line_color));
		mesh_boundingBox_surface_Program = CreateProgram(surface_vs_src, Gen_simple_FS(boundingbox_surface_color));
	}

	//
	//
	// OK,just return here, 
	// below code may still usefull,
	// but we currently not use it.
	//
	//
	// return;
	//

#define FOR_GET_VISIBLE_MESH 0  
#if FOR_GET_VISIBLE_MESH


	const std::string VS_SRC_OUT_POS_1 =
		"#version 330 core                                     \n"
		"layout(location = 0) in highp vec4 in_position;       \n"
		"void main()                                           \n"
		"{                                                     \n"
		"    gl_Position = vec4(in_position);                  \n"
		"}                                                     \n";

	mesh_vertex_Program                  = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(vertex_color));
	BoundaryVertex_Program         = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(boundary_vertex_color));

	BoundaryVertex_connect_Program = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(boundary_vertex_connect_color));
	mesh_line_Program                    = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(line_color));

	//Triangle_Mode_Program                 = CreateProgram(VS_SRC_OUT_POS_1, surface_fs_src);

	
	//uint8_t r = rand() % 256;
	//uint8_t g = rand() % 256;
	//uint8_t b = rand() % 256;

	uint8_t r = 255;
	uint8_t g = 0;
	uint8_t b = 0;
	Triangle_Mode_Program                 = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(glm::uvec3(r,g,b)));
	//Triangle_Mode_Program                 = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(boundingbox_vertex_color));

	mesh_boundingBox_vertex_Program      = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(boundingbox_vertex_color));
	mesh_boundingBox_line_Program        = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(boundingbox_line_color));
	mesh_boundingBox_surface_Program     = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(boundingbox_surface_color));
#endif



#define TO_DRAW_OUTPUT_MESH_WITH_MVP 0
#if TO_DRAW_OUTPUT_MESH_WITH_MVP

	const std::string VS_SRC_OUT_POS_WITH_MVP =
		"#version 330 core                                     \n"
		"layout(location = 0) in highp vec4 in_position;       \n"
		"uniform highp mat4 mvp;                               \n"
		"void main()                                           \n"
		"{                                                     \n"
	    "    gl_Position = mvp*vec4(in_position);              \n"
		//"    gl_Position = vec4(in_position);              \n"
		//"    gl_Position = mvp*vec4(in_position.xyz, 1.0); \n"
		"}                                                    \n";



	//Triangle_Mode_Program                 = CreateProgram(VS_SRC_OUT_POS_1, surface_fs_src);
	//Triangle_Mode_Program                 = CreateProgram(VS_SRC_OUT_POS_1, Gen_simple_FS(boundingbox_vertex_color));


	

	//uint8_t R = r * 255;
	//uint8_t G = g * 255;
	//uint8_t B = b * 255;
	//uint8_t A = 1.0 * 255;


	// uint8_t R = rand() % 256;
	// uint8_t G = rand() % 256;
	// uint8_t B = rand() % 256;
	// uint8_t A = 255;

	uint8_t R = 0, G = 0, B = 0, A = 0;
	
	if (is_out_side_normal_space == false) {//inside the normal space.
		R = 255;
		G = 255;
		B = 255;
		A = 255;
	}
	else{

		if (where_the_mesh[0]==true){
		}

		if (where_the_mesh[1] == true){
		}

		if (where_the_mesh[2] == true){
		}

		if (where_the_mesh[3] == true){
		}

		if (to_draw_clean_mesh==1){
			R = 255;
			G = 255;
			B = 255;
			A = 255;
		}
		else
		{
			if (true) {

				R = rand() % 256;
				G = rand() % 256;
				B = rand() % 256;
				glm::uvec3 color = glm::uvec3(R, G, B);

				static std::vector<glm::uvec3> used_colors;

				while (std::find(used_colors.begin(), used_colors.end(), color) != used_colors.end()) {
					R = rand() % 256;
					G = rand() % 256;
					B = rand() % 256;
				}
				used_colors.push_back(color);
			}
			else
			{
				//
				//float value = ((float)mesh_id / (float)40);
				//float r = 0.0f, g = 0.0f, b = 0.0f;
				//getHeatMapColor(value, &r, &g, &b);
				//

				//
				// R = r * 255;
				// G = g * 255;
				// B = b * 255;
				//

				glm::uvec3 colors[22] = {
					glm::uvec3(142,207,201),
					glm::uvec3(255,190,122),
					glm::uvec3(250,127,111),
					glm::uvec3(130,176,210),
					glm::uvec3(190,184,220),
					glm::uvec3(231,218,210),
					glm::uvec3(153,153,153),

					glm::uvec3(40, 120, 181),
					glm::uvec3(154,201,219),
					glm::uvec3(248,172,140),
					glm::uvec3(200,36,35),
					glm::uvec3(255,136,132),

					glm::uvec3(20, 81, 124),
					glm::uvec3(47, 127, 193),
					glm::uvec3(231,239,250),
					glm::uvec3(150,195,125),
					glm::uvec3(243,210,102),
					glm::uvec3(216, 56, 58),
					glm::uvec3(247, 255, 237),
					glm::uvec3(248,243,249),
					glm::uvec3(196,151,178),
					glm::uvec3(169,184,194)
				};

				static std::vector<uint32_t> used_colors;

				uint32_t index = rand() % (sizeof(colors) / sizeof(glm::uvec3));
				while (std::find(used_colors.begin(), used_colors.end(), index) != used_colors.end()) {
					index = rand() % (sizeof(colors) / sizeof(glm::uvec3));
				}
				used_colors.push_back(index);

				R = colors[index].x;
				G = colors[index].y;
				B = colors[index].z;
			}
		}
		
	}



	//Triangle_Mode_Program = CreateProgram(VS_SRC_OUT_POS_WITH_MVP, surface_fs_src);
	Triangle_Mode_Program = CreateProgram(VS_SRC_OUT_POS_WITH_MVP, Gen_simple_FS(glm::uvec3(R,G,B)));

	mesh_vertex_Program = CreateProgram(VS_SRC_OUT_POS_WITH_MVP, Gen_simple_FS(vertex_color));
	BoundaryVertex_Program = CreateProgram(VS_SRC_OUT_POS_WITH_MVP, Gen_simple_FS(boundary_vertex_color));


	BoundaryVertex_connect_Program = CreateProgram(VS_SRC_OUT_POS_WITH_MVP, Gen_simple_FS(boundary_vertex_connect_color));
	mesh_line_Program = CreateProgram(VS_SRC_OUT_POS_WITH_MVP, Gen_simple_FS(line_color));

	
	mesh_boundingBox_vertex_Program  = CreateProgram(VS_SRC_OUT_POS_WITH_MVP, Gen_simple_FS(boundingbox_vertex_color));
	mesh_boundingBox_line_Program    = CreateProgram(VS_SRC_OUT_POS_WITH_MVP, Gen_simple_FS(boundingbox_line_color));
	mesh_boundingBox_surface_Program = CreateProgram(VS_SRC_OUT_POS_WITH_MVP, Gen_simple_FS(boundingbox_surface_color));
#endif



// to draw 3D cube?
#define TO_DRAW_INPUT_MESH_WITH_MVP 0         
#if TO_DRAW_INPUT_MESH_WITH_MVP 

	const std::string default_vs_src =
		"#version 330 core                                     \n"
		"layout(location = 0) in highp vec3 in_position;       \n"
		"uniform highp mat4 mvp;                               \n"
		"void main()                                           \n"
		"{                                                     \n"
		"    gl_Position = mvp*vec4(in_position.xyz,1.0);      \n"
		"}                                                     \n";
	mesh_vertex_Program = CreateProgram(default_vs_src, Gen_simple_FS(vertex_color));
	BoundaryVertex_Program = CreateProgram(default_vs_src, Gen_simple_FS(boundary_vertex_color));

	BoundaryVertex_connect_Program = CreateProgram(default_vs_src, Gen_simple_FS(boundary_vertex_connect_color));
	mesh_line_Program = CreateProgram(default_vs_src, Gen_simple_FS(line_color));

	Triangle_Mode_Program = CreateProgram(surface_vs_src, surface_fs_src);

	mesh_boundingBox_vertex_Program = CreateProgram(default_vs_src, Gen_simple_FS(boundingbox_vertex_color));
	mesh_boundingBox_line_Program = CreateProgram(default_vs_src, Gen_simple_FS(boundingbox_line_color));
	mesh_boundingBox_surface_Program = CreateProgram(default_vs_src, Gen_simple_FS(boundingbox_surface_color));
#endif




	//To create the VAO object.

	//the original mesh information
	mesh_vertex_VA0 = Gen_Mesh_Vertex_VAO();

	//if vertex count equal to 3*n, we get it is a triangle mode.
	if ((mesh_vertex_count % 3) == 0) {
		//mesh type is triangle.
		Render_Mode = GL_TRIANGLES;

		//triangle LINE VAO.
		mesh_triangle_line_VA0 = Gen_Mesh_Triangle_Line_VAO();
	}
	else {
		//mesh type is point.
		Render_Mode = GL_POINTS;
	}


	//the original mesh information
	//mesh_surface_VA0 = Gen_Mesh_Vertex_VAO();

	//SHUTDOWN LINE VAO.
	//mesh_line_VA0          = Gen_Mesh_Triangle_Line_VAO();


	boundary_vertex_VAO = Gen_Boundary_Vertex_VAO();


	//the external boundingbox
	Gen_BoundingBox_Vertex();//from the 6 boundary vertex gen 8 boundingbox vertex.

	boundingbox_vertex_VA0  = Gen_BoundingBox_Vertex_VAO();
	boundingbox_surface_VA0 = Gen_BoundingBox_Surface_VAO();
	boundingbox_line_VA0    = Gen_BoundingBox_Line_VAO();


#ifdef show_local
	static uint32_t  mesh_id = 0;
	if (mesh_id == 2)
	{
		Gen_BoundingBox_Vertex_local();
		boundary_vertex_VAO_local = Gen_Boundary_Vertex_VAO_local();
		boundingbox_vertex_VAO_local = Gen_BoundingBox_Vertex_VAO_local();
		boundingbox_surface_VAO_local = Gen_BoundingBox_Surface_VAO_local();
		boundingbox_line_VAO_local = Gen_BoundingBox_Line_VAO_local();
	}
	mesh_id++;
#endif

	//all_mesh.push_back(this);

	if (tex_arg != nullptr) {
		for (size_t i = 0; i < 16; i++)
		{
			textures[i] = tex_arg[i];
		}
	}
	else
	{
		for (size_t i = 0; i < 16; i++)
		{
			textures[i] = 0;
		}
	}


	assert( (glGetError() == GL_NO_ERROR) && "GL ERROR");

	cout << "\t create mesh success. vertex_count : " << mesh_vertex_count << endl << endl;
};




void Mesh::Updata_BoundaryVertex(const Vertex &t_vertex, const GLuint t_vid) {

	//Updata max_x 
	if (t_vertex.Position.x > BoundaryVertex[0].Position.x) {
		BoundaryVertex[0] = t_vertex;
		BoundaryVertex_VID[0] = t_vid;
	}

	//Updata min_x 
	if (t_vertex.Position.x < BoundaryVertex[1].Position.x) {
		BoundaryVertex[1] = t_vertex;
		BoundaryVertex_VID[1] = t_vid;
	}

	//Updata max_y 
	if (t_vertex.Position.y > BoundaryVertex[2].Position.y) {
		BoundaryVertex[2] = t_vertex;
		BoundaryVertex_VID[2] = t_vid;
	}

	//Updata min_y
	if (t_vertex.Position.y < BoundaryVertex[3].Position.y) {
		BoundaryVertex[3] = t_vertex;
		BoundaryVertex_VID[3] = t_vid;
	}

	//Updata max_z 
	if (t_vertex.Position.z > BoundaryVertex[4].Position.z) {
		BoundaryVertex[4] = t_vertex;
		BoundaryVertex_VID[4] = t_vid;
	}

	//Updata min_z 
	if (t_vertex.Position.z < BoundaryVertex[5].Position.z) {
		BoundaryVertex[5] = t_vertex;
		BoundaryVertex_VID[5] = t_vid;
	}

}

GLuint Mesh::Gen_VAO(
		const std::vector<Vertex>& t_Vertices,
		const std::vector<Vertex>& t_Indices,
		const std::vector<string>& attribute_list)
{

	uint32_t mesh_vertex_count = t_Vertices.size();
	assert((t_Vertices.size() == 0) && "Error: Invalid input Vertices.");
	


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
};
