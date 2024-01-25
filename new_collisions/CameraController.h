#pragma once

#include<memory>
#include "Camera.h"
#include <cmath>
#include "GameObject.h"

class CameraController
{
private:
	shared_ptr<Camera> camera;
	shared_ptr<GameObject> player;

	float camera_focus_smooth;
	float camera_follow_smooth;

	float x_offset;
	float screen_sector_x;
	float screen_half_x;

	float sign;

	void UpdateSign();
	void UpdatePosition();

public:
	CameraController(shared_ptr<Camera> new_camera, shared_ptr<GameObject> new_player, float smooth_1, float smooth_2, int sector);


	void Move();

};

