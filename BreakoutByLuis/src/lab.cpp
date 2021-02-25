#include <chrono>
#include <SDL2/SDL.h>
// #include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <Paddle.hpp>
#include <Brick.hpp>
#include <Ball.hpp>
#include <Label.hpp>
#include <fstream>

using namespace std;

/*! \brief adjusts where bricks should appear with respect te top canvas
 *         
 */
const int TOP_OFFSET = 100;

/*! \brief adjusts where bricks should appear with respect te left canvas
 *         
 */
const int LEFT_OFFSET = 10;

/*! \brief represents all the potential keyboard inputs
 *         
 */
enum Buttons
{
	PaddleLeft = 0,
	PaddleRight,
	StartGame,
	ResetGame,
};

/*! \brief represents all the potential strings to be used in the game, each has been localized 
 *         
 *  @arg localization files can be found at ./strings
 */
enum Strings
{
	pressUp = 0,
	breakBricks,
	youWin,
	youLose,
};

/*! \brief runs the main game logic, requests both a language and a level to load.
 *         
 *  @arg language expects either 'es' for spanish or 'en' english
 *  @arg level expects a number between 0 and 1, more levels may be added in the future
 */
int main()
{

	//Request player settings
	char str[80];
	char str2[80];

	printf("Choose a language \n('en' for english,'es' for spanish): ");
	scanf("%79s", str);
	printf("Choose a level: ");
	scanf("%79s", str2);

	string localizationFile = "Assets/strings/";
	localizationFile.append(str);
	localizationFile.append(".txt");

	string levelFile = "Assets/levels/level";
	levelFile.append(str2);
	levelFile.append(".txt");

	// Initialize SDL components
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	TTF_Init();
	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_Window *window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// Initialize the font
	TTF_Font *scoreFont = TTF_OpenFont("Assets/fonts/DejaVuSansMono.ttf", 40);

	// Initialize sound effects
	//Mix_Chunk *wallHitSound = Mix_LoadWAV("WallHit.wav");
	//Mix_Chunk *paddleHitSound = Mix_LoadWAV("PaddleHit.wav");

	// Game logic
	{

		//	CREATE

		// Localization
		fstream newfile2;
		char localStrings[1000][1000];
		newfile2.open(localizationFile.c_str(), ios::in); //open a file to perform read operation using file object
		if (newfile2.is_open())
		{ //checking whether the file is open
			string tp;
			int i = 0;
			while (getline(newfile2, tp))
			{ //read data from file object and put it into string.
				strcpy(localStrings[i], tp.c_str());
				i++;
			}
			newfile2.close(); //close the file object.
		}
		else
		{
			printf("invalid language, please choose either es or en\n");
			return 1;
		}

		// Create the player score text fields
		Label livesText(Vec2(WINDOW_WIDTH / 4, 20), renderer, scoreFont);

		Label scoreText(Vec2(3 * WINDOW_WIDTH / 4, 20), renderer, scoreFont);

		// Create the paddles
		Paddle paddleOne(
			Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - PADDLE_HEIGHT - 50.0f),
			Vec2(0.0f, 0.0f));

		// Create the ball
		Ball ball(
			Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f),
			Vec2(BALL_SPEED / 2.0f, BALL_SPEED),
			&paddleOne);

		// Create Bricks
		int numberOfBricks = 23;
		int numberOfBrickRows = 10;
		fstream newfile;
		char lines[numberOfBrickRows * 2][numberOfBricks * 2];
		newfile.open(levelFile.c_str(), ios::in); //open a file to perform read operation using file object
		if (newfile.is_open())
		{ //checking whether the file is open
			string tp;
			int i = 0;
			while (getline(newfile, tp))
			{ //read data from file object and put it into string.
				strcpy(lines[i], tp.c_str());
				i++;
			}
			newfile.close(); //close the file object.
		}
		else
		{
			printf("invalid level, please choose either 0 or 1, you may try other numbers too but no guarantee\n");
			return 1;
		}

		Brick *allBricks[numberOfBricks][numberOfBrickRows];
		int bricksToBreak = 0;
		for (int ii = 0; ii < numberOfBricks; ii++)
		{
			for (int jj = 0; jj < numberOfBrickRows; jj++)
			{
				char status = lines[jj][ii * 2];
				int state = status == '0' ? 1 : 0;
				bricksToBreak += status == '0' ? 0 : 1;
				Brick *brickInstance = new Brick(
					Vec2((BRICK_WIDTH + (BRICK_WIDTH / 10)) * ii + LEFT_OFFSET, (BRICK_HEIGHT + (BRICK_HEIGHT / 10)) * jj + TOP_OFFSET),
					Vec2(0, 0), state);
				allBricks[ii][jj] = brickInstance;
			}
		}

		// Create Final Message
		Label finalMessage(Vec2((WINDOW_WIDTH / 2) - 10, 20), renderer, scoreFont, localStrings[pressUp]);

		// GAME SETTINGS

		int lives = 3;
		livesText.SetScore(lives);
		int score = 0;

		bool running = true;
		bool buttons[4] = {};

		float dt = 0.0f;

		while (running)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					running = false;
				}
				else if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
					{
						running = false;
					}
					else if (event.key.keysym.sym == SDLK_LEFT)
					{
						buttons[Buttons::PaddleLeft] = true;
					}
					else if (event.key.keysym.sym == SDLK_RIGHT)
					{
						buttons[Buttons::PaddleRight] = true;
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						buttons[Buttons::StartGame] = true;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						buttons[Buttons::ResetGame] = true;
					}
				}
				else if (event.type == SDL_KEYUP)
				{
					if (event.key.keysym.sym == SDLK_LEFT)
					{
						buttons[Buttons::PaddleLeft] = false;
					}
					else if (event.key.keysym.sym == SDLK_RIGHT)
					{
						buttons[Buttons::PaddleRight] = false;
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						buttons[Buttons::StartGame] = false;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						buttons[Buttons::ResetGame] = false;
					}
				}
			}

			if (buttons[Buttons::PaddleLeft])
			{
				paddleOne.velocity.x = -PADDLE_SPEED;
			}
			else if (buttons[Buttons::PaddleRight])
			{
				paddleOne.velocity.x = PADDLE_SPEED;
			}
			else
			{
				paddleOne.velocity.x = 0.0f;
			}

			if (buttons[Buttons::StartGame])
			{
				ball.state = 1;
				ball.velocity = Vec2(BALL_SPEED / -2.0f, -BALL_SPEED);
			}
			if (buttons[Buttons::ResetGame])
			{
				ball.state = 0;
			}

			// Update the paddle positions
			paddleOne.Update(dt);
			//paddleTwo.Update(dt);

			// Update the ball position
			ball.Update(dt);

			// HANDLE GAME STATE
			if (lives < 1)
			{
				ball.state = 0;
				finalMessage.SetMessage(localStrings[youLose]);
			}
			else if (score >= bricksToBreak)
			{
				ball.state = 0;
				finalMessage.SetMessage(localStrings[youWin]);
			}
			else if (ball.state > 0)
			{
				finalMessage.SetMessage(localStrings[breakBricks]);
			}
			else
			{
				finalMessage.SetMessage(localStrings[pressUp]);
			}

			//Update
			for (int ii = 0; ii < numberOfBricks; ii++)
			{
				for (int jj = 0; jj < numberOfBrickRows; jj++)
				{
					Brick brickInstance = *allBricks[ii][jj];
					brickInstance.Update(dt);
				}
			}

			// Check collisions

			for (int ii = 0; ii < numberOfBricks; ii++)
			{
				for (int jj = 0; jj < numberOfBrickRows; jj++)
				{
					Brick brickInstance = *allBricks[ii][jj];
					if (brickInstance.state == 0)
					{
						if (Contact contact = CheckBrickCollision(ball, brickInstance);
							contact.type != CollisionType::None)
						{
							ball.CollideWithBrick(contact);
							allBricks[ii][jj] = brickInstance.Break(); // this means brocken;
							++score;
							scoreText.SetScore(score);

							//Mix_PlayChannel(-1, paddleHitSound, 0);
						}
					}
				}
			}
			if (Contact contact = CheckPaddleCollision(ball, paddleOne);
				contact.type != CollisionType::None)
			{
				ball.CollideWithPaddle(contact);

				//Mix_PlayChannel(-1, paddleHitSound, 0);
			}
			else if (contact = CheckWallCollision(ball);
					 contact.type != CollisionType::None)
			{
				ball.CollideWithWall(contact);

				if (contact.type == CollisionType::Bottom)
				{
					--lives;
					livesText.SetScore(lives);
					ball.state = 0;
				}
			}

			// Clear the window to black
			SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
			SDL_RenderClear(renderer);

			// Set the draw color to be white
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

			// Draw the ball
			ball.Draw(renderer);

			// Draw the paddles
			paddleOne.Draw(renderer);
			//paddleTwo.Draw(renderer);

			// Draw Bricks
			for (int ii = 0; ii < numberOfBricks; ii++)
			{
				for (int jj = 0; jj < numberOfBrickRows; jj++)
				{
					Brick brickInstance = *allBricks[ii][jj];
					brickInstance.Draw(renderer);
				}
			}

			// Display the scores
			livesText.Draw();
			scoreText.Draw();

			// Display final Message
			finalMessage.Draw();

			// Present the backbuffer
			SDL_RenderPresent(renderer);

			// Calculate frame time
			auto stopTime = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
		}
	}

	//Mix_FreeChunk(wallHitSound);
	//Mix_FreeChunk(paddleHitSound);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(scoreFont);
	//Mix_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}
