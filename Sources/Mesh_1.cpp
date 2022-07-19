
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
