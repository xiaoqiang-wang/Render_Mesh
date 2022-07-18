



#include "Common.h"



void __VERIFY_GL_STATUS__(const char *file, int line)
{
	GLenum err(glGetError());
	static int numErrors = 0;
	while (err != GL_NO_ERROR)
	{
		std::string error;
		numErrors++;
		switch (err)
		{
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}

		std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;

		if (numErrors > 10)
		{
			exit(1);
		}

		err = glGetError();
	}
}









GLuint Reset_GL_Context() {
	VERIFY_GL(glGetError());

	//Rest  CONTEXT
	////////////////////////////////////////////////////////////////////////////////////
	{
		//disable
		for (uint32_t i = 0; i < 16; i++) {
			VERIFY_GL(glDisableVertexAttribArray(i));
		}
		VERIFY_GL(glBindVertexArray(0));
		VERIFY_GL(glUseProgram(0));

		VERIFY_GL(glDisable(GL_PROGRAM_POINT_SIZE));
		VERIFY_GL(glLineWidth(1.0));
	}

	VERIFY_GL(glGetError());


	return 0;
}



GLuint CreateShader(GLenum type, const std::string &str) {

	//Build and compile our shader program
	// Vertex shader
	GLuint t_Shader = 0;
	t_Shader = glCreateShader(type);
	//VERIFY_GL(glShaderSource(t_Shader, 1, (const  GLchar * const*)str.data(), NULL));

/*

		*”	Draw_mesh	d : \vs_project\opengl\project\draw_mesh\draw_mesh\draw_mesh.cpp	62
		*/
	const GLchar * src_data[1] = { str.c_str() };
	VERIFY_GL(glShaderSource(t_Shader, 1, src_data, NULL));
	VERIFY_GL(glCompileShader(t_Shader));

	// Check for compile time errors
	const GLuint MAX_LOG_LENGHT = 1024;
	GLchar infoLog[MAX_LOG_LENGHT];

	GLint success = GL_FALSE;
	VERIFY_GL(glGetShaderiv(t_Shader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		VERIFY_GL(glGetShaderInfoLog(t_Shader, MAX_LOG_LENGHT, NULL, infoLog));
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		assert(0 && "error:shader compile error!");
	}
	return t_Shader;
}


GLuint CreateProgram(const std::string &vs_str, const std::string &fs_str) {
	// Link shaders

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vs_str);

	GLuint	fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fs_str);

	GLuint shaderProgram = glCreateProgram();
	VERIFY_GL(glAttachShader(shaderProgram, vertexShader));
	VERIFY_GL(glAttachShader(shaderProgram, fragmentShader));
	VERIFY_GL(glLinkProgram(shaderProgram));

	//check link time error
	const GLuint MAX_LOG_LENGHT = 1024;
	GLchar infoLog[MAX_LOG_LENGHT];

	GLint success = GL_FALSE;
	VERIFY_GL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
	if (!success) {
		VERIFY_GL(glGetProgramInfoLog(shaderProgram, MAX_LOG_LENGHT, NULL, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		assert(0 && "ERROR: Fail to create shader program!");
	}

	VERIFY_GL(glUseProgram(shaderProgram));
	VERIFY_GL(glUseProgram(0));

	return shaderProgram;
}

GLuint Query_Uniform_Location(GLuint Program, const std::string& uniform_name) {

	VERIFY_GL(glValidateProgram(Program));

	GLuint t_location = -1;
	VERIFY_GL_RET(t_location, glGetUniformLocation(Program, uniform_name.data()));
	if (t_location == -1) {//-1 for invalid, uniform variable.
		std::cout << std::endl << std::endl << "Error: fail to query uniform location:" << uniform_name << std::endl << std::endl;
		assert(0 && "Error: fail to query uniforms");
	}

	return t_location;
}

GLuint Set_Uniform_Mat4(GLuint Program, const std::string &uniform_name, const glm::mat4 &mat) {
	VERIFY_GL(glValidateProgram(Program));

	GLuint t_location = Query_Uniform_Location(Program, uniform_name);
	VERIFY_GL(glUniformMatrix4fv(t_location, 1, false, glm::value_ptr(mat)));
	return 0;
}

GLuint Set_Uniform_Vec4(GLuint Program, const std::string &uniform_name, const glm::vec4 &vec) {
	VERIFY_GL(glValidateProgram(Program));

	GLuint t_location = Query_Uniform_Location(Program, uniform_name);
	VERIFY_GL(glUniform4fv(t_location, 1, glm::value_ptr(vec)));

	return 0;
}

GLuint Set_Uniform_Vec3(GLuint Program, const std::string &uniform_name, const glm::vec3 &vec) {
	VERIFY_GL(glValidateProgram(Program));

	GLuint t_location = Query_Uniform_Location(Program, uniform_name);
	VERIFY_GL(glUniform3fv(t_location, 1, glm::value_ptr(vec)));

	return 0;
}


GLuint Set_Uniform_int1(GLuint Program, const std::string &uniform_name, const int value) {
	VERIFY_GL(glValidateProgram(Program));

	GLuint t_location = Query_Uniform_Location(Program, uniform_name);
	VERIFY_GL(glUniform1f(t_location, value));

	return 0;
}

GLuint Set_Uniform_float1(GLuint Program, const std::string &uniform_name, const float value) {
	VERIFY_GL(glValidateProgram(Program));

	GLuint t_location = Query_Uniform_Location(Program, uniform_name);
	VERIFY_GL(glUniform1f(t_location, value));

	return 0;
}



GLuint Query_Sampler_Location(GLuint Program, const std::string& sampler_name) {
	VERIFY_GL(glValidateProgram(Program));
	return Query_Uniform_Location(Program, sampler_name);
}


GLuint Set_Sampler_Texture(GLuint Program, const std::string& sampler_name, GLuint Texture_Unit_idx){

	uint32_t t_sampler_location = Query_Sampler_Location(Program, sampler_name);
	VERIFY_GL(glUniform1i(t_sampler_location, Texture_Unit_idx));

	return 0;
}


GLuint Query_Attribute_Location(GLuint Program, const std::string& attr_name) {
	VERIFY_GL(glValidateProgram(Program));


	GLuint t_location = -1;
	VERIFY_GL_RET(t_location, glGetAttribLocation(Program, attr_name.data()));
	if (t_location == -1) {
		cout << std::endl<< "error: fail to get attribute from program, please check attribute_name in shader program: " << attr_name <<std::endl;
		assert(0 && "error: fail to get attribute from program, please check attribute_name in shader program.");
	}
	return t_location;
}

GLuint Enable_Attribute_Location(GLuint Program, const std::string& attr_name) {
	VERIFY_GL(glValidateProgram(Program));
	GLuint t_location = Query_Attribute_Location(Program,  attr_name);
	VERIFY_GL(glEnableVertexAttribArray(t_location));
	return 0;
}



std::string Read_file_to_str(const std::string& filename) {

	std::ifstream file(filename);
	if (!file.is_open()) {
		assert(0 && "Error: fail to open file.");
	}
	
	std::string temp_str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return temp_str;
}




bool getHeatMapColor(float value, float *red, float *green, float *blue)
{

	//https://www.andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients
	//  NUM of color to use.
#if 0
	const int NUM_COLORS = 5;
	static float color[NUM_COLORS][3] = { {0,0,1},{0,1,1}, {0,1,0}, {1,1,0}, {1,0,0} };
	// A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.
#else
	const int NUM_COLORS = 7;
	static float color[NUM_COLORS][3] = { {0,0,0}, {0,0,1},{0,1,1}, {0,1,0}, {1,1,0}, {1,0,0},{1,1,1} };
	// A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.
#endif

	int idx1;        // |-- Our desired color will be between these two indexes in "color".
	int idx2;        // |
	float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.

	if (value <= 0) { idx1 = idx2 = 0; }    // accounts for an input <=0
	else if (value >= 1) { idx1 = idx2 = NUM_COLORS - 1; }    // accounts for an input >=0
	else
	{
		value = value * (NUM_COLORS - 1);        // Will multiply value by 3.
		idx1 = floor(value);                  // Our desired color will be after this index.
		idx2 = idx1 + 1;                        // ... and before this index (inclusive).
		fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).
	}

	*red = (color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0];
	*green = (color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1];
	*blue = (color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2];

	return true;
}






void saveFrameBuff(std::string fileName)
{
	constexpr uint32_t BMP_Header_Length = 54;
	GLubyte BMP_Header[BMP_Header_Length] = { 0 };

	GLint i, j;
	GLint PixelDataLength;

	// 因为是窗口渲染，此处设置读取的数据来源于“前端窗口”（也可以是后端，详细见http://blog.csdn.net/hust_sheng/article/details/75268410）
	//glReadBuffer(GL_BACK);
	glReadBuffer(GL_FRONT);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	// 计算像素数据的实际长度
	// 默认是3通道
	uint32_t width = WIDTH;
	uint32_t height = HEIGHT;
	i = width * 4;      // 得到每一行的像素数据长度
	while (i % 4 != 0)  // 补充数据，直到 i 是的倍数
		++i;

	PixelDataLength = i * height;
	GLubyte * pPixelData = new GLubyte[PixelDataLength * 4]();
	// 但这里仅追求直观，对速度没有太高要求PixelDataLength = i * WindowHeight;
	// 分配内存和打开文件
	//pPixelData = (GLubyte*)malloc(PixelDataLength);
	//if (pPixelData == 0)
	//	exit(0);

	//std::string boarder_image_file = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\floor.png";
	//std::string boarder_image_file = "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\floor.png";
	FILE* pDummyFile = fopen("F:\\000_BACKUP\\M_RJ\\figures\\template.bmp", "rb");
	if (pDummyFile == nullptr) {
		assert(0 && "error: fail to find bmp a template!");
		exit(0);
	}


	FILE* pWritingFile = fopen(fileName.c_str(), "wb");
	if (pWritingFile == nullptr) {
		assert(0 && "error: fail to open target file!");
		exit(0);
	}

	// 读取像素
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);
	//glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	// 把 dummy.bmp 的文件头复制为新文件的文件头
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);



	//fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);
	i = width;
	j = height;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	// 释放内存和关闭文件
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);

	cout << "Capture frames success, saved to : " << fileName << endl;
	//exit(0);
};




void listFiles(const char * dir)
{
	
	std::vector<uint32_t> res;

	intptr_t handle;
	_finddata_t findData;
	handle = _findfirst(dir, &findData);    // 查找目录中的第一个文件
	if (handle == -1)
	{
		assert(0 && "Failed to find first file!\n");
	}

	do
	{
		if (findData.attrib & _A_SUBDIR && strcmp(findData.name, ".") == 0 && strcmp(findData.name, "..") == 0) {
			// 是否是子目录并且不为"."或".."
			cout << findData.name << "\t<dir>\n";
		}
		else {
			cout << findData.name << "\t" << findData.size << endl;

			std::string file_name = findData.name;

			if (file_name.find(".csv") != std::string::npos) {
				//We find a expected csv file.
				std::string prefix = file_name.substr(0, file_name.find(".csv"));
				uint32_t digital = std::stoi(prefix);

				res.push_back(digital);
			}
		}
	} while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件

	_findclose(handle);    // 关闭搜索句柄

	sort(res.begin(), res.end());
	for (size_t i = 0; i < res.size(); i++)
	{
		cout << res[i] << endl;
	}
}



bool is_Number(const std::string &s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}


std::vector<std::string> ListFiles(string path, std::string suffix)
{
	// float value for instanced draw.
	map<float, string> drawid_name_map;

	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, exdName;
	// \\* 代表要遍历所有的类型
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1) {
		assert(0 && "error: fail to open Directory!");
	}

	do
	{
		//判断文件的属性是文件夹还是文件
		//cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]") << endl;

		if(!(fileInfo.attrib&_A_SUBDIR)){
			//not a dir
			std::string file_name = fileInfo.name;

			if (file_name.find(suffix) != std::string::npos) {
				//We find a expected csv file.

				float drawid_value = 0.0f;
				std::string prefix = file_name.substr(0, file_name.find(suffix));

				size_t _pos = prefix.find("_");
				if (_pos != std::string::npos)
				{
					// file name like "3334_123.csv";
					// replace "_" to ".";
					std::string top_str = prefix.substr(0,_pos);
					std::string sub_str = prefix.substr(_pos + 1, prefix.length());

					//sub_index give it a value less than zero.
					if (is_Number(sub_str))
					{
						drawid_value = std::stof(top_str) + (std::stof(sub_str) * 0.001);
					}
					else{
						drawid_value = std::stof(top_str);
					}

				}
				else{
					//file name like "334.csv"
					drawid_value = std::stof(prefix);
				}

				drawid_name_map.insert(make_pair(drawid_value, prefix));
			}
		}

	} while (_findnext(hFile, &fileInfo) == 0);

	_findclose(hFile);

	//sorting mesh name from less to greater. draw_0 ~ draw_99 
	//sort(res.begin(), res.end());


	std::vector<std::string> mesh_file_list;
	for (map<float, string>::iterator ite = drawid_name_map.begin(); ite != drawid_name_map.end(); ++ite){
		std::string mesh_file_name = path + "\\" + ite->second + suffix;
		mesh_file_list.push_back(mesh_file_name);
	}

	return mesh_file_list;
}




long long systemtime()
{
	timeb t;
	ftime(&t);
	return t.time * 1000 + t.millitm;
}
