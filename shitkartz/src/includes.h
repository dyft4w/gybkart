#pragma once
#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <SDL_image.h>
#include <SDL_vulkan.h>

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include <vulkan/vulkan.h>

#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <optional>
#include <deque>
#include <map>
#include <set>
#include <cstddef>
#include <limits>
#include <memory>
// self explanatory
constexpr char NAME[] = "Shitkartz";
constexpr uint32_t SCREEN_HEIGHT=256;
constexpr uint32_t SCREEN_WIDTH=256;
constexpr char ASSET_PATH[] = "../../";
