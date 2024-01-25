#include "GameObject.h"

GameObject::GameObject(float x, float y, SDL_Texture* new_texture, bool is_it_circle)
{
	this->position = vec2(x, y);
	this->speed = vec2(0.0f, 0.0f);
	this->draw_rect = make_shared<SDL_Rect>();
	this->draw_rect->x = x - TILE_WIDTH * 0.5f;
	this->draw_rect->y = y - TILE_HEIGHT * 0.5f;
	this->draw_rect->w = TILE_WIDTH;
	this->draw_rect->h = TILE_HEIGHT;
	this->texture = new_texture;
	this->is_circle = is_it_circle;
	this->radius = RADIUS;
	this->movable = true;
}

GameObject::GameObject(float x, float y, SDL_Texture* new_texture, int resize)
{
	this->position = vec2(x, y);
	this->speed = vec2(0.0f, 0.0f);
	this->draw_rect = make_shared<SDL_Rect>();
	this->draw_rect->x = x - resize * 0.5f;
	this->draw_rect->y = y - resize * 0.5f;
	this->draw_rect->w = resize;
	this->draw_rect->h = resize;
	this->texture = new_texture;
	this->is_circle = false;
	this->radius = resize;
	this->movable = true;
}

void GameObject::UpdatePosition()
{
	SetPosition(position + speed);
}



void GameObject::SetPosition(const vec2& new_position)
{
	if (movable)
	{
		vec2 old_position = position;
		this->position = new_position;

		/*if (position.x < 0)
		{
			position.x += -position.x;
		}
		else if (position.x > LEVEL_WIDTH)
		{
			int a = position.x - LEVEL_WIDTH;
			position.x -= a;
		}

		if (position.y < 0)
		{
			position.y += -position.y;
		}
		else if (position.y > LEVEL_HEIGHT)
		{
			int a = position.y - LEVEL_HEIGHT;
			position.y -= a;
		}*/

		this->draw_rect->x = position.x - TILE_WIDTH * 0.5f;
		this->draw_rect->y = position.y - TILE_HEIGHT * 0.5f;
	}
}

vector<float> GameObject::GetCollider()
{
	if (!is_circle)
	{
		return { (float)draw_rect->x, (float)draw_rect->y, (float)draw_rect->w, (float)draw_rect->h };
	}
	else
	{
		return { radius };
	}
}

int GameObject::Render(SDL_Renderer* renderer, shared_ptr<Camera> camera)
{
    int retValue = -1;
	
	if (camera != NULL)
	{
		this->draw_rect->x -= camera->position.x;
		this->draw_rect->y -= camera->position.y;
	}

	if (!SDL_RenderCopy(renderer, texture, NULL, draw_rect.get()))
	{
		retValue = 0;
	}

    return retValue;
}