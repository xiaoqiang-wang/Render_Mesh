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





