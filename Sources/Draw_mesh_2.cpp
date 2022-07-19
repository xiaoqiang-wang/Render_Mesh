
#else

			//output mesh names.
			std::vector<std::string> mesh_file_names = ListFiles(mesh_path, "_out.csv");
			if (mesh_file_names.empty()){
				mesh_file_names = ListFiles(mesh_path, ".csv");
			}
			assert(!mesh_file_names.empty() && "error: not find mesh files.");

			//
			//
			// uint32_t rigid_transform_count = 0;
			//
			//
	

			uint32_t number_of_meshes = mesh_file_names.size();
	
			uint32_t seed = time(nullptr);
			srand(seed);
			//to save all meshes' ptr
			std::vector<Mesh *> mesh_list;
			for (size_t i = 0; i < mesh_file_names.size(); i++) {

				//cout << res[i] << endl;
				//cout << mesh_name[i] << endl;
				//std::string trex_out_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\TREX_OUT.csv";

				std::string t_mesh_file_name = mesh_file_names[i];


				std::string  VS_SRC_OUT_POS =
					"#version 330 core                                     \n"
					"layout(location = 0) in highp vec4 in_position;       \n"
					"                                                      \n"
					"uniform highp mat4 mvp;                               \n"
					"                                                      \n"
					"out vec2 TexCoord;                                    \n"
					"                                                      \n"
					"void main()                                           \n"
					"{                                                     \n"
					"    gl_Position = mvp*vec4(-in_position.x,in_position.y,in_position.z,in_position.w);\n"
					//"    gl_Position = vec4(-in_position.x,in_position.y,in_position.z,in_position.w);\n"
					//"    gl_Position = vec4(in_position);              \n"
					"    TexCoord = vec2(((gl_Position.x/gl_Position.w)+1)/2, ((gl_Position.y/gl_Position.w)+1)/2); \n"
					"}                                                     \n";



				std::string  FS_SRC_OUT_POS =
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
					"	color =  vec4(1.0, 1.0,1.0,1.0);                   \n"
					//"	color =  vec4(global_time.x);                      \n"
					//"	color = (texture(texture1, TexCoord)*0.8);         \n"
					"}                                                     \n";


				std::cout << " Mesh: " << i << "/" << number_of_meshes << endl;
				Mesh * t_mesh = new Mesh(
					t_mesh_file_name,        // mesh_input_file_name
					VS_SRC_OUT_POS,          // surface_vs_src
					FS_SRC_OUT_POS,          // surface_fs_src
				
					glm::uvec3(0),           //vertex_color,
					glm::uvec3(19, 0, 0),    //line_color,
					glm::uvec3(0, 255, 0),   //boundary_vertex_color,
					glm::uvec3(0, 255, 0),   //boundary_vertex_connect_color,

					glm::uvec3(255, 0, 0),   //boundingbox_vertex_color,
					glm::uvec3(255, 0, 0),   //boundingbox_line_color,
					glm::uvec3(255, 0, 0),   //boundingbox_surface_color,

					glm::vec3(0.0f),         //init_trans_arg
					glm::vec3(1.0f),         //init_scale_arg
					glm::uvec3(0U),          //init_rotate_arg
					false,                   //need_init_model_matrix
					nullptr                  //texture resources.
				);

				mesh_list.push_back(t_mesh);
			}

#endif 

			/*
			bool to_draw_trex_in = true;
			std::string trex_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\TREX_IN.csv";
			Mesh trex_in(
				trex_file_name,  //mesh_input_file_name
				VS_SRC_BASIC,        //surface_vs_src
				Gen_simple_FS(glm::vec3(188, 222, 232)), //surface_fs_src

				glm::uvec3(0),//vertex_color,
				glm::uvec3(19, 0, 0),//line_color,
				glm::uvec3(0, 255, 0),//boundary_vertex_color,
				glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

				glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
				glm::uvec3(255, 0, 0), //boundingbox_line_color,
				glm::uvec3(255, 0, 0), //boundingbox_surface_color,

				glm::vec3(0.0f, -0.2f, 0.0f),//init_trans_arg
				glm::vec3(0.60f),//init_scale_arg
				glm::uvec3(180U, 0U, 0U),//init_rotate_arg
				true, // need_init_model_matrix
				nullptr
			);



			bool to_draw_trex_out = false;
			std::string trex_out_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\TREX_OUT.csv";
			Mesh trex_out(
				trex_out_file_name,    //mesh_input_file_name
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





			std::string soft_in_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\soft_vs_in.txt";
			Mesh soft_in(
				soft_in_file_name, //mesh_input_file_name
				VS_SRC_BASIC,      //surface_vs_src
				FS_SRC_BASIC_surface_color,        //surface_fs_src

				glm::uvec3(0),//vertex_color,
				glm::uvec3(69, 0, 0),//line_color,
				glm::uvec3(0, 255, 0),//boundary_vertex_color,
				glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

				glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
				glm::uvec3(255, 0, 0), //boundingbox_line_color,
				glm::uvec3(255, 0, 0), //boundingbox_surface_color,

				glm::vec3(0.0f,0.5f,-0.2f),//init_trans_arg
				glm::vec3(5.0f),//init_scale_arg
				glm::uvec3(0U, 0U, 0U),//init_rotate_arg
				true // need_init_model_matrix
			);


			std::string soft_out_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\soft_vs_out.txt";
			Mesh soft_out(
				soft_out_file_name, //mesh_input_file_name
				VS_SRC_BASIC,        //surface_vs_src
				FS_SRC_BASIC_surface_color,        //surface_fs_src

				glm::uvec3(0),//vertex_color,
				glm::uvec3(69, 0, 0),//line_color,
				glm::uvec3(0, 255, 0),//boundary_vertex_color,
				glm::uvec3(0, 255, 0),//boundary_vertex_connect_color,

				glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
				glm::uvec3(255, 0, 0), //boundingbox_line_color,
				glm::uvec3(255, 0, 0), //boundingbox_surface_color,

				glm::vec3(0.0f,-5000.0f,0.0f),//init_trans_arg
				glm::vec3(0.00025f),//init_scale_arg
				glm::uvec3(0U, 0U, 0U),//init_rotate_arg
				true // need_init_model_matrix
			);
			*/

			/*
			GLuint ball_textures[16] = { 0 };
			int ball_width = 0, ball_height = 0;
			unsigned char* ball_image = nullptr;
			std::string ball_image_file = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\al.png";
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\Beautiful-stone-texture.jpg"
			ball_image = SOIL_load_image(ball_image_file.c_str(), &ball_width, &ball_height, 0, SOIL_LOAD_RGB);
			//color light_color(195, 235, 252, 255);
			//color dark_color(96, 123, 145, 255);

			if (ball_image != nullptr)
			{
				GLuint texture1 = 0;
				glGenTextures(1, &texture1);
				glBindTexture(GL_TEXTURE_2D, texture1);
				// set the texture wrapping parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				// set texture filtering parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ball_width, ball_height, 0, GL_RGB, GL_UNSIGNED_BYTE, ball_image);
				glGenerateMipmap(GL_TEXTURE_2D);
				ball_textures[0] = texture1;
			}
			else
			{
				assert(0 && "error: failed to load image!");
			}


			std::string ball_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\demo_ball_in_pos.txt";
			//Mesh ball(ball_file_name, VS_SRC_T, FS_SRC_BASIC_surface_color, VS_SRC_T, FS_line_color, glm::vec3(0.0f), glm::vec3(1.0f), glm::uvec3(0U), true);
			Mesh ball(
				ball_file_name, //mesh_input_file_name
				VS_SRC_board,        //surface_vs_src
				FS_SRC_board,        //surface_fs_src

				glm::uvec3(0),//vertex_color,
				glm::uvec3(69,0,0),//line_color,
				glm::uvec3(0, 255, 0),//boundary_vertex_color,
				glm::uvec3(0, 128, 0),//boundary_vertex_connect_color,

				glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
				glm::uvec3(255, 0, 0), //boundingbox_line_color,
				glm::uvec3(255, 0, 0), //boundingbox_surface_color,

				glm::vec3(2.0f, 0.0f, 0.0f),//init_trans_arg
				glm::vec3(1.0f),//init_scale_arg
				glm::uvec3(0U, 0U, 0U),//init_rotate_arg
				true, // need_init_model_matrix
				ball_textures
			);



			GLuint hunman_textures[16] = { 0 };
			int hunman_width = 0, hunman_height = 0;
			unsigned char* hunman_image = nullptr;
			std::string hunman_image_file = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\cu.png";
			//"D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\Beautiful-stone-texture.jpg"
			hunman_image = SOIL_load_image(hunman_image_file.c_str(), &hunman_width, &hunman_height, 0, SOIL_LOAD_RGB);
			//color light_color(195, 235, 252, 255);
			//color dark_color(96, 123, 145, 255);

			if (hunman_image != nullptr)
			{
				GLuint texture1 = 0;
				glGenTextures(1, &texture1);
				glBindTexture(GL_TEXTURE_2D, texture1);
				// set the texture wrapping parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				// set texture filtering parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hunman_width, hunman_height, 0, GL_RGB, GL_UNSIGNED_BYTE, hunman_image);
				glGenerateMipmap(GL_TEXTURE_2D);
				hunman_textures[0] = texture1;
			}
			else
			{
				assert(0 && "error: failed to load image!");
			}


			std::string human_file_name = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\mesh_data\\demo_human_in_pos.txt";
			//Mesh human(human_file_name, VS_SRC_T, FS_SRC_BASIC_surface_color, VS_SRC_T, FS_line_color, glm::vec3(0.0f), glm::vec3(1.5f), glm::uvec3(180U, 0U, 0U), true);
			Mesh human(
				human_file_name, //mesh_input_file_name
				VS_SRC_board,        //surface_vs_src
				FS_SRC_board,        //surface_fs_src

				glm::uvec3(0),//vertex_color,
				glm::uvec3(69, 0, 0),//line_color,
				glm::uvec3(0, 255, 0),//boundary_vertex_color,
				glm::uvec3(0, 128, 0),//boundary_vertex_connect_color,

				glm::uvec3(255, 0, 0),//boundingbox_vertex_color,
				glm::uvec3(255, 0, 0), //boundingbox_line_color,
				glm::uvec3(255, 0, 0), //boundingbox_surface_color,

				glm::vec3(-2.0f,0.0f, 0.0f),//init_trans_arg
				glm::vec3(1.5f),//init_scale_arg
				glm::uvec3(180U, 0U, 0U),//init_rotate_arg
				true, // need_init_model_matrix
				hunman_textures
			);
			*/
#endif

			VERIFY_GL(glEnable(GL_DEPTH_TEST));
			VERIFY_GL(glDepthRangef(0.0f, 1.0f));
			VERIFY_GL(glDepthFunc(GL_LESS));

			VERIFY_GL(glClearColor(1.0, 1.0, 1.0, 1.0));
			VERIFY_GL(glClearDepthf(1.0f));
			VERIFY_GL(glClear(GL_COLOR_BUFFER_BIT));
			VERIFY_GL(glClear(GL_DEPTH_BUFFER_BIT));

			//VERIFY_GL(glClear(GL_STENCIL_BUFFER_BIT));

			glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
			assert(glGetError() == GL_NO_ERROR);




			VERIFY_GL(glEnable(GL_MULTISAMPLE));


			if (false) {
				GLuint fbo, rbo;
				VERIFY_GL(glGenFramebuffers(1, &fbo));
				VERIFY_GL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

				//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, &textureColorBufferMultiSampled, 0);

				VERIFY_GL(glGenRenderbuffers(1, &rbo));
				VERIFY_GL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
				VERIFY_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, WIDTH, HEIGHT));
				//VERIFY_GL(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, WIDTH, HEIGHT));
				//VERIFY_GL(glNamedRenderbufferStorageMultisample(rbo, 16, GL_RGB, WIDTH, HEIGHT));
				VERIFY_GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));



				GLuint depth;
				VERIFY_GL(glGenRenderbuffers(1, &depth));
				VERIFY_GL(glBindRenderbuffer(GL_RENDERBUFFER, depth));
				VERIFY_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, WIDTH, HEIGHT));
				//VERIFY_GL(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT32, WIDTH, HEIGHT));
				VERIFY_GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));


				VERIFY_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo));
				VERIFY_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth));

				VERIFY_GL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
				VERIFY_GL(glDrawBuffer(GL_COLOR_ATTACHMENT0));

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
					assert(0 && "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
				}
			}



			bool use_offline_FBO = false;
			if (use_offline_FBO == true) {

				GLuint framebuffer = 0;
				GLuint intermediateFBO = 0;
				/*
				// configure MSAA framebuffer
				// --------------------------
				glGenFramebuffers(1, &framebuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
				// create a multisampled color attachment texture
				//GLuint textureColorBufferMultiSampled;
				//glGenTextures(1, &textureColorBufferMultiSampled);
				//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
				//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, WIDTH, HEIGHT, GL_TRUE);
				//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
				//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
				//glRenderbufferStorageMultisample(GL_RENDERBUFFER,4, GL_RGBA8, WIDTH, HEIGHT);



				const GLuint NUM_OF_RBO = 6;
				GLuint rbo[NUM_OF_RBO] = {0};
				glGenRenderbuffers(NUM_OF_RBO, rbo);

				glBindRenderbuffer(GL_RENDERBUFFER, rbo[0]);

				glBindRenderbuffer(GL_RENDERBUFFER, rbo[0]);
				glRenderbufferStorageMultisample(GL_DRAW_FRAMEBUFFER, 4, GL_RGBA8, WIDTH, HEIGHT);

				glBindRenderbuffer(GL_RENDERBUFFER, rbo[0]);
				glRenderbufferStorageMultisample(GL_DRAW_FRAMEBUFFER, 4, GL_RGBA8, WIDTH, HEIGHT);
				glBindRenderbuffer(GL_RENDERBUFFER, rbo[1]);
				glRenderbufferStorageMultisample(GL_DRAW_FRAMEBUFFER, 4, GL_RGBA8, WIDTH, HEIGHT);
				glBindRenderbuffer(GL_RENDERBUFFER, rbo[2]);
				glRenderbufferStorageMultisample(GL_DRAW_FRAMEBUFFER, 4, GL_RGBA8, WIDTH, HEIGHT);
				glBindRenderbuffer(GL_RENDERBUFFER, rbo[3]);
				glRenderbufferStorageMultisample(GL_DRAW_FRAMEBUFFER, 4, GL_RGBA8, WIDTH, HEIGHT);

				glBindRenderbuffer(GL_RENDERBUFFER, rbo[4]);
				glRenderbufferStorageMultisample(GL_DRAW_FRAMEBUFFER, 4, GL_DEPTH_COMPONENT32F, WIDTH, HEIGHT);

				glBindRenderbuffer(GL_RENDERBUFFER, rbo[5]);
				glRenderbufferStorageMultisample(GL_DRAW_FRAMEBUFFER, 4, GL_STENCIL_INDEX16, WIDTH, HEIGHT);

				glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo[0]);
				glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, rbo[1]);
				glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_RENDERBUFFER, rbo[2]);
				glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_RENDERBUFFER, rbo[3]);

				glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, rbo[4]);
				glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo[5]);



				// create a (also multisampled) renderbuffer object for depth and stencil attachments
				unsigned int rbo;
				glGenRenderbuffers(1, &rbo);
				glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
						assert(0 && "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
				}

				glBindFramebuffer(GL_FRAMEBUFFER, 0);


				// configure second post-processing framebuffer
				glGenFramebuffers(1, &intermediateFBO);
				glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
				// create a color attachment texture
				GLuint screenTexture;
				glGenTextures(1, &screenTexture);
				glBindTexture(GL_TEXTURE_2D, screenTexture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
					assert(0 && "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
				}



				glBindFramebuffer(GL_FRAMEBUFFER, 0);


				*/


				// configure MSAA framebuffer
				// --------------------------

				glGenFramebuffers(1, &framebuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
				// create a multisampled color attachment texture
				unsigned int textureColorBufferMultiSampled;
				glGenTextures(1, &textureColorBufferMultiSampled);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, WIDTH, HEIGHT, GL_TRUE);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
				// create a (also multisampled) renderbuffer object for depth and stencil attachments
				unsigned int rbo;
				glGenRenderbuffers(1, &rbo);
				glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
					assert(0 && "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				// configure second post-processing framebuffer
				glGenFramebuffers(1, &intermediateFBO);
				glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
				// create a color attachment texture
				unsigned int screenTexture;
				glGenTextures(1, &screenTexture);
				glBindTexture(GL_TEXTURE_2D, screenTexture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
					assert(0 && "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}



			/*
			for (uint32_t i = 0; i < NUM_OF_OQ; i++)
			{
				GLboolean res = GL_FALSE;
				//VERIFY_GL_RET(res, glIsQuery(OQ_objs[i]));
				res = glIsQuery(OQ_objs[i]);
				assert(res == GL_TRUE && "error: fail to get OQ objects!");
			}
			*/

			//GLint zero = 0xFF;
			//VERIFY_GL(glClearBufferiv(GL_COLOR,GL_COLOR_ATTACHMENT0,&zero));

			std::cout << "Start drawing :" << std::endl;
			uint32_t frame_idx = 0;
			
			frame_info.x = 0;

			//
			//初始化矩阵
			//
			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::scale(model, glm::vec3(scaling_value, scaling_value, scaling_value));

			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			//glm::mat4 projection = glm::mat4(1.0f); //创建投影矩阵，第二个值应该为宽高比
			//glm::mat4 projection = glm::perspective(glm::radians(fov), float(WIDTH) / float(HEIGHT), nearClipPlane, farClipPlane);
			glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClipPlane, farClipPlane);
			

			uint32_t welcome_frame_count = 2000;
			uint32_t colored_texture_frame_count = 1000;
			uint32_t shinning_frame_count = 1000;
			uint32_t fade_out_frame_count  = 2000;

			uint32_t viewpos_move_back_frame_count = 2000;
			uint32_t viewpos_move_left_frame_count = 500;
			uint32_t viewpos_move_right_frame_count = viewpos_move_left_frame_count*2;

			uint32_t static_scene_frame_count =
				welcome_frame_count +
				colored_texture_frame_count +
				shinning_frame_count +
				fade_out_frame_count;

			uint32_t move_scene_frame_count =
				viewpos_move_back_frame_count +
				viewpos_move_left_frame_count +
				viewpos_move_right_frame_count;


			uint32_t total_frame_count =
				static_scene_frame_count +
				move_scene_frame_count;
				

			auto update_frame_info = [&]() {
				//to show welcome
				uint32_t scene_0_start = 0;
				uint32_t scene_0_end = welcome_frame_count;

				//to show colored texture
				uint32_t scene_1_start = scene_0_end;
				uint32_t scene_1_end = scene_1_start + colored_texture_frame_count;
					
				//to shinning texture and mesh
				uint32_t scene_2_start = scene_1_end;
				uint32_t scene_2_end = scene_2_start + shinning_frame_count;
					
				//texture fading out
				uint32_t scene_3_start = scene_2_end;
				uint32_t scene_3_end = scene_3_start + fade_out_frame_count;
					


				//to move back direction
				uint32_t scene_4_start = scene_3_end;
				uint32_t scene_4_end = scene_4_start + viewpos_move_back_frame_count;
					
				//to move left direction
				uint32_t scene_5_start = scene_4_end;
				uint32_t scene_5_end = scene_5_start + viewpos_move_left_frame_count;
					
				//to move right direction
				uint32_t scene_6_start = scene_5_end;
				uint32_t scene_6_end = scene_6_start + viewpos_move_right_frame_count;
					

				
				if ((scene_0_start < frame_info.x) && (frame_info.x <= scene_0_end)) {
					frame_info.y = 0;// to show welcome texture.
				}

				if ((scene_1_start < frame_info.x) && (frame_info.x <= scene_1_end)) {
					// to show colored surface
					frame_info.y = 1;
				}

				if ((scene_2_start < frame_info.x) && (frame_info.x <= scene_2_end)) {
					//to shining mesh and texture	
					uint32_t show_whitch = (frame_info.x / 200) % 2;
					frame_info.y = ((show_whitch == 0) ? 1 : 2);
				}

				if ((scene_3_start < frame_info.x) && (frame_info.x <= scene_3_end)) {
					//the texture surface to fade-out.

					frame_info.y = 3;

					//the texture to fade out.
					frame_info.z = frame_info.x - scene_3_start;
					frame_info.w = (scene_3_end- scene_3_start);
				}


				//to move back direction
				if ((scene_4_start < frame_info.x) && (frame_info.x <= scene_4_end)) {
					cameraPos.z -= 0.005;
				}

				//to move left direction
				if ((scene_5_start < frame_info.x) && (frame_info.x <= scene_5_end)) {
					//to shining mesh and texture	
					cameraPos.x += 0.005;
				}

				//to move right direction
				if ((scene_6_start < frame_info.x) && (frame_info.x <= scene_6_end)) {
					cameraPos.x -= 0.005;
				}

			};

			// Game loop
			while (!glfwWindowShouldClose(window))
			{
				//
				//update the time
				//

				frame_info.x += 1;
				std::cout << "frame_info.x : " << frame_info.x << endl;
				
				//global_time.x: to indicate global_frame_idx
				//global_time.y: to indicate which scene
				//global_time.z: to indicate mix ration 

			


				if (frame_info.x> total_frame_count){
					//to reset the scene.

					frame_info.x = 0;
					cameraPos = glm::vec3(0.155289, -0.14855, -0.460349);
					cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
					cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

					aspectRatio = 1.02266;

				}

				float currentFrameTime = glfwGetTime();
				deltaTime = currentFrameTime - lastFrameTime;
				lastFrameTime = currentFrameTime;


				//
				// Check if any events have been activiated (key pressed, mouse moved etc.)
				// and call corresponding response functions
				//
				glfwPollEvents();
				process_keyboard_input(window);



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




				/*
				for (uint32_t i = 0; i < mesh_list.size(); i++) {

					bool to_draw_mesh_surface = false;
					bool to_draw_mesh_vertex = false;

					Mesh *t_mesh = mesh_list[i];
					if (t_mesh->mesh_vertex_count % 3 == 0) {
						to_draw_mesh_surface = true;
					}
					else {
						to_draw_mesh_vertex = true;
					}


					
					t_mesh->Draw(
						projection,
						view,
						model,
						to_draw_mesh_vertex,
						false, // to_draw_BoundaryVertex,
						false, // to_draw_BoundaryVertex_connect,
						false, // to_draw_mesh_line,
						to_draw_mesh_surface,
						false, // to_draw_boundingbox_vertex,
						false, // to_draw_boundingbox_line,
						false  // to_draw_boundingbox_surface
					);
				}

				

				*/

				update_frame_info();
				if (frame_info.x<=static_scene_frame_count){

					//if (frame_info.x<=1000){
					//	frame_info.y = 0;// to show welcome full screen texture.
					//}
					//if ((1000<frame_info.x) && (frame_info.x <= 2000)){
					//	frame_info.y = 1;// to show colored surface
					//}
					//if ((2000 < frame_info.x) && (frame_info.x <= 4000)) {
					//	    //1000~4000 to shining mesh and texture	
					//		uint32_t show_whitch =(frame_info.x / 200) % 2;
					//		frame_info.y = ((show_whitch == 0) ? 1 : 2);
					//}
					//if ((4000 < frame_info.x) && (frame_info.x <= 5000)){
					//	//the texture surface to fade-out.
					//
					//	frame_info.y = 3;
					//
					//	//the texture to fade out.
					//	frame_info.z = 4000 - frame_info.x;
					//	frame_info.w = 1000;
					//}

				
					VERIFY_GL(glDisable(GL_DEPTH_TEST));
					simple_mesh->Draw(
						projection,
						view,
						model,
						false, // to_draw_mesh_vertex,
						false, // to_draw_BoundaryVertex,
						false, // to_draw_BoundaryVertex_connect,
						false, // to_draw_mesh_line,
						true,  // to_draw_mesh_surface,
						false, // to_draw_boundingbox_vertex,
						false, // to_draw_boundingbox_line,
						false  // to_draw_boundingbox_surface
					);

				}
				else{

					//
					//初始化矩阵
					//

					model = glm::mat4(1.0f);
					//model = glm::scale(model, glm::vec3(scaling_value, scaling_value, scaling_value));

					view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

					//glm::mat4 projection = glm::mat4(1.0f); //创建投影矩阵，第二个值应该为宽高比
					projection = glm::perspective(glm::radians(fov), aspectRatio, nearClipPlane, farClipPlane);

					



					VERIFY_GL(glEnable(GL_DEPTH_TEST));
					//mesh_list.size() //to skip the box
					for (uint32_t mesh_idx = 1; (mesh_idx < mesh_list.size() && mesh_idx < 84); mesh_idx++) {

						//cout << "Mesh:" << mesh_idx << endl;

						Mesh * t_mesh = mesh_list[mesh_idx];
						t_mesh->Draw(
							projection,
							view,
							model,
							false, // to_draw_mesh_vertex,
							false, // to_draw_BoundaryVertex,
							false, // to_draw_BoundaryVertex_connect,
							true,  // to_draw_mesh_line,
							true,  // to_draw_mesh_surface,
							false, // to_draw_boundingbox_vertex,
							false, // to_draw_boundingbox_line,
							false  // to_draw_boundingbox_surface
						);
					}

					//to_draw_normalized_cube.
					if (true) {

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

				}
			

			//	board->Draw(
			//		projection,
			//		view,
			//		model,
			//		false, // to_draw_mesh_vertex,
			//		false, // to_draw_BoundaryVertex,
			//		false, // to_draw_BoundaryVertex_connect,
			//		false, // to_draw_mesh_line,
			//		true,  // to_draw_mesh_surface,
			//		false, // to_draw_boundingbox_vertex,
			//		false, // to_draw_boundingbox_line,
			//		false  // to_draw_boundingbox_surface
			//	);

