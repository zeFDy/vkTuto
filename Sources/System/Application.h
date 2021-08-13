#pragma once

#include	"../../External/GLFW/glfw3.h"
#include	"../GraphicsAPI/vkApiInstance.h"
#include	"../GraphicsAPI/vkApiDevices.h"
#include	"../GraphicsAPI/vkApiSwapChain.h"

#define	WIDTH				800
#define HEIGHT				600
#define APPLICATION_NAME	"vkTuto"

class Application
{
public:				Application();
					~Application();
			void	Init();
			void	Run();

private:

				  bool	bDiscret = false;
				  bool	bFullScreen = false;

		   GLFWwindow*	myWindow;
		vkApiInstance*	myVkApiInstance;
		 vkApiDevices*	myVkApiDevices;	
	   vkApiSwapChain*	myVkApiSwapChain;
};

