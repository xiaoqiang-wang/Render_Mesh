#ifndef __COMMON_H__
#define __COMMON_H__

#pragma once


#include <iostream>
#include <time.h>       /* time */
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <io.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <vector>
#include <array>

#include <assert.h>
#include <windows.h>
#include <sys/timeb.h>
#include <string>

#include <filesystem>


#include <algorithm>
#include <ctype.h>
#include <iostream>


#include<profileapi.h>



using namespace std;

//SOIL
#include <SOIL/SOIL.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


//GLM
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>





using namespace std;




extern glm::uvec4 frame_info;


//const GLuint WIDTH = 1024;
//const GLuint HEIGHT = 610;

//2416*1600
//const GLuint WIDTH = 2416/2;
//const GLuint HEIGHT = 1600/2;


const GLuint WIDTH = 2048 / 2;
const GLuint HEIGHT = 1220 / 2;

constexpr float board_height = 0.2;



const uint32_t to_draw_clean_mesh = 0;

template<typename T> 
void safe_delete(T*& a) {
	delete a;
	a = NULL;
}




#define  VERIFY_GL(gl_API) \
{\
	gl_API;\
	GLenum t_res = glGetError();\
	if (t_res != GL_NO_ERROR) {\
		if (t_res == GL_INVALID_ENUM) { std::cout << "GL_INVALID_ENUM" << std::endl; }\
		if (t_res == GL_INVALID_VALUE) { std::cout << "GL_INVALID_VALUE" << std::endl; }\
		if (t_res == GL_INVALID_OPERATION) { std::cout << "GL_INVALID_OPERATION" << std::endl; }\
		if (t_res == GL_INVALID_FRAMEBUFFER_OPERATION) { std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; }\
		if (t_res == GL_OUT_OF_MEMORY) { std::cout << "GL_OUT_OF_MEMORY" << std::endl; }\
		if (t_res == GL_STACK_UNDERFLOW) { std::cout << "GL_STACK_UNDERFLOW" << std::endl; }\
		if (t_res == GL_STACK_OVERFLOW) { std::cout << "GL_STACK_OVERFLOW" << std::endl; }\
		assert(0 && "error: OpenGL API error.");\
	}\
}



#define  VERIFY_GL_RET(left_value, gl_API) \
{\
	left_value = gl_API;\
	GLenum t_res = glGetError();\
	if (t_res != GL_NO_ERROR) {\
		if (t_res == GL_INVALID_ENUM) { std::cout << "GL_INVALID_ENUM" << std::endl; }\
		if (t_res == GL_INVALID_VALUE) { std::cout << "GL_INVALID_VALUE" << std::endl; }\
		if (t_res == GL_INVALID_OPERATION) { std::cout << "GL_INVALID_OPERATION" << std::endl; }\
		if (t_res == GL_INVALID_FRAMEBUFFER_OPERATION) { std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; }\
		if (t_res == GL_OUT_OF_MEMORY) { std::cout << "GL_OUT_OF_MEMORY" << std::endl; }\
		if (t_res == GL_STACK_UNDERFLOW) { std::cout << "GL_STACK_UNDERFLOW" << std::endl; }\
		if (t_res == GL_STACK_OVERFLOW) { std::cout << "GL_STACK_OVERFLOW" << std::endl; }\
		assert(0 && "error: OpenGL API error.");\
	}\
}


void __VERIFY_GL_STATUS__(const char *file, int line);

#define VERIFY_GL_STATUS() __VERIFY_GL_STATUS__(__FILE__,__LINE__)



GLuint CreateShader(GLenum type, const std::string &str);

GLuint CreateProgram(const std::string &vs_str, const std::string &fs_str);


//
// FIXME: todo to move uniforms to UBOs.
//

GLuint Query_Uniform_Location(GLuint Program, const std::string& attr_name);

GLuint Set_Uniform_Mat4(GLuint Program, const std::string& uniform_name, const glm::mat4 &mat);

GLuint Set_Uniform_Vec4(GLuint Program, const std::string& uniform_name, const glm::vec4 &vec);

GLuint Set_Uniform_Vec3(GLuint Program, const std::string& uniform_name, const glm::vec3 &vec);
GLuint Set_Uniform_int1(GLuint Program, const std::string &uniform_name, const int value);
GLuint Set_Uniform_float1(GLuint Program, const std::string &uniform_name, const float value);



GLuint Query_Sampler_Location(GLuint Program, const std::string& sampler_name);


GLuint Set_Sampler_Texture(GLuint Program, const std::string& sampler_name, GLuint Texture_Unit_idx);


GLuint Query_Attribute_Location(GLuint Program, const std::string& attr_name);

GLuint Enable_Attribute_Location(GLuint Program, const std::string& attr_name);

GLuint Set_Attribute_ptr(GLuint Program, const std::string& attr_name);

GLuint Reset_GL_Context();





std::string Read_file_to_str(const std::string& filename);



//
// this Function used to generate RGB value for a value.
//
//
bool getHeatMapColor(float value, float *red, float *green, float *blue);







void saveFrameBuff(std::string fileName);


/*
void grab(void)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	uint32_t WindowWidth = WIDTH;
	uint32_t WindowHeight = HEIGHT;

	GLint    i, j;
	GLint    PixelDataLength;   //BMP文件数据总长度

	// 计算像素数据的实际长度
	i = WindowWidth * 3;   // 得到每一行的像素数据长度
	while (i % 4 != 0)     // 补充数据，直到i是的倍数
		++i;
	PixelDataLength = i * WindowHeight;  //补齐后的总位数


	//指向新的空的内存，用于保存截图bmp文件数据
	GLubyte * pPixelData = new GLubyte[PixelDataLength]();
	if (pPixelData == nullptr) {
		assert(0 && "error: fail to new memory space!");
		exit(0);
	}

	//指向另一bmp文件，用于复制它的文件头和信息头数据
	FILE*    pDummyFile = fopen("F:\\000_BACKUP\\M_RJ\\figures\\template.bmp", "rb");//只读形式打开
	if (pDummyFile == 0) {
		assert(0 && "error: fail open sample bmp file!");
		exit(0);
	}

	//指向要保存截图的bmp文件
	FILE*    pWritingFile = fopen("grab.bmp", "wb"); //只写形式打开
	if (pWritingFile == 0)
	{
		assert(0 && "error: fail open dst bmp file!");
		exit(0);
	}

	const uint32_t BMP_Header_Length = 54;
	GLubyte  BMP_Header[BMP_Header_Length] = { 0 };
	//把读入的bmp文件的文件头和信息头数据复制，并修改宽高数据
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //读取文件头和信息头，占据54字节
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET); //移动到0X0012处，指向图像宽度所在内存
	i = WindowWidth;
	j = WindowHeight;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 读取当前画板上图像的像素数据
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //设置4位对齐方式
	glReadPixels(0, 0, WindowWidth, WindowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	// 写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	//把完整的BMP文件数据写入pWritingFile
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	// 释放内存和关闭文件
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}

*/


/*
void grab(void)
{

	//glReadBuffer(GL_FRONT);
	//glReadBuffer(GL_COLOR_ATTACHMENT0);

	uint32_t WindowWidth = WIDTH;
	uint32_t WindowHeight = HEIGHT;

	const uint32_t BMP_Header_Length = 54;


	GLubyte* pPixelData;    //指向新的空的内存，用于保存截图bmp文件数据
	GLubyte  BMP_Header[BMP_Header_Length];
	GLint    i, j;
	GLint    PixelDataLength;   //BMP文件数据总长度

	// 计算像素数据的实际长度
	i = WindowWidth * 3;   // 得到每一行的像素数据长度
	while (i % 4 != 0)      // 补充数据，直到i是的倍数
		++i;
	PixelDataLength = i * WindowHeight;  //补齐后的总位数

	// 分配内存和打开文件
	pPixelData = new GLubyte[PixelDataLength];
	if (pPixelData == nullptr) {
		assert(0 && "error: fail to new memory space!");
		exit(0);
	}

	//指向另一bmp文件，用于复制它的文件头和信息头数据
	FILE*    pDummyFile = fopen("F:\\000_BACKUP\\M_RJ\\figures\\template.bmp", "rb");//只读形式打开
	if (pDummyFile == 0) {
		assert(0 && "error: fail open sample bmp file!");
		exit(0);
	}

	//指向要保存截图的bmp文件
	FILE*    pWritingFile = fopen("grab.bmp", "wb"); //只写形式打开
	if (pWritingFile == 0)
	{
		assert(0 && "error: fail open dst bmp file!");
		exit(0);
	}


	//把读入的bmp文件的文件头和信息头数据复制，并修改宽高数据
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //读取文件头和信息头，占据54字节
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET); //移动到0X0012处，指向图像宽度所在内存
	i = WindowWidth;
	j = WindowHeight;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 读取当前画板上图像的像素数据
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //设置4位对齐方式
	glReadPixels(0, 0, WindowWidth, WindowHeight,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	// 写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	//把完整的BMP文件数据写入pWritingFile
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	// 释放内存和关闭文件
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}
*/





void listFiles(const char * dir);
std::vector<std::string> ListFiles(std::string path, std::string suffix);

//

long long systemtime();




#endif
