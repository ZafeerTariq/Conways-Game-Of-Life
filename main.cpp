#include <iostream>
#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include "Grid.h"

int main() {
	sf::RenderWindow window( sf::VideoMode( 1600, 900 ), "Game Of Life" );
	window.setFramerateLimit(60);

	const int rows = 30;
	const int cols = 52;

	Grid grid( rows, cols );

	sf::Clock clock;
    sf::Time deltaTime;
	float time = 0;

	if ( !ImGui::SFML::Init(window) ) {
		std::cout << "Could not init imgui\n";
		return -1;
	}

	bool running = false;
	bool paused = false;
	float tickSpeed = 0.25;
	char* labelText = new char[20];
	strcpy( labelText, "Not Running..." );

	while ( window.isOpen() ) {
		sf::Event event;
        while ( window.pollEvent( event ) ) {
			ImGui::SFML::ProcessEvent(window, event);
            if ( event.type == sf::Event::Closed ) {
                window.close();
            } else if ( event.type == sf::Event::MouseButtonPressed ) {
				if ( event.mouseButton.button == sf::Mouse::Left && !running ) {
					sf::Vector2i mousePosition =  sf::Mouse::getPosition(window);
					grid.turnCellOn(mousePosition.x, mousePosition.y);
				}
			}
        }

		deltaTime = clock.restart();
		time += deltaTime.asSeconds();
		bool buttonPressed = false;

		ImGui::SFML::Update(window, deltaTime);
		ImGui::Begin("Settings");
		ImGui::LabelText( labelText, "State" );
		if ( ImGui::Button( "Next Tick" ) ) {
			buttonPressed = true;
		}
		if ( ImGui::Button( "Start" ) ) {
			running = true;
			paused = false;
			strcpy( labelText, "Running..." );
		}
		if ( ImGui::Button( "Stop" ) ) {
			running = false;
			grid.resetTicks();
			strcpy( labelText, "Not Running..." );
		}
		if ( ImGui::Button( "Pause" ) && running ) {
			paused = !paused;
			strcpy( labelText, "Running - Paused" );
		}
		if ( ImGui::Button( "Clear" ) ) {
			grid.clearGrid();
		}
		ImGui::SliderFloat( "Tick Speed", &tickSpeed, 0.1f, 2 );
		ImGui::End();

		if ( ( time > tickSpeed && running && !paused ) || buttonPressed ) {
			grid.update();
			time = 0;
		}

		window.clear( sf::Color( 32, 32, 32, 255 ) );
		grid.draw(window);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}