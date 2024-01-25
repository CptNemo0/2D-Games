#pragma once

#include "GameObject.h"
#include <SDL.h>
#include <SDL_image.h>
#include "spdlog/spdlog.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "constants.hpp"
#include <random>
#include <iostream>
#include "Camera.h"

using namespace std;

bool Init(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& screenSurface, int w, int h);

bool LoadTextures(std::vector<SDL_Texture*>& textures_obj, std::vector<std::string>& textures_paths, SDL_Renderer*& renderer);

bool GetTexturePathsFromFile(std::string path, std::vector<std::string>& textures_paths);

vector<glm::vec2> GetLevelFromFile(std::string path, std::vector<shared_ptr<GameObject>>& level, vector<SDL_Texture*> textures, vector<shared_ptr<GameObject>>& actors);

void RenderLevel(std::vector<shared_ptr<GameObject>>& level, SDL_Renderer* renderer, shared_ptr<Camera> camera);

SDL_Texture* CreateBackgroundTexture(std::vector<std::vector<int>> level, SDL_Renderer* renderer, std::vector<SDL_Texture*>& textures_obj, int tw, int th, int cw, int ch);

void Close(SDL_Window*& window, SDL_Renderer*& renderer);

void PollEvents(bool& q, SDL_Event& e);

float Clamp(float x, float min, float max);

float RandomFloat();

void ResolvePossibleCollision(shared_ptr<GameObject> A, shared_ptr<GameObject> B, bool separate, bool collide);
bool SeparateCircles(shared_ptr<GameObject> A, shared_ptr<GameObject> B, bool separate);
bool SeparateRectangles(shared_ptr<GameObject> A, shared_ptr<GameObject> B, bool separate);
bool SeparateCircleRectangle(shared_ptr<GameObject> A, shared_ptr<GameObject> B, bool separate);
glm::vec2 SeparateCircleRectangle(shared_ptr<GameObject> A, shared_ptr<GameObject> B);
void CollideCircles(shared_ptr<GameObject> A, shared_ptr<GameObject> B);
void CollideRectangles(shared_ptr<GameObject> A, shared_ptr<GameObject> B);
void CollideCircleRectangle(shared_ptr<GameObject> A, shared_ptr<GameObject> B);
void Collide(shared_ptr<GameObject> A, shared_ptr<GameObject> B);