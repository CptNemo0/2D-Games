#pragma once

#include <glm/glm.hpp>
#include "GameObject.h"
#include "memory"

using namespace std;
using namespace glm;

class MovementController
{
public:
	vec2 target_speed;
	float max_speed;
	float smoothing_factor;
	shared_ptr<GameObject> player;
	
	MovementController() = default;
	MovementController(const vec2& new_ts, const float& new_ms, float new_sf, shared_ptr<GameObject> new_player);

	void Move();
	void HandleInputs(bool wsad);
};

