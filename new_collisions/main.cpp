#include <SDL.h>
#include "GameObject.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include "utility.h"
#include "constants.hpp"
#include <iostream>
#include "MovementController.h"
#include "CameraController.h"
#include "Camera.h"
#include <chrono>
#include <thread>
#include <random>

using namespace std;
using namespace spdlog;
using namespace glm;

float get_jump_vx(float jump_distance, float frames_to_reach_max_height)
{
	return jump_distance * 0.5f / frames_to_reach_max_height;
}

float get_v_0(float max_height, float jump_vx, float jump_distance)
{
	return -2.0f * max_height * jump_vx / jump_distance;
}

float get_gravity(float max_height, float jump_vx, float jump_distance)
{
	return 2.0f * max_height * (jump_vx * jump_vx) / (jump_distance * jump_distance * 0.25f);
}

int main(int argc, char* argv[])		
{
#pragma region Variables

	// Window
	SDL_Window* window = NULL;

	// Surface
	SDL_Surface* screenSurface = NULL;

	// Renderer
	SDL_Renderer* renderer = NULL;

	auto player_texture_paths = vector<string>();
	auto player_texture_objs = vector<SDL_Texture*>();

	auto level_texture_paths = vector<string>();
	auto level_texture_objs = vector<SDL_Texture*>();

	auto level_tiles = vector<shared_ptr<GameObject>>();
	auto level_colliders = vector<shared_ptr<GameObject>>();

	auto placeholder_colliders = vector<shared_ptr<GameObject>>();

	auto layer_1_tiles = vector<shared_ptr<GameObject>>();
	auto layer_2_tiles = vector<shared_ptr<GameObject>>();
	auto layer_3_tiles = vector<shared_ptr<GameObject>>();

	float layer_1_speed = 4;
	float layer_2_speed = 4;
	float layer_3_speed = 4;
#pragma endregion
	
#pragma region Initialization

	srand(time(NULL));

	if (!Init(window, renderer, screenSurface, VIEWPORT_WIDTH, VIEWPORT_HEIGHT))
	{
		return 1;
	}
	spdlog::info("Init");

	if (!GetTexturePathsFromFile(PLAYER_TEXTURE_FILE_PATH, player_texture_paths))
	{
		return 1;
	}
	spdlog::info("Texture paths extracted");

	if (!GetTexturePathsFromFile(LEVEL_TEXTURE_FILE_PATH, level_texture_paths))
	{
		return 1;
	}
	spdlog::info("Texture paths extracted");


	if (!LoadTextures(player_texture_objs, player_texture_paths, renderer))
	{
		return 1;
	}
	spdlog::info("Player textures loaded");

	if (!LoadTextures(level_texture_objs, level_texture_paths, renderer))
	{
		return 1;
	}
	spdlog::info("Level textures loaded");

	vector<glm::vec2> bounds = GetLevelFromFile("level.csv", level_tiles, level_texture_objs, level_colliders);
	auto placeholder_layer_1 = GetLevelFromFile("layer_1.csv", layer_1_tiles, level_texture_objs, placeholder_colliders);
	auto placeholder_layer_2 = GetLevelFromFile("layer_2.csv", layer_2_tiles, level_texture_objs, placeholder_colliders);
	auto placeholder_layer_3 = GetLevelFromFile("layer_3.csv", layer_3_tiles, level_texture_objs, placeholder_colliders);

#pragma endregion

#pragma region Actors
	auto player_1 = make_shared<GameObject>(100, 600, player_texture_objs[0], true);

	auto mc_1 = MovementController(vec2(0.0f, 0.0f), MAX_SPEED, 0.7, player_1);

	auto camera = make_shared<Camera>(vec2(0.0f, 0.0f), vec2( VIEWPORT_WIDTH, VIEWPORT_HEIGHT));
	auto camera_controller = CameraController(camera, player_1, 0.2f, 0.01f, 3.0f);
#pragma endregion

	int previous_round = 1;
	int round_ctr = 1;
	SDL_Event e;
	bool quit = false;
	float sign = 1;
	int frame = 0;

	bool jumping = false;
	bool can_jump = false;
	int jump_frames = 0;
	float frames_to_reach_max_height = 45.0f;
	int jump_limit = 2.0f * frames_to_reach_max_height;
	float jump_distance = TILE_WIDTH * 5.0f;
	float max_height = TILE_HEIGHT * 5.0f;
	float jump_vx = get_jump_vx(jump_distance, jump_limit);
	float initial_jump_velocity = get_v_0(max_height, jump_vx, jump_distance);
	float gravity_multiplier = get_gravity(max_height, jump_vx, jump_distance);
	
	int jump_speed_sign = 0;
	vec2 start_jump_position{ 0.0f, 0.0f };
	vec2 jump_position{ 0.0f, 0.0f };
	vec2 jump_velocity{ 0.0f, 0.0f };
	vec2 gravity{ 0.0f, 1.0f };
	int NOW = SDL_GetPerformanceCounter();
	int LAST = 0;
	double deltaTime = 0;
	float faster_fall = 1.0f;
	float higher_jump = 0.5f;
	int space_ctr = 0;
	

	while (!quit)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

		PollEvents(quit, e);

		mc_1.HandleInputs(true);
		mc_1.Move();
		
		//if (jumping)
		//{
		//	
		//	if (jump_frames == 0)
		//	{
		//		
		//		if (player_1->speed.x > 0.0001f)
		//		{
		//			jump_speed_sign = 1.0f;
		//		}

		//		else if (player_1->speed.x < -0.0001f)
		//		{
		//			jump_speed_sign = -1.0f;
		//		}

		//		else 
		//		{
		//			jump_speed_sign = 0;
		//		}
		//		jump_velocity = { jump_vx * jump_speed_sign, initial_jump_velocity };
		//		jump_position = player_1->position;
		//	}

		//	if (jump_frames > frames_to_reach_max_height)
		//	{
		//		faster_fall = 10.0f;
		//	}
		//	else
		//	{
		//		faster_fall = 1.0f;
		//	}

		//	/*if (space_ctr > frames_to_reach_max_height * 0.5)
		//	{
		//		higher_jump = 2.0f;
		//	}
		//	else
		//	{
		//		higher_jump = 1.0f;
		//	}*/

		//	player_1->speed = vec2{ 0.0f, 0.0f };
		//	jump_position += jump_velocity * higher_jump + 0.5f * gravity * gravity_multiplier * faster_fall;
		//	jump_velocity += gravity * gravity_multiplier;
		//	
		//	cout << "jump_velocity.x: " << jump_velocity.x << " jump_velocity.y: " << jump_velocity.y << endl;
		//	//cout << "jump_position.: " << jump_position.x << "jump_position.y" << jump_position.y << endl;

		//	player_1->speed = (jump_position - player_1->position);
		//	
		//	jump_frames++;

		//	if (jump_frames == jump_limit - 1)
		//	{
		//		jumping = false;
		//		jump_frames = 0;
		//		jump_velocity = { 0.0f, 0.0f };
		//		jump_position = { 0.0f, 0.0f };
		//	}
		//}
		//else
		//{
		//	player_1->speed.y += gravity_multiplier;
		//}
		player_1->UpdatePosition();
		
		/*for (auto& tile : layer_3_tiles)
		{
			tile->UpdatePosition();
		}

		for (auto& tile : layer_2_tiles)
		{
			tile->UpdatePosition();
		}

		for (auto& tile : layer_1_tiles)
		{
			tile->UpdatePosition();
		}*/

		vec2 gravity_direction = vec2(1.0f, 1.0f);
		for (int i = 0; i < level_colliders.size(); i++)
		{
			level_colliders[i]->UpdatePosition();
			vec2 tmp = SeparateCircleRectangle(player_1, level_colliders[i]);

			if (tmp == vec2(1.0f, 0.0f))
			{
				jumping = false;
				jump_frames = 0;
				jump_velocity = { 0.0f, 0.0f };
				jump_position = { 0.0f, 0.0f };
				tmp = vec2(0.0f, 0.0f);
			}

			if (gravity_direction != vec2(0.0f, 0.0f))
			{
				gravity_direction = tmp;
				
			}
		}

		can_jump = false;
		if (gravity_direction == vec2(0.0f, 0.0f))
		{
			can_jump = true;
			space_ctr = 0;
			player_1->speed.y = 0;
		}

		camera_controller.Move();
		
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	
		if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			
			for (int i = 0; i < layer_3_tiles.size(); i++)
			{
				auto new_position = layer_3_tiles[i]->position + glm::vec2(layer_3_speed, 0.0f);
				layer_3_tiles[i]->SetPosition(new_position);
			}
			for (int i = 0; i < layer_2_tiles.size(); i++)
			{
				auto new_position = layer_2_tiles[i]->position + glm::vec2(layer_2_speed, 0.0f);
				layer_2_tiles[i]->SetPosition(new_position);
			}
			for (int i = 0; i < layer_1_tiles.size(); i++)
			{
				auto new_position = layer_1_tiles[i]->position + glm::vec2(layer_1_speed, 0.0f);
				layer_1_tiles[i]->SetPosition(new_position);
			}
				
			/*for (auto& tile : layer_2_tiles)
			{

				tile->SetPosition(tile->position + glm::vec2(layer_2_speed, 0.0f));
			}

			for (auto& tile : layer_1_tiles)
			{
				tile->SetPosition(tile->position + glm::vec2(layer_1_speed, 0.0f));
			}*/
		}

		if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			for (int i = 0; i < layer_3_tiles.size(); i++)
			{
				auto new_position = layer_3_tiles[i]->position + glm::vec2(-layer_3_speed, 0.0f);
				layer_3_tiles[i]->SetPosition(new_position);
			}
			for (int i = 0; i < layer_2_tiles.size(); i++)
			{
				auto new_position = layer_2_tiles[i]->position + glm::vec2(-layer_2_speed, 0.0f);
				layer_2_tiles[i]->SetPosition(new_position);
			}
			for (int i = 0; i < layer_1_tiles.size(); i++)
			{
				auto new_position = layer_1_tiles[i]->position + glm::vec2(-layer_1_speed, 0.0f);
				layer_1_tiles[i]->SetPosition(new_position);
			}
		}


		if (currentKeyStates[SDL_SCANCODE_I])
		{
			jump_distance += 5.0f;
			jump_vx = get_jump_vx(jump_distance, jump_limit);
			initial_jump_velocity = get_v_0(max_height, jump_vx, jump_distance);
			gravity_multiplier = get_gravity(max_height, jump_vx, jump_distance);
			cout << "jump_distance: " << jump_distance << endl;
		}

		if (currentKeyStates[SDL_SCANCODE_K])
		{
			jump_distance -= 5.0f;
			jump_vx = get_jump_vx(jump_distance, jump_limit);
			initial_jump_velocity = get_v_0(max_height, jump_vx, jump_distance);
			gravity_multiplier = get_gravity(max_height, jump_vx, jump_distance);
			cout << "jump_distance: " << jump_distance << endl;
		}

		if (currentKeyStates[SDL_SCANCODE_O])
		{
			max_height += 5.0f;
			jump_vx = get_jump_vx(jump_distance, jump_limit);
			initial_jump_velocity = get_v_0(max_height, jump_vx, jump_distance);
			gravity_multiplier = get_gravity(max_height, jump_vx, jump_distance);
			cout << "max_height: " << max_height << endl;
		}

		if (currentKeyStates[SDL_SCANCODE_L])
		{
			max_height -= 5.0f;
			jump_vx = get_jump_vx(jump_distance, jump_limit);
			initial_jump_velocity = get_v_0(max_height, jump_vx, jump_distance);
			gravity_multiplier = get_gravity(max_height, jump_vx, jump_distance);
			cout << "max_height: " << max_height << endl;
		}
		
		// Background
		SDL_SetRenderDrawColor(renderer, WINDOW_R, WINDOW_G, WINDOW_B, 255);
		SDL_RenderClear(renderer);

		RenderLevel(layer_3_tiles, renderer, camera);
		RenderLevel(layer_2_tiles, renderer, camera);
		RenderLevel(layer_1_tiles, renderer, camera);

		RenderLevel(level_tiles, renderer, camera);
		player_1->Render(renderer, camera);

		//cout << space_ctr << endl;
		SDL_RenderPresent(renderer);
		SDL_Delay(FRAME_DELAY); 
	}

	Close(window, renderer);
	return 0;
}