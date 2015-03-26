#include <iostream>
#include "GL/glew.h"
#include "SDL.h"
#include "shaders.h"
#include "object.h"
#include "window.h"
#include "events.h"
#include "player.h"
#include "mesh.h"
#include "kdmesh.h"
#include "linemesh.h"
#include "sphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define ROOT_DIR "C:/Users/Stuart/C/temp/"

int main(int argc, char **argv)
{
	int scale = 2;
	Window window = Window(scale * 640, scale * 480);
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	SDL_SetRelativeMouseMode(SDL_TRUE);

	Program prog = Program(ROOT_DIR "data/simple.vertex", ROOT_DIR "data/simple.fragment");
	prog.Use();

	glm::mat4 projection = glm::perspective(60.0f * (float)(180.0f / M_PI), 640/480.0f, 0.01f, 1000.0f);
	glm::mat4 model      = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -10));
	glm::mat4 view       = glm::mat4(1.0);

	prog.SetModel(model);
	prog.SetView(view);
	prog.SetProjection(projection);
	
	window.SwapBuffer();

	EventHandler event_handler;
	Player   player;

	std::vector<Object  *> objects;
	objects.push_back(&player);
#if 0
	objects.push_back(new KDMesh<10,1>(ROOT_DIR "data/target.ply", ROOT_DIR "data/wood.bmp"));
	objects.push_back(new LineCube(glm::vec3(0, 0, 0), glm::vec3(1, 1, 2)));
	objects.push_back(new Mesh(ROOT_DIR "data/floor.ply", ROOT_DIR "data/grid.bmp"));
	objects.push_back(new Mesh(ROOT_DIR "data/skybox.ply", ROOT_DIR "data/skybox.bmp"));
#endif
	GravitySphere *s0 = new GravitySphere(glm::vec3(3, 0, 0), glm::vec3(-3, 0, 0));
	GravitySphere *s1 = new GravitySphere(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0));
	objects.push_back(s0);
	objects.push_back(s1);

	uint32_t time = SDL_GetTicks();
	uint32_t frame_time, new_time;
	uint32_t accumulator = 0;
	uint32_t dt = 20; // 50Hz
	while(1)
	{
		new_time     = SDL_GetTicks();
		frame_time   = new_time - time;
		time         = new_time;
		accumulator += frame_time;
		while(accumulator >= dt) //Simulation Update
		{
			event_handler.HandleEvents();
			for(auto obj : objects)
			{
				obj->Simulate(dt / 1000.0f);
			}
			accumulator -= dt;
		}

		prog.SetView(player.GetViewMatrix());
		

		for(auto obj : objects)
		{
			obj->Draw(prog);
		}
		window.SwapBuffer();

	}
    return 0;
}
