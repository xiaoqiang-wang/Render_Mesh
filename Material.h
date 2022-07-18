#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#pragma once

#include "common.h"

class Material{
public:
	// Material name
	std::string name;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 transmittance;
	glm::vec3 emission;
	float shininess;
	float ior;      // index of refraction
	float dissolve; // 1 == opaque; 0 == fully transparent, some times called Tr
	// illumination model (see http://www.fileformat.info/format/material/)
	int illum;

	//Texture file names

	// Ambient Texture Map
	std::string ambientTexName;

	// Diffuse Texture Map
	std::string diffuseTexName;

	// Specular Texture Map
	std::string specularTexName;

	// Specular Hightlight Map
	std::string specularHightlightTexName;

	// Alpha Texture Map
	std::string AlphaTexName;

	// Bump Map
	std::string BumpMapTexName;

	//general texture?
	std::string normalTexName;
};


#endif // _MATERIAL_H_
