#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#pragma comment(lib,"thor-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#pragma comment(lib,"thor.lib") 
#endif 

#include "Game.h"

int main()
{
	try
	{
		Game game;
		game.run();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (std::string& e)
	{
		std::cout << e << std::endl;
	}
	catch (...)
	{
		std::cout << "An uncaught error occured!" << std::endl;
	}

	return 0;
}