
GLuint Mesh::Gen_BoundingBox_Vertex_VAO() {
	assert(mesh_boundingbox_vertex.size() == 8 && "error: expect the 6 boundary vertex position.");
	GLuint attribute_vec_size = 4;

	uint32_t t_buffer_size = mesh_boundingbox_vertex.size() * attribute_vec_size;
	GLfloat * t_vertex_buffer = new GLfloat[t_buffer_size];

	for (size_t i = 0; i < mesh_boundingbox_vertex.size(); i++) {
		t_vertex_buffer[i * 4 + 0] = mesh_boundingbox_vertex[i].x;
		t_vertex_buffer[i * 4 + 1] = mesh_boundingbox_vertex[i].y;
		t_vertex_buffer[i * 4 + 2] = mesh_boundingbox_vertex[i].z;
		t_vertex_buffer[i * 4 + 3] = mesh_boundingbox_vertex[i].w;
	}

	GLuint  VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*t_buffer_size, t_vertex_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));

	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));


	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	delete[] t_vertex_buffer;
	return VAO;
}

GLuint Mesh::Gen_BoundingBox_Surface_VAO() {


	assert((BoundaryVertex.size() == 6) && "error: invalid boundary vertex count.");

	float max_x = BoundaryVertex[0].Position.x;
	float min_x = BoundaryVertex[1].Position.x;
										  
	float max_y = BoundaryVertex[2].Position.y;
	float min_y = BoundaryVertex[3].Position.y;
										  
	float max_z = BoundaryVertex[4].Position.z;
	float min_z = BoundaryVertex[5].Position.z;


	float x_length = (max_x - min_x);
	float y_length = (max_y - min_y);
	float z_length = (max_z - min_z);


	float mid_x = (x_length / 2) + min_x;
	float mid_y = (y_length / 2) + min_y;
	float mid_z = (z_length / 2) + min_z;


	std::vector<glm::vec4> triangle_pos;

	float array[3] = { 0.0f, 0.0f, 0.0f };
	array[0] = x_length / 2;
	array[1] = y_length / 2;
	array[2] = z_length / 2;

	for (uint32_t index_0 = 0; index_0 < 3; index_0++) {
		//which surface we choosed as base.
		for (uint32_t pos = 0; pos < 2; pos++) {

			float point[4][3] = {
					{ 0.0f, 0.0f, 0.0f},
					{ 0.0f, 0.0f, 0.0f},
					{ 0.0f, 0.0f, 0.0f},
					{ 0.0f, 0.0f, 0.0f},
			};

			glm::vec4 vertex[4] = {};

			//0 for positive, 1 for negtive
			float pos_0 = (pos == 0) ? 1.0f : -1.0f;
			float value_0 = pos_0 * array[index_0];

			uint32_t index_1 = 0;
			uint32_t index_2 = 0;

			if (index_0 == 0) {
				index_1 = 1;
				index_2 = 2;
			}

			if (index_0 == 1) {
				index_1 = 0;
				index_2 = 2;
			}

			if (index_0 == 2) {
				index_1 = 0;
				index_2 = 1;
			}

			for (uint32_t k = 0; k < 4; k++) {

				//0 for positive, 1 for negtive
				float pos_1 = ((k & 0x1) == 0) ? 1.0 : -1.0;
				float pos_2 = ((k & 0x2) == 0) ? 1.0 : -1.0;

				float value_1 = pos_1 * array[index_1];
				float value_2 = pos_2 * array[index_2];

				point[k][index_0] = value_0;
				point[k][index_1] = value_1;
				point[k][index_2] = value_2;
			}

			//out << "0, 0, " << point[0][0] << ", " << point[0][1] << ", " << point[0][2] << endl;
			//out << "0, 0, " << point[1][0] << ", " << point[1][1] << ", " << point[1][2] << endl;
			//out << "0, 0, " << point[2][0] << ", " << point[2][1] << ", " << point[2][2] << endl;
			//
			//
			//out << "0, 0, " << point[3][0] << ", " << point[3][1] << ", " << point[3][2] << endl;
			//out << "0, 0, " << point[1][0] << ", " << point[1][1] << ", " << point[1][2] << endl;
			//out << "0, 0, " << point[2][0] << ", " << point[2][1] << ", " << point[2][2] << endl;

			//cout<<"============"<<endl;

			glm::vec4 pos0, pos1, pos2, pos3;
			pos0.x = point[0][0]; pos0.y = point[0][1]; pos0.z = point[0][2]; pos0.w = 1.0f;
			pos1.x = point[1][0]; pos1.y = point[1][1]; pos1.z = point[1][2]; pos1.w = 1.0f;
			pos2.x = point[2][0]; pos2.y = point[2][1]; pos2.z = point[2][2]; pos2.w = 1.0f;
			triangle_pos.push_back(pos0);
			triangle_pos.push_back(pos1);
			triangle_pos.push_back(pos2);

			pos3.x = point[3][0]; pos3.y = point[3][1]; pos3.z = point[3][2]; pos3.w = 1.0f;
			pos1.x = point[1][0]; pos1.y = point[1][1]; pos1.z = point[1][2]; pos1.w = 1.0f;
			pos2.x = point[2][0]; pos2.y = point[2][1]; pos2.z = point[2][2]; pos2.w = 1.0f;
			triangle_pos.push_back(pos3);
			triangle_pos.push_back(pos1);
			triangle_pos.push_back(pos2);

		}
	}

	assert((triangle_pos.size() == 36) && "error: invalid triangle count for build boundingbox");

	float * t_position_buffer = new float[triangle_pos.size() * 4];
	for (size_t i = 0; i < triangle_pos.size(); i++) {
		t_position_buffer[i * 4 + 0] = triangle_pos[i].x + mid_x;
		t_position_buffer[i * 4 + 1] = triangle_pos[i].y + mid_y;
		t_position_buffer[i * 4 + 2] = triangle_pos[i].z + mid_z;
		t_position_buffer[i * 4 + 3] = triangle_pos[i].w;

		//t_position_buffer[i * 4 + 0] = triangle_pos[i].x;
		//t_position_buffer[i * 4 + 1] = triangle_pos[i].y;
		//t_position_buffer[i * 4 + 2] = triangle_pos[i].z;
		//t_position_buffer[i * 4 + 3] = triangle_pos[i].w;
	}



	uint32_t attribute_vec_size = 4;
	GLuint   VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*triangle_pos.size() * 4, t_position_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));


	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));

	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	delete[] t_position_buffer;
	return VAO;

}

GLuint Mesh::Gen_BoundingBox_Line_VAO() {

	assert(mesh_boundingbox_vertex.size() == 8 && "error: expect the 6 boundary vertex position.");
	GLuint attribute_vec_size = 4;


	//12 lines, each line 2 vertex, each vertex is vec4;
	uint32_t t_line_vertex[12][2] = {
		//
		{0, 1},
		{1, 3},
		{3, 2},
		{2, 0},
		//
		{4, 5},
		{5, 7},
		{7, 6},
		{6, 4},
		//
		{0, 4},
		{1, 5},
		{3, 7},
		{2, 6},
	};

	GLfloat * t_vertex_buffer = new GLfloat[12 * 2 * 4];

	for (size_t i = 0; i < 12; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			uint32_t t_vid = t_line_vertex[i][j];
			t_vertex_buffer[(i * 2 + j) * 4 + 0] = mesh_boundingbox_vertex[t_vid].x;
			t_vertex_buffer[(i * 2 + j) * 4 + 1] = mesh_boundingbox_vertex[t_vid].y;
			t_vertex_buffer[(i * 2 + j) * 4 + 2] = mesh_boundingbox_vertex[t_vid].z;
			t_vertex_buffer[(i * 2 + j) * 4 + 3] = mesh_boundingbox_vertex[t_vid].w;
		}
	}

	GLuint  VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 2 * 4, t_vertex_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(mesh_line_Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));

	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	delete[] t_vertex_buffer;
	return VAO;
}






void Mesh::Gen_Init_Model_Matrix(
	bool need_init_model_matrix,
	bool use_global_init,
	const glm::vec3& t_Init_Scale,
	const glm::vec3& t_Init_Rotate,
	const glm::vec3& t_Init_Trans
) {

	//
	// Model matrix operation step:
	// 1. scale 
	// 2. rotate
	// 3. shift
	//
	
	init_model_matrix = glm::mat4(1.0f);
	if (need_init_model_matrix == true) {

		if (use_global_init) {
			std::cout<<"Mesh use GLOBAL Init Matrix Arg."<< std::endl;
			//step 1. do scale.
			init_model_matrix = glm::scale(init_model_matrix, glm::vec3(t_Init_Scale.x, t_Init_Scale.y, t_Init_Scale.z));

			//step 2. do rotate. rotate on x, y, z aix
			glm::mat4 init_rotate_mat = glm::mat4(1.0f);
			init_rotate_mat = glm::rotate(init_rotate_mat, glm::radians(float(t_Init_Rotate.x)), glm::vec3(1.0, 0.0f, 0.f));
			init_rotate_mat = glm::rotate(init_rotate_mat, glm::radians(float(t_Init_Rotate.y)), glm::vec3(0.0f, 1.0, 0.f));
			init_rotate_mat = glm::rotate(init_rotate_mat, glm::radians(float(t_Init_Rotate.z)), glm::vec3(0.0f, 0.f, 1.f));
			init_model_matrix = init_rotate_mat * init_model_matrix;

			//
			//step 3. do trans, shift mesh mid_pos to (0, 0, 0);
			init_model_matrix = glm::translate(init_model_matrix, glm::vec3(t_Init_Trans.x, t_Init_Trans.y, t_Init_Trans.z));
		}
		else {

			std::cout << "Mesh use LOCAL Init Matrix Arg." << std::endl;
			//force mesh to world center

			float max_x = BoundaryVertex[0].Position.x;
			float min_x = BoundaryVertex[1].Position.x;

			float max_y = BoundaryVertex[2].Position.y;
			float min_y = BoundaryVertex[3].Position.y;

			float max_z = BoundaryVertex[4].Position.z;
			float min_z = BoundaryVertex[5].Position.z;


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
			glm::vec3 t_init_scale_vec = glm::vec3(scale_arg);
			cout << "\t init_scale_vec: (" << t_init_scale_vec.x << "," << t_init_scale_vec.y << "," << t_init_scale_vec.z << ")" << std::endl;


			//step 2. get init rotate matrix,
			//actually currently do not do do init rotate.
			glm::vec3 t_init_rotate_vec = glm::vec3(0.0f);
			std::cout << "\t init_rotate_vec: (" << t_init_rotate_vec.x << "," << t_init_rotate_vec.y << "," << t_init_rotate_vec.z << ")" << std::endl;


			//step 3. get init shift matrix
			std::cout << "\t mesh mid_pos : " << " \t(" << mid_x << ", " << mid_y << ", " << mid_z << ")" << std::endl;
			glm::vec3 t_init_trans_vec = glm::vec3(0.0f - mid_x, 0.0f - mid_y, 0.0f - mid_z);
			std::cout << "\t move mesh mid_pos to origin(0, 0, 0), init_trans_vec: (" << t_init_trans_vec.x << "," << t_init_trans_vec.y << "," << t_init_trans_vec.z << ")" << std::endl;

			std::cout << std::endl << std::endl;


			//step 1. do scale.
			init_model_matrix = glm::scale(init_model_matrix, glm::vec3(t_init_scale_vec.x, t_init_scale_vec.y, t_init_scale_vec.z));


			//step 2. do rotate. rotate on x, y, z aix
			glm::mat4 init_rotate_mat = glm::mat4(1.0f);
			init_rotate_mat = glm::rotate(init_rotate_mat, glm::radians(float(t_init_rotate_vec.x)), glm::vec3(1.0, 0.0f, 0.f));
			init_rotate_mat = glm::rotate(init_rotate_mat, glm::radians(float(t_init_rotate_vec.y)), glm::vec3(0.0f, 1.0, 0.f));
			init_rotate_mat = glm::rotate(init_rotate_mat, glm::radians(float(t_init_rotate_vec.z)), glm::vec3(0.0f, 0.f, 1.f));
			init_model_matrix = init_rotate_mat * init_model_matrix;


			//
			//step 3. do trans, shift mesh mid_pos to (0, 0, 0);
			init_model_matrix = glm::translate(init_model_matrix, glm::vec3(t_init_trans_vec.x, t_init_trans_vec.y, t_init_trans_vec.z));
			//trans to zero.
		}
	}

#if 0 
{

		//after sacling reset to board
		float max_x_1 = max_x * scale_vec.x;
		float min_x_1 = min_x * scale_vec.x;

		float max_y_1 = max_y * scale_vec.y;
		float min_y_1 = min_y * scale_vec.y;

		float max_z_1 = max_z * scale_vec.z;
		float min_z_1 = min_z * scale_vec.z;

		float x_length_1 = x_length * scale_vec.x;
		float y_length_1 = y_length * scale_vec.y;
		float z_length_1 = z_length * scale_vec.z;

		//from orignal mid get new mid_pos;
		float mid_x_1 = (x_length_1 / 2) + min_x_1;
		float mid_y_1 = (y_length_1 / 2) + min_y_1;
		float mid_z_1 = (z_length_1 / 2) + min_z_1;



		//assert(y_length_1>0.0f);
		if (rotate_vec.x == 0)
		{
			init_model_matrix = glm::translate(init_model_matrix, glm::vec3(0.0, (0.0 - (y_length_1 / 2)), 0.0));//shift
			init_model_matrix = glm::translate(init_model_matrix, glm::vec3(0.0f, (0.0 - (board_height / 2)), 0.0f));
		}
		if (rotate_vec.x == 180)
		{
			init_model_matrix = glm::translate(init_model_matrix, glm::vec3(0.0, (0.0 + (y_length / 2)), 0.0));//shift//??? why???
			init_model_matrix = glm::translate(init_model_matrix, glm::vec3(0.0f, (0.0 + (board_height / 2)), 0.0f));
			//init_model = glm::translate(init_model, glm::vec3(0.0 , 0.0 - min_x, 0.0));
		}
		//init_model = glm::translate(init_model, glm::vec3(-18.423, 0.0, 0.0));

		//init_model = glm::translate(init_model, glm::vec3(init_trans_vec.x, init_trans_vec.y, init_trans_vec.z));

		init_model_matrix = glm::translate(init_model_matrix, trans_vec);

		//glm::vec3 init_shift_vec = glm::vec3(0.0f - mid_x_1, 0.0f - mid_y_1, 0.0f - mid_z_1);
		//cout << "\t Init trans vec:(" << init_shift_vec.x << "," << init_shift_vec.y << "," << init_shift_vec.z << ")" << endl;

		//
		//init_model = init_model * init_rotate_mat;





		//init_model = glm::translate(init_model, glm::vec3(0.0, init_shift_vec.y, 0.0));
		//init_model = glm::translate(init_model, glm::vec3(0.0f, (0.0- y_length_1/2) -(board_height/2), 0.0f));


		//nit_model = glm::rotate(init_model,  glm::radians(float(180)), glm::vec3(1.0f, 0.0f, 0.0f));//rotate.
		//init_model = glm::rotate(init_model, glm::radians(float(180)), glm::vec3(0.0f, mid_y, 0.0f));
		//init_model = glm::rotate(init_model, glm::radians(float(90)), glm::vec3(0.0f, 0.0f, mid_z));
	}
#endif


}



#ifdef show_local


GLuint Mesh::Gen_Boundary_Vertex_VAO_local() {

	assert(BoundaryVertex_local.size() == 6 && "error: expect the 6 boundary vertex position.");


	constexpr GLuint attribute_vec_size = 4;
	uint32_t t_buffer_size = BoundaryVertex_local.size() * attribute_vec_size;

	uint32_t boundary_vertex_count = 6;
	GLfloat * t_vertex_buffer = new GLfloat[t_buffer_size];
	memset(t_vertex_buffer, 0, t_buffer_size);

	for (size_t i = 0; i < BoundaryVertex_local.size(); i++) {
		t_vertex_buffer[i * 4 + 0] = BoundaryVertex_local[i].x;
		t_vertex_buffer[i * 4 + 1] = BoundaryVertex_local[i].y;
		t_vertex_buffer[i * 4 + 2] = BoundaryVertex_local[i].z;
		t_vertex_buffer[i * 4 + 3] = BoundaryVertex_local[i].w;
	}

	GLuint  VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*boundary_vertex_count*attribute_vec_size, t_vertex_buffer, GL_STATIC_DRAW));


	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));


	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	delete[] t_vertex_buffer;
	return VAO;
}

void Mesh::Gen_BoundingBox_Vertex_local() {

	assert(BoundaryVertex_local.size() == 6 && "error:  boundary_vertex size must be 6.");
	assert(mesh_boundingbox_vertex_local.size() == 0 && "error: before we generate boundingbox vertex. boundingbox_vertex size must be 0.");


	float max_x = BoundaryVertex_local[0].x;
	float min_x = BoundaryVertex_local[1].x;

	float max_y = BoundaryVertex_local[2].y;
	float min_y = BoundaryVertex_local[3].y;

	float max_z = BoundaryVertex_local[4].z;
	float min_z = BoundaryVertex_local[5].z;

	//{
	//max_x
	glm::vec4 t_vertex[8];
	for (size_t i = 0; i < 8; i++) {
		//init it.
		t_vertex[0].x = 0.0f;
		t_vertex[0].y = 0.0f;
		t_vertex[0].z = 0.0f;
		t_vertex[0].w = 1.0f;
	}

	t_vertex[0].x = max_x;
	t_vertex[0].y = max_y;
	t_vertex[0].z = max_z;
	t_vertex[0].w = 1.0f;

	t_vertex[1].x = max_x;
	t_vertex[1].y = max_y;
	t_vertex[1].z = min_z;
	t_vertex[1].w = 1.0f;

	t_vertex[2].x = max_x;
	t_vertex[2].y = min_y;
	t_vertex[2].z = max_z;
	t_vertex[2].w = 1.0f;

	t_vertex[3].x = max_x;
	t_vertex[3].y = min_y;
	t_vertex[3].z = min_z;
	t_vertex[3].w = 1.0f;
	//}

	//{
	//min_x
	t_vertex[4].x = min_x;
	t_vertex[4].y = max_y;
	t_vertex[4].z = max_z;
	t_vertex[4].w = 1.0f;

	t_vertex[5].x = min_x;
	t_vertex[5].y = max_y;
	t_vertex[5].z = min_z;
	t_vertex[5].w = 1.0f;

	t_vertex[6].x = min_x;
	t_vertex[6].y = min_y;
	t_vertex[6].z = max_z;
	t_vertex[6].w = 1.0f;

	t_vertex[7].x = min_x;
	t_vertex[7].y = min_y;
	t_vertex[7].z = min_z;
	t_vertex[7].w = 1.0f;
	//}


	cout << "\t Gen BoundingBox vertex success:" << endl;
	for (size_t i = 0; i < 8; i++)
	{
		cout << "\t \t vertex[" << i << "]= (" << t_vertex[i].x << ", " << t_vertex[i].y << "," << t_vertex[i].z << ")" << endl;
		mesh_boundingbox_vertex_local.push_back(t_vertex[i]);
	}
}

GLuint Mesh::Gen_BoundingBox_Vertex_VAO_local() {
	assert(mesh_boundingbox_vertex_local.size() == 8 && "error: expect the 6 boundary vertex position.");
	GLuint attribute_vec_size = 4;

	uint32_t t_buffer_size = mesh_boundingbox_vertex_local.size() * attribute_vec_size;
	GLfloat * t_vertex_buffer = new GLfloat[t_buffer_size];

	for (size_t i = 0; i < mesh_boundingbox_vertex_local.size(); i++) {
		t_vertex_buffer[i * 4 + 0] = mesh_boundingbox_vertex_local[i].x;
		t_vertex_buffer[i * 4 + 1] = mesh_boundingbox_vertex_local[i].y;
		t_vertex_buffer[i * 4 + 2] = mesh_boundingbox_vertex_local[i].z;
		t_vertex_buffer[i * 4 + 3] = mesh_boundingbox_vertex_local[i].w;
	}

	GLuint  VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*t_buffer_size, t_vertex_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));

	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));


	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	delete[] t_vertex_buffer;
	return VAO;
}

GLuint Mesh::Gen_BoundingBox_Surface_VAO_local() {


	assert((BoundaryVertex_local.size() == 6) && "error: invalid boundary vertex count.");

	float max_x = BoundaryVertex_local[0].x;
	float min_x = BoundaryVertex_local[1].x;

	float max_y = BoundaryVertex_local[2].y;
	float min_y = BoundaryVertex_local[3].y;

	float max_z = BoundaryVertex_local[4].z;
	float min_z = BoundaryVertex_local[5].z;


	float x_length = (max_x - min_x);
	float y_length = (max_y - min_y);
	float z_length = (max_z - min_z);


	float mid_x = (x_length / 2) + min_x;
	float mid_y = (y_length / 2) + min_y;
	float mid_z = (z_length / 2) + min_z;


	std::vector<glm::vec4> triangle_pos;

	float array[3] = { 0.0f, 0.0f, 0.0f };
	array[0] = x_length / 2;
	array[1] = y_length / 2;
	array[2] = z_length / 2;

	for (uint32_t index_0 = 0; index_0 < 3; index_0++) {
		//which surface we choosed as base.
		for (uint32_t pos = 0; pos < 2; pos++) {

			float point[4][3] = {
					{ 0.0f, 0.0f, 0.0f},
					{ 0.0f, 0.0f, 0.0f},
					{ 0.0f, 0.0f, 0.0f},
					{ 0.0f, 0.0f, 0.0f},
			};

			glm::vec4 vertex[4] = {};

			//0 for positive, 1 for negtive
			float pos_0 = (pos == 0) ? 1.0f : -1.0f;
			float value_0 = pos_0 * array[index_0];

			uint32_t index_1 = 0;
			uint32_t index_2 = 0;

			if (index_0 == 0) {
				index_1 = 1;
				index_2 = 2;
			}

			if (index_0 == 1) {
				index_1 = 0;
				index_2 = 2;
			}

			if (index_0 == 2) {
				index_1 = 0;
				index_2 = 1;
			}

			for (uint32_t k = 0; k < 4; k++) {

				//0 for positive, 1 for negtive
				float pos_1 = ((k & 0x1) == 0) ? 1.0 : -1.0;
				float pos_2 = ((k & 0x2) == 0) ? 1.0 : -1.0;

				float value_1 = pos_1 * array[index_1];
				float value_2 = pos_2 * array[index_2];

				point[k][index_0] = value_0;
				point[k][index_1] = value_1;
				point[k][index_2] = value_2;
			}

			//out << "0, 0, " << point[0][0] << ", " << point[0][1] << ", " << point[0][2] << endl;
			//out << "0, 0, " << point[1][0] << ", " << point[1][1] << ", " << point[1][2] << endl;
			//out << "0, 0, " << point[2][0] << ", " << point[2][1] << ", " << point[2][2] << endl;
			//
			//
			//out << "0, 0, " << point[3][0] << ", " << point[3][1] << ", " << point[3][2] << endl;
			//out << "0, 0, " << point[1][0] << ", " << point[1][1] << ", " << point[1][2] << endl;
			//out << "0, 0, " << point[2][0] << ", " << point[2][1] << ", " << point[2][2] << endl;

			//cout<<"============"<<endl;

			glm::vec4 pos0, pos1, pos2, pos3;
			pos0.x = point[0][0]; pos0.y = point[0][1]; pos0.z = point[0][2]; pos0.w = 1.0f;
			pos1.x = point[1][0]; pos1.y = point[1][1]; pos1.z = point[1][2]; pos1.w = 1.0f;
			pos2.x = point[2][0]; pos2.y = point[2][1]; pos2.z = point[2][2]; pos2.w = 1.0f;
			triangle_pos.push_back(pos0);
			triangle_pos.push_back(pos1);
			triangle_pos.push_back(pos2);

			pos3.x = point[3][0]; pos3.y = point[3][1]; pos3.z = point[3][2]; pos3.w = 1.0f;
			pos1.x = point[1][0]; pos1.y = point[1][1]; pos1.z = point[1][2]; pos1.w = 1.0f;
			pos2.x = point[2][0]; pos2.y = point[2][1]; pos2.z = point[2][2]; pos2.w = 1.0f;
			triangle_pos.push_back(pos3);
			triangle_pos.push_back(pos1);
			triangle_pos.push_back(pos2);

		}
	}

	assert((triangle_pos.size() == 36) && "error: invalid triangle count for build boundingbox");

	float * t_position_buffer = new float[triangle_pos.size() * 4];
	for (size_t i = 0; i < triangle_pos.size(); i++) {
		t_position_buffer[i * 4 + 0] = triangle_pos[i].x + mid_x;
		t_position_buffer[i * 4 + 1] = triangle_pos[i].y + mid_y;
		t_position_buffer[i * 4 + 2] = triangle_pos[i].z + mid_z;
		t_position_buffer[i * 4 + 3] = triangle_pos[i].w;

		//t_position_buffer[i * 4 + 0] = triangle_pos[i].x;
		//t_position_buffer[i * 4 + 1] = triangle_pos[i].y;
		//t_position_buffer[i * 4 + 2] = triangle_pos[i].z;
		//t_position_buffer[i * 4 + 3] = triangle_pos[i].w;
	}



	uint32_t attribute_vec_size = 4;
	GLuint   VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*triangle_pos.size() * 4, t_position_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));


	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));

	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	delete[] t_position_buffer;
	return VAO;

}

GLuint Mesh::Gen_BoundingBox_Line_VAO_local() {

	assert(mesh_boundingbox_vertex_local.size() == 8 && "error: expect the 6 boundary vertex position.");
	GLuint attribute_vec_size = 4;


	//12 lines, each line 2 vertex, each vertex is vec4;
	uint32_t t_line_vertex[12][2] = {
		//
		{0, 1},
		{1, 3},
		{3, 2},
		{2, 0},
		//
		{4, 5},
		{5, 7},
		{7, 6},
		{6, 4},
		//
		{0, 4},
		{1, 5},
		{3, 7},
		{2, 6},
	};

	GLfloat * t_vertex_buffer = new GLfloat[12 * 2 * 4];

	for (size_t i = 0; i < 12; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			uint32_t t_vid = t_line_vertex[i][j];
			t_vertex_buffer[(i * 2 + j) * 4 + 0] = mesh_boundingbox_vertex_local[t_vid].x;
			t_vertex_buffer[(i * 2 + j) * 4 + 1] = mesh_boundingbox_vertex_local[t_vid].y;
			t_vertex_buffer[(i * 2 + j) * 4 + 2] = mesh_boundingbox_vertex_local[t_vid].z;
			t_vertex_buffer[(i * 2 + j) * 4 + 3] = mesh_boundingbox_vertex_local[t_vid].w;
		}
	}

	GLuint  VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 2 * 4, t_vertex_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(mesh_line_Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));

	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	delete[] t_vertex_buffer;
	return VAO;
}

#endif



void Mesh::Draw(glm::mat4 projection,
	glm::mat4 view,
	glm::mat4 model,

	bool to_draw_mesh_vertex,
	bool to_draw_BoundaryVertex,
	bool to_draw_BoundaryVertex_connect,
	bool to_draw_mesh_line,
	bool to_draw_mesh_surface,
	bool to_draw_boundingbox_vertex,
	bool to_draw_boundingbox_line,
	bool to_draw_boundingbox_surface)
{
	//cout<<"Drawing Mesh:"<<input_file<<endl;
	//glm::mat4 mvp = projection * view * model * init_model;
	glm::mat4 mvp = projection * view * model;
	
	//glm::mat4 mvp = glm::mat4(1.0);


	float default_point_size = 1.5f;
	float default_line_width = 2.5f;

	if (to_draw_mesh_vertex)
	{
		VERIFY_GL(glUseProgram(mesh_vertex_Program));

		VERIFY_GL(glEnable(GL_PROGRAM_POINT_SIZE));
		VERIFY_GL(glPointSize(default_point_size));
			
		
		//UNIFORMS
		////////////////////////////////////////////////////////////////////////////////////
		{
			GLint mvp_location = -1;
			VERIFY_GL_RET(mvp_location, glGetUniformLocation(mesh_vertex_Program, "mvp"));
			if (mvp_location != -1) {
				VERIFY_GL(glUniformMatrix4fv(mvp_location, 1, false, glm::value_ptr(mvp)));
			}
		}
		

		//ATTRIBUTES
		////////////////////////////////////////////////////////////////////////////////////
		{
			VERIFY_GL(glBindVertexArray(mesh_vertex_VA0));

			GLint pos_location = -1;
			VERIFY_GL_RET(pos_location, glGetAttribLocation(mesh_vertex_Program, "in_position"));
			if (pos_location == -1) {
				assert(0 && "error: fail to get \"in_position\" from program, please check attribute_name in shader program.");
			}

			VERIFY_GL(glEnableVertexAttribArray(pos_location));

		}
		
		//TO DRAW THE MESH.
		////////////////////////////////////////////////////////////////////////////////////
		{
			VERIFY_GL(glDrawArrays(GL_POINTS, 0, mesh_vertex_count));
		}


		//CLEAN  CONTEXT
		////////////////////////////////////////////////////////////////////////////////////
		{
			for (uint32_t i = 0; i < 16; i++) {
				VERIFY_GL(glDisableVertexAttribArray(i));
			}

			VERIFY_GL(glBindVertexArray(0));
			VERIFY_GL(glUseProgram(0));
			VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));
		}

		assert(glGetError() == GL_NO_ERROR);
	}


	if (to_draw_BoundaryVertex)
	{
		VERIFY_GL(glUseProgram(BoundaryVertex_Program));

		VERIFY_GL(glEnable(GL_PROGRAM_POINT_SIZE));
		VERIFY_GL(glPointSize(default_point_size));


		//UNIFORMS
		////////////////////////////////////////////////////////////////////////////////////
		{
			GLint mvp_location = -1;
			VERIFY_GL_RET(mvp_location, glGetUniformLocation(mesh_vertex_Program, "mvp"));
			if (mvp_location != -1) {
				VERIFY_GL(glUniformMatrix4fv(mvp_location, 1, false, glm::value_ptr(mvp)));
			}
		}
		

		//ATTRIBUTES
		////////////////////////////////////////////////////////////////////////////////////
		{
			VERIFY_GL(glBindVertexArray(boundary_vertex_VAO));

			GLint pos_location = -1;
			VERIFY_GL_RET(pos_location, glGetAttribLocation(mesh_vertex_Program, "in_position"));
			if (pos_location == -1) {
				assert(0 && "error: fail to get \"in_position\" from program, please check attribute_name in shader program.");
			}
			VERIFY_GL(glEnableVertexAttribArray(pos_location));
		}

		//TO DRAW THE MESH.
		////////////////////////////////////////////////////////////////////////////////////
		{
			//the mesh cotain 6 boundary vertex.
			constexpr uint32_t BoundaryVertex_count = 6;
			VERIFY_GL(glDrawArrays(GL_POINTS, 0, BoundaryVertex_count));
		}
		


		//CLEAN  CONTEXT
		////////////////////////////////////////////////////////////////////////////////////
		{
			for (uint32_t i = 0; i < 16; i++) {
				VERIFY_GL(glDisableVertexAttribArray(i));
			}

			VERIFY_GL(glBindVertexArray(0));
			VERIFY_GL(glUseProgram(0));
			VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));
		}

		assert(glGetError() == GL_NO_ERROR);
	}


	if (to_draw_BoundaryVertex_connect)
	{
		VERIFY_GL(glUseProgram(BoundaryVertex_connect_Program));

		VERIFY_GL(glEnable(GL_LINE_SMOOTH));
		VERIFY_GL(glLineWidth(default_line_width));

		
		//UNIFORMS
		////////////////////////////////////////////////////////////////////////////////////
		{
			GLint mvp_location = -1;
			VERIFY_GL_RET(mvp_location, glGetUniformLocation(mesh_vertex_Program, "mvp"));
			if (mvp_location != -1) {
				VERIFY_GL(glUniformMatrix4fv(mvp_location, 1, false, glm::value_ptr(mvp)));
			}
		}


		//ATTRIBUTES
		////////////////////////////////////////////////////////////////////////////////////
		{
			VERIFY_GL(glBindVertexArray(boundary_vertex_VAO));

			GLint pos_location = -1;
			VERIFY_GL_RET(pos_location, glGetAttribLocation(mesh_vertex_Program, "in_position"));
			if (pos_location == -1) {
				assert(0 && "error: fail to get \"in_position\" from program, please check attribute_name in shader program.");
			}
			VERIFY_GL(glEnableVertexAttribArray(pos_location));
		}


		//TO DRAW THE MESH.
		////////////////////////////////////////////////////////////////////////////////////
		{
			//the mesh cotain 6 boundary vertex.
			constexpr uint32_t BoundaryVertex_count = 6;
			VERIFY_GL(glDrawArrays(GL_LINES, 0, BoundaryVertex_count));
		}
		

		//CLEAN  CONTEXT
		////////////////////////////////////////////////////////////////////////////////////
		{
			for (uint32_t i = 0; i<16 ; i++){
				VERIFY_GL(glDisableVertexAttribArray(i));
			}
			
			VERIFY_GL(glBindVertexArray(0));
			VERIFY_GL(glUseProgram(0));

			VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));
			VERIFY_GL(glLineWidth(1.0));
		}

		assert(glGetError() == GL_NO_ERROR);
	}


	
	

	if (to_draw_mesh_surface) {

		//for triangles we may need to draw triangles surface and triangle line.
		if (Render_Mode == GL_TRIANGLES) {

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
				GLuint mvp_location = -1;
				VERIFY_GL_RET(mvp_location, glGetUniformLocation(Triangle_Mode_Program, "mvp"));
				if (mvp_location != -1) {//-1 for invalid, uniform variable.
					VERIFY_GL(glUniformMatrix4fv(mvp_location, 1, false, glm::value_ptr(mvp)));
				}

				GLuint time_location = -1;
				VERIFY_GL_RET(time_location, glGetUniformLocation(Triangle_Mode_Program, "frame_info"));
				if (time_location != -1) {//-1 for invalid, uniform variable.
					VERIFY_GL(glUniform4uiv(time_location, 1, glm::value_ptr(frame_info)));
				}
			}

			


			//ATTRIBUTES
			////////////////////////////////////////////////////////////////////////////////////
			{
				VERIFY_GL(glBindVertexArray(mesh_vertex_VA0));
				GLuint pos_location = -1;
				VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_position"));
				if (pos_location == -1) {
					assert(0 && "error: fail to get \"in_position\" from program, please check attribute_name in shader program.");
				}
				VERIFY_GL(glEnableVertexAttribArray(pos_location));
			}

			//TO DRAW THE MESH.
			////////////////////////////////////////////////////////////////////////////////////
			{
				VERIFY_GL(glDrawArrays(GL_TRIANGLES, 0, mesh_vertex_count));
			}

			//CLEAN  CONTEXT
			////////////////////////////////////////////////////////////////////////////////////
			{
				for (uint32_t i = 0; i < 16; i++) {
					VERIFY_GL(glDisableVertexAttribArray(i));
				}

				VERIFY_GL(glBindVertexArray(0));
				VERIFY_GL(glUseProgram(0));

				VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));
				VERIFY_GL(glLineWidth(1.0));
			}

			assert(glGetError() == GL_NO_ERROR);
		}
	}


	if (to_draw_mesh_line) {

		//update the depth to only show front line.
		{
			bool only_show_front_mesh_line = false;
			if (only_show_front_mesh_line) {

				{
					VERIFY_GL(glDisable(GL_DEPTH_TEST));
					VERIFY_GL(glDepthMask(GL_TRUE));

					//disable update color buffer, and only update depth
					VERIFY_GL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
				}

				//for triangles we may need to draw triangles surface and triangle line.
				if (Render_Mode == GL_TRIANGLES) {

					VERIFY_GL(glUseProgram(Triangle_Mode_Program));

					for (uint32_t i = 0; i < 16; i++) {
						if (textures[i] != 0) {
							VERIFY_GL(glActiveTexture(GL_TEXTURE0 + i));
							VERIFY_GL(glBindTexture(GL_TEXTURE_2D, textures[i]));
							GLuint sampler_pos = -1;
							VERIFY_GL_RET(sampler_pos, glGetUniformLocation(Triangle_Mode_Program, "texture1"));
							VERIFY_GL(glUniform1i(sampler_pos, i));
						}
					}

					//UNIFORMS
					////////////////////////////////////////////////////////////////////////////////////
					{
						GLuint mvp_location = -1;
						VERIFY_GL_RET(mvp_location, glGetUniformLocation(Triangle_Mode_Program, "mvp"));
						if (mvp_location != -1) {//-1 for invalid, uniform variable.
							VERIFY_GL(glUniformMatrix4fv(mvp_location, 1, false, glm::value_ptr(mvp)));
						}

						GLuint time_location = -1;
						VERIFY_GL_RET(time_location, glGetUniformLocation(Triangle_Mode_Program, "frame_info"));
						if (time_location != -1) {//-1 for invalid, uniform variable.
							VERIFY_GL(glUniform4uiv(time_location, 1, glm::value_ptr(frame_info)));
						}
					}




					//ATTRIBUTES
					////////////////////////////////////////////////////////////////////////////////////
					{
						VERIFY_GL(glBindVertexArray(mesh_vertex_VA0));
						GLuint pos_location = -1;
						VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_position"));
						if (pos_location == -1) {
							assert(0 && "error: fail to get \"in_position\" from program, please check attribute_name in shader program.");
						}
						VERIFY_GL(glEnableVertexAttribArray(pos_location));
					}

					//TO DRAW THE MESH.
					////////////////////////////////////////////////////////////////////////////////////
					{
						VERIFY_GL(glDrawArrays(GL_TRIANGLES, 0, mesh_vertex_count));
					}

					//CLEAN  CONTEXT
					////////////////////////////////////////////////////////////////////////////////////
					{
						for (uint32_t i = 0; i < 16; i++) {
							VERIFY_GL(glDisableVertexAttribArray(i));
						}

						VERIFY_GL(glBindVertexArray(0));
						VERIFY_GL(glUseProgram(0));

						VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));
						VERIFY_GL(glLineWidth(1.0));
					}

					assert(glGetError() == GL_NO_ERROR);
				}
				
				//enable update color buffer
				VERIFY_GL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
			}
		}


		VERIFY_GL(glUseProgram(mesh_line_Program));

		VERIFY_GL(glEnable(GL_LINE_SMOOTH));
		VERIFY_GL(glLineWidth(default_line_width));


		//UNIFORMS
		////////////////////////////////////////////////////////////////////////////////////
		{
			GLuint line_mvp_location = -1;
			VERIFY_GL_RET(line_mvp_location, glGetUniformLocation(mesh_line_Program, "mvp"));
			if (line_mvp_location != -1) {
				VERIFY_GL(glUniformMatrix4fv(line_mvp_location, 1, false, glm::value_ptr(mvp)));
			}

			GLuint time_location = -1;
			VERIFY_GL_RET(time_location, glGetUniformLocation(Triangle_Mode_Program, "global_time"));
			// if (time_location != -1) {//-1 for invalid, uniform variable.
			// 	VERIFY_GL(glUniform4fv(time_location, 1, glm::value_ptr(global_time)));
			// }
		}
		

		//ATTRIBUTES
		////////////////////////////////////////////////////////////////////////////////////
		{
			VERIFY_GL(glBindVertexArray(mesh_triangle_line_VA0));
			GLuint pos_location = -1;
			VERIFY_GL_RET(pos_location, glGetAttribLocation(mesh_line_Program, "in_position"));
			if (pos_location == -1) {
				assert(0 && "error: fail to get \"in_position\" from program, please check attribute_name in shader program.");
			}
			VERIFY_GL(glEnableVertexAttribArray(pos_location));
		}
		
		//TO DRAW THE MESH.
		////////////////////////////////////////////////////////////////////////////////////
		{
			VERIFY_GL(glDrawArrays(GL_LINES, 0, mesh_vertex_count * 2));
		}
		
		


		//CLEAN  CONTEXT
		////////////////////////////////////////////////////////////////////////////////////
		{
			for (uint32_t i = 0; i < 16; i++) {
				VERIFY_GL(glDisableVertexAttribArray(i));
			}

			VERIFY_GL(glBindVertexArray(0));
			VERIFY_GL(glUseProgram(0));

			VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));
			VERIFY_GL(glLineWidth(1.0));
		}

		
		assert(glGetError() == GL_NO_ERROR);
	}
	
	
	///////////////////////////////////////////////////////////////////////////////


	if (to_draw_boundingbox_vertex) {

		VERIFY_GL(glEnable(GL_PROGRAM_POINT_SIZE));
		VERIFY_GL(glPointSize(default_point_size));

		VERIFY_GL(glUseProgram(mesh_boundingBox_vertex_Program));

		GLuint boundingbox_surface_mvp_location = -1;
		VERIFY_GL_RET(boundingbox_surface_mvp_location, glGetUniformLocation(mesh_boundingBox_vertex_Program, "mvp"));
		VERIFY_GL(glUniformMatrix4fv(boundingbox_surface_mvp_location, 1, false, glm::value_ptr(mvp)));

		VERIFY_GL(glBindVertexArray(boundingbox_vertex_VA0));
		GLint t_boundingbox_surface_location = glGetAttribLocation(mesh_boundingBox_vertex_Program, "in_position");
		VERIFY_GL(glEnableVertexAttribArray(t_boundingbox_surface_location));
		constexpr uint32_t boundingbox_vertex_count = 8;  //boundingbox 8 vertex on it.
		VERIFY_GL(glDrawArrays(GL_POINTS, 0, boundingbox_vertex_count));//for the bounding, it contain 8 vertex on it.
		VERIFY_GL(glDisableVertexAttribArray(t_boundingbox_surface_location));
		VERIFY_GL(glBindVertexArray(0));
		VERIFY_GL(glUseProgram(0));

		VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));

		assert(glGetError() == GL_NO_ERROR);
	}


	if (to_draw_boundingbox_line) {

		VERIFY_GL(glUseProgram(mesh_boundingBox_line_Program));

		VERIFY_GL(glEnable(GL_LINE_SMOOTH));
		VERIFY_GL(glLineWidth(default_line_width));

		GLuint boundingbox_surface_mvp_location = -1;
		VERIFY_GL_RET(boundingbox_surface_mvp_location, glGetUniformLocation(mesh_boundingBox_line_Program, "mvp"));
		VERIFY_GL(glUniformMatrix4fv(boundingbox_surface_mvp_location, 1, false, glm::value_ptr(mvp)));

		VERIFY_GL(glBindVertexArray(boundingbox_line_VA0));
		GLint t_boundingbox_surface_location = glGetAttribLocation(mesh_boundingBox_line_Program, "in_position");
		VERIFY_GL(glEnableVertexAttribArray(t_boundingbox_surface_location));
		constexpr uint32_t boundingbox_line_vertex_count = 24;  //12 lines, 24 points
		VERIFY_GL(glDrawArrays(GL_LINES, 0, boundingbox_line_vertex_count));//for the bounding box lines, it contain 24 vertex on it.
		VERIFY_GL(glDisableVertexAttribArray(t_boundingbox_surface_location));
		VERIFY_GL(glBindVertexArray(0));
		VERIFY_GL(glUseProgram(0));
		assert(glGetError() == GL_NO_ERROR);
	}


	if (to_draw_boundingbox_surface) {

		VERIFY_GL(glEnable(GL_BLEND));
		VERIFY_GL(glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR));

		VERIFY_GL(glUseProgram(mesh_boundingBox_surface_Program));

		GLuint boundingbox_surface_mvp_location = -1;
		VERIFY_GL_RET(boundingbox_surface_mvp_location, glGetUniformLocation(mesh_boundingBox_surface_Program, "mvp"));
		VERIFY_GL(glUniformMatrix4fv(boundingbox_surface_mvp_location, 1, false, glm::value_ptr(mvp)));

		VERIFY_GL(glBindVertexArray(boundingbox_surface_VA0));
		GLint t_pos_location = glGetAttribLocation(mesh_boundingBox_surface_Program, "in_position");
		VERIFY_GL(glEnableVertexAttribArray(t_pos_location));
		constexpr uint32_t boundingbox_surface_vertex_count = 36;
		VERIFY_GL(glDrawArrays(GL_TRIANGLES, 0, boundingbox_surface_vertex_count));//for the bounding box,it contain 36 vertex on it.
		VERIFY_GL(glDisableVertexAttribArray(t_pos_location));
		VERIFY_GL(glBindVertexArray(0));
		VERIFY_GL(glUseProgram(0));

		VERIFY_GL(glDisable(GL_BLEND));

		assert(glGetError() == GL_NO_ERROR);
	}





	/////////////////////////////////////////////////////////////////////////////////////
#ifdef show_local

	static uint32_t t_mesh_id = 0;
	if (t_mesh_id % 2 == 1)
	{


		if (true)
		{
			VERIFY_GL(glUseProgram(Triangle_Mode_Program));

			VERIFY_GL(glEnable(GL_PROGRAM_POINT_SIZE));
			VERIFY_GL(glPointSize(5.0));

			GLuint line_mvp_location = -1;
			VERIFY_GL_RET(line_mvp_location, glGetUniformLocation(Triangle_Mode_Program, "mvp"));
			VERIFY_GL(glUniformMatrix4fv(line_mvp_location, 1, false, glm::value_ptr(mvp)));

			VERIFY_GL(glBindVertexArray(boundary_vertex_VAO_local));
			GLint t_pos_location = glGetAttribLocation(Triangle_Mode_Program, "in_position");
			VERIFY_GL(glEnableVertexAttribArray(t_pos_location));

			//the mesh cotain 6 boundary vertex.
			constexpr uint32_t BoundaryVertex_count = 6;
			VERIFY_GL(glDrawArrays(GL_POINTS, 0, BoundaryVertex_count));
			//VERIFY_GL(glDrawArrays(GL_LINES, 0, BoundaryVertex_count));
			VERIFY_GL(glDisableVertexAttribArray(t_pos_location));
			VERIFY_GL(glBindVertexArray(0));
			VERIFY_GL(glUseProgram(0));

			VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));

			assert(glGetError() == GL_NO_ERROR);
		}



		if (false)
		{
			VERIFY_GL(glUseProgram(BoundaryVertex_connect_Program));

			VERIFY_GL(glEnable(GL_PROGRAM_POINT_SIZE));
			VERIFY_GL(glPointSize(5.0f));

			GLuint line_mvp_location = -1;
			VERIFY_GL_RET(line_mvp_location, glGetUniformLocation(BoundaryVertex_connect_Program, "mvp"));
			VERIFY_GL(glUniformMatrix4fv(line_mvp_location, 1, false, glm::value_ptr(mvp)));

			VERIFY_GL(glBindVertexArray(boundary_vertex_VAO_local));
			GLint t_pos_location = glGetAttribLocation(BoundaryVertex_connect_Program, "in_position");
			VERIFY_GL(glEnableVertexAttribArray(t_pos_location));

			//the mesh cotain 6 boundary vertex.
			constexpr uint32_t BoundaryVertex_count = 6;
			//VERIFY_GL(glDrawArrays(GL_POINTS, 0, BoundaryVertex_count));
			VERIFY_GL(glDrawArrays(GL_LINES, 0, BoundaryVertex_count));
			VERIFY_GL(glDisableVertexAttribArray(t_pos_location));
			VERIFY_GL(glBindVertexArray(0));
			VERIFY_GL(glUseProgram(0));

			VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));

			assert(glGetError() == GL_NO_ERROR);
		}

		if (true) {

			VERIFY_GL(glEnable(GL_PROGRAM_POINT_SIZE));
			VERIFY_GL(glPointSize(defalt_point_size));

			VERIFY_GL(glUseProgram(mesh_boundingBox_vertex_Program));

			GLuint boundingbox_surface_mvp_location = -1;
			VERIFY_GL_RET(boundingbox_surface_mvp_location, glGetUniformLocation(mesh_boundingBox_vertex_Program, "mvp"));
			VERIFY_GL(glUniformMatrix4fv(boundingbox_surface_mvp_location, 1, false, glm::value_ptr(mvp)));

			VERIFY_GL(glBindVertexArray(boundingbox_vertex_VAO_local));
			GLint t_boundingbox_surface_location = glGetAttribLocation(mesh_boundingBox_vertex_Program, "in_position");
			VERIFY_GL(glEnableVertexAttribArray(t_boundingbox_surface_location));
			constexpr uint32_t boundingbox_vertex_count = 8;  //boundingbox 8 vertex on it.
			VERIFY_GL(glDrawArrays(GL_POINTS, 0, boundingbox_vertex_count));//for the bounding, it contain 8 vertex on it.
			VERIFY_GL(glDisableVertexAttribArray(t_boundingbox_surface_location));
			VERIFY_GL(glBindVertexArray(0));
			VERIFY_GL(glUseProgram(0));

			VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));

			assert(glGetError() == GL_NO_ERROR);
		}


		if (true) {

			VERIFY_GL(glUseProgram(mesh_boundingBox_line_Program));

			GLuint boundingbox_surface_mvp_location = -1;
			VERIFY_GL_RET(boundingbox_surface_mvp_location, glGetUniformLocation(mesh_boundingBox_line_Program, "mvp"));
			VERIFY_GL(glUniformMatrix4fv(boundingbox_surface_mvp_location, 1, false, glm::value_ptr(mvp)));

			VERIFY_GL(glBindVertexArray(boundingbox_line_VAO_local));
			GLint t_boundingbox_surface_location = glGetAttribLocation(mesh_boundingBox_line_Program, "in_position");
			VERIFY_GL(glEnableVertexAttribArray(t_boundingbox_surface_location));
			constexpr uint32_t boundingbox_line_vertex_count = 24;  //12 lines, 24 points
			VERIFY_GL(glDrawArrays(GL_LINES, 0, boundingbox_line_vertex_count));//for the bounding box lines, it contain 24 vertex on it.
			VERIFY_GL(glDisableVertexAttribArray(t_boundingbox_surface_location));
			VERIFY_GL(glBindVertexArray(0));
			VERIFY_GL(glUseProgram(0));
			assert(glGetError() == GL_NO_ERROR);
		}


		if (true) {

			VERIFY_GL(glEnable(GL_BLEND));
			VERIFY_GL(glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR));

			VERIFY_GL(glUseProgram(mesh_boundingBox_surface_Program));

			GLuint boundingbox_surface_mvp_location = -1;
			VERIFY_GL_RET(boundingbox_surface_mvp_location, glGetUniformLocation(mesh_boundingBox_surface_Program, "mvp"));
			VERIFY_GL(glUniformMatrix4fv(boundingbox_surface_mvp_location, 1, false, glm::value_ptr(mvp)));

			VERIFY_GL(glBindVertexArray(boundingbox_surface_VAO_local));
			GLint t_pos_location = glGetAttribLocation(mesh_boundingBox_surface_Program, "in_position");
			VERIFY_GL(glEnableVertexAttribArray(t_pos_location));
			constexpr uint32_t boundingbox_surface_vertex_count = 36;
			VERIFY_GL(glDrawArrays(GL_TRIANGLES, 0, boundingbox_surface_vertex_count));//for the bounding box,it contain 36 vertex on it.
			VERIFY_GL(glDisableVertexAttribArray(t_pos_location));
			VERIFY_GL(glBindVertexArray(0));
			VERIFY_GL(glUseProgram(0));

			VERIFY_GL(glDisable(GL_BLEND));

			assert(glGetError() == GL_NO_ERROR);
		}
	}
	t_mesh_id++;
#endif

}
