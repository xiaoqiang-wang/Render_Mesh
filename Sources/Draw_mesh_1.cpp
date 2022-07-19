
int main(int argc, char *argv[]){

	for (int i = 0; i < argc; i++){
		std::cout << "argument[" << i << "] is : " << argv[i] << endl;
	}

	std::string global_path = "UNKNOW PATH";
	if (argc == 2){
		global_path = argv[1];
	}

	cout << "global_path = " << global_path << endl;
	//char c0=getchar();




	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindowHint(GLFW_SAMPLES, 16);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "DRAW_MESH", nullptr, nullptr);
	if (window == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// Set the required callback functions
	//glfwSetKeyCallback(window, key_callback);

	glfwSetKeyCallback(window, key_callback_1);


	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK){
		std::cout << "Failed to initialize GLEW" << std::endl;

		assert(0 && "error:Failed to initialize GLEW.");
		return -1;
	}

	VERIFY_GL(glEnable(GL_MULTISAMPLE));

	// Define the viewport dimensions
	//int width =0, height=0;
	//glfwGetFramebufferSize(window, &width, &height);
	//VERIFY_GL(glViewport(0, 0, width, height));

	//int width = WIDTH, height = HEIGHT;
	//glfwGetFramebufferSize(window, &width, &height);
	//
	VERIFY_GL(glViewport(0, 0, WIDTH, HEIGHT));

	//WIDTH = 2368, HEIGHT = 1440;


	



	//使用回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_posititon_callback);

	//mouse button
	//glfwSetMouseButtonCallback(window, mouse_button_callback);

	/*
	*/



	
	// to use obj loader to render a scenen
	bool USE_OBJ_LOADER = true;
	if (USE_OBJ_LOADER){
		
		//to save all meshes' ptr
		std::vector<Mesh *> mesh_list;

		// some OBJ file can be down load from this page.
		//
		//  http://casual-effects.com/data/index.html
		//
		//
		std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\10_Cube\\cube.obj";
		//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\09_Crytek_Sponza\\sponza.obj";
		//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\13_Erato_broken\\erato-1.obj";
		//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\\34_Lat_Long_Sphere\\sphere-cubecoords.obj";
		//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\34_Lat_Long_Sphere\\sphere-cylcoords-16k.obj";
		//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\34_Lat_Long_Sphere\\sphere-cubecoords-4k.obj";
		//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\34_Lat_Long_Sphere\\sphere-cylcoords-16k.obj";

		uint32_t mesh_count = Load_Model_from_OBJ(model_path, mesh_list);
		assert(mesh_list.size() >0 && "Error: fail to get mesh from .OBJ file");

		//
		//初始化矩阵
		//
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(scaling_value, scaling_value, scaling_value));

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//glm::mat4 projection = glm::mat4(1.0f); //创建投影矩阵，第二个值应该为宽高比
		//glm::mat4 projection = glm::perspective(glm::radians(fov), float(WIDTH) / float(HEIGHT), nearClipPlane, farClipPlane);
		glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClipPlane, farClipPlane);

		{
			unsigned int depthMapFBO;
			glGenFramebuffers(1, &depthMapFBO);

			//depth mapping path
			const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

			GLuint depthMap = 0;
			{
				glGenTextures(1, &depthMap);
				glBindTexture(GL_TEXTURE_2D, depthMap);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}


		// Game loop
		std::cout << "Start drawing :" << std::endl;
		uint32_t frame_idx = 0;
		while (!glfwWindowShouldClose(window))
		{

			//shadow pass


			

			//
			// Check if any events have been activiated (key pressed, mouse moved etc.)
			// and call corresponding response functions
			//

			glfwPollEvents();
			process_keyboard_input(window);

			//
			//update frame count time:
			//Sleep(50);
			//cout << " Frame: " << frame_idx++ << endl;
			

			float currentFrameTime = glfwGetTime();
			deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;
			//shadow pass
			{
				//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
				//glClear(GL_DEPTH_BUFFER_BIT);
				//ConfigureShaderAndMatrices();
				//RenderScene();
				//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}



			//lighting pass

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
			VERIFY_GL(glEnable(GL_DEPTH_TEST));


			//
			//
			// 更新控制矩阵。
			// update control matrix
			//
			//

			model = glm::mat4(1.0f);
			//model = glm::scale(model, glm::vec3(scaling_value, scaling_value, scaling_value));

			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			//glm::mat4 projection = glm::mat4(1.0f); //创建投影矩阵，第二个值应该为宽高比
			projection = glm::perspective(glm::radians(fov), aspectRatio, nearClipPlane, farClipPlane);



			for (uint32_t mesh_idx = 0; mesh_idx < mesh_list.size(); mesh_idx++) {
				//cout << "Drawing Mesh :" << mesh_idx << endl;

				Mesh * t_mesh = mesh_list[mesh_idx];
				t_mesh->Draw_1(projection, view, model);
			}



			// Swap the screen buffers
			glfwSwapBuffers(window);



			//char t = getchar();
			if (to_save_screen) {
				//exit(-1);
				std::cout << "capturing frame..." << std::endl;

				time_t nowtime = time(NULL);
				struct tm* current_time = localtime(&nowtime);

				//".\\test_output.bmp";
				std::string output_file_name = std::to_string(current_time->tm_year + 1900) + "_" +
					std::to_string(current_time->tm_mon)  + "_" +
					std::to_string(current_time->tm_mday) + "_" +
					std::to_string(current_time->tm_hour) + "_" +
					std::to_string(current_time->tm_min)  + "_" +
					std::to_string(current_time->tm_sec)  + ".bmp";

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


	//
	// uint32_t on_debug = 3;
	//


	/***************** OLD_CODE *****************/
	// to do draw local mesh or do occlusion query.
	if (true)
	{
		std::vector<Mesh *> t_mesh_list;

		//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\cube\\cube.obj";
		std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\09_Crytek_Sponza\\sponza.obj";
		//std::string model_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\models\\13_Erato_broken\\erato-1.obj";
		uint32_t count = Load_Model_from_OBJ(model_path, t_mesh_list);


//#define VS_OUT  true

#ifdef  VS_OUT
		bool is_wordspace = true;
		//std::string file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\trex_pos_all.txt";
		//std::string file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mha_all.txt";
		std::string file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\demo_human_out_pos.txt";

		Mesh m(file_name, VS_SRC_board, FS_SRC_board, VS_SRC, FS_line_color, GL_TRIANGLES, is_wordspace);

#else
		bool is_local_space = true;
		//std::string file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\frog_man_pos.txt";
		//std::string file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\demo_human_in_pos.txt";


		//color light_color(195, 235, 252, 255);
		//color dark_color(96, 123, 145, 255);

		bool to_draw_boarder = true;
		Mesh * board = nullptr;
		{
			if (to_draw_boarder == true){

				//the bottom boarder with simple textures.
				GLuint boarder_textures[16] = { 0 };
				memset(boarder_textures,0,sizeof(GLuint)*16);

				//std::string boarder_image_file = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\t1.png";
				//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\Beautiful-stone-texture.jpg"

				//currently only supprt PNG file. jpg not supported.
				std::string image_filename_0 = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\floor.png";
				boarder_textures[0] = gen_simple_2D_texture(image_filename_0);

				std::string image_filename_1 = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\floor.png";
				boarder_textures[1] = gen_simple_2D_texture(image_filename_1);


				//the vertex shader to draw boader.
				const std::string VS_SRC_board =
					"#version 330 core                                     \n"
					"layout(location = 0) in highp vec4 in_position;       \n"
					"uniform highp mat4 mvp;                               \n"
					"out vec2 TexCoord;                                    \n"
					"void main()                                           \n"
					"{                                                     \n"
					"    gl_Position = mvp*vec4(in_position.xyz,1.0);      \n"
					"    TexCoord = vec2(in_position.x/4, in_position.y/4); \n"
					//"    TexCoord = vec2(abs(in_position.x)/4, abs(in_position.y)/4);\n"
					"}                                                     \n";

				const std::string FS_SRC_board =
					"#version 330 core                                     \n"
					"                                                      \n"
					"in vec2 TexCoord;                                     \n"
					"                                                      \n"
					"out vec4 color;                                       \n"
					"// texture samplers                                   \n"
					"uniform sampler2D texture1;                           \n"
					"uniform sampler2D texture2;                           \n"
					"                                                      \n"
					"void main()                                           \n"
					"{                                                     \n"
					"	// linearly interpolate between both textures (80% container, 20% awesomeface)    \n"
					"	color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);   \n"
					"}                                                     \n";

				/*
				//// to sample 2 textures
					const std::string VS_SRC_board_1 =
					"#version 330 core                                     \n"
					"layout(location = 0) in highp vec4 in_position;       \n"
					"uniform highp mat4 mvp;                               \n"
					"out vec2 TexCoord0;                                   \n"
					"out vec2 TexCoord1;                                   \n"
					"void main()                                           \n"
					"{                                                     \n"
					"    gl_Position = mvp*vec4(in_position.xyz,1.0);      \n"
					"    TexCoord0 = vec2(in_position.x/4, in_position.y/4); \n"
					"    TexCoord1 = vec2(in_position.x/4, in_position.y/4); \n"
					//"    TexCoord = vec2(abs(in_position.x)/4, abs(in_position.y)/4);\n"
					"}                                                     \n";

					const std::string FS_SRC_board_1 =
					"#version 330 core                                     \n"
					"                                                      \n"
					"in vec2 TexCoord0;                                     \n"
					"in vec2 TexCoord1;                                     \n"
					"                                                      \n"
					"out vec4 color;                                       \n"
					"// texture samplers                                   \n"
					"uniform sampler2D texture1;                           \n"
					"uniform sampler2D texture2;                           \n"
					"                                                      \n"
					"void main()                                           \n"
					"{                                                     \n"
					"	// linearly interpolate between both textures (80% container, 20% awesomeface)    \n"
					"	color = mix(texture(texture1, TexCoord0), texture(texture2, TexCoord1), 0.9);   \n"
					"}                                                     \n";
					*/
				

				std::string board_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\rectangle_box.txt";
				board = new Mesh(
					board_file_name, //mesh_input_file_name
					VS_SRC_board,    //surface_vs_src
					FS_SRC_board,    //surface_fs_src

					glm::uvec3(0),//vertex_color,
					glm::uvec3(69, 0, 0),//line_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

					glm::uvec3(255, 0, 0), //boundingbox_vertex_color,
					glm::uvec3(255, 0, 0), //boundingbox_line_color,
					glm::uvec3(255, 0, 0), //boundingbox_surface_color,


					glm::vec3(0.0f),//init_trans_arg
					glm::vec3(1.0f), //init_scale_arg
					glm::uvec3(0U), //init_rotate_arg
					false,// need_init_model_matrix
					boarder_textures
				);
			}
		}


		bool to_draw_sufrace = false;
		Mesh *surface = nullptr;
		{
			if (to_draw_sufrace == true)
			{
				const std::string VS_SRC_surface =
					"#version 330 core                                     \n"
					"layout(location = 0) in highp vec4 in_position;       \n"
					"uniform highp mat4 mvp;                               \n"
					"out vec3 TexCoord;                                    \n"
					"void main()                                           \n"
					"{                                                     \n"
					"    gl_Position = mvp*vec4(in_position.xyz,1.0);      \n"
					//"    TexCoord = vec2(in_position.x, in_position.y); \n"
					"    TexCoord = vec3((in_position.x)/10+0.5, (in_position.y)/4+0.5, (in_position.z));\n"
					"}                                                     \n";

				const std::string FS_SRC_surface =
					"#version 330 core                                     \n"
					"                                                      \n"
					"in vec3 TexCoord;                                     \n"
					"                                                      \n"
					"out vec4 color;                                       \n"
					"// texture samplers                                   \n"
					"uniform sampler2D texture1;                           \n"
					//"uniform sampler2D texture2;                         \n"
					"                                                      \n"
					"void main()                                           \n"
					"{                                                     \n"
					"	// linearly interpolate between both textures (80% container, 20% awesomeface)    \n"
					//"	color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);   \n"
					//"	color = vec4(texture(texture1, TexCoord).xyz, 1.0); \n"
					//"	color = vec4(texture(texture1, TexCoord).xyz, 1.0); \n"
					"	if ((TexCoord.z == -0.1)|| (TexCoord.z == 0.1)) {                           \n"
					"	    vec4 color_0 = vec4(texture(texture1, vec2(TexCoord.x, TexCoord.y)).xyz, 1.0); \n"
					"        if( color.x != 0.0){                                                    \n"
					"	           color = vec4(213.0/255.0, 233.0/255.0, 253.0/255.0, 1.0);         \n"
					"        }                                                                       \n"
					"       else{                                                                    \n"
					"              color = color_0;                                                  \n"
					"        }                                                                       \n"
					"	}									                \n"
					"	else {                                              \n"
					"	    color = vec4(213.0/255.0, 233.0/255.0, 253.0/255.0, 1.0);               \n"
					"	}                                                   \n"
					"}                                                      \n";


				//textures
				GLuint surface_textures[16] = { 0 };
				std::string image_filename_0 = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\VS.png";

				surface_textures[0] = gen_simple_2D_texture(image_filename_0);


				//std::string surface_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\rectangle_surface.txt";
				std::string surface_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\rectangle_box.txt";
				surface = new Mesh(
					surface_file_name,   //mesh_input_file_name
					VS_SRC_surface,      //surface_vs_src
					FS_SRC_surface,      //surface_fs_src

					glm::uvec3(0),//vertex_color,
					glm::uvec3(69, 0, 0),//line_color,
					//glm::uvec3(255, 255, 255),//line_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_color,
					glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

					glm::uvec3(255, 0, 0), //boundingbox_vertex_color,
					glm::uvec3(255, 0, 0), //boundingbox_line_color,
					glm::uvec3(255, 0, 0), //boundingbox_surface_color,


					glm::vec3(0.0f, -1.0f, 0.0f),//init_trans_arg
					glm::vec3(1.0f), //init_scale_arg
					glm::uvec3(0U), //init_rotate_arg
					true,         // need_init_model_matrix
					surface_textures
				);
			}
		}

		/*
		std::string building2_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\2_out_vs_in_pos.txt";
		Mesh building2(
			building2_file_name, //mesh_input_file_name
			VS_SRC_BASIC,        //surface_vs_src
			FS_SRC_BASIC_surface_color,        //surface_fs_src

			glm::uvec3(0),//vertex_color,
			glm::uvec3(69, 0, 0),//line_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

			glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
			glm::uvec3(255, 0, 0), //boundingbox_line_color,
			glm::uvec3(255, 0, 0), //boundingbox_surface_color,

			glm::vec3(0.0f),//init_trans_arg
			glm::vec3(1.5f),//init_scale_arg
			glm::uvec3(180U, 0U, 0U),//init_rotate_arg
			true, // need_init_model_matrix
			nullptr
		);



		std::string building3_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\3_out_vs_in_pos.txt";
		//Mesh building3(building3_file_name, VS_SRC_T, FS_SRC_BASIC_surface_color, VS_SRC_T, FS_line_color, glm::vec3(0.0f), glm::vec3(0.1f), glm::uvec3(180U, 0U, 0U), true);
		Mesh building3(
			building3_file_name, //mesh_input_file_name
			VS_SRC_BASIC,        //surface_vs_src
			FS_SRC_BASIC_surface_color,    //surface_fs_src

			glm::uvec3(0),//vertex_color,
			glm::uvec3(69, 0, 0),//line_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

			glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
			glm::uvec3(255, 0, 0), //boundingbox_line_color,
			glm::uvec3(255, 0, 0), //boundingbox_surface_color,

			glm::vec3(0.0f),//init_trans_arg
			glm::vec3(0.1f),//init_scale_arg
			glm::uvec3(180U, 0U, 0U),//init_rotate_arg
			true // need_init_model_matrix
		);


		std::string building4_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\MHA_15_OUT\\out_building_EID_3281_VC_89481.csv";
		Mesh building4(
			building4_file_name, //mesh_input_file_name
			VS_SRC_BASIC,        //surface_vs_src
			FS_SRC_BASIC_surface_color,        //surface_fs_src

			glm::uvec3(0),//vertex_color,
			glm::uvec3(69, 0, 0),//line_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

			glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
			glm::uvec3(255, 0, 0), //boundingbox_line_color,
			glm::uvec3(255, 0, 0), //boundingbox_surface_color,

			glm::vec3(0.0f),//init_trans_arg
			glm::vec3(0.03f),//init_scale_arg
			glm::uvec3(180U, 0U, 0U),//init_rotate_arg
			true // need_init_model_matrix
		);


		std::string building5_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\MHA_15_OUT\\out_building_EID_2820_VC_941931.csv";
		Mesh building5(
			building5_file_name, //mesh_input_file_name
			VS_SRC_BASIC,        //surface_vs_src
			FS_SRC_BASIC_surface_color,        //surface_fs_src

			glm::uvec3(0),//vertex_color,
			glm::uvec3(69, 0, 0),//line_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

			glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
			glm::uvec3(255, 0, 0), //boundingbox_line_color,
			glm::uvec3(255, 0, 0), //boundingbox_surface_color,

			glm::vec3(0.0f),//init_trans_arg
			glm::vec3(0.03f),//init_scale_arg
			glm::uvec3(180U, 0U, 0U),//init_rotate_arg
			true // need_init_model_matrix
		);

		std::string building6_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\MHA_15_OUT\\out_building_EID_2720_VC_27244.csv";
		Mesh building6(
			building6_file_name, //mesh_input_file_name
			VS_SRC_BASIC,        //surface_vs_src
			FS_SRC_BASIC_surface_color,        //surface_fs_src

			glm::uvec3(0),//vertex_color,
			glm::uvec3(69, 0, 0),//line_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

			glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
			glm::uvec3(255, 0, 0), //boundingbox_line_color,
			glm::uvec3(255, 0, 0), //boundingbox_surface_color,

			glm::vec3(-17.7f,0.0f,0.0f),//init_trans_arg
			glm::vec3(0.08f),//init_scale_arg
			glm::uvec3(0U, 90U, 90U),//init_rotate_arg
			true // need_init_model_matrix
		);


		std::string frog_man_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\frog_man.csv";
		Mesh frog_man(
			frog_man_file_name,  //mesh_input_file_name
			VS_SRC_BASIC,        //surface_vs_src
			Gen_simple_FS(glm::vec3(188, 222, 232)), //surface_fs_src

			glm::uvec3(0),//vertex_color,
			glm::uvec3(19, 0, 0),//line_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_color,
			glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

			glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
			glm::uvec3(255, 0, 0), //boundingbox_line_color,
			glm::uvec3(255, 0, 0), //boundingbox_surface_color,

			glm::vec3(0.0f,0.0f,0.0f),//init_trans_arg
			glm::vec3(3.00f),//init_scale_arg
			glm::uvec3(180U,0U,0U),//init_rotate_arg
			true, // need_init_model_matrix
			nullptr
		);
		*/

		//listFiles("D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Benchmakr_MHA_31_LIGHT");

		//used  to draw simple mesh, such as a rectange, tiangle...
		bool to_draw_simple_mesh = true;
		Mesh *simple_mesh = nullptr;
		{
			if (to_draw_simple_mesh == true) {

				GLuint surface_textures[16] = { 0 };
				//std::string image_filename_0 = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\apple.png";
				std::string image_filename_0 = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\welcome_page.png";
				std::string image_filename_1 = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\manhattan_light_3_1.txt_result_0000.bmp";
				std::string image_filename_2 = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\mha_surface_mesh.bmp";
				

				surface_textures[0] = gen_simple_2D_texture(image_filename_0);
				surface_textures[1] = gen_simple_2D_texture(image_filename_1);
				surface_textures[2] = gen_simple_2D_texture(image_filename_2);

				//std::string mesh_filename = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F0_OQ_Test_1\\1_test_triangle_out.csv";

				std::string mesh_filename = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F0_OQ_Test_1\\0_test_rectange_out.csv";

				//the vertex shader to draw boader.
				const std::string t_basic_vs_src =
					"#version 330 core                                     \n"
					"layout(location = 0) in highp vec4 in_position;       \n"
					"uniform highp mat4 mvp;                               \n"
					"out vec2 TexCoord;                                    \n"
					"void main()                                           \n"
					"{                                                     \n"
					"    gl_Position = in_position;                        \n"
					"    TexCoord = vec2((in_position.x+1)/2, (-in_position.y+1)/2); \n"
					//"    TexCoord = vec2((in_position.x+1)/2, (in_position.y+1)/2); \n"
					//"    TexCoord = vec2(abs(in_position.x)/4, abs(in_position.y)/4);\n"
					"}                                                     \n";


				//const std::string t_basic_fs_src = Gen_simple_FS(glm::uvec3(255,0,0));

				const std::string t_basic_fs_src =
					"#version 330 core                                     \n"
					"                                                      \n"
					"in vec2 TexCoord;                                     \n"
					"uniform uvec4 global_time;                                    \n"
					"out vec4 color;                                       \n"
					"// texture samplers                                   \n"
					"uniform sampler2D texture0;                           \n"
					"uniform sampler2D texture1;                           \n"
					"                                                      \n"
					"void main()                                           \n"
					"{                                                     \n"
					"	// linearly interpolate between both textures (80% container, 20% awesomeface)              \n"
					"	color = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), global_time.x);       \n"
					//"	color = texture(texture1, TexCoord);               \n"
					//"	color = (texture(texture1, TexCoord)*(1-global_time.x) + (texture(texture2, TexCoord)*(global_time.x);      \n"
					//"	color = (texture(texture1, TexCoord)*0.8);      \n"
					"}                                                     \n";


				const std::string t_basic_fs_src1 =
					"#version 330 core                                     									          \n"
					"                                                      									          \n"
					"in vec2 TexCoord;                                     									          \n"
					"                                                      									          \n"
					"uniform uvec4 frame_info;                            									          \n"
					"                                                      									          \n"
					"// texture samplers                                   									          \n"
					"uniform sampler2D texture0;    //welcome                       								  \n"
					"uniform sampler2D texture1;    //colored texture                       						  \n"
					"uniform sampler2D texture2;    //mesh surface                    							      \n"
					"                                                      									          \n"
					"out vec4 color;                                       									          \n"
					"                                                      									          \n"
					"void main()                                           									          \n"
					"{                                                     									          \n"
					"	                                                   									          \n"
					"  switch(frame_info.y){                                                                          \n"
					"        case 0:     //show welcome                                                               \n"
					"            color = texture(texture0, TexCoord);                                                 \n"
					"            break;                                                                               \n"
					"                                                                                                 \n"
					"        case 1:       //show colored texture                                                     \n"
					"            color = texture(texture1, TexCoord);                                                 \n"
					"            break;                                                                               \n"
					"                                                                                                 \n"
					"                                                                                                 \n"
					"        case 2:       //show mesh texture                                                        \n"
					"            color = texture(texture2, TexCoord);                                                 \n"
					"            break;                                                                               \n"
					"                                                                                                 \n"
					"                                                                                                 \n"
					"        case 3:     //to colored texture to fade-out                                             \n"
					"            float up  = float(frame_info.z);                                                     \n"
					"            float down  = float(frame_info.w);                                                   \n"
					"            float mix_ratio  = up/down;                                                          \n"
					//"            float mix_ratio  = frame_info.z/frame_info.w;                                        \n"
					"            color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mix_ratio);    \n"
					//"            color = vec4(1.0, 0.0f, 0.0f, 0.0f)*  mix_ratio;                                     \n"
					"			 break;                                                                               \n"
					"                                                                                                 \n"
					"    }                                                                                            \n"
					" }						                                                                          \n";






				
				simple_mesh = new Mesh(
					mesh_filename,         //mesh_input_file_name
					t_basic_vs_src,        //surface_vs_src
					t_basic_fs_src1,        //surface_fs_src

					glm::uvec3(0),          //vertex_color,
					glm::uvec3(69, 0, 0),   //line_color,
					glm::uvec3(0, 255, 0),  //boundary_vertex_color,
					glm::uvec3(0, 255, 0),  //boundary_vertex_connect_color,

					glm::uvec3(255, 0, 0),  //boundingbox_vertex_color,
					glm::uvec3(255, 0, 0),  //boundingbox_line_color,
					glm::uvec3(255, 0, 0),  //boundingbox_surface_color,


					glm::vec3(0.0f, -1.0f, 0.0f),//init_trans_arg
					glm::vec3(1.0f), //init_scale_arg
					glm::uvec3(0U),  //init_rotate_arg
					true,            // need_init_model_matrix
					surface_textures
				);
			}
		}


		//std::string mesh_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Benchmakr_MHA_31_LIGHT";

		std::string path_set[] = {
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F3",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\4_CIJI\\F1",
			 "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\1_MHA\\F3",
			 //"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F3"
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F3",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\3_CarChase\\F3",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\4_CIJI\\F5",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\5_BAOLEI\\F5",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\6_NBA2K\\F3",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\1_MHA\\F1",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\1_MHA\\F2",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F0_OQ_Test",
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F0_OQ_Test_1",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F0",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F1",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F2",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F3",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\Frames\\F4",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F1",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F2",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F3",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F4",
			// "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\Frames\\samples\\2_TREX\\F5",
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
			ofs.open(visible_data_path.data(), std::ofstream::out | std::ofstream::trunc);//clear it first.
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
