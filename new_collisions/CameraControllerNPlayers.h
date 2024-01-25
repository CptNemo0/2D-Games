#pragma once
#include "GameObject.h"
#include "Camera.h"

class CameraControllerNPlayers
{
private:
	shared_ptr < Camera>camera;
	vector<shared_ptr<GameObject>> players;
	int focus;

public:
	CameraControllerNPlayers(shared_ptr <Camera>& new_camera, const vector<shared_ptr<GameObject>>& new_players);
	vector<int> Resize(int width_add, int height_add);
	void Move();
	vec2 ChangeFocus();
};

