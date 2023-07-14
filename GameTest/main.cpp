
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Arrow.h"

using namespace sf;

int main()
{
	// The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	// Start with the GAME_OVER state
	State state = State::GAME_OVER;

	// Get the screen resolution and create an SFML window

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Zombie Arena", Style::Fullscreen);

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Here is our clock for timing everything
	Clock clock;
	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Create an instance of the Player class
	Player player;

	// The boundaries of the arena
	IntRect arena;

	//texture of the background
	sf::Texture t;
	t.loadFromFile("graphics/grass.png");
	sf::Sprite s(t);

	//texture of the loading screen
	sf::Texture load;
	load.loadFromFile("graphics/loading.png");
	sf::Sprite s_load(load);
	//s_load.setScale((float)resolution.x, (float)resolution.y);


	// 100 arrow should do
	Arrow arrow[100];
	int currentArrow = 0;
	int arrowSpare = 24;
	int arrowInQuiver = 24;         // initial number of arrow in quiver
	int quiverSize = 24;            //size of the quiver to hold the arrow
	float fireRate = 1;
	// When was the fire button last pressed?
	Time lastPressed;



	// The main game loop
	while (window.isOpen())
	{
		

		/*
		************
		Handle input
		************
		*/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			
			if (event.type == Event::KeyPressed)
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING)
				{
					// Reloading
					if (event.key.code == Keyboard::R)
					{
						if (arrowSpare >= quiverSize)
						{
							// Plenty of arrow. Reload.
							arrowInQuiver = quiverSize;
							arrowSpare -= quiverSize;
						}
						else if (arrowSpare > 0)
						{
							// Only few arrow left
							arrowInQuiver = arrowSpare;
							arrowSpare = 0;
						}
						else
						{
							// More here soon?!
						}
					}

				}

			}
		}// End event polling


		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}

			// Fire a ArrowcurrentArrow
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 1000 / fireRate && arrowInQuiver > 0)
				{

					// Pass the centre of the player 
					// and the centre of the cross-hair
					// to the shoot function
					arrow[currentArrow].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentArrow++;
					if (currentArrow > 99)
					{
						currentArrow = 0;
					}
					lastPressed = gameTimeTotal;

					arrowInQuiver--;
				}

			}// End fire a ArrowcurrentArrow


		}// End WASD while playing

		// Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			//this following commented section is to be edited for level upgrades .. for now it is fine to go
			
			/*
			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}
			*/

			//here state is made playing just easiness for testing purpose
			//later on above code will be uncommented and further code will be added
			state = State::PLAYING;

			//game starts here
			if (state == State::PLAYING)
			{
				// Prepare thelevel
				// We will modify the next two lines later
				arena.width = resolution.x;
				arena.height = resolution.y;
				arena.left = 0;
				arena.top = 0;

				// We will modify this line of code later
				int tileSize = 50;

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				// Reset the clock so there isn't a frame jump
				clock.restart();
			}
		}// End levelling up

		/*
		****************
		UPDATE THE FRAME
		****************
		*/
		if (state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();
			// Update the total game time
			gameTimeTotal += dt;
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the players new position

			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player				
			//mainView.setCenter(player.getCenter());        //update view with player in the center

			// Update any arrow that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (arrow[i].isInFlight())
				{
					arrow[i].update(dtAsSeconds);
				}
			}



		}// End updating the scene

		/*
		**************
		Draw the scene
		**************
		*/

		if (state == State::PLAYING)
		{
			window.clear();
			
			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);
			window.draw(s);
			// Draw the player
			window.draw(player.getSprite());
		}

		for (int i = 0; i < 100; i++)
		{
			if (arrow[i].isInFlight())
			{
				window.draw(arrow[i].getSprite());
			}
		}

		if (state == State::LEVELING_UP)
		{
		}

		if (state == State::PAUSED)
		{
		}

		if (state == State::GAME_OVER)
		{
		}

		window.display();

	}// End game loop

	return 0;
}