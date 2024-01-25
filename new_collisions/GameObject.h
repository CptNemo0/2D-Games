#pragma once
#include "constants.hpp"
#include "glm/glm.hpp"
#include <memory>
#include <SDL.h>
#include "spdlog/spdlog.h"
#include "sstream"
#include "string"
#include "Camera.h"

using namespace std;
using namespace glm;
using namespace spdlog;

class GameObject
{


public:
	SDL_Texture* texture;
	shared_ptr <SDL_Rect> draw_rect;
	vec2 position;
	vec2 speed;
	bool is_circle;
	bool movable;
	float radius;

	GameObject(float x, float y, SDL_Texture* new_texture, bool is_it_circle);
	GameObject(float x, float y, SDL_Texture* new_texture, int resize);

	void UpdatePosition();
	void SetPosition(const vec2& new_position);

	vector<float> GetCollider();

	int Render(SDL_Renderer* renderer, shared_ptr<Camera> camera);
};



