#include	"Application.h"
#include	<iostream>
#include	<iomanip>
#include	<memory>
#include	<vector>

#include	"Log.h"
#include	"../../External/GLFW/glfw3.h"
#include	"../GraphicsAPI/vkApiInstance.h"
#include	"../GraphicsAPI/vkApiDevices.h"
#include	"../GraphicsAPI/vkApiSwapChain.h"



Application::Application()
{
		std::cout << "Application()\n";
		LogInsert("Application()\n");
}

Application::~Application()
{
		std::cout << "~Application()\n";
		LogInsert("~Application()\n");

		delete(myVkApiSwapChain);
		delete(myVkApiDevices);
		delete(myVkApiInstance);

		glfwDestroyWindow(myWindow);
		glfwTerminate();
}

void	Application::Init()
{
		LogInsert("init();\n");
			

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
								

		#if false
        // attention, le mode plein ecran ne fonctionne pas (encore)
        // car cela n�cessite une destruction et reconstruction de la swapchain
        // et cela n'est pas fini de coder avec les nouvelles textures & descriptorSets

        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();                                                  // fullscreen
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);                                             // fullscreen

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);                                                           // fullscreen
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);                                                       // fullscreen
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);                                                         // fullscreen
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);                                                   // fullscreen

        myWindow = glfwCreateWindow(mode->width, mode->height, "vkTuto", primaryMonitor, NULL);               // fullscreen
        #else
        myWindow = glfwCreateWindow(WIDTH, HEIGHT, "vkTuto", nullptr, nullptr);									// windowed
        #endif // false

		//std::cout << "myGraphicAPI is " << myGraphicAPI->GetTypeString() << " - " << myGraphicAPI->GetType() << "\n";
		glfwSetWindowUserPointer(myWindow, this);
		
		myVkApiInstance		= new vkApiInstance((char*)APPLICATION_NAME, myWindow);
		myVkApiDevices		= new vkApiDevices(myVkApiInstance);
		myVkApiSwapChain	= new vkApiSwapChain(myVkApiInstance, myVkApiDevices);

}

void	Application::Run()
{
		//myGraphicAPI->TransferSceneToGraphicsAPI(myScene);
		//myGraphicAPI->Run(myWindow);
}

