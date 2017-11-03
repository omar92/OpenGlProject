#include "Camera.h"
Camera::Camera(PROJECTION_TYPE _projection_type, float _aspect_ratio)
{
	Init(projection_type, _aspect_ratio);
}
//Camera::Camera(const Camera &Camera2) {
//	position = Camera2.position;
//	up = Camera2.up;
//	look = Camera2.look;

//	z_near = Camera2.z_near;
//	z_far = Camera2.z_far;
//	aspect_ratio = Camera2.aspect_ratio;

//	fov = Camera2.fov;

//	projection_type = Camera2.projection_type;

//	is_view_dirty = true;
//	is_proj_dirty = true;
//}

Camera::Camera(PROJECTION_TYPE _projection_type, glm::vec2 resolution)
{
	Init(_projection_type, (resolution.x / resolution.y));
}
void Camera::Init(PROJECTION_TYPE _projection_type, float _aspect_ratio) {
	position = glm::vec3(0, 0, 0);
	up = glm::vec3(0.0, 1.0, 0.0);
	look = glm::vec3(0, 0, -1);

	z_near = 1.0f;
	z_far = 30.0f;
	aspect_ratio = _aspect_ratio;

	fov = 60.0f;

	projection_type = _projection_type;

	is_view_dirty = true;
	is_proj_dirty = true;
}
Camera::~Camera()
{
}

glm::mat4 Camera::get_view()
{
	if (is_view_dirty) {
		view_mat = glm::lookAt(position, look, up);
		is_view_dirty = false;
	}
	return view_mat;
}
//w    8
//1 h  9

glm::mat4 Camera::get_proj()
{
	if (is_proj_dirty) {
		switch (projection_type)
		{
		case PROJECTION_TYPE::PERSPECTIVE:
			proj_mat = glm::perspectiveFov(fov, aspect_ratio, 1.0f, z_near, z_far);
			break;
		case PROJECTION_TYPE::ORTHOGRAPHIC:
			proj_mat = glm::ortho(-1 * (fov / 10)*(aspect_ratio), 1 * (fov / 10)*(aspect_ratio), -1 * (fov / 10), 1 * (fov / 10), z_near, z_far);
		default:
			//proj_mat = glm::perspectiveFov(fov, aspect_ratio, 1.0f, z_near, z_far);
			break;
		}
		is_proj_dirty = false;
	}
	return proj_mat;
}

void Camera::set_position(glm::vec3 _position)
{
	position = _position;
	is_view_dirty = true;
}
void Camera::lookAt(glm::vec3 _position)
{
	look = _position;
	is_view_dirty = true;
}

void Camera::set_aspect_ratio(float aspect_ratio)
{
	this->aspect_ratio = aspect_ratio;
	is_proj_dirty = true;
}
void Camera::set_fov(float fov)
{
	this->fov = fov;
	is_proj_dirty = true;
}

glm::vec3 Camera::get_position()
{
	return position;
}
glm::vec3 Camera::get_up()
{
	return up;
}
