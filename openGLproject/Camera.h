#pragma once
#include <gl\glm\glm.hpp>
#include<gl\glm\gtx\transform.hpp>


enum class PROJECTION_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE
};
class Camera

{
protected:
	//view
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 look;
	//glm::vec3 right;
	glm::mat4 view_mat;
	bool is_view_dirty;
	//proj
	float z_near;
	float z_far;
	float aspect_ratio;
	//orthographic
	float height;
	//perspectvie
	float fov;
	glm::mat4 proj_mat;
	bool is_proj_dirty;
	PROJECTION_TYPE projection_type;
public:
	Camera(PROJECTION_TYPE _projection_type, float _aspect_ratio);
	Camera(PROJECTION_TYPE _projection_type, glm::vec2 resolution);
	Camera::Camera(const Camera &Camera2);
	void Init(PROJECTION_TYPE _projection_type, float _aspect_ratio);
	~Camera();
	glm::mat4 virtual get_view();
	glm::mat4 virtual get_proj();
	//void virtual yaw(float value);
	//void virtual pitch(float value);
	//void virtual roll(float value);
	void set_position(glm::vec3 _position);
	void set_aspect_ratio(float aspect_ratio);
	void set_fov(float fov);
	void lookAt(glm::vec3 _position);
	glm::vec3 get_position();
	glm::vec3 get_up();
};
