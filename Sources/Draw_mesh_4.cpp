
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//std::vector<Mesh *> in_mesh_list;
	//std::vector<Mesh *> out_mesh_list;
	//
	//all mesh will saved in the list.
	//
	//

	std::vector<Mesh *> mesh_list;
	//create mesh and render as normal rountine.

	if (true){

		std::string path_set[] = {
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\4_CIJI\\F1",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\6_NBA2K\\F1",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F3",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F3",
			"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F0_OQ_Test",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F0",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F1",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F2",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F3",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F4",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F1",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F2",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F3",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F4",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F5",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\1_MHA\\F1",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\5_BAOLEI\\F4",
			//
		};

		for (size_t index = 0; index < 1; index++)
		{
			std::string mesh_path = path_set[index];
			if (argc == 2)
			{
				mesh_path = global_path;
			}

			std::string visible_data_path = mesh_path + "\\" + "data.txt";
			std::ofstream ofs;
			ofs.open(visible_data_path.data(), std::ofstream::out | std::ofstream::trunc);//clear_it_first.
			assert(ofs.is_open() && "error: fail to open target file.");

#if 0
			//input mesh names, 
			std::vector<std::string> in_names = ListFiles(mesh_path, "_in.csv");

			//output mesh names.
			std::vector<std::string> out_names = ListFiles(mesh_path, "_out.csv");

			assert(in_names.size() == out_names.size() && "error: input mesh files count must equal with output mesh files.");


			uint32_t rigid_transform_count = 0;


			for (size_t i = 0; i < in_names.size(); i++)
			{
				//cout << res[i] << endl;
				//cout << mesh_name[i] << endl;
				//std::string trex_out_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\TREX_OUT.csv";

				std::string t_in_mesh_file_name = in_names[i];
				std::string t_out_mesh_file_name = out_names[i];

				Mesh * t_in_mesh = new Mesh(
					t_in_mesh_file_name,        //mesh_input_file_name
					VS_SRC_OUT_POS,        //surface_vs_src
					Gen_simple_FS(glm::vec3(188, 222, 232)), //surface_fs_src

					glm::uvec3(0),//vertex_color,
					glm::uvec3(19, 0, 0),//line_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

					glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
					glm::uvec3(255, 0, 0), //boundingbox_line_color,
					glm::uvec3(255, 0, 0), //boundingbox_surface_color,

					glm::vec3(0.0f),//init_trans_arg
					glm::vec3(1.0f),//init_scale_arg
					glm::uvec3(0U),//init_rotate_arg
					false, // need_init_model_matrix
					nullptr
				);

				Mesh * t_out_mesh = new Mesh(
					t_out_mesh_file_name,        //mesh_input_file_name
					VS_SRC_OUT_POS,        //surface_vs_src
					Gen_simple_FS(glm::vec3(188, 222, 232)), //surface_fs_src

					glm::uvec3(0),//vertex_color,
					glm::uvec3(19, 0, 0),//line_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

					glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
					glm::uvec3(255, 0, 0), //boundingbox_line_color,
					glm::uvec3(255, 0, 0), //boundingbox_surface_color,

					glm::vec3(0.0f),//init_trans_arg
					glm::vec3(1.0f),//init_scale_arg
					glm::uvec3(0U),//init_rotate_arg
					false, // need_init_model_matrix
					nullptr
				);

				assert(t_in_mesh->mesh_vertex_count == t_in_mesh->mesh_vertex_count && "error: vertex count, input mesh must equal with output mesh.");

				in_mesh_list.push_back(t_in_mesh);
				out_mesh_list.push_back(t_out_mesh);

				/*
				bool is_rigid = true;
				for (size_t i = 0; i < 6; i++)
				{
					if (t_in_mesh->BoundaryVertex_VID[i] != t_out_mesh->BoundaryVertex_VID[i])
					{
						//not a rigid tranform.
						is_rigid = false;
						break;
					}
				}
				if (is_rigid == true){
					cout << "===>> Find a rigid transform:"<< endl;
					rigid_transform_count++;
					t_in_mesh->is_rigid  = true;
					t_out_mesh->is_rigid = true;
				}
				else
				{
					t_in_mesh->is_rigid  = false;
					t_out_mesh->is_rigid = false;
				}
				*/
			}

#else

			
			//output mesh names.
			std::vector<std::string> mesh_file_names = ListFiles(mesh_path, "_out.csv");
			if (mesh_file_names.empty()) {
				mesh_file_names = ListFiles(mesh_path, ".csv");
			}
			assert(!mesh_file_names.empty() && "error: not find mesh files.");

			const std::string VS_SRC_OUT_POS_WITH_MVP =
				"#version 330 core                                     \n"
				"layout(location = 0) in highp vec4 in_position;       \n"
				"uniform highp mat4 mvp;                               \n"
				"void main()                                           \n"
				"{                                                     \n"
				"    gl_Position = mvp*vec4(in_position);              \n"
				"}                                                     \n";

			uint32_t seed = time(nullptr);
			srand(seed);

			//creat meshes from input file.
			uint32_t number_of_meshes = mesh_file_names.size();
			for (size_t i = 0; i < number_of_meshes; i++){

				//a test demo
				//std::string trex_out_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\TREX_OUT.csv";


				std::string t_mesh_file_name = mesh_file_names[i];
				std::cout << "Mesh: " << i << "/" << number_of_meshes << endl;
				std::cout <<"\t Mesh file:" <<t_mesh_file_name << endl;


				// float value = ((float)i / (float) number_of_meshes);
				// float r = 0.0f, g = 0.0f, b = 0.0f;
				// getHeatMapColor(value, &r, &g, &b);

				// uint8_t R = 0, G = 0, B = 0, A = 0;

				//uint8_t R = r * 255;
				//uint8_t G = g * 255;
				//uint8_t B = b * 255;
				//uint8_t A = 1.0 * 255;

			
				// uint8_t R = rand() % 256;
				// uint8_t G = rand() % 256;
				// uint8_t B = rand() % 256;
				// uint8_t A = 255;

				uint8_t R = 1.0 * 255;
				uint8_t G = 1.0 * 255;
				uint8_t B = 1.0 * 255;
				uint8_t A = 1.0 * 255;


				
				

				

				Mesh * t_out_mesh = new Mesh(
					t_mesh_file_name,         //mesh_file_name
					VS_SRC_OUT_POS_WITH_MVP,  //surface_vs_src
					Gen_simple_FS(glm::vec3(R, G, B)),   //surface_color,

					glm::uvec3(0),       //vertex_color,
					glm::uvec3(19, 0, 0),//line_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

					glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
					glm::uvec3(255, 0, 0),//boundingbox_line_color,
					glm::uvec3(255, 0, 0),//boundingbox_surface_color,

					glm::vec3(0.0f),//init_trans_arg
					glm::vec3(1.0f),//init_scale_arg
					glm::uvec3(0U), //init_rotate_arg
					false,          //need_init_model_matrix
					nullptr         //texture arg.
				);
				mesh_list.push_back(t_out_mesh);
			}

#endif 	
		}
	}


	//使窗口隐藏光标并且捕捉它
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	VERIFY_GL(glEnable(GL_DEPTH_TEST));
	VERIFY_GL(glDepthRangef(0.0f, 1.0f));
	VERIFY_GL(glDepthFunc(GL_LESS));

	VERIFY_GL(glClearColor(1.0, 1.0, 1.0, 1.0));
	VERIFY_GL(glClearDepthf(1.0f));
	VERIFY_GL(glClear(GL_COLOR_BUFFER_BIT));
	VERIFY_GL(glClear(GL_DEPTH_BUFFER_BIT));
	//VERIFY_GL(glClear(GL_STENCIL_BUFFER_BIT)); //no stencil buffer here.
	assert(glGetError() == GL_NO_ERROR);
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO





	uint32_t frame_idx = 0;
	cout << "Start drawing :" << endl;

	while (!glfwWindowShouldClose(window))
	{
		//cout<<"Frame:"<<(frame_idx++)<<endl;
		frame_idx++;
		{
			//to show FPS.
			float currentFrameTime = glfwGetTime();
			deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			if (frame_idx % 50 == 0) {
				float FPS = (1 / deltaTime);
				//cout << "FPS:" << FPS << endl;
				std::stringstream ss;
				ss << "DRAW_MESH " << " [" << std::fixed << std::setprecision(1) << FPS << " FPS ]";
				glfwSetWindowTitle(window, ss.str().c_str());
			}
		}

		// Check if any events have been activiated (key pressed, mouse moved etc.) 
		// and call corresponding response functions
		glfwPollEvents();
		process_keyboard_input(window);


		//cameraPos = glm::vec3(0.0, 0.0, 10.00);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//glm::mat4 projection = glm::mat4(1.0f); //创建投影矩阵，第二个值应该为宽高比
		glm::mat4  projection = glm::perspective(glm::radians(fov), float(WIDTH) / float(HEIGHT), nearClipPlane, farClipPlane);

		//glm::mat4 view = glm::mat4(1.0f);
		//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//glm::mat4 view = glm::mat4(1.0f);

		//Sleep(50);
		//cout << " Frame: " << frame_idx++ << endl;

		// render
		// ------
		//Render
		//Clear the colorbuffer
		//VERIFY_GL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));

		//VERIFY_GL(glClearColor(115.0f/255.0f, 123.0f/255.0f, 124.0f/ 255.0f, 1.0f));
		//VERIFY_GL(glClearColor(232.0f / 255.0f, 237.0f / 255.0f, 241.0f / 255.0f, 1.0f));
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		VERIFY_GL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
		//VERIFY_GL(glClearColor(0.75f, 0.75f, 0.75f, 0.75f));
		VERIFY_GL(glClearDepthf(1.0f));
		VERIFY_GL(glClear(GL_COLOR_BUFFER_BIT));
		VERIFY_GL(glClear(GL_DEPTH_BUFFER_BIT));

		//
		//
		//cameraPos.z = (cameraPos.z + 1.0);
		//
		//
		//

		//初始化矩阵
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(scaling_value, scaling_value, scaling_value));
		//绕着y轴旋转
		//rotate_value.x = (rotate_value.x + 1) % 360;
		//rotate_value.y = (rotate_value.y + 1) % 360;
		//cout<< rotate_value.y <<endl;
		//rotate_value.y = 90;
		//rotate_value.y = 75;
		//rotate_value.z = (rotate_value.z + 1) % 360;
		//model = glm::rotate(model, glm::radians(float(rotate_value.x)), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(float(rotate_value.y)), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(float(rotate_value.z)), glm::vec3(0.0f, 0.0f, 1.0f));


		
		//We draw the normal cube at last.
		for (uint32_t i = 1; i < mesh_list.size(); i++) {

			mesh_list[i]->Draw(projection,
				view,
				model,
				false, // to_draw_mesh_vertex
				false, // to_draw_BoundaryVertex
				false, // to_draw_BoundaryVertex_connect
				true,  // to_draw_mesh_line
				true,  // to_draw_mesh_surface
				false, // to_draw_boundingbox_vertex
				false, // to_draw_boundingbox_line
				false  // to_draw_boundingbox_surface
			);
		}
		
	
		if (to_draw_clean_mesh == 0){

			//We draw the normal cube at last.
			if (mesh_list.size() != 0) {
				//to draw the normal cube.
				mesh_list[0]->Draw(projection,
					view,
					model,
					false,  // to_draw_mesh_vertex
					false,  // to_draw_BoundaryVertex
					false,  // to_draw_BoundaryVertex_connect
					false,  // to_draw_mesh_line
					false,  // to_draw_mesh_surface
					false,  // to_draw_boundingbox_vertex
					true,   // to_draw_boundingbox_line
					false    // to_draw_boundingbox_surface
				);
			}
		}
		

		// Swap the screen buffers
		glfwSwapBuffers(window);






		
		if (to_save_screen){
			//exit(-1);

			time_t nowtime=time(NULL);
			struct tm* current_time = localtime(&nowtime);

			//".\\test_output.bmp";
			std::string output_file_name = std::to_string(current_time->tm_year) + "_" +
				std::to_string(current_time->tm_mon) + "_" +
				std::to_string(current_time->tm_mday) + "_" +
				std::to_string(current_time->tm_hour) + "_" +
				std::to_string(current_time->tm_min) + "_" +
				std::to_string(current_time->tm_sec)+".bmp";

			saveFrameBuff(output_file_name);
			//grab();
			//saveFrameBuff(output_file_name);
			to_save_screen = false;
			//exit(0);
		}
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}





GLuint gen_frame_buffer(
	uint32_t widht,
	uint32_t height
) {

	//GLuint idx = 0;
	//glGenFramebuffers(1, &idx);

	//step 1. generate a buffer
	GLuint frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);


	//step 2. bind and give attachment.
	//
	//
	// GL_DRAW_FRAMEBUFFER
	// GL_READ_FRAMEBUFFER
	// GL_FRAMEBUFFER
	//
	//
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);





	GLuint rboDepthStencil;
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);



	//OK 
	GLenum res = glCheckFramebufferStatus(frameBuffer);



	if (res== GL_FRAMEBUFFER_COMPLETE){
		return frameBuffer;
	}
	else{


		glDeleteFramebuffers(1, &frameBuffer); //in-complete buffer delete it.
		assert(0 && "error: never go here.");
		return 0;
	}
}




