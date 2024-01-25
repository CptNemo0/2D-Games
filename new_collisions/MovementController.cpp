#include "MovementController.h"

MovementController::MovementController(const vec2& new_ts, const float& new_ms, float new_sf, shared_ptr<GameObject> new_player)
{
	this->smoothing_factor = new_sf;
	this->player = new_player;
	this->target_speed = new_ts;
	this->max_speed = new_ms;
}

void MovementController::Move()
{
	if (target_speed.x != 0 && target_speed.y != 0)
	{
		
	}
	//normalize(target_speed);
	//this->target_speed = target_speed * max_speed;
	float pspeed = player->speed.x;
	float lerp = target_speed.x * smoothing_factor + player->speed.x * (1 - smoothing_factor);
	
	player->speed = vec2(lerp, player->speed.y);

	//player->UpdatePosition();

	target_speed = vec2(0.0f, 0.0f);
}

void MovementController::HandleInputs(bool wsad)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	vec2 vector;

	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		vector = vec2(0, -max_speed);
		this->target_speed = target_speed + (vector);
	}

	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		vector = vec2(-max_speed, 0.0f);
		this->target_speed = target_speed + (vector);
	}

	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		vector = vec2(max_speed, 0.0f);
		this->target_speed = target_speed + (vector);
	}

	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		vector = vec2(0.0f, max_speed);
		this->target_speed = target_speed + (vector);
	}
}
