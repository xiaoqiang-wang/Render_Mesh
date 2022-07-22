

#include "Mesh.h"


#include "OBJ_Loader.h"

#include <algorithm>



uint32_t Triangle::is_out_of_screen() {

	/*
			   |y     (1,1)
			   | /1.0
			   |/
		 ------/--------x
			  /|
			 /z|
	(-1,-1)    |

	*/
	/*
	   if the triangle's x|y both greater than the box,then the triangle be rejected.
	*/


	uint32_t res = 0;
	if (
		(position[0].x > RIGHT_X_BOUNDARY) &&
		(position[1].x > RIGHT_X_BOUNDARY) &&
		(position[2].x > RIGHT_X_BOUNDARY)
		)
	{
		res = 1;
	}



	if (
		(position[0].x < LEFT_X_BOUNDARY) &&
		(position[0].x < LEFT_X_BOUNDARY) &&
		(position[2].x < LEFT_X_BOUNDARY)
		)
	{
		res = 2;
	}



	if (
		(position[0].y > UPPER_Y_BOUNDARY) &&
		(position[1].y > UPPER_Y_BOUNDARY) &&
		(position[2].y > UPPER_Y_BOUNDARY)
		)
	{
		res = 3;
	}


	if (
		(position[0].y < DOWN_Y_BOUNDARY) &&
		(position[1].y < DOWN_Y_BOUNDARY) &&
		(position[2].y < DOWN_Y_BOUNDARY)
		)
	{
		res = 4;
	}



	if (
		(position[0].z > FAR_Z_BOUNDARY) &&
		(position[1].z > FAR_Z_BOUNDARY) &&
		(position[2].z > FAR_Z_BOUNDARY)
		)
	{
		res = 5;
	}

	if (
		(position[0].z < NEAR_Z_BOUNDARY) &&
		(position[1].z < NEAR_Z_BOUNDARY) &&
		(position[2].z < NEAR_Z_BOUNDARY)
		)
	{
		res = 6;
	}

	return res;
};




uint32_t Point::is_out_of_screen() {

	/*
			   |y     (1,1)
			   | /1.0
			   |/
		 ------/--------x
			  /|
			 /z|
	(-1,-1)    |

	*/
	/*
	   if the triangle's x|y both greater than the box,then the triangle be rejected.
	*/


	uint32_t res = 0;
	if (position[0].x > RIGHT_X_BOUNDARY) 
	{
		res = 1;
	}

	
	if(position[0].x < LEFT_X_BOUNDARY)	
	{
		res = 2;
	}



	if(position[0].y > UPPER_Y_BOUNDARY) 
	{
		res = 3;
	}


	if (position[0].y < DOWN_Y_BOUNDARY)
	{
		res = 4;
	}

	if(position[0].z > FAR_Z_BOUNDARY)
	{
		res = 5;
	}

	if (position[0].z < NEAR_Z_BOUNDARY){
		res = 6;
	}

	return res;
};





void Read_Mesh_in_local_space(const std::string &file_name, GLuint &vertex_count, GLfloat **vbo, string &pos_x_name, string &pos_y_name, string &pos_z_name)
{
	assert(!pos_x_name.empty() && "error: need a valid x name!");
	assert(!pos_y_name.empty() && "error: need a valid y name!");
	assert(!pos_z_name.empty() && "error: need a valid z name!");

	struct vec4 {
		float x = 0.0;
		float y = 0.0;
		float z = 0.0;
		float w = 0.0;
	};

	std::vector<vec4> pos_set;

	ifstream ifs;
	ifs.open(file_name.data(), std::ifstream::in);
	assert(ifs.is_open() == true && "error: fail to open file.");

	uint32_t line_idx = 0;
	std::string t_line;

	uint32_t vtx_idx = -1;
	uint32_t idx_idx = -1;
	uint32_t pos_x_idx = -1;
	uint32_t pos_y_idx = -1;
	uint32_t pos_z_idx = -1;


	float max_value[3] = { -100000000000.0,-100000000000.0 ,-100000000000.0 };
	float min_value[3] = { 100000000000.0, 100000000000.0,100000000000.0 };


	while (getline(ifs, t_line))
	{
		vec4 t_pos;

		std::istringstream ss(t_line);
		std::string token;
		uint32_t token_idx = 0;


		while (std::getline(ss, token, ',')) {
			//std::cout << token << '\n';
			if (line_idx == 0) {
				if (token.find("VTX") != std::string::npos) {
					vtx_idx = token_idx;
					assert(vtx_idx == 0);
				}

				if (token.find("IDX") != std::string::npos) {
					idx_idx = token_idx;
					assert(idx_idx == 1);
				}

				if (token.find(pos_x_name) != std::string::npos) {
					pos_x_idx = token_idx;
					assert(pos_x_idx != -1);
				}

				if (token.find(pos_y_name) != std::string::npos) {
					pos_y_idx = token_idx;
					assert(pos_y_idx != -1);
				}

				if (token.find(pos_z_name) != std::string::npos) {
					pos_z_idx = token_idx;
					assert(pos_z_idx != -1);
				}
			}
			else {
				if (token_idx == vtx_idx) {
					//cout << "vtx_token : " << token << endl;
					uint32_t current_vtx = std::stoi(token);
				}

				if (token_idx == idx_idx) {
					//cout << "idx_token : " << token << endl;
					uint32_t current_idx = std::stoi(token);
				}

				if (token_idx == pos_x_idx) {
					//cout << "x_token : " << token << endl;
					float val = std::stof(token);

					t_pos.x = val;
				}

				if (token_idx == pos_y_idx) {
					//cout << "y_token : " << token << endl;
					float val = std::stof(token);

					t_pos.y = val;
				}

				if (token_idx == pos_z_idx) {
					//cout << "z_token : " << token << endl;
					float val = std::stof(token);

					t_pos.z = val;
				}

				if (token_idx >= pos_z_idx)
				{
					//t_pos.w = sqrtf(pow(t_pos.x, 2) + pow(t_pos.y, 2) + pow(t_pos.z, 2));
					float ratio = 0.1;
					//t_pos.x = t_pos.x * ratio;
					//t_pos.y = t_pos.y * ratio;
					//t_pos.z = t_pos.z * ratio;

					max_value[0] = max_value[0] > t_pos.x ? max_value[0] : t_pos.x;
					max_value[1] = max_value[1] > t_pos.y ? max_value[1] : t_pos.y;
					max_value[2] = max_value[2] > t_pos.z ? max_value[2] : t_pos.z;

					min_value[0] = min_value[0] < t_pos.x ? min_value[0] : t_pos.x;
					min_value[1] = min_value[1] < t_pos.y ? min_value[1] : t_pos.y;
					min_value[2] = min_value[2] < t_pos.z ? min_value[2] : t_pos.z;

					t_pos.w = 1.0;

					//t_pos.w = 1.0;
					pos_set.push_back(t_pos);//finish parse a vertex push it to the set.
				}
			}
			token_idx++;
		}
		line_idx++;
	}
	ifs.close();

	assert((pos_set.size() != 0) && "error: number of vertex can't be zero.");
	assert((pos_set.size() % 3 == 0) && "error: number of vertex mode 3 must be zero.");


	vertex_count = pos_set.size();

	GLfloat *t_VBO = new float[sizeof(GLfloat) * 4 * vertex_count];
	memset(t_VBO, 0, (sizeof(GLfloat) * 4 * vertex_count));
	*vbo = t_VBO;


	float distance_value[3] = { 0.0f, 0.0f, 0.0f };
	distance_value[0] = max_value[0] - min_value[0];
	distance_value[1] = max_value[1] - min_value[1];
	distance_value[2] = max_value[2] - min_value[2];

	cout << "Mesh info:" << endl;
	cout << "\t \t" << "max_x = " << max_value[0] << "\t min_x = " << min_value[0] << "\t distance = " << distance_value[0] << endl;
	cout << "\t \t" << "max_y = " << max_value[1] << "\t min_y = " << min_value[1] << "\t distance = " << distance_value[1] << endl;
	cout << "\t \t" << "max_z = " << max_value[2] << "\t min_z = " << min_value[2] << "\t distance = " << distance_value[2] << endl;


	float shift_value[3] = { 0.0f,0.0f, 0.0f };

	for (size_t i = 0; i < 3; i++)
	{
		//x, y, z 3 dimension

		float length_on_demension = (max_value[i] - min_value[i]);

		float mid_pos = min_value[i] + (length_on_demension / 2);


		if (mid_pos < 0.0f) {
			shift_value[i] = abs(mid_pos);
		}
		else {
			shift_value[i] = -mid_pos;
		}
	}


	std::vector<vec4>::const_iterator ite = pos_set.begin();
	for (; ite != pos_set.end(); ite++)
	{
		uint32_t vid = ite - pos_set.begin();
		vec4 t_vertex = (*ite);

		//t_VBO[(vid * 4) + 0] = (t_vertex.x + shift_value[0]) / (distance_value[0]);
		//t_VBO[(vid * 4) + 1] = (t_vertex.y + shift_value[1]) / (distance_value[1]);
		//t_VBO[(vid * 4) + 2] = (t_vertex.z + shift_value[2]) / (distance_value[2]);
		//t_VBO[(vid * 4) + 3] = t_vertex.w;


		t_VBO[(vid * 4) + 0] = t_vertex.x;
		t_VBO[(vid * 4) + 1] = t_vertex.y;
		t_VBO[(vid * 4) + 2] = t_vertex.z;
		t_VBO[(vid * 4) + 3] = t_vertex.w;
	}
}



void Read_VS_output(const std::string &file_name, GLuint &vertex_count, GLfloat **vbo) {


	struct vec4 {
		float x = 0.0;
		float y = 0.0;
		float z = 0.0;
		float w = 0.0;
	};

	std::vector<vec4> pos_set;

	ifstream ifs;
	ifs.open(file_name.data(), std::ifstream::in);
	assert(ifs.is_open() == true && "error: fail to open file.");

	uint32_t line_idx = 0;
	std::string t_line;

	uint32_t vtx_idx = -1;
	uint32_t idx_idx = -1;
	uint32_t pos_x_idx = -1;
	uint32_t pos_y_idx = -1;
	uint32_t pos_z_idx = -1;
	uint32_t pos_w_idx = -1;
	while (getline(ifs, t_line))
	{
		vec4 t_pos;

		std::istringstream ss(t_line);
		std::string token;
		uint32_t token_idx = 0;

		while (std::getline(ss, token, ',')) {
			//std::cout << token << '\n';
			if (line_idx == 0) {
				if (token.find("VTX") != std::string::npos) {
					vtx_idx = token_idx;
					assert(vtx_idx == 0);
				}

				if (token.find("IDX") != std::string::npos) {
					idx_idx = token_idx;
					assert(idx_idx == 1);
				}

				if (token.find("gl_Position.x") != std::string::npos) {
					pos_x_idx = token_idx;
					assert(pos_x_idx == 2);
				}

				if (token.find("gl_Position.y") != std::string::npos) {
					pos_y_idx = token_idx;
					assert(pos_y_idx == 3);
				}

				if (token.find("gl_Position.z") != std::string::npos) {
					pos_z_idx = token_idx;
					assert(pos_z_idx == 4);
				}

				if (token.find("gl_Position.w") != std::string::npos) {
					pos_w_idx = token_idx;
					assert(pos_w_idx == 5);
				}
			}
			else {
				if (token_idx == vtx_idx) {
					//cout << "vtx_token : " << token << endl;
					uint32_t current_vtx = std::stoi(token);
				}

				if (token_idx == idx_idx) {
					//cout << "idx_token : " << token << endl;
					uint32_t current_idx = std::stoi(token);
				}

				if (token_idx == pos_x_idx) {
					//cout << "x_token : " << token << endl;
					float val = std::stof(token);

					t_pos.x = val;
				}

				if (token_idx == pos_y_idx) {
					//cout << "y_token : " << token << endl;
					float val = std::stof(token);

					t_pos.y = val;
				}

				if (token_idx == pos_z_idx) {
					//cout << "z_token : " << token << endl;
					float val = std::stof(token);

					t_pos.z = val;
				}

				if (token_idx == pos_w_idx) {
					//cout << "w_token : " << token << endl;
					float val = std::stof(token);

					t_pos.w = val;
				}

				if (token_idx >= pos_w_idx)
				{
					pos_set.push_back(t_pos);//finish parse a vertex push it to the set.
				}
			}
			token_idx++;
		}
		line_idx++;
	}
	ifs.close();

	assert((pos_set.size() != 0) && "error: number of vertex can't be zero.");
	assert((pos_set.size() % 3 == 0) && "error: number of vertex mode 3 must be zero.");


	vertex_count = pos_set.size();

	GLfloat *t_VBO = new float[sizeof(GLfloat) * 4 * vertex_count];
	memset(t_VBO, 0, (sizeof(GLfloat) * 4 * vertex_count));
	*vbo = t_VBO;

	std::vector<vec4>::const_iterator ite = pos_set.begin();
	for (; ite != pos_set.end(); ite++)
	{
		uint32_t vid = ite - pos_set.begin();
		vec4 t_vertex = (*ite);

		t_VBO[(vid * 4) + 0] = t_vertex.x;
		t_VBO[(vid * 4) + 1] = t_vertex.y;
		t_VBO[(vid * 4) + 2] = t_vertex.z;
		t_VBO[(vid * 4) + 3] = t_vertex.w;
	}
}



GLuint CreateVAO(GLuint  vertex_count = 0, GLfloat *vertex_buffer = nullptr, GLuint Program = 0) {

	GLuint attribute_vec_size = 4;


	GLuint  VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));


	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*attribute_vec_size* vertex_count, vertex_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(Program, "position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));

	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	/*
	VERIFY_GL(glGenBuffers(1, &EBO));
	VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	VERIFY_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	*/

	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	return VAO;
}


GLuint Create_IN_MESH_VAO(GLuint  vertex_count = 0, GLfloat *vertex_buffer = nullptr, GLuint Program = 0) {

	GLuint attribute_vec_size = 4;


	GLuint  VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));


	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*attribute_vec_size* vertex_count, vertex_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));

	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	/*
	VERIFY_GL(glGenBuffers(1, &EBO));
	VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	VERIFY_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	*/

	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	return VAO;
}


void Mesh_to_Line(GLuint vertex_count, GLfloat * vertex_buffer, GLuint &point_count, GLfloat ** point_vb_buffer_ptr) {

	GLuint triangle_count = (vertex_count / 3);

	point_count = triangle_count * 3 * 2;
	GLuint point_vb_buffer_size = point_count * 4;
	GLfloat * point_vb_buffer = new GLfloat[point_vb_buffer_size];
	(*point_vb_buffer_ptr) = point_vb_buffer;

	for (uint32_t i = 0; i < triangle_count; i++) {
		//cout << "triangle_" << i << endl;

		uint32_t vb_base = i * 12;
		GLfloat x0 = vertex_buffer[vb_base + 0];
		GLfloat y0 = vertex_buffer[vb_base + 1];
		GLfloat z0 = vertex_buffer[vb_base + 2];
		GLfloat w0 = vertex_buffer[vb_base + 3];

		GLfloat x1 = vertex_buffer[vb_base + 4 + 0];
		GLfloat y1 = vertex_buffer[vb_base + 4 + 1];
		GLfloat z1 = vertex_buffer[vb_base + 4 + 2];
		GLfloat w1 = vertex_buffer[vb_base + 4 + 3];

		GLfloat x2 = vertex_buffer[vb_base + 8 + 0];
		GLfloat y2 = vertex_buffer[vb_base + 8 + 1];
		GLfloat z2 = vertex_buffer[vb_base + 8 + 2];
		GLfloat w2 = vertex_buffer[vb_base + 8 + 3];


		//below give line info
		uint32_t line_vb_base = i * 24;
		uint32_t offset = -1;

		//a->b
		offset = line_vb_base + 0;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = x0;
		//cout << "write  0" << endl;

		offset = line_vb_base + 1;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = y0;
		//cout << "write  1" << endl;

		offset = line_vb_base + 2;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = z0;
		//cout << "write  2" << endl;

		offset = line_vb_base + 3;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = w0;
		//cout << "write  3" << endl;


		offset = line_vb_base + 4;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = x1;
		//cout << "write  4" << endl;

		offset = line_vb_base + 5;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = y1;
		//cout << "write  5" << endl;

		offset = line_vb_base + 6;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = z1;
		//cout << "write  6" << endl;

		offset = line_vb_base + 7;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = w1;
		//cout << "write  7" << endl;

		//b->c
		offset = line_vb_base + 8;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = x1;
		//cout << "write  8" << endl;

		offset = line_vb_base + 9;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = y1;
		//cout << "write  9" << endl;

		offset = line_vb_base + 10;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = z1;
		//cout << "write  10" << endl;

		offset = line_vb_base + 11;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = w1;
		//cout << "write  11" << endl;

		offset = line_vb_base + 12;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = x2;
		//cout << "write  12" << endl;

		offset = line_vb_base + 13;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = y2;
		//cout << "write  13" << endl;

		offset = line_vb_base + 14;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = z2;
		//cout << "write  14" << endl;

		offset = line_vb_base + 15;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = w2;
		//cout << "write  15" << endl;

		//c->a
		offset = line_vb_base + 16;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = x2;
		//cout << "write  16" << endl;

		offset = line_vb_base + 17;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = y2;
		//cout << "write  17" << endl;

		offset = line_vb_base + 18;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = z2;
		//cout << "write  18" << endl;

		offset = line_vb_base + 19;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = w2;
		//cout << "write  19" << endl;


		offset = line_vb_base + 20;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = x0;
		//cout << "write  20" << endl;

		offset = line_vb_base + 21;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = y0;
		//cout << "write  21" << endl;

		offset = line_vb_base + 22;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = z0;
		//cout << "write  22" << endl;

		offset = line_vb_base + 23;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		point_vb_buffer[offset] = w0;
		//cout << "write  23" << endl;
	}
	cout << "convert mesh to line success!" << endl;
}









std::string Gen_simple_FS(glm::uvec3 color_rgb) {

	std::string fs_header =
		"#version 330 core                                     \n"
		"precision highp float;                                \n"
		"out vec4 color;                                       \n"
		"void main()                                           \n"
		"{                                                     \n"
		"    color = vec4(";

	std::string FS_tail = " 1.0f);             \n"
		"}                                                        \n";

	std::string r = to_string((float)color_rgb.x / ((float)255));
	std::string g = to_string((float)color_rgb.y / ((float)255));
	std::string b = to_string((float)color_rgb.z / ((float)255));

	std::string res = fs_header + r + "f, " + g + "f, " + b + "f, " + FS_tail;

	//cout <<endl<< res << endl;
	return res;
}



//golobal mesh instance count, how many instance we have created.

uint32_t Mesh::global_mesh_instance_count = 0;



void Updata_Boundary_vertex_1(
	std::array<glm::vec3, 6> &t_boundary_vertex,
	std::array<uint32_t, 6>  &t_boundary_vertex_VID,
	const glm::vec3 &t_vertex,
	const GLuint t_vid)
{

	//Updata max_x 
	if (t_vertex.x > t_boundary_vertex[0].x) {
		t_boundary_vertex[0] = t_vertex;
		t_boundary_vertex_VID[0] = t_vid;
	}

	//Updata min_x 
	if (t_vertex.x < t_boundary_vertex[1].x) {
		t_boundary_vertex[1] = t_vertex;
		t_boundary_vertex_VID[1] = t_vid;
	}

	//Updata max_y 
	if (t_vertex.y > t_boundary_vertex[2].y) {
		t_boundary_vertex[2] = t_vertex;
		t_boundary_vertex_VID[2] = t_vid;
	}

	//Updata min_y
	if (t_vertex.y < t_boundary_vertex[3].y) {
		t_boundary_vertex[3] = t_vertex;
		t_boundary_vertex_VID[3] = t_vid;
	}


	//Updata max_z 
	if (t_vertex.z > t_boundary_vertex[4].z) {
		t_boundary_vertex[4] = t_vertex;
		t_boundary_vertex_VID[4] = t_vid;
	}

	//Updata min_z 
	if (t_vertex.z < t_boundary_vertex[5].z) {
		t_boundary_vertex[5] = t_vertex;
		t_boundary_vertex_VID[5] = t_vid;
	}
}





GLuint Mesh::Read_Mesh_Vertex(const std::string &file_name)
{
	cout<<"Reading Mesh Vertices..."<<endl;
	assert(!file_name.empty() && "error: invalid input mesh file name!");

	//uint32_t vtx_idx = -1;
	//uint32_t idx_idx = -1;


	//std::string attributes_name[MAX_NUM_OF_ATTRIBUTTES];//each attributes's name
	//uint32_t attributes_start_idx[MAX_NUM_OF_ATTRIBUTTES] = { 0 };//each attributes start index.
	//memset(attributes_start_idx, 0xFFFFFFFF, sizeof(uint32_t) * MAX_NUM_OF_ATTRIBUTTES); //-1 for invalid index.
	//Attributes t_attributes[MAX_NUM_OF_ATTRIBUTTES];


	uint32_t num_of_token = 0; //each line conatained token number.
	uint32_t line_idx = 0;
	std::string t_line;

	ifstream ifs;
	ifs.open(file_name.data(), std::ifstream::in);
	assert(ifs.is_open() == true && "error: fail to open file.");

	//first line to get split the those attributes.
	if (getline(ifs, t_line)){
		line_idx++;
		std::istringstream ss(t_line);

		//uint32_t token_idx = 0;

		std::vector<std::string> token_set;
		std::string t_token;
		while (std::getline(ss, t_token, ','))
		{
			token_set.push_back(t_token);
		}



		num_of_token = token_set.size();



		for (size_t i = 0; i < num_of_token; i++) {
			std::string token = token_set[i];
			if (token.find("VTX") != std::string::npos) {
				uint32_t vtx_idx = i;
				assert(vtx_idx == 0);

				i++;
				continue;
			}

			if (token.find("IDX") != std::string::npos) {
				uint32_t  idx_idx = i;
				assert(idx_idx == 1);

				i++;
				continue;
			}

			if (token.find(".x") != std::string::npos) {
				//new token
				//a new attributes was find.

				std::string  t_attri_name_x = token.substr(0, token.find(".x"));

				assert((i + 1) < num_of_token && "error: must be other token. after .x.");

				mesh_attributes[num_of_attributes].start_idx = i;
				mesh_attributes[num_of_attributes].name = t_attri_name_x;

				if ((i + 1) < num_of_token) //not end_of_line.
				{
					std::string n_one_token = token_set[i + 1];
					if (n_one_token.find(".y") != std::string::npos) {
						i++;  //index++; we find a token, i++
						std::string  t_attri_name_y = n_one_token.substr(0, n_one_token.find(".y"));

						assert(t_attri_name_y == t_attri_name_x && "error: name must be same.");


						if ((i + 1) < num_of_token) {
							std::string n_two_token = token_set[i + 1];
							if (n_two_token.find(".z") != std::string::npos) {
								i++;  //index++; we find a token, i++
								std::string  t_attri_name_z = n_two_token.substr(0, n_two_token.find(".z"));
								assert(t_attri_name_z == t_attri_name_y && "error: name must be same.");



								if ((i + 1) < num_of_token) {

									std::string n_three_token = token_set[i + 1];
									if (n_three_token.find(".w") != std::string::npos) {
										i++;  //index++; we find a token, i++
										std::string  t_attri_name_w = n_three_token.substr(0, n_three_token.find(".w"));
										assert(t_attri_name_w == t_attri_name_z && "error: name must be same.");

										mesh_attributes[num_of_attributes].dimension = 4; //vec4
										num_of_attributes++;
									}
									else
									{
										mesh_attributes[num_of_attributes].dimension = 3; //vec3
										num_of_attributes++;
									}
								}
								else {
									mesh_attributes[num_of_attributes].dimension = 3; //vec3
									num_of_attributes++;
								}
							}
							else
							{
								mesh_attributes[num_of_attributes].dimension = 2; //vec2
								//continue;
							}
						}
						else
						{
							mesh_attributes[num_of_attributes].dimension = 2; //vec2
							num_of_attributes++;
						}
					}
					else
					{
						assert(0 && "error: never go here!");

						mesh_attributes[num_of_attributes].dimension = 1; //vec1
						num_of_attributes++;
					}
				}
				else
				{
					mesh_attributes[num_of_attributes].start_idx = i;
					mesh_attributes[num_of_attributes].dimension = 1; //vec1
					mesh_attributes[num_of_attributes].name = t_attri_name_x;
					num_of_attributes++;
				}
			}
			else {
				
				//new token
				//a new attributes was find.

				//a attribute not have any .x, .y, .z, .w, but may have other special sub_elements. like "TexCoord.Data".
				// assert((token.find(".") == std::string::npos) && "error: expect a single varible without sub_element!");

				mesh_attributes[num_of_attributes].start_idx = i;
				mesh_attributes[num_of_attributes].dimension = 1; //vec1
				mesh_attributes[num_of_attributes].name = token;
				num_of_attributes++;
			}
		}
	}
	else{
		assert(0 && "fail to get file's first line.");
	}





	while (getline(ifs, t_line)){
		line_idx++;

		std::istringstream ss(t_line);
		std::string token;

		std::vector<string> token_set;
		while (std::getline(ss, token, ',')) {
			token_set.push_back(token);
		}
		assert(token_set.size() == num_of_token);

		for (uint32_t i = 0; i < num_of_attributes; i++) {

			assert(mesh_attributes[i].start_idx != -1 && "error: fail to get valid start_idx.");
			assert(mesh_attributes[i].dimension != -1 && "error: fail to get valid dimension.");
			assert((!mesh_attributes[i].name.empty()) && "error: fail to get valid attributes name.");


			uint32_t start_idx = mesh_attributes[i].start_idx;

			//uint32_t start_idx = t_attributes[i].start_idx;


			glm::vec4 t_attr=glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

			uint32_t start_pos   = mesh_attributes[i].start_idx;
			uint32_t t_dimension = mesh_attributes[i].dimension;
			assert(t_dimension != 0 && "error: invalid attribute demension!");
			assert(t_dimension <= 4 && "error: invalid attribute demension!");

			//accumulate all component
			num_of_attributes_components = num_of_attributes_components + t_dimension;

			if (t_dimension == 1)
			{
				float val_x = std::stof(token_set[start_pos]);
				t_attr.x = val_x;
			}

			if (t_dimension == 2)
			{
				float val_x = std::stof(token_set[start_pos]);
				t_attr.x = val_x;
				float val_y = std::stof(token_set[start_pos + 1]);
				t_attr.y = val_y;
			}

			if (t_dimension == 3)
			{
				float val_x = std::stof(token_set[start_pos]);
				t_attr.x = val_x;
				float val_y = std::stof(token_set[start_pos + 1]);
				t_attr.y = val_y;
				float val_z = std::stof(token_set[start_pos + 2]);
				t_attr.z = val_z;
			}

			if (t_dimension == 4)
			{
				float val_x = std::stof(token_set[start_pos]);
				t_attr.x = val_x;
				float val_y = std::stof(token_set[start_pos + 1]);
				t_attr.y = val_y;
				float val_z = std::stof(token_set[start_pos + 2]);
				t_attr.z = val_z;
				float val_w = std::stof(token_set[start_pos + 3]);
				t_attr.w = val_w;
			}
			mesh_attributes[i].attr.push_back(t_attr);//push
		}
	}




	/*
	while (getline(ifs, t_line))
	{
		line_idx++;

		std::istringstream ss(t_line);
		std::string token;

		if (line_idx == 1) {
			uint32_t token_idx = 0;
			while (std::getline(ss, token, ',')) {

				if (token.find("VTX") != std::string::npos) {
					uint32_t vtx_idx = token_idx;
					assert(vtx_idx == 0);
				}

				if (token.find("IDX") != std::string::npos) {
					uint32_t idx_idx = token_idx;
					assert(idx_idx == 1);
				}

				if (token.find(".x") != std::string::npos) {
					//a new attributes was find.

					std::string  t_attri_name = token.substr(0, token.find(".x"));
					attributes_name[num_of_attributes] = t_attri_name;
					attributes_start_idx[num_of_attributes] = token_idx;

					t_attributes[num_of_attributes].name = t_attri_name;
				   //attributes[num_of_attributes].dimension = -1;

					num_of_attributes++;
					assert((num_of_attributes) <= (MAX_NUM_OF_ATTRIBUTTES + 1) && "error: number of attributes greater than OpenGL Specific.");
				}
				num_of_token++;
				token_idx++;
			}
		}
		else {
			std::vector<string> token_set;
			while (std::getline(ss, token, ',')){
				token_set.push_back(token);
			}
			assert(token_set.size()==num_of_token);

			uint32_t attri_idx = 0;
			uint32_t token_idx = 0;
			glm::vec4 t_attr;

			for (size_t i = 0; i < num_of_attributes; i++)
			{
				uint32_t start_pos = attributes_start_idx[i];
				uint32_t end_pos = -1;
				if (i!= (num_of_attributes-1)){
					end_pos = attributes_start_idx[i + 1];
				}
				else{
					end_pos = token_set.size();
				}
				glm::vec4 t_attr;
				uint32_t t_dimension = end_pos - start_pos;

				assert(t_dimension != 0 && "error: invalid attribute demension!");
				assert(t_dimension <= 4 && "error: invalid attribute demension!");

				t_attributes[i].dimension = t_dimension;


				if (t_dimension ==1)
				{
					float val_x = std::stof(token_set[start_pos]);
					t_attr.x = val_x;
				}
				if (t_dimension == 2)
				{
					float val_x = std::stof(token_set[start_pos]);
					t_attr.x = val_x;
					float val_y = std::stof(token_set[start_pos + 1]);
					t_attr.y = val_y;
				}
				if (t_dimension == 3)
				{
					float val_x = std::stof(token_set[start_pos]);
					t_attr.x = val_x;
					float val_y = std::stof(token_set[start_pos + 1]);
					t_attr.y = val_y;
					float val_z = std::stof(token_set[start_pos + 2]);
					t_attr.z = val_z;
				}
				if (t_dimension == 4)
				{
					float val_x = std::stof(token_set[start_pos]);
					t_attr.x = val_x;
					float val_y = std::stof(token_set[start_pos + 1]);
					t_attr.y = val_y;
					float val_z = std::stof(token_set[start_pos + 2]);
					t_attr.z = val_z;
					float val_w = std::stof(token_set[start_pos + 3]);
					t_attr.w = val_w;
				}
				t_attributes[i].attr.push_back(t_attr);//push
			}
		}

	}
	*/



	ifs.close();


	//for (size_t i = 0; i < num_of_attributes; i++)
	//{
	//	assert(mesh_attributes[i].attr.size() != 0 && "error: invalid attributes numbers!");
	//	assert((mesh_attributes[i].attr.size() %3) == 0 && "error: invalid attributes numbers!");
	//
	//	mesh_attributes.push_back(t_attributes[i]);
	//}

	//OK here, we get the vertices count on the mesh.	
	mesh_vertex_count = mesh_attributes[0].attr.size();



	//remove all sapce in string.
	auto ClearAllSpace = [&](std::string str) {
		int index = 0;
		if (!str.empty())
		{
			while ((index = str.find(' ', index)) != string::npos)
			{
				str.erase(index, 1);
			}
		}
		return str;
	};

	auto is_in_position = [&](std::string name) {

		bool res = false;
		std::string clean_name = ClearAllSpace(name);

		uint32_t candiate_number = mesh_candidate_in_position_name.size();
		std::vector<std::string>::const_iterator ite = mesh_candidate_in_position_name.begin();
		for (; ite != mesh_candidate_in_position_name.end(); ite++) {
			if (clean_name == *ite) {
				res = true;
				return res;
			}
		}
		return res;
	};


	bool is_a_input_mesh = false;
	bool is_a_output_mesh = false;
	//std::vector<Attributes>::const_iterator attr_ite = mesh_attributes.begin();


	bool is_find_mesh_pos = false;
	for (size_t i = 0; i < num_of_attributes; i++) {

		std::string attr_name = mesh_attributes[i].name;
		if (is_in_position(attr_name) == true) {

			is_find_mesh_pos = true;

			Attributes * attr_ite = &(mesh_attributes[i]);
			assert((attr_ite->dimension) >= 3 && "error: for any mesh, input position must be greater than 3.");

			attr_ite->is_position = true;

			

			//used to save 
			//
			//(max_x, min_x)
			//(max_y, min_y)
			//(max_z, min_z)
			//
			std::array<Vertex, 6> BoundaryVertex;
			std::array<uint32_t, 6>  BoundaryVertex_VID;
			{
				BoundaryVertex[0].Position.x = -(std::numeric_limits<double>::infinity());
				BoundaryVertex[1].Position.x = +(std::numeric_limits<double>::infinity());
								 
				BoundaryVertex[2].Position.y = -(std::numeric_limits<double>::infinity());
				BoundaryVertex[3].Position.y = +(std::numeric_limits<double>::infinity());
								 
				BoundaryVertex[4].Position.z = -(std::numeric_limits<double>::infinity());
				BoundaryVertex[5].Position.z = +(std::numeric_limits<double>::infinity());
			}

			


			//to save mesh vertices data.
			uint32_t t_vertex_count = attr_ite->attr.size();

			mesh_position_buffer = new GLfloat[t_vertex_count * 4];
			memset(mesh_position_buffer, 0, (sizeof(GLfloat) * t_vertex_count * 4));

			std::vector<glm::vec4>::const_iterator ele_ite = attr_ite->attr.begin();
			for (; ele_ite != attr_ite->attr.end(); ele_ite++) {
				glm::vec4 t_element = *ele_ite;
				uint32_t t_vid = ele_ite - attr_ite->attr.begin();


#if 1
				//float root = sqrt(pow(t_element.x, 2) + pow(t_element.y, 2) + pow(t_element.z, 2));
				mesh_position_buffer[(t_vid * 4) + 0] = t_element.x;      // sign used to filp Right-and-Left
				mesh_position_buffer[(t_vid * 4) + 1] = t_element.y;
				mesh_position_buffer[(t_vid * 4) + 2] = t_element.z;
				mesh_position_buffer[(t_vid * 4) + 3] = t_element.w;
				//mesh_position_buffer[(t_vid * 4) + 3] = 1.0;
#else
				mesh_position_buffer[(t_vid * 4) + 0] = (t_element.x / t_element.w);
				mesh_position_buffer[(t_vid * 4) + 1] = (t_element.y / t_element.w);
				mesh_position_buffer[(t_vid * 4) + 2] = (t_element.z / t_element.w);
				mesh_position_buffer[(t_vid * 4) + 3] = 1.0;
#endif

				//update the boundrary vertex info
				glm::vec3 normalized_vertex = glm::vec3((t_element.x / t_element.w),(t_element.y / t_element.w), (t_element.z / t_element.w));

				//for nan and infinit
				//OpenGL SPEC will discard the triangle, thus we replace those value to max inifinit for easy operation.

				//nan number
				if (isnan(normalized_vertex.x)) {
					normalized_vertex.x = std::numeric_limits<float>::infinity();
					cout << "\t vertices[" << t_vid << "], normalized_vertex.x is NAN, replace to INFINITY." << endl;
				}
				if (isnan(normalized_vertex.y)) {
					normalized_vertex.y = std::numeric_limits<float>::infinity();
					cout << "\t vertices[" << t_vid << "], normalized_vertex.y is NAN, replace to INFINITY." << endl;
				}
				if (isnan(normalized_vertex.z)) {
					normalized_vertex.z = std::numeric_limits<float>::infinity();
					cout << "\t vertices[" << t_vid << "], normalized_vertex.z is NAN, replace to INFINITY." << endl;
				}

				//inifit number
				if (isinf(normalized_vertex.x)) {
					normalized_vertex.x = std::numeric_limits<float>::infinity();
					cout << "\t vertices[" << t_vid << "], normalized_vertex.x is NAN, replace to INFINITY." << endl;
				}
				if (isinf(normalized_vertex.y)) {
					normalized_vertex.y = std::numeric_limits<float>::infinity();
					cout << "\t vertices[" << t_vid << "], normalized_vertex.y is NAN, replace to INFINITY." << endl;
				}
				if (isinf(normalized_vertex.z)) {
					normalized_vertex.z = std::numeric_limits<float>::infinity();
					cout << "\t vertices[" << t_vid << "], normalized_vertex.z is NAN, replace to INFINITY." << endl;
				}

		
			

				// Updata_Boundary_vertex_1(
				// 	BoundaryVertex,
				// 	BoundaryVertex_VID,
				// 	glm::vec3(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z),
				// 	t_vid
				// );




				if (is_out_side_normal_space == true) {
					if (
						(abs(normalized_vertex.x) <= 1.0) &&
						(abs(normalized_vertex.y) <= 1.0) &&
						(abs(normalized_vertex.z) <= 1.0)
						) {
						is_out_side_normal_space = false;
					}
				}
			}

			// init the 6 mesh boundary vertex
			assert(0 && "FIXME: TO DO");
			//BoundaryVertex = BoundaryVertex;
			//BoundaryVertex = BoundaryVertex;
			//BoundaryVertex_VID = BoundaryVertex_VID;
			

			assert((!BoundaryVertex.empty()) &&
				"error: mesh boundary vertex is empty, please init BoundaryVertex during load mesh vertex data.");

			if (is_out_side_normal_space == true) {
				//whether the meshes is out ot normalize space.
				bool is_must_out_of_normalize_space = false;

				where_the_mesh[0] = false;
				where_the_mesh[1] = false;
				where_the_mesh[2] = false;
				where_the_mesh[3] = false;
				where_the_mesh[4] = false;
				where_the_mesh[5] = false;
				

				if (BoundaryVertex[0].Position.x > RIGHT_X_BOUNDARY) {
					//mesh'x greater than x=1.0
					where_the_mesh[0] = true;
					is_must_out_of_normalize_space = true;
				}
				else if (BoundaryVertex[1].Position.x < LEFT_X_BOUNDARY) {
					//mesh'x less than x=-1.0
					where_the_mesh[1] = true;
					is_must_out_of_normalize_space = true;
				}

				/////
				else if (BoundaryVertex[2].Position.y > UPPER_Y_BOUNDARY) {
					//mesh'y greater than y=1.0
					where_the_mesh[2] = true;
					is_must_out_of_normalize_space = true;
				}
				else if (BoundaryVertex[3].Position.y < DOWN_Y_BOUNDARY) {
					//mesh'y less than y=-1.0
					where_the_mesh[3] = true;
					is_must_out_of_normalize_space = true;
				}


				////
				else if (BoundaryVertex[4].Position.z > FAR_Z_BOUNDARY) {
					//mesh'z greater than z=1.0
					where_the_mesh[4] = true;
					is_must_out_of_normalize_space = true;
				}
				else if (BoundaryVertex[5].Position.z < NEAR_Z_BOUNDARY) {
					//mesh'z less than z=-1.0
					where_the_mesh[5] = true;
					is_must_out_of_normalize_space = true;
				}
				assert(is_must_out_of_normalize_space == true && "error: fail to location where is the mesh." );
			}

			/*
			static uint32_t mesh_id_1 = 0;
			if (mesh_id_1 == 2)
			{
				for (size_t i = 0; i < 6; i++)
				{
					//init it.
					BoundaryVertex_local.push_back(attr_ite->attr[0]);
				}
				BoundaryVertex_local[0] = attr_ite->attr[local_max_x_vid];
				BoundaryVertex_local[1] = attr_ite->attr[local_min_x_vid];

				BoundaryVertex_local[2] = attr_ite->attr[local_max_y_vid];
				BoundaryVertex_local[3] = attr_ite->attr[local_min_y_vid];

				BoundaryVertex_local[4] = attr_ite->attr[local_max_z_vid];
				BoundaryVertex_local[5] = attr_ite->attr[local_min_z_vid];
			}
			mesh_id_1++;
			*/
			//
			//

			cout << "Mesh boundary vertex info:" << endl;
			cout << "\t max_x_vid: " << BoundaryVertex_VID[0] << endl;
			cout << "\t min_x_vid: " << BoundaryVertex_VID[1] << endl;

			cout << "\t max_y_vid: " << BoundaryVertex_VID[2] << endl;
			cout << "\t min_y_vid: " << BoundaryVertex_VID[3] << endl;

			cout << "\t max_z_vid: " << BoundaryVertex_VID[4] << endl;
			cout << "\t min_z_vid: " << BoundaryVertex_VID[5] << endl;



			cout << "\t max_x_vertex: (" << BoundaryVertex[0].Position.x << ", " << BoundaryVertex[0].Position.y << ", " << BoundaryVertex[0].Position.z << ")" << endl;
			cout << "\t min_x_vertex: (" << BoundaryVertex[1].Position.x << ", " << BoundaryVertex[1].Position.y << ", " << BoundaryVertex[1].Position.z << ")" << endl;
																												 
			cout << "\t max_y_vertex: (" << BoundaryVertex[2].Position.x << ", " << BoundaryVertex[2].Position.y << ", " << BoundaryVertex[2].Position.z << ")" << endl;
			cout << "\t min_y_vertex: (" << BoundaryVertex[3].Position.x << ", " << BoundaryVertex[3].Position.y << ", " << BoundaryVertex[3].Position.z << ")" << endl;
																												 
			cout << "\t max_z_vertex: (" << BoundaryVertex[4].Position.x << ", " << BoundaryVertex[4].Position.y << ", " << BoundaryVertex[4].Position.z << ")" << endl;
			cout << "\t min_z_vertex: (" << BoundaryVertex[5].Position.x << ", " << BoundaryVertex[5].Position.y << ", " << BoundaryVertex[5].Position.z << ")" << endl;

		}
	}
	assert(is_find_mesh_pos != false && "error: fail find mesh position.");
	assert(mesh_vertex_count != 0 && "error: fail to reconginize the mesh.");
	



	cout << " Read Mesh Vertex success." << endl;
	return mesh_vertex_count;


	//assert((pos_set.size() != 0) && "error: number of vertex can't be zero.");
	//assert((pos_set.size() % 3 == 0) && "error: number of vertex mode 3 must be zero.");
	//
	//
	//vertex_count = pos_set.size();
	//
	//GLfloat *t_VBO = new float[sizeof(GLfloat) * 4 * vertex_count];
	//memset(t_VBO, 0, (sizeof(GLfloat) * 4 * vertex_count));
	//*vbo = t_VBO;
	//
	//
	//float distance_value[3] = { 0.0f, 0.0f, 0.0f };
	//distance_value[0] = max_value[0] - min_value[0];
	//distance_value[1] = max_value[1] - min_value[1];
	//distance_value[2] = max_value[2] - min_value[2];
	//
	//cout << "max_x = " << max_value[0] << "\t min_x = " << min_value[0] << "\t distance = " << distance_value[0] << endl;
	//cout << "max_y = " << max_value[1] << "\t min_y = " << min_value[1] << "\t distance = " << distance_value[1] << endl;
	//cout << "max_z = " << max_value[2] << "\t min_z = " << min_value[2] << "\t distance = " << distance_value[2] << endl;
	//
	//
	//float shift_value[3] = { 0.0f,0.0f, 0.0f };
	//
	//for (size_t i = 0; i < 3; i++)
	//{
	//	//x, y, z 3 dimension
	//
	//	float length_on_demension = (max_value[i] - min_value[i]);
	//
	//	float mid_pos = min_value[i] + (length_on_demension / 2);
	//
	//
	//	if (mid_pos < 0.0f) {
	//		shift_value[i] = abs(mid_pos);
	//	}
	//	else
	//	{
	//		shift_value[i] = -mid_pos;
	//	}
	//}

	//std::vector<vec4>::const_iterator ite = pos_set.begin();
	//for (; ite != pos_set.end(); ite++)
	//{
	//	uint32_t vid = ite - pos_set.begin();
	//	vec4 t_vertex = (*ite);
	//
	//	//t_VBO[(vid * 4) + 0] = (t_vertex.x + shift_value[0]) / (distance_value[0]);
	//	//t_VBO[(vid * 4) + 1] = (t_vertex.y + shift_value[1]) / (distance_value[1]);
	//	//t_VBO[(vid * 4) + 2] = (t_vertex.z + shift_value[2]) / (distance_value[2]);
	//	//t_VBO[(vid * 4) + 3] = t_vertex.w;
	//
	//
	//	t_VBO[(vid * 4) + 0] = t_vertex.x;
	//	t_VBO[(vid * 4) + 1] = t_vertex.y;
	//	t_VBO[(vid * 4) + 2] = t_vertex.z;
	//	t_VBO[(vid * 4) + 3] = t_vertex.w;
	//}
}


GLuint Mesh::Gen_Mesh_Vertex_VAO() {

	
	GLuint attribute_vec_size = 4;

	GLuint  VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));


	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*attribute_vec_size* mesh_vertex_count, mesh_position_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(Triangle_Mode_Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));

	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	/*
	VERIFY_GL(glGenBuffers(1, &EBO));
	VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	VERIFY_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	VERIFY_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	*/

	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	return VAO;
}

GLuint Mesh::Gen_Mesh_Triangle_Line_VAO() {


	assert(mesh_vertex_count != 0 && "error: invalid vertex count.");
	assert((mesh_vertex_count % 3) == 0 && "error: invalid vertex count.");

	uint32_t t_triangle_count = (mesh_vertex_count / 3);

	uint32_t t_mesh_point_mode_vertex_count = t_triangle_count * 3 * 2;
	GLuint point_vb_buffer_size = t_mesh_point_mode_vertex_count * 4;

	//contain all points
	GLfloat * t_point_vb_buffer = new GLfloat[point_vb_buffer_size];


	for (uint32_t i = 0; i < t_triangle_count; i++) {
		//cout << "triangle_" << i << endl;

		uint32_t vb_base = i * 12;
		GLfloat x0 = mesh_position_buffer[vb_base + 0];
		GLfloat y0 = mesh_position_buffer[vb_base + 1];
		GLfloat z0 = mesh_position_buffer[vb_base + 2];
		GLfloat w0 = mesh_position_buffer[vb_base + 3];

		GLfloat x1 = mesh_position_buffer[vb_base + 4 + 0];
		GLfloat y1 = mesh_position_buffer[vb_base + 4 + 1];
		GLfloat z1 = mesh_position_buffer[vb_base + 4 + 2];
		GLfloat w1 = mesh_position_buffer[vb_base + 4 + 3];

		GLfloat x2 = mesh_position_buffer[vb_base + 8 + 0];
		GLfloat y2 = mesh_position_buffer[vb_base + 8 + 1];
		GLfloat z2 = mesh_position_buffer[vb_base + 8 + 2];
		GLfloat w2 = mesh_position_buffer[vb_base + 8 + 3];


		//below give line info
		uint32_t line_vb_base = i * 24;
		uint32_t offset = -1;

		//a->b
		offset = line_vb_base + 0;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = x0;
		//cout << "write  0" << endl;

		offset = line_vb_base + 1;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = y0;
		//cout << "write  1" << endl;

		offset = line_vb_base + 2;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = z0;
		//cout << "write  2" << endl;

		offset = line_vb_base + 3;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = w0;
		//cout << "write  3" << endl;


		offset = line_vb_base + 4;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = x1;
		//cout << "write  4" << endl;

		offset = line_vb_base + 5;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = y1;
		//cout << "write  5" << endl;

		offset = line_vb_base + 6;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = z1;
		//cout << "write  6" << endl;

		offset = line_vb_base + 7;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = w1;
		//cout << "write  7" << endl;

		//b->c
		offset = line_vb_base + 8;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = x1;
		//cout << "write  8" << endl;

		offset = line_vb_base + 9;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = y1;
		//cout << "write  9" << endl;

		offset = line_vb_base + 10;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = z1;
		//cout << "write  10" << endl;

		offset = line_vb_base + 11;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = w1;
		//cout << "write  11" << endl;

		offset = line_vb_base + 12;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = x2;
		//cout << "write  12" << endl;

		offset = line_vb_base + 13;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = y2;
		//cout << "write  13" << endl;

		offset = line_vb_base + 14;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = z2;
		//cout << "write  14" << endl;

		offset = line_vb_base + 15;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = w2;
		//cout << "write  15" << endl;

		//c->a
		offset = line_vb_base + 16;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = x2;
		//cout << "write  16" << endl;

		offset = line_vb_base + 17;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = y2;
		//cout << "write  17" << endl;

		offset = line_vb_base + 18;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = z2;
		//cout << "write  18" << endl;

		offset = line_vb_base + 19;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = w2;
		//cout << "write  19" << endl;


		offset = line_vb_base + 20;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = x0;
		//cout << "write  20" << endl;

		offset = line_vb_base + 21;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = y0;
		//cout << "write  21" << endl;

		offset = line_vb_base + 22;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = z0;
		//cout << "write  22" << endl;

		offset = line_vb_base + 23;
		//cout << "offset\t " << offset << endl;
		assert(offset < point_vb_buffer_size);
		t_point_vb_buffer[offset] = w0;
		//cout << "write  23" << endl;
	}
	cout << "\t convert mesh to line success!" << endl;


	GLuint attribute_vec_size = 4;
	GLuint  VAO = 0, VBO = 0, EBO = 0;
	VERIFY_GL(glGenVertexArrays(1, &VAO));
	VERIFY_GL(glGenBuffers(1, &VBO));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	VERIFY_GL(glBindVertexArray(VAO));
	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VERIFY_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*t_mesh_point_mode_vertex_count*attribute_vec_size, t_point_vb_buffer, GL_STATIC_DRAW));

	GLint pos_location = -1;
	VERIFY_GL_RET(pos_location, glGetAttribLocation(mesh_line_Program, "in_position"));
	VERIFY_GL(glEnableVertexAttribArray(pos_location));
	VERIFY_GL(glVertexAttribPointer(pos_location, attribute_vec_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
	VERIFY_GL(glDisableVertexAttribArray(pos_location));

	VERIFY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));

	VERIFY_GL(glBindVertexArray(0)); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	delete[] t_point_vb_buffer;
	return VAO;

	return 0;
}


GLuint Mesh::Gen_Boundary_Vertex_VAO() {

	assert(BoundaryVertex.size() == 6 && "error: expect the 6 boundary vertex position.");


	constexpr GLuint attribute_vec_size = 4;
	uint32_t t_buffer_size = BoundaryVertex.size() * attribute_vec_size;

	uint32_t boundary_vertex_count = 6;
	GLfloat * t_vertex_buffer = new GLfloat[t_buffer_size];
	memset(t_vertex_buffer, 0, t_buffer_size);

	for (size_t i = 0; i < BoundaryVertex.size(); i++) {
		t_vertex_buffer[i * 4 + 0] = BoundaryVertex[i].Position.x;
		t_vertex_buffer[i * 4 + 1] = BoundaryVertex[i].Position.y;
		t_vertex_buffer[i * 4 + 2] = BoundaryVertex[i].Position.z;
		t_vertex_buffer[i * 4 + 3] = 1.0;
		//t_vertex_buffer[i * 4 + 3] = BoundaryVertex[i].w;
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

void Mesh::Gen_BoundingBox_Vertex() {

	assert(BoundaryVertex.size() == 6 && "error:  boundary_vertex size must be 6.");
	assert(mesh_boundingbox_vertex.size() == 0 && "error: before we generate boundingbox vertex. boundingbox_vertex size must be 0.");


	float max_x = BoundaryVertex[0].Position.x;
	float min_x = BoundaryVertex[1].Position.x;
										  	   
	float max_y = BoundaryVertex[2].Position.y;
	float min_y = BoundaryVertex[3].Position.y;
										  	   
	float max_z = BoundaryVertex[4].Position.z;
	float min_z = BoundaryVertex[5].Position.z;

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
	for (size_t i = 0; i < 8; i++){

		cout << "\t \t vertex[" << i << "]= (" << t_vertex[i].x << ", " << t_vertex[i].y << "," << t_vertex[i].z << ")" << endl;
		mesh_boundingbox_vertex[i] = t_vertex[i];
	}
}




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



