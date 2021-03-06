#include <SFML/Graphics.hpp>
#include <iostream>
#include "StateMachine_Game.h"
#include <pugixml.cpp>

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "PlateFormer");
	window.setFramerateLimit(60);

	StateMachine_Game gameMachine(&window);
	cout << "GameMachine created" << endl;

	//if (gameMachine.changeState(GAME_STATES::GAME))
	if (gameMachine.changeState(GAME_STATES::MENU))
	{
		cout << "State initalized" << endl;
	} else {
		cout << "Error during the state change init" << endl;
	}

	Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			gameMachine.getCurrentState()->processEvents(event);
		}

		gameMachine.getCurrentState()->processEvents();
		
		if (!gameMachine.operate())
		{
			cout << "Error during the operate of the stateMachine" << endl;
			return EXIT_FAILURE;
		}

	}
}