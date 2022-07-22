// Draw_mesh.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//#define show_local 0






#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <iomanip>
//#include <math.h>

#include "Common.h"


#include "Mesh.h"


#include "Camera.h"



using namespace std;

//using glm::cross;




// Window dimensions
//const GLuint WIDTH = 2400, HEIGHT = 1176;

//const GLuint WIDTH = 1224, HEIGHT = 600;

//const GLuint WIDTH = 2160, HEIGHT = 1080;

//const GLuint WIDTH = 2368, HEIGHT = 1440;

// The MAIN function, from here we start the application and run the game loop



glm::uvec4 frame_info;

float scaling_value     = 3.0;               //缩放分量
glm::vec3 shift_value   = glm::vec3(0.0f);//三个坐标轴上的平移分量
glm::uvec3 rotate_value = glm::uvec3(90,0,0);//三个坐标轴上的旋转分量

//glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 33.0f);//相机位置
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//面对的方向,以这个为尺度更改观看的，其实就是单位速度
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//头顶的方向



//glm::vec3 cameraPos   = glm::vec3(0.0f, 18.0f, 40.0f); //相机位置
//glm::vec3 cameraFront = glm::vec3(0.0f, -2.0f, -10.0f);//面对的方向,以这个为尺度更改观看的，其实就是单位速度
//glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);   //头顶的方向


//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbViewVolume
//
//  Defines a 3D view volume. For perspective projection, the view
//  volume is a frustum. For orthographic (parallel) projection, the
//  view volume is a rectangular prism.
//
//////////////////////////////////////////////////////////////////////////////
class ViewVolume {
private:
	int value = 0;
public:
	// Default constructor
	ViewVolume() {};
	~ViewVolume() {}
};

class Box
{
private:

public:
	Box() {};
	~Box() {};
};

#if 0

class Camera{
	//reference:               https://blog.csdn.net/hobbit1988/article/details/7956838
	//see the page for detail  https://blog.csdn.net/hobbit1988/article/details/7956838
private:
	
	glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 0.0f);  //Location of viewpoint, view position.
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);  //面对的方向,以这个为尺度更改观看的，其实就是单位速度, we want it be normalized
	glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);  //头顶的方向, we want it be normalized
	



	glm::vec3 upDirection;
	glm::vec3 frontDirection;
	glm::vec3 rightDriection;

	glm::vec3 u, v, n;




	glm::vec3 position;   // Location of viewpoint

	float nearPlane = 0.01;   // Distance from viewpoint to view plane
	//float farPlane = std::numeric_limits<float>::max();
	float farPlane = std::numeric_limits<float>::infinity(); // Distance from viewpoint to far clipping plane

	float fov ;           //angle, filed of view, viewAngle. maybe 60^C will be OK.
	float aspect;         //Ratio of width to height of view plane,   width/height.
	float focalDistance;  //Distance from viewpoint to point of focus.




	float slide_speed = 1.0;
	float move_speed  = 1.0;


public:
	Camera();
	~Camera();
	
	void SetCameraPos(const glm::vec3 &Pos);
	glm::vec3 GetCameraPos();

	void SetCameraFront(const glm::vec3 &Front);
	glm::vec3 GetCameraFront();

	void SetCamerauUp(const glm::vec3 &Up);
	glm::vec3 GetCameraUp();



	// Sets the orientation of the camera so that it points toward the
 // given target point while keeping the "up" direction of the
 // camera parallel to the positive y-axis. If this is not
 // possible, it uses the positive z-axis as "up".
 // C-api: name=ptAt
	void        pointAt(const glm::vec3 &targetPoint);

	// Scales the height of the camera. This is a virtual function.
	// Perspective cameras will scale their 'heightAngle' field here, and ortho
	// cameras will scale their 'height' field.
	// C-api: expose
	virtual void    scaleHeight(float scaleFactor) = 0;

	// Fills in a view volume structure, based on the camera. If the
	// useAspectRatio field is not 0.0 (the default), the camera uses
	// that ratio instead of the one it has.
	// C-api: expose
	// C-api: name=getViewVol
	virtual ViewVolume getViewVolume(float useAspectRatio = 0.0) const = 0;

	void SetCamera(glm::vec3 Pos, glm::vec3 Front, glm::vec3 up);

	
	void roll(float angle);  //revolve around front direction
	void pitch(float angle); //revolve around right direction
	void yaw(float angle);   //revolve around up direction


	void slide(float du, float dv, float dn);

	//get distance from camera pos to orignal center.
	float getDist();
	void setSharp(float viewAngle, float aspect, float Near, float Far);


	void Reset();
	void Update();
};

Camera::Camera() {};
Camera::~Camera() {};

void Camera::SetCamera(glm::vec3 Pos, glm::vec3 Front, glm::vec3 up) {
	cameraPos   = Pos;
	cameraFront = Front;
	cameraUp    = up;



	upDirection    = glm::normalize(cameraUp - cameraPos);
	frontDirection = glm::normalize(cameraFront - cameraPos);
	rightDriection = glm::normalize(glm::cross(upDirection, frontDirection));


	/*
	glm::vec3 upVec = cameraUp - cameraPos;

	n = cameraPos - cameraFront;//FIXME: does this make sense?
	u = glm::cross(upVec, n);
	v = glm::cross(n,u);

	//u.normalize();
	u = glm::normalize(u);
	v = glm::normalize(v);
	n = glm::normalize(n);
	*/

}


float Camera::getDist() {
	return sqrt(pow(cameraPos.x, 2) + pow(cameraPos.y, 2) + pow(cameraPos.z, 2));
}

void Camera::slide(float du, float dv, float dn){

	/*
	cameraPos.x += (u.x* du + v.x*dv + n.x * dn);
	cameraPos.y += (u.y* du + v.y*dv + n.y * dn);
	cameraPos.z += (u.z* du + v.z*dv + n.z * dn);

	cameraFront.x += (u.x* du + v.x*dv + n.x * dn);
	cameraFront.y += (u.y* du + v.y*dv + n.y * dn);
	cameraFront.z += (u.z* du + v.z*dv + n.z * dn);
	*/

	cameraPos.x += du;
	cameraPos.y += dv;
	cameraPos.z += dn;
}


void Camera::roll(float angle) {
	float cs = cos(angle* M_PI / 180);
	float sn = sin(angle* M_PI / 180);
	
	// 先平移到圆点，再旋转，再平移。
	// 


	//glm::vec3 front = cameraFront;
	//glm::vec3 up    = cameraUp;

	//upDirection.x = upDirection.x + sn;
	//upDirection.y = upDirection.y - cs;
	//upDirection.z = upDirection.z - cs;

	//glm::vec3 right = cameraRight;

	
	glm::vec3 t = u;
	glm::vec3 s = v;

	u = glm::vec3((cs*t.x - sn * s.x), (cs*t.y - sn * s.y), (cs*t.z - sn * s.z));
	v = glm::vec3((sn*t.x + cs * s.x), (sn*t.y + cs * s.y), (sn*t.z + cs * s.z));
	
	//glm::roll(u,v);

	glm::vec4 direction = glm::vec4(cameraUp.x, cameraUp.y, cameraUp.z, 0.0);
	glm::mat4 mat = glm::mat4(1.0);


	//glm::vec3 upDirection    = glm::normalize(cameraUp - cameraPos);
	//glm::vec3 frontDirection = glm::normalize(cameraFront - cameraPos);
	//glm::vec3 rightDriection = glm::normalize(glm::cross(upDirection,frontDirection));
 }

void Camera::pitch(float angle) {

	float cs = cos(angle*M_PI / 180);
	float sn = sin(angle*M_PI / 180);

	/*
	glm::vec3 t = n;
	glm::vec3 s = u;

	u = glm::vec3((cs*t.x - sn * s.x), (cs*t.y - sn * s.y), (cs*t.z - sn * s.z));
	u = glm::vec3((sn*t.x + cs * s.x), (sn*t.y + cs * s.y), (sn*t.z + cs * s.z));
	*/
}

void Camera::yaw(float angle) {
	float cs = cos(angle*M_PI / 180);
	float sn = sin(angle*M_PI / 180);

	/*
	glm::vec3 t = v;
	glm::vec3 s = n;

	v = glm::vec3((cs*t.x - sn * s.x), (cs*t.y - sn * s.y), (cs*t.z - sn * s.z));
	n = glm::vec3((sn*t.x + cs * s.x), (sn*t.y + cs * s.y), (sn*t.z + cs * s.z));
	*/
}





void Camera::Reset() {

	cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);  //相机位置
	cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);  //面对的方向,以这个为尺度更改观看的，其实就是单位速度
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);  //头顶的方向
	fov = 60;
	float nearPlane = 0.01;
	float farPlane = std::numeric_limits<float>::max();


}


void Camera::Update() {
	//
	int key = 0;
	//float deltaTime = 1.0;
	float cameraSpeed = 1.0f;
	switch (key)
	{
	case GLFW_KEY_W:
		cameraPos += cameraSpeed * cameraFront;
		break;
	case GLFW_KEY_S:
		cameraPos -= cameraSpeed * cameraFront;
		break;

	case GLFW_KEY_A:
		//glm::normalize(glm::cross(cameraFront, cameraUp))这个求的是标准化的右向量
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case GLFW_KEY_D:
		//cout << "key:" << "D" << endl;
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;

	default:
		break;
	}

	
	//mouse turn left-right
	if (true) {

		float mouse_shift_distance = 0;

		float theta = mouse_shift_distance / 2 * M_PI;

		float x = cos(theta);
		float y = sin(theta);

		//float theta = (2 * (3.151592)) / distance;

		glm::vec3 new_front(x, y, cameraFront.z);
		cameraFront = new_front;
	}


	//mouse turn up-down
	if (true) {
		float mouse_shift_distance = 0;

		float theta = mouse_shift_distance / 2 * M_PI;

		//we do not want view up-side down.
		if (theta> (M_PI/2) ){
			theta = M_PI / 2;
		}

		if (theta > -(M_PI / 2)) {
			theta = -(M_PI / 2);
		}


		float x = cos(theta);
		float y = sin(theta);
		float z = cos(theta);

		//float theta = (2 * (3.151592)) / distance;

		glm::vec3 new_front(cameraFront.x, cameraFront.y, x);
		cameraUp = new_front;
	}
};







class PerspectiveCamera :public Camera {

private:
	// Fields (some are inherited from SoCamera)
	float heightAngle = M_PI_4;  //Angle (in radians) of field of view, in height direction pi/4, 45 degree

public:
	// Constructor
	PerspectiveCamera() {};
	~PerspectiveCamera() {};


	// Scales the height of the camera, in this case, the 'heightAngle' field.
	virtual void   scaleHeight(float scaleFactor);


	// Fills in a view volume structure, based on the camera. If the
	// useAspectRatio field is not 0.0 (the default), the camera uses
	// that ratio instead of the one it has.
	virtual ViewVolume getViewVolume(float useAspectRatio = 0.0) const;
};










void PerspectiveCamera::scaleHeight(float scaleFactor) {
	if (scaleFactor == 0.0) //FIXME: why we need to skip the 0?
		return;
	heightAngle = scaleFactor * heightAngle;
}


class OrthographicCamera : public Camera {
private:
	float       height = 2.0;

public:
	OrthographicCamera();
	~OrthographicCamera();

	// Scales the height of the camera, in this case, the 'heightAngle' field.
	virtual void   scaleHeight(float scaleFactor);

	// Fills in a view volume structure, based on the camera. If the
   // useAspectRatio field is not 0.0 (the default), the camera uses
   // that ratio instead of the one it has.
	virtual ViewVolume getViewVolume(float useAspectRatio = 0.0) const;

	// Positions camera to view passed bounding box with given aspect
	// ratio and current height
	virtual void    viewBoundingBox(const Box &box, float aspect, float slack);
};

class Projection {
private:
	float fov     = 60;
	
	uint32_t type;

public:
	Projection() {};
	~Projection() {};
};




#endif


// PerspectiveCamera Perspective_Camera;




//generate a scaling matrix,
glm::mat4 gen_scaling_maxtrix(float Sx, float Sy, float Sz) {
	glm::mat4 res = glm::mat4(1.0);

	res[0][0] = Sx;
	res[1][1] = Sy;
	res[2][2] = Sz;

	return res;
}


//generate a rotate matrix, which axis, and how much radian
glm::mat4 gen_rotate_matrix(uint32_t axis, float theta) {
	glm::mat4 res = glm::mat4(1.0);

	//X
	if (axis == 0) {
		res[1][1] = cos(theta);
		res[1][2] = -sin(theta);

		res[2][1] = sin(theta);
		res[2][2] = cos(theta);
	}

	//Y
	if (axis == 1) {
		res[0][0] = cos(theta);
		res[0][2] = sin(theta);

		res[2][0] = -sin(theta);
		res[2][2] = cos(theta);
	}

	//Z
	if (axis == 2) {
		res[0][0] = cos(theta);
		res[0][1] = -sin(theta);

		res[1][0] = sin(theta);
		res[1][1] = cos(theta);
	}

	return res;
}


//generate a translate_matrix
glm::mat4 gen_translation_matrix(float Tx, float Ty, float Tz) {
	glm::mat4 res = glm::mat4(1.0);

	res[0][3] = Tx;
	res[1][3] = Ty;
	res[2][3] = Tz;

	return res;
}




//glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 0.0f);            // 相机位置
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -10.6429f);         // 相机位置
glm::vec3 cameraPos = glm::vec3(0.155289, -0.14855, -0.460349);   // 相机位置

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);   // 面对的方向,以这个为尺度更改观看的
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);   // 头顶的方向
//glm::vec3 cameraRight = glm::vec3(0.0);

float CameraMoveSpeed = 1.0f;
float ViewMoveSpeed   = 0.05f;

glm::vec3 cameraPos_backup;
glm::vec3 cameraFront_backup;
glm::vec3 cameraUp_backup;
bool is_default_camera = true;







float deltaTime = 0.0f;     //这一帧消耗的时间
float lastFrameTime = 0.0f; //上一帧结束的时间

bool to_save_screen = false;

float lastX = 0;
float lastY = 0;

float initX = 0;
float initY = 0;



float yaw = 0.0;
float pitch = 0.0;

float fov = 60.0f;//视野比例
//float aspectRatio = float(WIDTH) / float(HEIGHT);
float aspectRatio = 1.02266;

float nearClipPlane = 0.1f;
//float nearClipPlane = 0.1f;
//float farClipPlane = std::numeric_limits<float>::max();
float farClipPlane = 100000000.0f;

//how many deta need to change for press a key.
float Key_Delta = 1.0;


//to save keys events.
bool keys[1024];






//键盘的回调函数to change a 3D model
/*
*  @param[in] window The window that received the event.
*  @param[in] key The[keyboard key](@ref keys) that was pressed or released.
*  @param[in] scancode The system - specific scancode of the key.
*  @param[in] action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.  Future releases may add more actions.
*  @param[in] mods Bit field describing which[modifier keys](@ref mods) were held down.
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	std::cout << "key_press : " << key << std::endl;

	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
	//	glfwSetWindowShouldClose(window, GL_TRUE);
	//}
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_W:
			shift_value.y += (0.001*Key_Delta);
			break;

		case GLFW_KEY_S:
			shift_value.y -= (0.001*Key_Delta);
			break;

		case GLFW_KEY_A:
			shift_value.x += (0.001*Key_Delta);
			break;

		case GLFW_KEY_D:
			shift_value.x -= (0.001*Key_Delta);
			break;

		case GLFW_KEY_PAGE_UP:
			Key_Delta += 1.0;
			break;

		case GLFW_KEY_PAGE_DOWN:
			Key_Delta -= 1.0;
			if (Key_Delta < 1.0)
			{
				Key_Delta = 1.0;//delta never less than 1.0
			}
			break;
		default:
			break;
		}
	}

	////上键
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//{
	//	mesh_centrality += 0.01f;
	//}
	////下键
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//{
	//	mesh_centrality -= 0.01f;
	//}
}


//to change camera pos.
void key_callback_1(GLFWwindow* window, int key, int scancode, int action, int mode){
	//return;

	//帧间隔长，就让它移动的多一些，这样能间接保证速度
	//float cameraSpeed = 2.5f * deltaTime;
	
	/*
	if ((action == GLFW_PRESS) || (action== GLFW_REPEAT))
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			cout << "key:" << "ESC" << endl;
			glfwSetWindowShouldClose(window, true);//关闭窗户
			break;

		case GLFW_KEY_W:
			cout << "key:" << "W" << endl;
			cameraPos += cameraSpeed * cameraFront;
			break;

		case GLFW_KEY_S:
			cout << "key:" << "S" << endl;
			cameraPos -= cameraSpeed * cameraFront;
			break;

		case GLFW_KEY_A:
			cout << "key:" << "A" << endl;
			//glm::normalize(glm::cross(cameraFront, cameraUp))这个求的是标准化的右向量
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;

		case GLFW_KEY_D:
			cout << "key:" << "D" << endl;
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;

		case GLFW_KEY_SPACE:
			cout << "key:" << "SPACE" << endl;
			//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;

		case GLFW_KEY_PAGE_UP:
			cout << "key:" << "PAGE_UP" << endl;
			Key_Delta += 1.0;
			break;

		case GLFW_KEY_PAGE_DOWN:
			cout << "key:" << "PAGE_DOWN" << endl;
			Key_Delta -= 1.0;
			if (Key_Delta < 1.0)
			{
				Key_Delta = 1.0;//delta never less than 1.0
			}
			break;
		default:
			cout << "key:" << key << endl;
			break;
		}
	}
	*/
	

	if (action == GLFW_PRESS) {
		keys[key] = true;

		
		if (key==GLFW_KEY_R) {
			cout << "key:" << "R, Reset view Pos, viewFront, View Up." << endl;

			if (is_default_camera) {
				
				cout << "Goto defualt Camera, Current Camera Info:"  << endl;
				cout << "\t cameraPos=" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << endl;
				cout << "\t cameraFront=" << "(" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << endl;
				cout << "\t cameraUp=" << "(" << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << ")" << endl;

				cameraPos_backup = cameraPos;
				cameraFront_backup = cameraFront;
				cameraUp_backup = cameraUp;

				cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
				cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
				cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

				is_default_camera = false;
			}
			else {
				cout << "Goto backup Camera:" << endl;
				cameraPos = cameraPos_backup;
				cameraFront = cameraFront_backup;
				cameraUp = cameraUp_backup;

				is_default_camera = true;
			}
		}

		if (key == GLFW_KEY_F){
			static bool cull_back_face = true;
			cout << "key:" << "F, CullFace." << endl;
			if (cull_back_face){
				cout << "\t CullFace : " << "GL_BACK" << endl;
				VERIFY_GL(glCullFace(GL_BACK));
				cull_back_face = false;
			}
			else{
				cout << "\t CullFace : " << "GL_FRONT" << endl;
				VERIFY_GL(glCullFace(GL_FRONT));
				cull_back_face = true;
			}
		}

		if (key == GLFW_KEY_PRINT_SCREEN) {
			cout << "key:" << "GLFW_KEY_PRINT_SCREEN" << endl;
			
			cout << "\t cameraPos=" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << endl;
			cout << "\t cameraFront=" << "(" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << endl;
			cout << "\t cameraUp=" << "(" << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << ")" << endl;
			to_save_screen = true;
		}

	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}



	/*
	//按下Esc键的意思
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		cout << "key:" << "ESC" << endl;
		glfwSetWindowShouldClose(window, true);//关闭窗户
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cout << "key:" << "W" << endl;
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cout << "key:" << "S" << endl;
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cout << "key:" << "A" << endl;
		//glm::normalize(glm::cross(cameraFront, cameraUp))这个求的是标准化的右向量
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cout << "key:" << "D" << endl;
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cout << "key:" << "SPACE" << endl;
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	
	*/

	
}




//The callback function receives the new classification of the cursor.
void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered) {
		// The cursor entered the content area of the window
		// last pos
	}
	else {
		// The cursor left the content area of the window
	}
}











bool firstMouse = true;
//鼠标的光标移动的回调函数
void mouse_cursor_posititon_callback(GLFWwindow *window, double xpos, double ypos) {
	 
	//return;
	//the cursor_position, it means on which pixel in the frame.
	std::cout << "xpos:" << xpos << ",\t ypos:" << ypos << std::endl;
	if (firstMouse) // 这个bool变量初始时是设定为true的
	{
		//lastX = WIDTH/2;
		//lastY = HEIGHT/2;

		lastX = xpos;
		lastY = ypos;
		initY = ypos;

		firstMouse = false;
		return;
	}
	//return;


	if (true){
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		//float sensitivity = 0.05;
		float sensitivity = ViewMoveSpeed;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}
	else{
		//we treat the camera direction as in 0.0 point.

		//float radius = 1.0;
		// glm::vec2 mouse_cursor_last_pos = glm::vec2(xpos, ypos);
		// 
		// float deleta_X = xpos - mouse_cursor_last_pos.x;
		// float deleta_Y = ypos - mouse_cursor_last_pos.y;


		float delta_X = xpos - lastX;
		float delta_Y = ypos - lastY;
		//float delta_Y = ypos - initY;
		
		lastX = xpos;
		lastY = ypos;


		// view direction only change on horizontal direction.
		// yaw
		if (delta_X){
			cout << "delta_X:" << delta_X << endl;
			float sensitivity = 0.01f;//灵敏度
			delta_X *= (sensitivity/(2*M_PI));

			
			glm::mat4 rotate_matrix = gen_rotate_matrix(1, delta_X);

			glm::vec4 front = glm::vec4(cameraFront.x, cameraFront.y, cameraFront.z, 0.0);
			glm::vec4 new_front = rotate_matrix * front;

			cameraFront = glm::normalize(glm::vec3(new_front.x, new_front.y, new_front.z));

			glm::vec3 cameraRight = glm::cross(cameraFront,cameraUp);
		}


		return;
		//view direction only change on vertical direction.
		//pitch
		if (delta_Y) {
			
			//delta_Y = ypos - initY;
			delta_Y = -delta_Y;// if Y++, lookup, else lookdown.

			cout << "delta_Y:" << delta_Y << endl;

			float total_delta_Y = (ypos - initY);
			if (abs(total_delta_Y)>720){
				lastY = initY + total_delta_Y;
				//initY = ypos - 720;
				return;
			}
		
			delta_Y *= 0.01f;
			//delta_Y = delta_Y > 89 ? 89 : delta_Y;
			//delta_Y = delta_Y < -89 ? -89 : delta_Y;


			cout << " delta_Y_1: " << delta_Y << endl;

			//float sensitivity = 1.0f;//灵敏度
			//delta_Y *= (sensitivity / (2 * M_PI));

			float radian_Y = (delta_Y / (2*M_PI));


			glm::mat4 rotate_matrix = gen_rotate_matrix(0, radian_Y);


			//new Front
			glm::vec4 front = glm::vec4(cameraFront.x, cameraFront.y, cameraFront.z, 0.0);
			glm::vec4 new_front = rotate_matrix * front;
			cameraFront = glm::normalize(glm::vec3(new_front.x, new_front.y, new_front.z));

			
			//get new updirection
			glm::vec4 up = glm::vec4(cameraUp.x, cameraUp.y, cameraUp.z, 0.0);
			glm::vec4 new_up = rotate_matrix * up;
			cameraUp = glm::normalize(glm::vec3(new_up.x, new_up.y, new_up.z));

			glm::vec3 cameraRight = glm::cross(cameraFront, cameraUp);
		}

	}

}

//float oldXPos = 0.0, oldYPos = 0.0;
//glfwGetCursorPos(window->getGLFWWindow(), &mouseX, &mouseY);





//鼠标滚轮的回调
void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {

	cout << " xoffset : " <<  xoffset << "	yoffset:" << yoffset << endl;

	if (fov >= 1.0f && fov <= 180.0f) {
		fov -= yoffset;

		fov = (fov <= 1.0f) ? 1.0f : fov;
		fov = (fov >= 180.0f) ? 180.0f : fov;
	}


	cout << "fov:" << fov << endl;
}

//鼠标按键的回调
void mouse_button_callback(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		cout << "key:" << "GLFW_MOUSE_BUTTON_LEFT" << endl;
	}

	if (glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			cout << "key:" << "GLFW_MOUSE_BUTTON_RIGHT" << endl;
	}
	if (glfwGetKey(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		cout << "key:" << "GLFW_MOUSE_BUTTON_MIDDLE" << endl;
	}
}

//窗口大小回调函数
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}





// Is called whenever a key is pressed/released via GLFW
void process_keyboard_input(GLFWwindow *window){
	
	//帧间隔长，就让它移动的多一些，这样能间接保证速度
	float cameraSpeed = CameraMoveSpeed * deltaTime;

	// 摄像机控制
	if (keys[GLFW_KEY_ESCAPE]) {
		std::cout << "key:" << "ESC" << endl;
		glfwSetWindowShouldClose(window, true);//关闭窗口
	}

	if (keys[GLFW_KEY_W]) {
		std::cout << "key:" << "W" << endl;
		
		
		std::cout << "\t current cameraPos:" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
		std::cout << "\t current cameraFront:" << "(" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << std::endl;

		cameraPos += cameraSpeed * cameraFront;//to get new pos
		std::cout << "\t new cameraPos:" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
	}
	
	if (keys[GLFW_KEY_S]) {
		std::cout << "key:" << "S" << std::endl;
	
		std::cout << "\t current cameraPos:" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
		std::cout << "\t current cameraFront:" << "(" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << std::endl;

		cameraPos -= cameraSpeed * cameraFront;//to get new pos
		std::cout << "\t new cameraPos:" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
	}

	if (keys[GLFW_KEY_A]){
		std::cout << "key:" << "A" << std::endl;
		

		std::cout << "\t current cameraPos:" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
		std::cout << "\t current cameraFront:" << "(" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << std::endl;

		
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;////to get new pos
		std::cout << "\t new cameraPos:" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
	}
		
	if (keys[GLFW_KEY_D]) {
		std::cout << "key:" << "D" << std::endl;
		

		std::cout << "\t current cameraPos:" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
		std::cout << "\t current cameraFront:" << "(" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << std::endl;


		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;////to get new pos
		std::cout << "\t new cameraPos:" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;

	}
	if (keys[GLFW_KEY_SPACE]) {
		cout << "key:" << "SPACE" << endl;
		cameraPos += cameraSpeed * glm::vec3(0.0f, CameraMoveSpeed, 0.0f);
	}
	if (keys[GLFW_KEY_LEFT_CONTROL]) {
		cout << "key:" << "SPACE" << endl;
		cameraPos -= cameraSpeed * glm::vec3(0.0f, CameraMoveSpeed, 0.0f);
	}

	if (keys[GLFW_KEY_UP]) {
		cout << "key:" << "UP" << endl;
		CameraMoveSpeed += 0.05;
		cout << "\t CameraMoveSpeed:" << CameraMoveSpeed << endl;
	}

	if(keys[GLFW_KEY_DOWN]) {
		cout << "key:" << "DOWN" << endl;
		CameraMoveSpeed -= 0.5;
		if (CameraMoveSpeed<0.0001){
			CameraMoveSpeed = 0.0001;
		}
		cout << "\t CameraMoveSpeed:" << CameraMoveSpeed << endl;
	}

	if(keys[GLFW_KEY_PAGE_UP]) {
		cout << "key:" << "PAGE_UP" << endl;
		ViewMoveSpeed += 0.0001;
		cout << "\t ViewMoveSpeed:" << ViewMoveSpeed << endl;
	}

	if(keys[GLFW_KEY_PAGE_DOWN]) {
		cout << "key:" << "PAGE_DOWN" << endl;
		ViewMoveSpeed -= 0.0001;
		if (ViewMoveSpeed < 0.0001) {
			ViewMoveSpeed = 0.0001;
		}
		cout << "\t ViewMoveSpeed:" << ViewMoveSpeed << endl;
	}

	if (keys[GLFW_KEY_P]) {
		cout << "key:" << "P_DOWN" << endl;

		std::cout << "DUMP status:" << endl;
		std::cout << "\t current cameraPos:" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
		std::cout << "\t current cameraFront:" << "(" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << std::endl;
		std::cout << "\t current cameraUp:" << "(" << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << ")" << std::endl;
		
		std::cout << "\t aspectRatio:" << aspectRatio << std::endl;
		std::cout << "\t flov:" << fov << std::endl;
	}

	if (keys[GLFW_KEY_KP_ADD]) {
		cout << "key:" << "+" << endl;

		aspectRatio += 0.001;
		std::cout << "new aspectRatio:" << aspectRatio << std::endl;
	}

	if (keys[GLFW_KEY_KP_SUBTRACT]) {
		cout << "key:" << "-" << endl;
		aspectRatio -= 0.001;
		std::cout << "new aspectRatio:" << aspectRatio << std::endl;
	}


	if (keys[GLFW_KEY_R]) {
		cout << "key:" << "R, Reset view Pos, viewFront, View Up." << endl;

		if (is_default_camera) {

			cout << "Goto defualt Camera, Current Camera Info:" << endl;
			cout << "\t cameraPos=" << "(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << endl;
			cout << "\t cameraFront=" << "(" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << endl;
			cout << "\t cameraUp=" << "(" << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << ")" << endl;

			cameraPos_backup = cameraPos;
			cameraFront_backup = cameraFront;
			cameraUp_backup = cameraUp;

			cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
			cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

			is_default_camera = false;
		}
		else {
			cout << "Goto backup Camera:" << endl;
			cameraPos = cameraPos_backup;
			cameraFront = cameraFront_backup;
			cameraUp = cameraUp_backup;

			is_default_camera = true;
		}
	}

	/*
	//按下esc键的意思
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		cout << "key:" << "ESC" << endl;
		glfwSetWindowShouldClose(window, true);//关闭窗户
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cout << "key:" << "W" << endl;
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cout << "key:" << "S" << endl;
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cout << "key:" << "A" << endl;
		//glm::normalize(glm::cross(cameraFront, cameraUp))这个求的是标准化的右向量
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cout << "key:" << "D" << endl;
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cout << "key:" << "SPACE" << endl;
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	*/
}





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





