			if (0)
				{




					//cameraPos.z = (cameraPos.z + 1.0);

	


					/*
					if (to_draw_boarder == true){

						glm::uvec3 boarder_rotate_value = glm::uvec3(90, 0, 0); //三个坐标轴上的旋转分量
						model = glm::rotate(model, glm::radians(float(boarder_rotate_value.x)), glm::vec3(1.0f, 0.0f, 0.0f));

						//glm::mat4 mvp = projection * view * model;

						bool to_draw_mesh_vertex_0 = false;
						bool to_draw_BoundaryVertex_0 = false;
						bool to_draw_BoundaryVertex_connect_0 = false;
						bool to_draw_mesh_line_0 = false;
						bool to_draw_mesh_surface_0 = true;

						bool to_draw_boundingbox_vertex_0 = false;
						bool to_draw_boundingbox_line_0 = false;
						bool to_draw_boundingbox_surface_0 = false;


						board->Draw(projection,
							view,
							model,
							to_draw_mesh_vertex_0,
							to_draw_BoundaryVertex_0,
							to_draw_BoundaryVertex_connect_0,
							to_draw_mesh_line_0,
							to_draw_mesh_surface_0,
							to_draw_boundingbox_vertex_0,
							to_draw_boundingbox_line_0,
							to_draw_boundingbox_surface_0);

					}
					*/


					//绕着y轴旋转
					// rotate_value.x = (rotate_value.x + 1) % 360;
					// rotate_value.y = (rotate_value.y + 1) % 360;
					// cout<< rotate_value.y <<endl;
					// rotate_value.y = 90;
					// rotate_value.y = 75;
					// rotate_value.z = (rotate_value.z + 1) % 360;
					// model = glm::rotate(model, glm::radians(float(rotate_value.x)), glm::vec3(1.0f, 0.0f, 0.0f));
					// model = glm::rotate(model, glm::radians(float(rotate_value.y)), glm::vec3(0.0f, 1.0f, 0.0f));
					// model = glm::rotate(model, glm::radians(float(rotate_value.z)), glm::vec3(0.0f, 0.0f, 1.0f));


					// model = projection * view * model;

					//car0.Draw(model);
					//building0.Draw(projection, view, model,true,true,false, false, false, false, false);



					//bool to_draw_mesh_vertex = false;
					//bool to_draw_BoundaryVertex = true;
					//bool to_draw_BoundaryVertex_connect = true;
					//bool to_draw_mesh_line = true;
					//bool to_draw_mesh_surface = false;
					//
					//bool to_draw_boundingbox_vertex = true;
					//bool to_draw_boundingbox_line = true;
					//bool to_draw_boundingbox_surface = true;
					/*
					bool to_draw_mesh_vertex = false;
					bool to_draw_BoundaryVertex = false;
					bool to_draw_BoundaryVertex_connect = false;
					bool to_draw_mesh_line = false;
					bool to_draw_mesh_surface = true;

					bool to_draw_boundingbox_vertex = false;
					bool to_draw_boundingbox_line   = true;
					bool to_draw_boundingbox_surface = false;



					surface.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);
					*/
					/*
					ball.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);

					human.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);
					*/


					//////////////////////////////////////////////
					///below is building demo.
					/*
					building2.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);

					building3.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);


					building4.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);


					building5.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);


					building6.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);
					*/

					/*
					soft_in.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);

					soft_out.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);




					frog_man.Draw(projection,
						view,
						model,
						to_draw_mesh_vertex,
						to_draw_BoundaryVertex,
						to_draw_BoundaryVertex_connect,
						to_draw_mesh_line,
						to_draw_mesh_surface,
						to_draw_boundingbox_vertex,
						to_draw_boundingbox_line,
						to_draw_boundingbox_surface);
					*/


					//OQ objects
					const uint32_t NUM_OF_OQ_OBJ = 2;
					uint32_t OQ_objs[NUM_OF_OQ_OBJ] = { 0 };
					VERIFY_GL(glGenQueries(NUM_OF_OQ_OBJ, OQ_objs));

					uint64_t whole_frame_time = 0;
					for (uint32_t i = 0; i < mesh_list.size(); i++) //for (uint32_t i = 0; i < 0; i++)
					{

						//bool need_do_OQ = false;
						Mesh * t_mesh = mesh_list[i];

						const bool need_do_OQ = false;
						if (need_do_OQ == true) {

							/*
							GL_SAMPLES_PASSED
							GL_ANY_SAMPLES_PASSED
							GL_ANY_SAMPLES_PASSED_CONSERVATIVE
							GL_PRIMITIVES_GENERATED
							GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
							GL_TIME_ELAPSED
							*/
							long long start = 0;
							start = systemtime();

							LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
							LARGE_INTEGER Frequency;
							QueryPerformanceFrequency(&Frequency);

							QueryPerformanceCounter(&StartingTime);


							//DO visibility test.



							GLenum OQ_TRAGET = GL_ANY_SAMPLES_PASSED;
							//GLenum OQ_TRAGET = GL_TIME_ELAPSED;
							VERIFY_GL(glBeginQuery(OQ_TRAGET, OQ_objs[0]));
							{

								//disable update depth buffer during OQ
								VERIFY_GL(glDepthMask(GL_FALSE));

								//disable update color buffer during OQ.
								VERIFY_GL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

								//VERIFY_GL(glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
								//VERIFY_GL(glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
								//VERIFY_GL(glColorMaski(2, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
								//VERIFY_GL(glColorMaski(3, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

								//to Draw the mesh bounding-box.
								//to_draw_the_mesh;
								bool to_draw_mesh_vertex = false;
								bool to_draw_mesh_surface = false;

								if (t_mesh->mesh_vertex_count % 3 == 0) {
									to_draw_mesh_surface = true;
								}
								else {
									to_draw_mesh_vertex = true;
								}

								t_mesh->Draw(projection,
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
							VERIFY_GL(glEndQuery(OQ_TRAGET));


							uint32_t OQ_times = 0;
							GLint OQ_available = GL_FALSE;
							while (OQ_available == GL_FALSE) {
								//waiting for OQ Result:
								//cout << "waiting for OQ:" << (OQ_times++) << endl;
								VERIFY_GL(glGetQueryObjectiv(OQ_objs[0], GL_QUERY_RESULT_AVAILABLE, &OQ_available));
							}

							//cout << systemtime() - start << "ms" << endl;
							GLint is_visible = 0;
							VERIFY_GL(glGetQueryObjectiv(OQ_objs[0], GL_QUERY_RESULT, &is_visible));
							cout << "\t" << "\t OQ_res sample passed: " << is_visible << endl;
							//cout << systemtime() - start << "ms" << endl;

							QueryPerformanceCounter(&EndingTime);
							ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

							//cout << ElapsedMicroseconds.QuadPart << "ms" << endl;
							ElapsedMicroseconds.QuadPart *= 1000;
							double time_passed = (double)ElapsedMicroseconds.QuadPart / (double)Frequency.QuadPart;
							cout << "\t" << time_passed << "ms" << endl;
							//cout << Frequency.QuadPart << endl;
							//exit(0);


							uint32_t vertex_count = t_mesh->mesh_vertex_count;
							uint32_t mesh_count = t_mesh->mesh_id;
							uint32_t attributes_count = t_mesh->num_of_attributes;                       // how many attributes a mesh have.

							uint32_t attributes_components = t_mesh->num_of_attributes_components;

							assert((attributes_components % vertex_count) == 0 && "error: fail to get components count!");
							attributes_components = attributes_components / vertex_count;

							if (is_visible != 0)
							{
								ofs << "Mesh_file : \t" << t_mesh->name
									<< ", \t Draw : \t" << i
									<< ", \t vertex_count : \t" << vertex_count
									<< ", \t attributes_count : \t" << attributes_count
									<< ", \t attributes_components : \t" << attributes_components
									<< ", \t visibility : \t" << 1.0 //1.0 for visible
									<< endl;

								GLenum OQ_TRAGET = GL_TIME_ELAPSED;
								VERIFY_GL(glBeginQuery(OQ_TRAGET, OQ_objs[1]));
								{

									//disable update depth buffer during OQ
									VERIFY_GL(glDepthMask(GL_FALSE));

									//disable update color buffer during OQ.
									VERIFY_GL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

									//VERIFY_GL(glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
									//VERIFY_GL(glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
									//VERIFY_GL(glColorMaski(2, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
									//VERIFY_GL(glColorMaski(3, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

									//to Draw the mesh bounding-box.
									//to_draw_the_mesh;
									bool to_draw_mesh_vertex = false;
									bool to_draw_mesh_surface = false;

									if (t_mesh->mesh_vertex_count % 3 == 0) {
										to_draw_mesh_surface = true;
									}
									else {
										to_draw_mesh_vertex = true;
									}

									t_mesh->Draw(projection,
										view,
										model,
										false, // to_draw_mesh_vertex,
										false, // to_draw_BoundaryVertex,
										false, // to_draw_BoundaryVertex_connect,
										false, // to_draw_mesh_line,
										false, // to_draw_mesh_surface,
										true,  // to_draw_boundingbox_vertex,
										false, // to_draw_boundingbox_line,
										false  // to_draw_boundingbox_surface
									);


									//OK to enable depth and  color write mask 
									//enable update depth buffer during OQ
									VERIFY_GL(glDepthMask(GL_TRUE));

									//enable update color buffer during OQ
									VERIFY_GL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

									//to draw to full mesh.
									//glDraw XXXX;
									t_mesh->Draw(projection,
										view,
										model,
										to_draw_mesh_vertex,
										false, //to_draw_BoundaryVertex,
										false, //to_draw_BoundaryVertex_connect,
										false, //to_draw_mesh_line,
										to_draw_mesh_surface,
										false, //to_draw_boundingbox_vertex,
										false, //to_draw_boundingbox_line,
										false //to_draw_boundingbox_surface
									);
								}
								VERIFY_GL(glEndQuery(OQ_TRAGET));


								uint32_t OQ_times = 0;
								GLint time_available = GL_FALSE;
								while (time_available == GL_FALSE) {
									//waiting for OQ Result:
									//cout << "waiting for OQ:" << (OQ_times++) << endl;
									VERIFY_GL(glGetQueryObjectiv(OQ_objs[1], GL_QUERY_RESULT_AVAILABLE, &time_available));
								}

								//cout << systemtime() - start << "ms" << endl;
								GLint passed_time = 0;
								VERIFY_GL(glGetQueryObjectiv(OQ_objs[1], GL_QUERY_RESULT, &passed_time));

								//add whole frame time
								whole_frame_time += passed_time;
							}
							else
							{
#if 0
								//below is useless code, just comment it away.
								for (size_t i = 0; i < t_mesh->num_of_attributes; i++) {

									Attributes * attr_ite = &(t_mesh->mesh_attributes[i]);

									if (attr_ite->is_position == true)
									{

										assert(attr_ite->dimension == 4 && "fail to find out pos");
										assert((attr_ite->name.find("gl_Position") != std::string::npos) && "fail to find out pos");

										std::vector<glm::vec4>::const_iterator pos_ite = attr_ite->attr.begin();

										glm::vec4 position_0 = *pos_ite;
										for (size_t t_index = 0; t_index < 6; t_index++)
										{
											BoundaryVertex[i] = position_0;
										}

										// init the 6 mesh boundary vertex
										float max_x = attr_ite->attr[0].x;
										float min_x = attr_ite->attr[0].x;

										float max_y = attr_ite->attr[0].y;
										float min_y = attr_ite->attr[0].y;

										float max_z = attr_ite->attr[0].z;
										float min_z = attr_ite->attr[0].z;


										for (; pos_ite != attr_ite->attr.end(); pos_ite++) {

											glm::vec4 t_element = *pos_ite;

											t_element.x = t_element.x / t_element.w;
											t_element.y = t_element.y / t_element.w;
											t_element.z = t_element.z / t_element.w;
											t_element.w = t_element.w / t_element.w;


											//update the boundrary vertex infor
											if (t_element.x > max_x) {
												BoundaryVertex[0] = t_element;
												max_x = t_element.x;

											}

											if (t_element.x < min_x) {
												BoundaryVertex[1] = t_element;
												min_x = t_element.x;
											}

											if (t_element.y > max_y) {
												BoundaryVertex[2] = t_element;
												max_y = t_element.y;

											}

											if (t_element.y < min_y) {
												BoundaryVertex[3] = t_element;
												min_y = t_element.y;

											}

											if (t_element.z > max_z) {
												BoundaryVertex[4] = t_element;
												max_z = t_element.z;

											}

											if (t_element.z < min_z) {
												BoundaryVertex[5] = t_element;
												min_z = t_element.z;
											}
								}

										//we have find the position quite
										break;
							}

						}
#endif

								//invisible reason, default 0, ocludded by depth_buffer
								uint32_t invisible_reason = 0;
								if (t_mesh->is_out_side_normal_space == true) {
									for (size_t i = 0; i < 6; i++) {

										if (t_mesh->where_the_mesh[i] == true) {
											invisible_reason = i + 1;
											break;
											//1: mesh'x greather than x=1.0
											//2: mesh'x less than x=-1.0

											//3: mesh'y greather than y=1.0
											//4: mesh'y less than y=-1.0

											//5: mesh'z greather than z=1.0
											//6: mesh'z less than z=-1.0
										}
									}
								}
								else
								{
									//ocludded by depth_buffer
									invisible_reason = 0;
								}

								ofs << "Mesh_file : \t" << t_mesh->name
									<< ", \t Draw : \t" << i
									<< ", \t vertex_count : \t" << vertex_count
									<< ", \t attributes_count : \t" << attributes_count
									<< ", \t attributes_components : \t" << attributes_components
									<< ", \t visibility : \t" << -1.0 //-1.0 for in-visible
									<< ", \t reason : \t" << invisible_reason
									<< endl;
					}

							ofs.flush();
							ofs.flush();
						}

						else {

							GLenum OQ_TRAGET = GL_TIME_ELAPSED;
							VERIFY_GL(glBeginQuery(OQ_TRAGET, OQ_objs[1]));
							{
								//disable update depth buffer during OQ
								VERIFY_GL(glDepthMask(GL_TRUE));

								//disable update color buffer during OQ.
								//VERIFY_GL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));//for each channel.
								VERIFY_GL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

								//VERIFY_GL(glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
								//VERIFY_GL(glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
								//VERIFY_GL(glColorMaski(2, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
								//VERIFY_GL(glColorMaski(3, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

								//to Draw the mesh bounding-box.
								//to_draw_the_mesh;
								bool to_draw_mesh_vertex = false;
								bool to_draw_mesh_surface = false;

								if (t_mesh->mesh_vertex_count % 3 == 0) {
									to_draw_mesh_surface = true;
								}
								else {
									to_draw_mesh_vertex = true;
								}

								//to draw to full mesh.
								//glDraw XXXX;
								t_mesh->Draw(projection,
									view,
									model,
									to_draw_mesh_vertex,
									false, //to_draw_BoundaryVertex,
									false, //to_draw_BoundaryVertex_connect,
									false, //to_draw_mesh_line,
									to_draw_mesh_surface,
									false, //to_draw_boundingbox_vertex,
									false, //to_draw_boundingbox_line,
									false //to_draw_boundingbox_surface
								);
							}
							VERIFY_GL(glEndQuery(OQ_TRAGET));


							uint32_t OQ_times = 0;
							GLint time_available = GL_FALSE;
							while (time_available == GL_FALSE) {
								//waiting for OQ Result:
								//cout << "waiting for OQ:" << (OQ_times++) << endl;
								VERIFY_GL(glGetQueryObjectiv(OQ_objs[1], GL_QUERY_RESULT_AVAILABLE, &time_available));
							}

							//cout << systemtime() - start << "ms" << endl;
							GLint passed_time = 0;
							VERIFY_GL(glGetQueryObjectiv(OQ_objs[1], GL_QUERY_RESULT, &passed_time));

							//add whole frame time
							whole_frame_time += passed_time;
						}


			}

					ofs.flush();
					ofs.flush();
					ofs.close();



					/*

					if (to_draw_trex_in==true) {
						//绕着y轴旋转
						//rotate_value.x = (rotate_value.x + 1) % 360;
						//rotate_value.y = (rotate_value.y + 1) % 360;
						//cout<< rotate_value.y <<endl;
						//rotate_value.y = 0;
						rotate_value.y = 90;
						//rotate_value.z = (rotate_value.z + 1) % 360;


						bool to_draw_mesh_vertex = true;
						bool to_draw_BoundaryVertex = false;
						bool to_draw_BoundaryVertex_connect = false;
						bool to_draw_mesh_line = false;
						bool to_draw_mesh_surface = false;

						bool to_draw_boundingbox_vertex = false;
						bool to_draw_boundingbox_line = false;
						bool to_draw_boundingbox_surface = false;

						trex_in.Draw(projection,
							view,
							model,
							to_draw_mesh_vertex,
							to_draw_BoundaryVertex,
							to_draw_BoundaryVertex_connect,
							to_draw_mesh_line,
							to_draw_mesh_surface,
							to_draw_boundingbox_vertex,
							to_draw_boundingbox_line,
							to_draw_boundingbox_surface);
					}

					if (to_draw_trex_out==true) {

						//绕着y轴旋转
						//rotate_value.x = (rotate_value.x + 1) % 360;
						//rotate_value.y = (rotate_value.y + 1) % 360;
						//cout<< rotate_value.y <<endl;
						//rotate_value.y = 0;
						//rotate_value.y = 75;
						//rotate_value.z = (rotate_value.z + 1) % 360;


						bool to_draw_mesh_vertex = true;
						bool to_draw_BoundaryVertex = false;
						bool to_draw_BoundaryVertex_connect = false;
						bool to_draw_mesh_line = false;
						bool to_draw_mesh_surface = false;

						bool to_draw_boundingbox_vertex = false;
						bool to_draw_boundingbox_line = false;
						bool to_draw_boundingbox_surface = false;

						trex_out.Draw(projection,
							view,
							model,
							to_draw_mesh_vertex,
							to_draw_BoundaryVertex,
							to_draw_BoundaryVertex_connect,
							to_draw_mesh_line,
							to_draw_mesh_surface,
							to_draw_boundingbox_vertex,
							to_draw_boundingbox_line,
							to_draw_boundingbox_surface);
					}
					*/
					// 2. now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
					//glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
					//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
					//glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);





					std::string time_info_path = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\time_info.txt";
					std::ofstream ofs_1;
					ofs_1.open(time_info_path.data(), std::ofstream::out | std::ofstream::app);
					assert(ofs_1.is_open() && "error: fail to open target file.");


					ofs_1 << mesh_path << ", whole_frame_time: " << whole_frame_time << endl;
					ofs_1.flush();
					ofs_1.close();
					std::cout << "DO JOB finish." << endl;
				}


				// Swap the screen buffers
				glfwSwapBuffers(window);



				//char t = getchar();
				//exit(0);
				
				if (to_save_screen) {
					//exit(-1);

					time_t nowtime = time(NULL);
					struct tm* current_time = localtime(&nowtime);

					//".\\test_output.bmp";
					std::string output_file_name = std::to_string(current_time->tm_year+1900) + "_" +
						std::to_string(current_time->tm_mon) + "_" +
						std::to_string(current_time->tm_mday) + "_" +
						std::to_string(current_time->tm_hour) + "_" +
						std::to_string(current_time->tm_min) + "_" +
						std::to_string(current_time->tm_sec) + ".bmp";

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
	}

