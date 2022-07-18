
#include "texture.h"








GLuint gen_simple_2D_texture(const std::string &image_filename) 
{
	//
	// currently only supprt PNG file as input texture.
	// demo: "D:\\VS_Project\\OpenGL\\Project\\Draw_mesh\\texture\\floor.png"
	// std::size_t dot_pos = image_filename.find('.');
	//

	std::size_t dot_pos = image_filename.find_last_of(".");

	if (dot_pos != std::string::npos) {

		std::size_t dir_split = image_filename.find_last_of("/\\");
		std::string file_name = image_filename.substr(dir_split + 1, ((dot_pos - dir_split) - 1));
		std::string  file_suffix = image_filename.substr((dot_pos + 1), image_filename.length());

		//assert((file_suffix == "png") && "error: currently only supprt PNG file as input texture.");
	}
	else{
		assert(0 && "error: invalid image_filename.");
	}


	int image_width = 0;
	int image_height = 0;
	int channels = 0;

	//NOTE: for texture errors, please check the image format, between RGBA, and RGB.

	//we only use the RGB channel of a image.
	unsigned char* image_data = SOIL_load_image(image_filename.c_str(), &image_width, &image_height, &channels, SOIL_LOAD_RGBA);
	//unsigned char* image_data = SOIL_load_image(image_filename.c_str(), &image_width, &image_height, 0, SOIL_LOAD_RGB);
	
	if (image_data == nullptr){
		cout << std::endl << std::endl << "Error: failed to load image file:" << image_filename << std::endl << std::endl;
		assert((image_data != nullptr) && "Error: failed to load image file!");
	}
	else{
		//int mode = GL_RGBA;
		//int mode = GL_RGB;
		

		GLuint temp_texture = 0;
		glGenTextures(1, &temp_texture);
		glBindTexture(GL_TEXTURE_2D, temp_texture);
		
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image_data);

		return temp_texture;
	}
}
