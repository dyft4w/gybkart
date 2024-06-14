#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <cstring>
#include <iostream>
#include <SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <nlohmann/json_fwd.hpp>
#include <rapidjson/reader.h>
#include <vector>
#include <deque>
#include <map>
#include <nlohmann/json.hpp>
#include <fstream>
// self explanatory
constexpr char NAME[] = "Shitkartz";
constexpr int SCREEN_HEIGHT=256;
constexpr int SCREEN_WIDTH=256;
constexpr char ASSET_PATH[] = "../../";
//constexpr int SCALE=1;


// window and its master surface
SDL_Window* window=NULL;
SDL_Surface* screenSurface=NULL;
// to not destroy the surfaces
std::deque<SDL_Surface*> surfaces;
std::map<std::string, SDL_Surface*> asset_surfaces;
std::vector<SDL_Event*> events;


void clearSurfaces(){
	// clear surfaces but last
	surfaces.erase(surfaces.begin(),surfaces.end()-1);
}
void loadImage(std::string path){
	SDL_Surface* image;
	image=SDL_LoadBMP(path.c_str());
	SDL_BlitSurface(image,NULL,screenSurface,NULL);
	surfaces.push_back(image);
	SDL_UpdateWindowSurface(window);
}
void loadColor(int r, int g, int b){
	SDL_Surface* color=NULL;
	color=SDL_CreateRGBSurface(0, SCREEN_WIDTH,SCREEN_HEIGHT,32, 0,0,0,0);
	SDL_FillRect(color, NULL, SDL_MapRGB(color->format, 0xff,0xff,0xff));
	SDL_BlitSurface(color,NULL,screenSurface,NULL);
	surfaces.push_back(color);
	SDL_UpdateWindowSurface(window);
}
void loadSurfaces(){
  std::ifstream f(std::string(ASSET_PATH)+"inventory.json"); 
  nlohmann::json data = nlohmann::json::parse(f);
  for(int i = 0; i<data["surfaces"].size();i++){
    std::string temp=std::string(ASSET_PATH);
    asset_surfaces[data["surfaces"][i]["name"]] = IMG_Load((temp+"inventory.json").c_str());
  } 
  f.close();
}
void initVulkan(){
  uint32_t extensionCount;
  const char** extensionNames = 0;
  SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
  extensionNames = new const char *[extensionCount];
  SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames);
  const VkInstanceCreateInfo instInfo = {
      VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, // sType
      nullptr,                                // pNext
      0,                                      // flags
      nullptr,                                // pApplicationInfo
      0,                                      // enabledLayerCount
      nullptr,                                // ppEnabledLayerNames
      extensionCount,                         // enabledExtensionCount
      extensionNames,                         // ppEnabledExtensionNames
  };
  VkInstance vkInst;
  vkCreateInstance(&instInfo, nullptr, &vkInst);
}
bool init(){
	bool success=true;
  loadSurfaces();
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		std::cerr << "error : could not start sdl context" << std::endl;
		success=false;
	}else{
		window = SDL_CreateWindow(NAME,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
		screenSurface = SDL_GetWindowSurface(window);
		SDL_FillRect(screenSurface,NULL,SDL_MapRGB(screenSurface->format, 0x00,0x00,0x00));
		loadImage("andiwonder.bmp");
	}
	return success;
}
void loop(){
	SDL_Event* e= new SDL_Event;
	while(true){
		SDL_PollEvent(e);
		if (e->type==SDL_QUIT) {
			break;
		}
		if(e->type==SDL_MOUSEBUTTONDOWN){
			clearSurfaces();
			loadColor(0xff,0xff,0xff);
		}
	}
}

int main(){
	if(!init()){
		std::cout << "error : could not start sdl context" << std::endl;
		return -1;
	}else {
		loop();
	}
	return 0;	
}
