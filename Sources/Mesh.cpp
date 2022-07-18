

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


