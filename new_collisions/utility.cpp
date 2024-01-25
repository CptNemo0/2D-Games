#include "utility.h"

bool Init(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& screenSurface, int w, int h)
{
	bool retValue = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		spdlog::error("Unable to initialize SDL");
		spdlog::error(SDL_GetError());
		retValue = false;
	}

	//Create window
	window = SDL_CreateWindow("2dgk - Laboratorium 4 | Pawel Stus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		spdlog::error("Unable to create window");
		spdlog::error(SDL_GetError());
		retValue = false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		spdlog::error("Unable to initialize renderer");
		spdlog::error(SDL_GetError());
		retValue = false;
	}
	SDL_RenderSetScale(renderer, 1.0, 1.0);

	screenSurface = SDL_GetWindowSurface(window);
	if (screenSurface == NULL)
	{
		spdlog::error("Unable to get window surface");
		spdlog::error(SDL_GetError());
		retValue = false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		spdlog::error("Unable to initialize SDL_image");
		spdlog::error(IMG_GetError());
		retValue = false;
	}

	return retValue;
}

bool LoadTextures(std::vector<SDL_Texture*>& textures_obj, std::vector<std::string>& textures_paths, SDL_Renderer*& renderer)
{
	bool retValue = true;

	(void)SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int w = TILE_WIDTH;
	int h = TILE_HEIGHT;

	for (int i = 0; i < textures_paths.size(); i++)
	{
		textures_obj.push_back(NULL);
		textures_obj[i] = IMG_LoadTexture(renderer, textures_paths[i].c_str());

		if (textures_obj[i] == NULL)
		{
			spdlog::error(SDL_GetError());
			retValue = false;
			break;
		}
	}

	return retValue;
}

bool GetTexturePathsFromFile(std::string path, std::vector<std::string>& textures_paths)
{
	bool retValue = true;
	std::ifstream inputFile1(path);

	if (inputFile1.is_open())
	{
		std::string line;
		while (std::getline(inputFile1, line))
		{
			textures_paths.push_back(line);
		}
		inputFile1.close();
	}
	else
	{
		retValue = false;
		spdlog::error("Trouble oppening file!!!");
	}

	return retValue;
}

vector<glm::vec2> GetLevelFromFile(std::string path,std::vector<shared_ptr<GameObject>>& level, vector<SDL_Texture*> textures, vector<shared_ptr<GameObject>>& actors)
{
	auto return_value = vector<glm::vec2>();
	return_value.resize(2);
	bool first_set = false;

	ifstream inputFile(path);
	string line;

	int x_counter = TILE_WIDTH;
	int y_counter = TILE_HEIGHT;

	while (std::getline(inputFile, line))
	{
		istringstream ss(line);
		string value;

		while (std::getline(ss, value, ','))
		{
			
			int intValue = std::stoi(value);
			shared_ptr<GameObject> level_tile;
			switch (intValue)
			{
				case 0:
					break;
				case 1:
					level_tile = make_shared<GameObject>(x_counter, y_counter, textures.at(0), false);
					break;
				case 2:
					level_tile = make_shared<GameObject>(x_counter, y_counter, textures.at(1), false);
					break;
				case 3:
					level_tile = make_shared<GameObject>(x_counter, y_counter, textures.at(2), false);
					break;
				case 4:
					level_tile = make_shared<GameObject>(x_counter, y_counter, textures.at(3), false);
					break;
				case 5:
					level_tile = make_shared<GameObject>(x_counter, y_counter, textures.at(4), false);
					break;
				case 6:
					level_tile = make_shared<GameObject>(x_counter, y_counter, textures.at(5), false);
					break;
			}
			if (level_tile != nullptr)
			{
				level_tile->movable = false;
				level.push_back(level_tile);
				actors.push_back(level_tile);

				if (!first_set)
				{
					first_set = true;
					return_value[0] = vec2(x_counter + TILE_WIDTH, y_counter + TILE_HEIGHT);
				}
				else
				{
					return_value[1] = vec2(x_counter + TILE_WIDTH, y_counter + TILE_HEIGHT);
				}

			}
			x_counter += TILE_WIDTH;
		}
		y_counter += TILE_HEIGHT;
		x_counter = -TILE_WIDTH;
	}
	return return_value;
}

void RenderLevel(std::vector<shared_ptr<GameObject>>& level, SDL_Renderer* renderer, shared_ptr<Camera> camera)
{
	for (auto& tile : level)
	{
		tile->draw_rect->x = tile->position.x - TILE_WIDTH * 0.5f;
		tile->draw_rect->y = tile->position.y - TILE_HEIGHT * 0.5f;

		tile->Render(renderer, camera);
	}
}

//void RenderLevel(std::vector<std::vector<int>> level, std::vector<SDL_Rect*>& tiles, SDL_Renderer* renderer, std::vector<SDL_Texture*>& textures_obj, int tw, int th, Camera* camera)
//{
//	int x_counter = -tw; //tile width
//	int y_counter = -th; //tile height
//
//	for (const auto& row : level)
//	{
//		for (int value : row)
//		{
//			x_counter += tw;
//
//			if (value)
//			{
//
//				SDL_Rect* new_tile = new SDL_Rect();
//				new_tile->x = x_counter;
//				new_tile->y = y_counter;
//				new_tile->w = tw;
//				new_tile->h = th;
//
//				if (camera != NULL)
//				{
//					new_tile->x -= camera->GetPosition()->x;
//					new_tile->y -= camera->GetPosition()->y;
//				}
//
//				tiles.push_back(new_tile);
//
//				SDL_RenderCopyEx(renderer, textures_obj[value - 1], NULL, new_tile, 0.0, NULL, SDL_FLIP_NONE);
//			}
//		}
//		y_counter += th;
//		x_counter = -tw;
//	}
//
//	for (const auto& tile : tiles)
//	{
//		delete tile;
//	}
//
//	tiles.clear();
//}

SDL_Texture* CreateBackgroundTexture(std::vector<std::vector<int>> level, SDL_Renderer* renderer, std::vector<SDL_Texture*>& textures_obj, int tw, int th, int cw, int ch)
{
	int x_counter = -tw; //tile width
	int y_counter = -th; //tile height

	SDL_Texture* canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, cw, ch);
	SDL_SetRenderTarget(renderer, canvasTexture);
	SDL_SetRenderDrawColor(renderer, 107, 140, 255, 255);
	SDL_RenderClear(renderer);

	for (const auto& row : level)
	{
		for (int value : row)
		{
			x_counter += tw;

			if (value)
			{

				SDL_Rect* new_tile = new SDL_Rect();
				new_tile->x = x_counter;
				new_tile->y = y_counter;
				new_tile->w = tw;
				new_tile->h = th;

				SDL_SetRenderTarget(renderer, canvasTexture);
				SDL_RenderCopy(renderer, textures_obj[value - 1], NULL, new_tile);
			}
		}
		y_counter += th;
		x_counter = -tw;
	}
	SDL_SetRenderTarget(renderer, NULL);
	return canvasTexture;
}

void Close(SDL_Window*& window, SDL_Renderer*& renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;


	SDL_Quit();
}

void PollEvents(bool& q, SDL_Event& e)
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			q = true;
		}
	}
}


float RandomFloat()
{
	static std::random_device rd;
	static std::default_random_engine eng(rd());
	static std::uniform_real_distribution<double> distr(-1.0, 1.0);
	return (float)distr(eng);
}


//Collisions
float Clamp(float x, float min, float max)
{
	if (x < min)
	{
		return min;
	}
	else if (min < x && x < max)
	{
		return x;
	}
	else if (max < x)
	{
		return max;
	}
}

void ResolvePossibleCollision(shared_ptr<GameObject> A, shared_ptr<GameObject> B, bool separate, bool collide)
{
	if (A->movable || B->movable)
	{

		if (A->is_circle && B->is_circle)
		{
			if (SeparateCircles(A, B, separate))
			{
				if (collide)
				{
					Collide(A, B);
				}
			}

		}
		else if (!A->is_circle && !B->is_circle)
		{
			if (SeparateRectangles(A, B, separate))
			{
				if (collide)
				{
					Collide(A, B);
				}
			}
		}
		else if (!A->is_circle && B->is_circle)
		{
			if (SeparateCircleRectangle(B, A, separate))
			{
				if (collide)
				{
					Collide(B, A);
				}
			}
		}
		else if (A->is_circle && !B->is_circle)
		{
			if (SeparateCircleRectangle(A, B, separate))
			{
				if (collide)
				{
					Collide(A, B);
				}
			}
		}

	}
}

bool SeparateCircles(shared_ptr<GameObject> A, shared_ptr<GameObject> B, bool separate)
{
	bool return_value = false;

	float distance = glm::distance(A->position, B->position);
	if (distance < A->radius + B->radius)
	{
		return_value = true;

		if (separate)
		{
			vec2 dir_A = glm::normalize(A->position - B->position);
			vec2 dir_B = dir_A * -1.0f;

			float mag = A->radius + B->radius - distance;

			dir_A *= mag * 0.5f;
			dir_B *= mag * 0.5f;

			if (!B->movable)
			{
				dir_A *= 2.0f;
			}

			if (!A->movable)
			{
				dir_B *= 2.0f;
			}

			A->SetPosition(A->position + dir_A);
			B->SetPosition(B->position + dir_B);
		}
	}
	else if (distance == A->radius + B->radius)
	{
		return_value = true;
	}
	return return_value;
}

bool SeparateRectangles(shared_ptr<GameObject> A, shared_ptr<GameObject> B, bool separate)
{
	bool return_value = false;

	float l1 = A->position.x - A->draw_rect->w / 2.0f;
	float r1 = A->position.x + A->draw_rect->w / 2.0f;
	float t1 = A->position.y - A->draw_rect->h / 2.0f;
	float b1 = A->position.y + A->draw_rect->h / 2.0f;

	float l2 = B->position.x - B->draw_rect->w / 2.0f;
	float r2 = B->position.x + B->draw_rect->w / 2.0f;
	float t2 = B->position.y - B->draw_rect->h / 2.0f;
	float b2 = B->position.y + B->draw_rect->h / 2.0f;

	float left = r1 - l2;
	float right = r2 - l1;
	float top = b1 - t2;
	float bot = b2 - t1;

	if (left >= 0 && right >= 0 && top >= 0 && bot >= 0)
	{
		return_value = true;

		if (separate) 
		{
			vec2 v{ 0.0f, 0.0f };
			left < right ? v.x = -left : v.x = right;
			top < bot ? v.y = -top : v.y = bot;
			abs(v.x) < abs(v.y) ? v.y = 0 : v.x = 0;

			if (!B->movable)
			{
				v *= 2.0f;
			}

			if (!A->movable)
			{
				v *= 2.0f;
			}


			A->SetPosition(A->position + v);
			B->SetPosition(B->position + -1.0f * v);
		}
	}

	return return_value;
}

bool SeparateCircleRectangle(shared_ptr<GameObject> A, shared_ptr<GameObject> B, bool separate)
{
	bool return_value = false;

	float l = B->position.x - TILE_WIDTH / 2.0f;
	float r = B->position.x + TILE_WIDTH / 2.0f;
	float t = B->position.y - TILE_HEIGHT / 2.0f;
	float b = B->position.y + TILE_HEIGHT / 2.0f;

	vec2 f = {Clamp(A->position.x, l, r), Clamp(A->position.y, t, b)};
	float distance = glm::length(A->position - f);
	
	if (distance <= A->radius)
	{
		return_value = true;

		if (separate)
		{
			if (A->position == f)
			{
				float left = A->radius + A->position.x - l;
				float right = A->radius - A->position.x + r + TILE_WIDTH;
				float top = A->radius + A->position.y - t;
				float bot = A->radius - A->position.y + b + TILE_HEIGHT;

				vec2 v{ 0.0f, 0.0f };
				left < right ? v.x = -left : v.x = right;
				top < bot ? v.y = -top : v.y = bot;
				abs(v.x) < abs(v.y) ? v.y = 0 : v.x = 0;

				if (!B->movable)
				{
					v *= 2.0f;
				}

				if (!A->movable)
				{
					v *= 2.0f;
				}

				A->SetPosition(A->position + v);
				B->SetPosition(B->position + -1.0f * v);
			}
			else
			{
				vec2 v = (A->position - f) / glm::length(A->position - f) * (A->radius - glm::length(A->position - f));

				if (!B->movable)
				{
					v *= 2.0f;
				}

				if (!A->movable)
				{
					v *= 2.0f;
				}

				A->SetPosition(A->position + v * 0.5f);
				B->SetPosition(B->position - v * 0.5f);
			}
		}
	}
	return return_value;
}

glm::vec2 SeparateCircleRectangle(shared_ptr<GameObject> A, shared_ptr<GameObject> B)
{
	float l = B->position.x - TILE_WIDTH / 2.0f;
	float r = B->position.x + TILE_WIDTH / 2.0f;
	float t = B->position.y - TILE_HEIGHT / 2.0f;
	float b = B->position.y + TILE_HEIGHT / 2.0f;

	vec2 f = { Clamp(A->position.x, l, r), Clamp(A->position.y, t, b) };
	float distance = glm::length(A->position - f);

	if (distance <= A->radius)
	{
		if (A->position == f)
		{
			float left = A->radius + A->position.x - l;
			float right = A->radius - A->position.x + r + TILE_WIDTH;
			float top = A->radius + A->position.y - t;
			float bot = A->radius - A->position.y + b + TILE_HEIGHT;

			vec2 v{ 0.0f, 0.0f };
			left < right ? v.x = -left : v.x = right;
			top < bot ? v.y = -top : v.y = bot;
			abs(v.x) < abs(v.y) ? v.y = 0 : v.x = 0;

			if (!B->movable)
			{
				v *= 2.0f;
				A->position = (A->position + v);
			}

			if (!A->movable)
			{
				v *= 2.0f;
				B->position = (B->position + -1.0f * v);
			}	
		}
		else
		{
			vec2 v = (A->position - f) / glm::length(A->position - f) * (A->radius - glm::length(A->position - f));

			if (!B->movable)
			{
				v *= 2.0f;
				A->position = (A->position + v * 0.5f);
			}

			if (!A->movable)
			{
				v *= 2.0f;
				B->position = (B->position - v * 0.5f);
			}	
		}

		vec2 difference = A->position - B->position;
		difference = normalize(difference);
		float cosine = dot(difference, glm::vec2(1.0f, 0.0f));

		if (abs(cosine) < 0.45f)
		{
			return vec2(1.0f, 0.0f);
		}
		else
		{
			return vec2(0.0f, 1.0f);
		}
	}
	else
	{
		return vec2(0.0f, 1.0f);
	}
}

void Collide(shared_ptr<GameObject> A, shared_ptr<GameObject> B)
{
	if (A->movable && B->movable)
	{
		vec2 tmp = A->speed;
		A->speed = B->speed;
		B->speed = tmp;
	}
	else if (!A->movable && B->movable)
	{
		B->speed = -B->speed;
	}
	else if (A->movable && !B->movable)
	{
		A->speed = -A->speed;
	}
	
}

void CollideCircles(shared_ptr<GameObject> A, shared_ptr<GameObject> B)
{
	vec2 tmp = A->speed;
	A->speed = B->speed;
	B->speed = tmp;
}

void CollideRectangles(shared_ptr<GameObject> A, shared_ptr<GameObject> B)
{
	vec2 tmp = A->speed;
	A->speed = B->speed;
	B->speed = tmp;
}

void CollideCircleRectangle(shared_ptr<GameObject> A, shared_ptr<GameObject> B)
{
	vec2 tmp = A->speed;
	A->speed = B->speed;
	B->speed = tmp;
}
