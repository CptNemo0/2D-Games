#include "CameraController.h"

void CameraController::UpdateSign()
{
	if (player->speed.x > 0)
	{
		sign = sign * (1.0f - camera_focus_smooth) + camera_focus_smooth * 1.0f;
	}
	else if (player->speed.x < 0)
	{
		sign = sign * (1.0f - camera_focus_smooth) + camera_focus_smooth * -1.0f;
	}

	x_offset = screen_half_x + sign * screen_sector_x;
}

void CameraController::UpdatePosition()
{
	glm::vec2 current_position = camera->position;
	glm::vec2 desired_position = glm::vec2(player->position.x - x_offset, camera->position.y);
	glm::vec2 new_position = desired_position * camera_follow_smooth + current_position * (1 - camera_follow_smooth);  
	camera->position = new_position;
}

CameraController::CameraController(shared_ptr<Camera> new_camera, shared_ptr<GameObject> new_player, float smooth_1, float smooth_2, int sector)
{
	this->camera = new_camera;
	this->player = new_player;

	this->camera_follow_smooth = smooth_1;
	this->camera_focus_smooth = smooth_2;

	this->x_offset = 0;

	this->screen_half_x = new_camera->size.x / 2;
	this->screen_sector_x = new_camera->size.x / -sector;

	this->sign = 1;
}

void CameraController::Move()
{
	UpdateSign();
	UpdatePosition();


	if (camera->position.x < 0)
	{
		camera->position.x = 0;
	}
	if (camera->position.y < 0)
	{
		camera->position.y = 0;
	}
	if (camera->position.x > LEVEL_WIDTH - camera->size.x)
	{
		camera->position.x = LEVEL_WIDTH - camera->size.x;
	}
	if (camera->position.y > LEVEL_HEIGHT - camera->size.y)
	{
		camera->position.y = LEVEL_HEIGHT - camera->size.y;
	}
}
