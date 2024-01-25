#include "CameraControllerNPlayers.h"

CameraControllerNPlayers::CameraControllerNPlayers(shared_ptr <Camera>& new_camera, const vector<shared_ptr<GameObject>>& new_players)
{
	this->camera = new_camera;
	this->players = new_players;
	info(players.size());
}

vector<int> CameraControllerNPlayers::Resize(int width_add, int height_add)
{
	constexpr float a = (VIEWPORT_WIDTH / 3 -  VIEWPORT_WIDTH/2) / (1000.0f - 100.0f);
	constexpr float b = VIEWPORT_WIDTH / 3 - a * 1000.0f;

	vector<int> return_value = vector<int>();

	return_value.push_back(camera->size.x);
	return_value.push_back(camera->size.y);

	float my_distance = distance(players[0]->position + 40.0f, players[1]->position - 40.0f);

	if (my_distance < 100)
	{
		my_distance == 100;
	}

	if (my_distance > 1000)
	{
		my_distance == 1000;
	}

	float multiplier = a * my_distance + b;

	float bigger_x, smaller_x, bigger_y, smaller_y;

	if (players[0]->position.x > players[1]->position.x)
	{
		bigger_x = players[0]->position.x;
		smaller_x = players[1]->position.x;
	}
	else
	{
		bigger_x = players[1]->position.x;
		smaller_x = players[0]->position.x;
	}

	if (players[0]->position.y > players[1]->position.y)
	{
		bigger_y = players[0]->position.y;
		smaller_y = players[1]->position.y;
	}
	else
	{
		bigger_y = players[1]->position.y;
		smaller_y = players[0]->position.y;
	}

	int new_width = bigger_x - smaller_x + 2 * multiplier;
	int new_height = bigger_y - smaller_y + 2 * multiplier;

	return_value[0] = new_width;
	return_value[1] = new_height;

	return return_value;
}

void CameraControllerNPlayers::Move()
{
	vec2 target_position = (players[1]->position - players[0]->position) / 2.0f;
	target_position += players[0]->position;
	
	target_position -= (camera->size * 0.5f);

	/*for (auto& player : this->players)
	{
		target_position = target_position + player->position;
	}*/
	//target_position = target_position * (1.0f / (float)players.size());
	//target_position -= (camera->size * 0.5f);

	vec2 new_position = target_position * 0.01f + camera->position * (1 - 0.01f);

	camera->position = target_position;
}

vec2 CameraControllerNPlayers::ChangeFocus()
{
	return vec2();
}
