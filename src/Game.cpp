#pragma once

#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

#include "Global\Global.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Game.h"
#include "characters.h"
#include "ControlGfx.h"
#include "Objects.h"
#include "Enemies.h"
#include "TImers.h"
#include "Paralaxlayers.h"
#include "Animals.h"
#include "Bullets.h"
#include "MainMenu.h"
#include "Credits.h"
#include "Collision.h"
#include "OutroFinish.h"
#include "GetInput.h"
#include "CBoss.h"
#include "World\CWorld.h"

Gamestate gamestate;

Gamestate::Gamestate()
{
	WorldController.CreateWorld();
	cout << "Initializing Gamestate...." << endl;

	this->GameState.push(MENU_MAIN_STATE);

	gBoss.SetSurface(1);

	Parallax = 0.0f;
	DeltaTime = 0.0f;
}
void Gamestate::KeyMapping(SDL_Event _event)
{
	bool KEYS[322];  // 322 is the number of SDLK_DOWN events


	for(int i = 0; i < 322; i++) { // init them all to false
	   KEYS[i] = false;
	}

	SDL_EnableKeyRepeat(0,0); // you can configure this how you want, but it makes it nice for when you want to register a key continuously being held down

	if( BCPlayer.GetState() == BaseCharacter::State::MOVING_RIGHT)
		cout << "DO DA MOVA MOVA!!!!!" << endl;

	switch (_event.type) 
	{
		case SDL_KEYDOWN:
			KEYS[_event.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            KEYS[_event.key.keysym.sym] = false;
            break;
	}  
}



void Game::HandleEvents( SDL_Event _event )
{	
	if( _event.type == SDL_KEYUP )
	{
		BCPlayer.SetState(BaseCharacter::State::IDLE);

		switch( _event.key.keysym.sym )		
		{
		case SDLK_ESCAPE:
			{
				std::cout << "Trying to get to the menu eeeyyy!!" << endl;
				gamestate.GameState.push(MENU_MAIN_STATE);
				gamestate.State = MENU_MAIN_STATE;
			} break;
		case SDLK_RIGHT:
			{
				BCPlayer.xVelocity = 0.0f;
			} break;
		case SDLK_LEFT:
			{
				BCPlayer.xVelocity = 0.0f;
			} break;
		case SDLK_UP:
			{
				BCPlayer.yVelocity = 0.0f;
			} break;
		case SDLK_DOWN:
			{
				BCPlayer.yVelocity = 0.0f;
			} break;
		case SDLK_SPACE:
			{
				//cout << "Released SPACEBAR key" << endl;
			} break;
		case SDLK_LALT:
			{
				//cout << "Released the LEFT ALT key" << endl;
			} break;
		}
	}
	else if( _event.type == SDL_QUIT  )
	{
		Quit = true;
	}
	else if( _event.type == SDL_ACTIVEEVENT  )
	{
		std::cout << "Hey stop focusing on other windows, get back here!" << endl;
	}
	else if( _event.type == SDL_ACTIVEEVENT  )
	{
		std::cout << "Hey stop focusing on other windows, get back here!" << endl;
	}
	else if( _event.type == SDL_KEYDOWN )
	{
		switch( _event.key.keysym.sym )
		{
		case SDLK_RIGHT:
			{
				BCPlayer.AddAction("RIGHT");
				BCPlayer.xVelocity = 1.0f;
			} break;
		case SDLK_LEFT:
			{
				BCPlayer.AddAction("LEFT");
				BCPlayer.xVelocity = -1.0f;
			} break;
		case SDLK_UP:
			{
				BCPlayer.AddAction("UP");
				BCPlayer.yVelocity = 1.0f;
			} break;
		case SDLK_DOWN:
			{
				BCPlayer.AddAction("DOWN");
				BCPlayer.yVelocity = -1.0f;
			} break;
		case SDLK_SPACE:
			{
				BulletController.Create_Bullets();
				BCPlayer.AddAction("FIRE");
				BCPlayer.AddBeam("Laser");
				cout << BCPlayer.GetBeam() << endl;
				Gfx.FLIP();
			} break;
		case SDLK_LALT:
			{
				BCPlayer.AddAction("FIRE_SPECIAL");
			} break;
		case SDLK_RETURN:
			{
				BCPlayer.AddAction("RETURN");
			} break;
		default:
			{
				BCPlayer.AddAction("DEFAULT");
			}
		}
	}
	else
	{
		BCPlayer.SetState(BaseCharacter::State::IDLE);
	}
	// if intro checks 'position and checks for presses
	//if( gamestate.State == MENU_MAIN_STATE )

	if(gamestate.GameState.top() == MENU_MAIN_STATE)
	{
		SDL_GetMouseState(&MouseXCoordinates, &MouseYCoordinates);
		cout << "(" << MouseXCoordinates << "," << MouseYCoordinates << ")" << endl;
		for( int i = 0; i < 8; i++ )
		{
			if(MouseXCoordinates > gamestate.MainMenuScreen->ButtonClips[ i ].x && 
			MouseXCoordinates < gamestate.MainMenuScreen->ButtonClips[ i ].x + gamestate.MainMenuScreen->ButtonClips[ i ].w &&
			MouseYCoordinates > gamestate.MainMenuScreen->ButtonClips[ i ].y &&
			MouseYCoordinates < gamestate.MainMenuScreen->ButtonClips[ i ].y + gamestate.MainMenuScreen->ButtonClips[ i ].h )
			{
				cout << "Entering button " << i << "..." << endl;
			}
		}
		if( _event.type == SDL_MOUSEBUTTONDOWN )
		{
			// if mouse click within boundries of one of the buttons
			for( int i = 4; i < 8; i++ )
			{
					if( _event.button.x > gamestate.MainMenuScreen->ButtonClips[ i ].x && 
					_event.button.x < gamestate.MainMenuScreen->ButtonClips[ i ].x + gamestate.MainMenuScreen->ButtonClips[ i ].w &&
					_event.button.y > gamestate.MainMenuScreen->ButtonClips[ i ].y &&
					_event.button.y < gamestate.MainMenuScreen->ButtonClips[ i ].y + gamestate.MainMenuScreen->ButtonClips[ i ].h )
					{
						if( i == 7 )
							Game::Quit = true;
						if( i == 6 )
						{
							gamestate.GameState.pop();
							gamestate.GameState.push(GAME_CREDITS_STATE);
						}
						if( i == 4 )
						{
							gamestate.GameState.pop();
							gamestate.GameState.push(GAME_OPTIONS_STATE);
						}
						cout << "Hit button..." << i << endl;
					}
			}

			if( gamestate.MainMenuScreen->ButtonOptions == true )
			{
				for( int i = 4; i < 8; i++ )
				{
					if( _event.button.x > gamestate.MainMenuScreen->ButtonClips[ i ].x && 
					_event.button.x < gamestate.MainMenuScreen->ButtonClips[ i ].x + gamestate.MainMenuScreen->ButtonClips[ i ].w &&
					_event.button.y > gamestate.MainMenuScreen->ButtonClips[ i ].y &&
					_event.button.y < gamestate.MainMenuScreen->ButtonClips[ i ].y + gamestate.MainMenuScreen->ButtonClips[ i ].h )
					
					if( _event.type == SDL_MOUSEBUTTONDOWN )
					{
						switch( i )
						{
							// Buttons within the options screen
							//case 4: gamestate.MainMenuScreen->ButtonHighScore = true; break;
							//case 5: gamestate.MainMenuScreen->ButtonCredits = true; break;
							//case 6: gamestate.MainMenuScreen->ButtonSound = true; break;
							//case 7: gamestate.MainMenuScreen->ButtonBack = true; break;
						}
					}
				}

			}
			else
			{
				// checks if mousebutton is pressed at newgame, options or quit
				for( int i = 0; i < 3; i++ )
				{
					if( _event.button.x > gamestate.MainMenuScreen->ButtonClips[ i ].x && 
						_event.button.x < gamestate.MainMenuScreen->ButtonClips[ i ].x + gamestate.MainMenuScreen->ButtonClips[ i ].w &&
						_event.button.y > gamestate.MainMenuScreen->ButtonClips[ i ].y &&
						_event.button.y < gamestate.MainMenuScreen->ButtonClips[ i ].y + gamestate.MainMenuScreen->ButtonClips[ i ].h )
					{
						switch( i )
						{
							case BUTTON_NEW_GAME: cout << "Hoovering new game button" << endl ; break;
							case BUTTON_OPTIONS: cout << "Hoovering options button" << endl; break;
							case BUTTON_QUIT: cout << "Hoovering quit button" << endl; Game::Quit = true; break;
						}
						if( _event.type == SDL_MOUSEBUTTONDOWN )
						{
							switch( i )
							{
								case BUTTON_NEW_GAME: gamestate.MainMenuScreen->ButtonNewgame = true; break;
								case BUTTON_OPTIONS: gamestate.MainMenuScreen->ButtonOptions = true; break;
							}
						}
					}					
				}
			}
		}


	}
	else
	{
		return;
	}
}


Game::Game()
{
	cout << "Resetting Level progress to 0..." << endl;
	LevelProgress = 0;
	Quit = false;

	// Setup of the application icons
	SDL_WM_SetCaption("", "res/big.ico");
	SDL_WM_SetIcon(SDL_LoadBMP("res/small.bmp"), NULL);

	Init( Gfx.screen );
	
	gamestate.load_files();

	//new setclips function
	BCPlayer.SetClips();

	
	// New button stuff
	
	objRectangle(_Button["BName::RESUME_GAME"], 401, 130, 350, 45 );
	objRectangle(_Button["BName::LOAD_GAME" ], 401, 242, 350, 45 );
	objRectangle(_Button["BName::SAVE_GAME" ], 401, 298, 350, 45 );
	objRectangle(_Button["BName::OPTIONS" ], 401, 355, 350, 45 );
	objRectangle(_Button["BName::UNKNOWN" ], 401, 410, 350, 45 );
	objRectangle(_Button["BName::CREDITS "], 401, 465, 350, 45 );
	objRectangle(_Button["BName::QUIT_GAME" ], 401, 519, 350, 45 );
	objRectangle(_Button["BName::UNKNOWN" ], 0, 0, 0, 0 );
	objRectangle(_Button["BName::UNKNOWN" ], 0, 0, 0, 0 );


}

// loads all graphic files and all new files and the font
void Gamestate::load_files()
{
		std::ifstream file;
		file.open("graphics.txt");
		if (!file)
		{
			cout << "CFG: File couldn't be found!\n" << endl;
			exit(1);
		}


		std::string line;
		size_t lineNo = 0;
		while (std::getline(file, line))
		{
			lineNo++;
			std::string temp = line;

			if (temp.empty())
				continue;
		}

		file.close();

	m_srfCity = Gfx.Load_imageAlpha( "Graphics/srfCity.png", 0, 0, 0 );
	m_srfClouds = Gfx.Load_imageAlpha( "Graphics/srfClouds.png", 255, 255, 255 );
	m_srfBlack = Gfx.Load_imageAlpha( "Graphics/srfBlack.png", 0, 0, 0 );
	m_srfSky = Gfx.Load_imageAlpha( "Graphics/srfSky.png", 0, 0, 0 );
	BCPlayer._Surface = Gfx.Load_imageAlpha( "Graphics/demonSurface.png", 255, 255, 255 );
	m_srfEnemyZombie = Gfx.Load_imageAlpha( "Graphics/srfEnemyZombie.png", 255, 0, 255 );
	m_srfCrow = Gfx.Load_imageAlpha( "Graphics/srfCrow.png", 255, 255, 255 );
	m_srfBoss = Gfx.Load_imageAlpha( "Graphics/srfBoss.png", 255, 255, 255 );
	m_srfdemonLife = Gfx.Load_imageAlpha( "Graphics/srfdemonLife.png", 255, 255, 255 );
	m_srfdemonHealthAndFire = Gfx.Load_imageAlpha( "Graphics/srfdemonHealthAndFire.png", 0, 0, 0 );
	m_srfDragon = Gfx.Load_imageAlpha( "Graphics/srfDragon.png", 0, 0, 0 );
	m_srfStart = Gfx.Load_imageAlpha( "Graphics/srfStart.png", 237, 234, 214 );
	m_srfButtons = Gfx.Load_imageAlpha( "Graphics/srfButtons.png", 255, 255, 255 );
	m_srfIntro = Gfx.Load_imageAlpha( "Graphics/srfIntro.png", 255, 255, 255 );
	m_srfMorphing = Gfx.Load_imageAlpha( "Graphics/srfMorphing.png", 255, 255, 241 );
	m_srfOutro = Gfx.Load_imageAlpha( "Graphics/srfOutro.png", 0, 0, 0 );
	m_srfButton = Gfx.Load_imageAlpha( "Graphics/srfButton.png", 0, 0, 0 );
	m_srfHealth = Gfx.Load_imageAlpha( "Graphics/srfHealth.png", 0, 0, 0 );
	m_srfLaser = Gfx.Load_imageAlpha( "Graphics/srfLaser.png", 255, 255, 255 );
	m_srfCredits = Gfx.Load_imageAlpha( "Graphics/srfCredits.png", 255, 255, 255 );
	m_srfOptions = Gfx.Load_imageAlpha( "Graphics/srfOptions.png", 255, 255, 255 );
	
	MainMenuScreen = new MainMenu( 290,  m_srfStart, m_srfButtons );
	CreditsScreen = new Credits( 290,  m_srfCredits, m_srfButtons ); 
	name = new StringInput();

	setUpParallaxLayers();
}


// ----------------------------------------------------------------------------
// CreateBoss() - Creates the boss gives collisionCircle and pos
// ----------------------------------------------------------------------------
Boss * Gamestate::CreateBoss( int xPos, int yPos, int surface )
{
	Boss * temp = new Boss;
	temp->Surface = surface;
	temp->_Position.x = xPos;
	temp->_Position.y = yPos;

	temp->Radius = ( temp->BossWidth > temp->BossHeight ) ? temp->BossWidth / 2 : temp->BossHeight  / 2;

	return temp;
}

void Gamestate::ResetBoss()
{
}

void Gamestate::ResetEnemies()
{
	if(EnemyController.Enemies.size() != NULL )
	{
		EnemyController.Enemies.clear();
	}
}

// ----------------------------------------------------------------------------
// ResetObjects() - resets all object to its starting values
// ----------------------------------------------------------------------------
void Gamestate::ResetObjects()
{
	if( ObjectController.List_Coffins.size() != NULL )
	{
		ObjectController.List_Coffins.clear();
	}

	if( ObjectController.List_FireBalls.size() != NULL )
	{
		ObjectController.List_FireBalls.clear();
	}

	ObjectController.FrameHealth = 0;

	ObjectController.WhichLifeToShow = 0;
	
	return;
}

void Game::Audiotonic()
{
}
// ----------------------------------------------------------------------------
// Update() - Updates the whole game depending on which state it is in
// ----------------------------------------------------------------------------
void Game::Update( SDL_Event input, int iElapsedTime )
{
	Asteroid SnowAsteroid(0,0,0);
	// calling base class function instead of derived class
	SnowAsteroid.Asteroid::isActive();
	ObjectController.Report(SnowAsteroid);

	std::list<Bullet> test;

	ObjectController.RemoveActiveObjects();
	test = BulletController.GetBulletsByReference();
	//CollisionController.ListByReference(BulletController.GetBulletsByReference(),AnimalController.GetAnimal());

	CollisionController.Box(BulletController.GetBullets(), AnimalController.GetAnimal());
	cout << BCPlayer.GetAction() << endl;
	// WhereIsEnd is @ image width + screenwidth 800+5100
	//if( demon.WhereIsEnd >= 5700 ) 
	//if( gamestate.LevelProgress >= 0 )
	//{
	//	gamestate.GameCondition = GS_LEVEL1BOSS;
	//}

	// Check game state
	switch( gamestate.GameState.top() )
	{
		case MENU_MAIN_STATE:
			{
				gamestate.MainScreen(iElapsedTime);
			} break;
		case GAME_CREDITS_STATE:
			{
				gamestate.CreditScreen(iElapsedTime);
			} break;
		case GAME_OPTIONS_STATE:
			{
				gamestate.OptionScreen(iElapsedTime);
			} break;

		case GAME_LOADING_STATE:
			{
				gamestate.Loading();
			} break;
		case GAME_RUNNING_STATE:
			{
				Gfx.DrawParallaxLayers();
				Gfx.DrawObjects();
				Gfx.DrawSprite();
				Gfx.DrawScore(300,25);
				
				gamestate.DrawAllText();
			} break;
		case GAME_BOSS_STATE:
			{
				// handles events what the user does with the character
				//Handle_events( input );

				//draws layers
				//gamestate.drawParallaxLayers();
				//Gfx.DrawParallaxLayers();
				//gamestate.DrawObjects();
				//Gfx.DrawObjects();
				//gamestate.DrawBoss();
				//Gfx.DrawBoss();
				//gamestate.DrawAllText();
				//gamestate.DrawSprite();
				//Gfx.DrawSprite();
				Gfx.DrawScore(300,0);
				//gamestate.FLIP();
				//Gfx.FLIP();

				//if( gamestate.pBoss->BossDead == true )
				//{
				//	gamestate.GameCondition = GS_OUTRO;
				//}
			} break;
		case GAME_OUTRO_STATE:
			{
				gamestate.PlayOutro();
			} break;
		case GAME_PLAYER_DIED_STATE:
			{
				BCPlayer.Died();
			} break;
	}
}

// ----------------------------------------------------------------------------
// PlayOutro() - Plays the whole outro sequence
// ----------------------------------------------------------------------------
void Gamestate::PlayOutro()
{
	SDL_BlitSurface(	Gfx.GetSurface( gamestate.m_srfOutro ),
					&ScreenSize, Gfx.BackBuffer, &ScreenSize );
	Gfx.FLIP();

	SDL_Event input;

	while( gamestate.GameState.top() == GAME_OUTRO_STATE )
	{
		SDL_PollEvent( &input );
		if( input.type == SDL_KEYDOWN )
		{
  			switch( input.key.keysym.sym )
			{
			case SDLK_SPACE:
				//gamestate.State = MENU_MAIN_STATE;
				gamestate.GameState.push(MENU_MAIN_STATE);
				break;
			}
		}
	}

	gamestate.RestartGame();

	return;
}

// ----------------------------------------------------------------------------
// demonDied() - Plays the whole death sequence
// ----------------------------------------------------------------------------
//void Gamestate::demonDied()
//{
//	SDL_Rect srcRect = { 0, 0, 800, 600 };
//	SDL_Rect destRect = { 0, 0, 800, 600 };
//
//
//	SDL_Event input;
//
//	while( gamestate.GameCondition == GS_DEAD )
//	{
//		SDL_PollEvent( &input );
//		if( input.type == SDL_KEYDOWN )
//		{
//  			switch( input.key.keysym.sym )
//			{
//			case SDLK_SPACE:
//				gamestate.GameCondition = GS_INTRO;
//				break;
//			}
//		}
//		Gfx.DrawBackgroundBlack();
//		SDL_BlitSurface( Gfx.GetSurface( gamestate.m_srfOutro ),	&srcRect, Gfx.BackBuffer, &destRect );
//		//SDL_Color textColor = { 255, 255, 255 };
//		//SDL_Color textColor2 = { 0, 0, 0 };
//		//sprintf_s( gamestate.Text, 256, " Press Space For Menu " );
//		Gfx.srfText = TTF_RenderText_Shaded( Gfx.DefaultFont, " Press Space For Menu ", Gfx.WhiteRGB, Gfx.BlackRGB );
//		//gamestate.apply_surface( 250, 500, gamestate.textIntro, gamestate.BackBuffer );
//		Gfx.apply_surface( 250, 500, Gfx.srfText, Gfx.BackBuffer );
//		//gamestate.FLIP();
//		Gfx.FLIP();
//	}
//	
//	ListHighScore->sort( gamestate.name->str.c_str(), _Score );
//	ListHighScore->Save();
//
//	int demonDieState = 0;
//	outro = new Outro;
//	int Letter = 0;
//	int LetterWidth = 0;
//	int Line = 0;
//	int Counter = 0;
//	string DeathTalks = " Your sole is mine fallen one, At this rate all the souls of the earth will be mine soon!!! Ha Ha Ha ";
//	string DeathTalkSlow[ 8 ];
//
//	SDL_Color textColor = { 255, 255, 255 };
//	SDL_Event input;
//
//	SDL_Color Stone = { 105, 105, 136 };
//	SDL_Color StoneFront = { 0, 0, 0, 255 };
//
//	bool PlayDeadAnimation = true; 
//	while( PlayDeadAnimation == true )
//	{
//		SDL_PollEvent( &input );
//		if( input.type == SDL_KEYDOWN )
//		{
//			switch( input.key.keysym.sym )
//			{
//			case SDLK_SPACE:
//				demonDieState = 2;
//				break;
//			}
//		}
//
//		DrawBackgroundBlack();
//		gamestate.DrawAllText();
//		switch( demonDieState )
//		{
//		case 0:
//			{
//				if( timer.Timer_ShowDead > 10 )
//				{
//					demonDieState = 1;
//					timer.Timer_ShowDead = 0.0f;
//				}
//
//				timer.Timer_ShowDead++;
//				//SDL_BlitSurface(	Gfx.GetSurface( outro->surface ), &outro->ClipsOutro[ 0 ],
//				//					gamestate.BackBuffer, &outro->ClipsOutro[ 1 ] );
//				break;
//			}
//		case 1:
//			{
//				if( Counter > 5 )
//				{
//					Counter = 0;
//
//					if( Letter < DeathTalks.length() )
//					{
//						Letter++;
//					}
//					else
//					{
//						demonDieState = 2;
//					}
//
//					DeathTalkSlow[ Line ] += DeathTalks[ Letter ];
//					for( int i = 0; i < 8; i++ )
//					{
//						DeathSurface[ 0 ] = TTF_RenderText_Solid( gamestate.font, DeathTalkSlow[ i ].c_str(), textColor );
//						gamestate.apply_surface( 300, i * 40, DeathSurface[ 0 ], gamestate.BackBuffer );
//					}
//
//					LetterWidth++;
//				}
//						
//				SDL_BlitSurface(	Gfx.GetSurface( outro->surface ), &outro->ClipsOutro[ 0 ],
//									gamestate.BackBuffer, &outro->ClipsOutro[ 1 ] );
//
//				for( int i = 0; i < 8; i++ )
//				{
//					DeathSurface[ 0 ] = TTF_RenderText_Solid( gamestate.font, DeathTalkSlow[ i ].c_str(), textColor );
//					gamestate.apply_surface( 300, i * 40, DeathSurface[ 0 ], gamestate.BackBuffer );
//				}
//				
//					
//				if( LetterWidth > 20 )
//				{
//					LetterWidth = 0;
//					Line++;
//				}
//
//				Counter++;
//				break;
//			}
//		case 2:
//			{
//				PlayDeadAnimation = false;
//				break;
//			}	
//		}
//		
//		DeathSurface[ 1 ] = TTF_RenderText_Blended( gamestate.font, gamestate.name->str.c_str(), StoneFront );
//		gamestate.apply_surface( 160, 450, DeathSurface[ 1 ], gamestate.BackBuffer );
//		//gamestate.FLIP();
//		Gfx.FLIP();
//	}
//
//	gamestate.GameCondition = GS_INTRO;	
//
//}

// ----------------------------------------------------------------------------
// DrawAllText() - draws all text thats currently shown on the screen.
// ----------------------------------------------------------------------------
void Gamestate::DrawAllText()
{
	if( State == GAME_STORY_STATE || State == GAME_PLAYER_DIED_STATE )
	{
		//SDL_Color textColor = { 255, 255, 255 };
		//SDL_Color textColor2 = { 0, 0, 0 };
		
		if( State == GAME_PLAYER_DIED_STATE )
		{
			//sprintf_s( gamestate.Text, 256, " Press Space For Menu " );
			Gfx.srfText = TTF_RenderText_Shaded( Gfx.DefaultFont, " Press Space For Menu ", Gfx.WhiteRGB, Gfx.BlackRGB );
			//gamestate.apply_surface( 200, 500, gamestate.textIntro, gamestate.BackBuffer );
			Gfx.apply_surface( 200, 500, Gfx.srfText, Gfx.BackBuffer );
		}
		else
		{
			//sprintf_s( gamestate.Text, 256, " Press Space To Start " );		
			Gfx.srfText = TTF_RenderText_Shaded( Gfx.DefaultFont, " Press Space To Start ", Gfx.WhiteRGB, Gfx.BlackRGB );
			//gamestate.apply_surface( 200, 500, gamestate.textIntro, gamestate.BackBuffer );
			Gfx.apply_surface( 200, 500, Gfx.srfText, Gfx.BackBuffer );
		}
	}
	else
	{
		//The color of the font
		//SDL_Color textColor = { 251, 245, 32 };

		// print out the score
		//sprintf_s(gamestate.Text, 256, "%i ", gamestate.GetScore() );		
		//gamestate.srfText = TTF_RenderText_Solid( gamestate.font, Text, Gfx.WhiteRGB );
		//gamestate.apply_surface( 100, 20, gamestate.srfText, gamestate.BackBuffer );
		//Gfx.apply_surface( 100, 20, gamestate.srfText, Gfx.BackBuffer );
	}
}

// ----------------------------------------------------------------------------
// Loading() - draws the loading screen on the screen. Dragon dancing
// ----------------------------------------------------------------------------
void Gamestate::Loading()
{
}

// ----------------------------------------------------------------------------
// DrawBoss() - hmmm.
// ----------------------------------------------------------------------------
//void Gamestate::DrawBoss()
//{
//	//SDL_Rect srcRect = { 0, 0, 150, 300 };
//	//gamestate.pBoss->UpdateFrame();
//	//				SDL_BlitSurface( Gfx.GetSurface( gamestate.pBoss->Surface ), 
//	//				&gamestate.pBoss->GetClips( gamestate.pBoss->GetFrame() ),
//	//							gamestate.BackBuffer, &srcRect );
//	//Sleep(100);
//	//gamestate.pBoss->UpdateBoss();
//	gBoss.Update();
//}

// ----------------------------------------------------------------------------
// GetSurface() - hmmm.
// ----------------------------------------------------------------------------
//SDL_Surface * Gamestate::GetSurface( int WhichSurface )
//{
//	return m_surfaceList[ WhichSurface ];	
//}

// ----------------------------------------------------------------------------
// GetSurface() - gives backbuffer to destination buffer streches and all
// ----------------------------------------------------------------------------
//void Gamestate::FLIP()
//{
//	//SDL_Rect srcRect = { 0, 0, gamestate.BackBuffer->w, gamestate.BackBuffer->h };
//	//SDL_Rect destRect = { 0, 0, gamestate.SCREEN_WIDTH, gamestate.SCREEN_HEIGHT };
//					
//	//gamestate.PasteScreenToAnother( srcRect, destRect );
//	Gfx.PasteScreenToAnother( SDL_GetVideoSurface()->clip_rect, SDL_GetVideoSurface()->clip_rect);
//	//flips screen
//	if( SDL_Flip( gamestate.screen ) == -1)
//	{
//		gamestate.GameOK = false;
//	}
//}

// ----------------------------------------------------------------------------
// MainScreen() - Draws the mainscreen, checks conditions. MenuScreen
// ----------------------------------------------------------------------------
void Gamestate::MainScreen(int iElapsedTime)
{
	SDL_BlitSurface( Gfx.GetSurface( MainMenuScreen->surface ), &SDL_GetVideoSurface()->clip_rect, Gfx.BackBuffer, &SDL_GetVideoSurface()->clip_rect );
	

	//SDL_FillRect(Gfx.GetSurface( TitleScreen->surface),&TitleScreen->ButtonClips[ 0 ],SDL_MapRGB(Gfx.GetSurface( TitleScreen->surface)->format,255,0,255) );
	//SDL_FillRect(Gfx.GetSurface( TitleScreen->surface),&TitleScreen->ButtonClips[ 1 ],SDL_MapRGB(Gfx.GetSurface( TitleScreen->surface)->format,255,0,255) );
	//SDL_FillRect(Gfx.GetSurface( TitleScreen->surface),&TitleScreen->ButtonClips[ 2 ],SDL_MapRGB(Gfx.GetSurface( TitleScreen->surface)->format,255,0,255) );
	//SDL_FillRect(Gfx.GetSurface( TitleScreen->surface),&TitleScreen->ButtonClips[ 3 ],SDL_MapRGB(Gfx.GetSurface( TitleScreen->surface)->format,255,0,255) );
	//SDL_FillRect(Gfx.GetSurface( TitleScreen->surface),&TitleScreen->ButtonClips[ 4 ],SDL_MapRGB(Gfx.GetSurface( TitleScreen->surface)->format,255,0,255) );
	//SDL_FillRect(Gfx.GetSurface( TitleScreen->surface),&TitleScreen->ButtonClips[ 5 ],SDL_MapRGB(Gfx.GetSurface( TitleScreen->surface)->format,255,0,255) );
	//SDL_FillRect(Gfx.GetSurface( TitleScreen->surface),&TitleScreen->ButtonClips[ 6 ],SDL_MapRGB(Gfx.GetSurface( TitleScreen->surface)->format,255,0,255) );
	//SDL_FillRect(Gfx.GetSurface( TitleScreen->surface),&TitleScreen->ButtonClips[ 7 ],SDL_MapRGB(Gfx.GetSurface( TitleScreen->surface)->format,255,0,255) );
	//SDL_FillRect(Gfx.GetSurface( TitleScreen->surface),&TitleScreen->ButtonClips[ 8 ],SDL_MapRGB(Gfx.GetSurface( TitleScreen->surface)->format,255,0,255) );
	stringstream ss;
	ss << iElapsedTime;
	string str = "MainScreen @ ";
	str.append(ss.str());
	SDL_Surface * srfElapsedTime;
	srfElapsedTime = TTF_RenderText_Solid( Gfx.DefaultFont, str.c_str(), Gfx.WhiteRGB );
	Gfx.apply_surface( 0, 0, srfElapsedTime, Gfx.BackBuffer );
	if( MainMenuScreen->ButtonNewgame == true )
	{
		gamestate.GameState.push(GAME_RUNNING_STATE);
		MainMenuScreen->ButtonNewgame = false;
	}
	return;

	//SDL_Surface * Surface_Credits = NULL;
	//SDL_Surface * Surface_HighScore = NULL;

	//ParallaxLayer  * MyParaBackGround;
	//MyParaBackGround = Parallax->getLayer( TitleScreen->surface );

	/*
	for( int i = 0; i < 4; i++ )
	{
		//SDL_FillRect(gamestate.GetSurface( TitleScreen->SurfaceButt),&TitleScreen->DestClips[ i ],SDL_MapRGB(gamestate.GetSurface( TitleScreen->SurfaceButt)->format,255,0,255) );
			
		SDL_BlitSurface(	Gfx.GetSurface( TitleScreen->SurfaceButt ), 
							&TitleScreen->ButtonClips[ i ],
							Gfx.BackBuffer, &TitleScreen->DestClips[ i ] ); 
	}*/



	//gamestate.apply_surface( 200, 400, srfEnter, gamestate.BackBuffer );
	//if( gamestate.TitleScreen->ButtonHighScore == true )
	//{
	//	ListHighScore->sort( gamestate.name->str.c_str(), gamestate.GetScore() );
	//	ListHighScore->Save();

	//	delete ListHighScore;
	//	ListHighScore = new FillHighScore;

	//	for( int i = 0; i < 5; i++ )
	//	{
	//		char temp[ 256 ];
	//		sprintf_s(	temp, 256, "Name: %s Score: %i", ListHighScore->list[ i ].name.c_str(), ListHighScore->list[ i ].Score );	
	//		gamestate.HighScoreList[ i ] = temp;
	//	}
	//}

	//if( TitleScreen->ButtonOptions == false )
	//{
	//	for( int i = 0; i < 4; i++ )
	//	{
	//		//SDL_FillRect(gamestate.GetSurface( TitleScreen->SurfaceButt),&TitleScreen->DestClips[ i ],SDL_MapRGB(gamestate.GetSurface( TitleScreen->SurfaceButt)->format,255,0,255) );
	//		
	//		SDL_BlitSurface(	Gfx.GetSurface( TitleScreen->SurfaceButt ), 
	//							&TitleScreen->ButtonClips[ i ],
	//							gamestate.BackBuffer, &TitleScreen->DestClips[ i ] ); 
	//	}
	//}
	//else if( TitleScreen->ButtonHighScore == true )
	//{
	//	/*
	//	SDL_BlitSurface(	gamestate.GetSurface( TitleScreen->SurfaceButt ),
	//						&TitleScreen->ButtonClips[ 4 ],
	//						gamestate.BackBuffer, &TitleScreen->DestClips[ 8 ] ); 	
	//						*/

	//	SDL_BlitSurface(	Gfx.GetSurface( TitleScreen->SurfaceButt ),
	//						&TitleScreen->ButtonClips[ 7 ],
	//						gamestate.BackBuffer, &TitleScreen->DestClips[ 7 ] ); 

	//	SDL_Color textColor = { 0,0,0 };
	//	
	//	for( int i = 0; i < 5; i++ )
	//	{
	//		Surface_HighScore =		TTF_RenderText_Solid( gamestate.font, 
	//								gamestate.HighScoreList[ i ].c_str(), textColor );
	//		apply_surface( 50, 270 + i * 40, Surface_HighScore, gamestate.BackBuffer );
	//	}

	//}
	//else if( TitleScreen->ButtonCredits == true )
	//{
	//	SDL_BlitSurface(	Gfx.GetSurface( TitleScreen->SurfaceButt ),
	//						&TitleScreen->ButtonClips[ 8 ],
	//						gamestate.BackBuffer, &TitleScreen->DestClips[ 9 ] ); 

	//	SDL_BlitSurface(	Gfx.GetSurface( TitleScreen->SurfaceButt ),
	//						&TitleScreen->ButtonClips[ 7 ],
	//						gamestate.BackBuffer, &TitleScreen->DestClips[ 7 ] ); 

	//	
	//	SDL_Color textColor = { 0,255,0 };
	//	
	//
	//	Surface_Credits =		TTF_RenderText_Solid( gamestate.font, 
	//							" A Risk Production ", textColor );
	//	apply_surface( 50, 270, Surface_Credits, gamestate.BackBuffer );
	//}
	//else
	//{
	//	for( int i = 0; i < 8; i++ )
	//	{
	//		if( i == 2 || i == 0 || i == 3 )
	//		{
	//										
	//		}
	//		else
	//		{
	//			SDL_BlitSurface(	Gfx.GetSurface( TitleScreen->SurfaceButt ), &TitleScreen->ButtonClips[ i ],
	//								gamestate.BackBuffer, &TitleScreen->DestClips[ i ] ); 
	//		}
	//	}
	//}
	//
	//if( TitleScreen->ButtonNewgame == true )
	//{
	//	gamestate.GameCondition = GS_INTROSTORY;
	//	TitleScreen->ButtonNewgame = false;

	//	if( Surface_Credits != NULL )
	//	{
	//		SDL_FreeSurface( Surface_Credits );
	//	}
	//	if( Surface_HighScore != NULL )
	//	{
	//		SDL_FreeSurface( Surface_HighScore );
	//	}

	//	gamestate.RestartGame();
	//}
	//if( TitleScreen->ButtonQuit == true )
	//{
	//	gamestate.GameOK = false;
	//	TitleScreen->ButtonQuit == false;
	//}

	//if( TitleScreen->ButtonBack == true )
	//{
	//	TitleScreen->ButtonOptions = false;
	//	TitleScreen->ButtonBack = false;
	//	TitleScreen->ButtonHighScore = false;
	//	TitleScreen->ButtonCredits = false;
	//	TitleScreen->ButtonBack = false;
	//} 					  	   
	//
}
// ----------------------------------------------------------------------------
// CreditScreen() - Draws the credit screen
// ----------------------------------------------------------------------------
void Gamestate::CreditScreen(int iElapsedTime)
{
		std::cout << "Rendering credits screen like a god!!!!" << endl;
	SDL_BlitSurface( Gfx.GetSurface( CreditsScreen->surface ), &SDL_GetVideoSurface()->clip_rect, Gfx.BackBuffer, &SDL_GetVideoSurface()->clip_rect );
	
	stringstream ss;
	ss << (float)iElapsedTime / 1000000;
	string str = "CreditsScreen @";
	str.append(ss.str());
	SDL_Surface * srfElapsedTime;
	srfElapsedTime = TTF_RenderText_Solid( Gfx.DefaultFont, str.c_str(), Gfx.WhiteRGB );
	Gfx.apply_surface( 0, 0, srfElapsedTime, Gfx.BackBuffer );
	if( CreditsScreen->ButtonNewgame == true )
	{
		gamestate.GameState.push(GAME_RUNNING_STATE);
		CreditsScreen->ButtonNewgame = false;
	}
	return;
}
// ----------------------------------------------------------------------------
// OptionScreen() - Draws the option screen
// ----------------------------------------------------------------------------
void Gamestate::OptionScreen(int iElapsedTime)
{
		std::cout << "Rendering options screen like a god!!!!" << endl;
	SDL_BlitSurface( Gfx.GetSurface( CreditsScreen->surface ), &SDL_GetVideoSurface()->clip_rect, Gfx.BackBuffer, &SDL_GetVideoSurface()->clip_rect );
	
	stringstream ss;
	ss << (float)iElapsedTime / 1000000;
	string str = "OptionsScreen @";
	str.append(ss.str());
	SDL_Surface * srfElapsedTime;
	srfElapsedTime = TTF_RenderText_Solid( Gfx.DefaultFont, str.c_str(), Gfx.WhiteRGB );
	Gfx.apply_surface( 0, 0, srfElapsedTime, Gfx.BackBuffer );
	if( CreditsScreen->ButtonNewgame == true )
	{
		gamestate.GameState.push(GAME_RUNNING_STATE);
		CreditsScreen->ButtonNewgame = false;
	}
	return;
}
// ----------------------------------------------------------------------------
// EnterName() - checks for input demon name
// ----------------------------------------------------------------------------
void Gamestate::EnterName()
{
	SDL_BlitSurface( Gfx.GetSurface( MainMenuScreen->surface ), &ScreenSize, Gfx.BackBuffer, &ScreenSize );
	SDL_Surface * srfEnter = 0;
	//srfEnter = TTF_RenderText_Solid( Gfx.DefaultFont, gamestate.demonName.c_str(), Gfx.BlackRGB );
	Gfx.apply_surface( 200, 400, srfEnter, Gfx.BackBuffer );

	if( gamestate.name->handle_input(  ) == false )
	{
		gamestate.GameState.push(GAME_STORY_STATE);
	}
}

void Gamestate::RestartGame()
{
	BCPlayer.Reset();
	ResetBoss();
	ResetEnemies();
	ResetObjects();
	ResetRest();

	timer.RestartTimers();

	//BCPlayer.Initiatedemon( BCPlayer.Surface, GROUND_X, 0, demonHEIGHT, demonWIDTH );
	//gamestate.Score = 0;
	//gamestate.m_parallax = 0;
	gamestate.Parallax = 0.0f;
	gamestate.State = MENU_MAIN_STATE;
}

void Gamestate::ResetRest()
{
	if( MainMenuScreen != NULL )
	{
		delete MainMenuScreen;
	}
	if( CreditsScreen != NULL )
	{
		delete CreditsScreen;
	}
}

// Frees surfaces and deletes thing thats not NULL
void Gamestate::EndAll()
{
	for( int i = 0; i < ParallaxBG->getLayerCount(); i++ )
	{
		SDL_FreeSurface( m_surfaceList[ i ] );
	}
	if( gamestate.name != NULL )
	{
		delete name;
	}
}

void Game::Cleanup()
{
	TTF_Quit();
	SDL_Quit();
}

// loads image with chosen value to not show
//int Gamestate::Load_imageAlpha( std::string filename, int r, int g, int b )
//{
//	//temp storage for the image loaded
//	SDL_Surface * loadedimage = NULL;
//
//	//optimized image for storage and flipping
//	SDL_Surface * optimizedImage = NULL;
//
//	//load image 
//	loadedimage = IMG_Load( filename.c_str() );
//
//	//if something went wrong
//	if( loadedimage != NULL )
//	{
//		//create an optimized image 
//		optimizedImage = SDL_DisplayFormat( loadedimage );
//
//		//free old image
//		SDL_FreeSurface( loadedimage );
//	}
//
//	if(optimizedImage != NULL)
//	{
//		SDL_SetColorKey(optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, r, g, b ) );
//	}
//	
//	int index = findAvailableIndex();
//	if( index == -1 )
//	{
//		return -1;
//	}
//
//	m_surfaceList[ index ] = optimizedImage;
//	return index;
//}

// inits sdl, font and videomode
bool Game::Init(SDL_Surface * &screen)
{
	Gfx.screen = 0;


	//initialize all SDL subystems
/*	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1)
	{
		return false;
	} */


	//set up the screen
	Gfx.screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	SDL_Rect** modes;
	int i;
   
    /* Get available fullscreen/hardware modes */
    modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);
    
    /* Check if there are any modes available */
    if (modes == (SDL_Rect**)0) 
	{
        printf("No modes available!\n");
       exit(-1);
	}
   
   /* Check if our resolution is restricted */
   if (modes == (SDL_Rect**)-1) 
   {
       printf("All resolutions available.\n");
   }
   else
   {
       /* Print valid modes */
       printf("Available Modes\n");
       for (i=0; modes[i]; ++i)
         printf("  %d x %d\n", modes[i]->w, modes[i]->h);
   }

	if( screen == NULL )
	{
		return false;
	}

/*	if( TTF_Init() == -1 )
	{
		return false;
	}
	else
	{
		gamestate.font = TTF_OpenFont( "cour.ttf", 28 );
	} */
	
	//set window caption
	SDL_WM_SetCaption( " S-TYPE ", NULL);

	/* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
       as expected by OpenGL for textures */
    Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	   on the endianness (byte order) of the machine */

	rmask = 0x00000000;
	gmask = 0x00000000;
	bmask = 0x00000000;
	amask = 0x00000000;

	Gfx.BackBuffer = SDL_CreateRGBSurface( SDL_HWSURFACE, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h, SDL_GetVideoSurface()->format->BitsPerPixel,
								   rmask, gmask, bmask, amask);
	
	if( Gfx.BackBuffer == NULL )
	{
		return false;																							
	}
	return true;
}

// ----------------------------------------------------------------------------
// drawParallaxLayers() - renders parallax-layers to backbuffer and gives it to Main Surface
// ----------------------------------------------------------------------------
//void Gamestate::drawParallaxLayers()
//{
//	if( gamestate.GameCondition != GS_LEVEL1BOSS && gamestate.GameCondition != GS_OUTRO )
//	{
//		/*
//		if( demon.xVel >= STARTSCROLLING - 50 )
//		{
//			// Updating the background to scroll when character is moving
//			if( demon.isMovingRight )
//			{
//				gamestate.m_paralax += 20;
//			}
//			else if( demon.isMovingLeft )
//			{
//				gamestate.m_paralax -= 20;
//			}
//		}
//		*/
//		gamestate.CreateAll();
//	}
//	else
//	{
//		//if( !BossStart )
//		//{
//		//	//demon.demonHunter = true;
//		//	//demon.SmallHunter = false;
//		//	gamestate.pBoss = gamestate.CreateBoss( SDL_GetVideoSurface()->w - 180, GROUND_Y - 210, m_srfBoss );
//		//	BossStart = true;
//		//}
//	}
//
//		//// Draw parallax layers
//		ParallaxLayer  * MyParaBackGround;
//		MyParaBackGround = Parallax->getLayer( 0 );
//
//		SDL_Rect scRect = { 0, 0,	MyParaBackGround->m_width, 
//									MyParaBackGround->m_height };
//
//		SDL_Rect dtRect = {	0, 0, MyParaBackGround->DW, MyParaBackGround->DH };
//
//		SDL_BlitSurface( Gfx.GetSurface(MyParaBackGround->m_surface), &scRect, gamestate.BackBuffer, &dtRect );
//		//SDL_BlitSurface( m_surfaceList[ MyParaBackGround->m_surface ], &scRect, gamestate.BackBuffer, &dtRect ); 
//
//		//gamestate.stretchPicToBackBuffer( MyParaBackGround, scRect, dtRect );
//
//		int x = 0;
//		for( int i = 1; i < Parallax->getLayerCount(); ++i )
//		{		
//			// Calc rects
//			MyParaBackGround = Parallax->getLayer( i );
//			if( MyParaBackGround->m_surface == m_srfClouds )
//			{
//				MyParaBackGround->AnimClouds += 100.0f * gamestate.DeltaTime;
//
//				//////// Calc parallax position
//				x = (int)( MyParaBackGround->m_parallax * (float)( +MyParaBackGround->AnimClouds ) );  
//				if( x < 0 )	// neg -> pos
//				{
//					x *= -1;	// invert sign, because % only works with positive integers
//					x = MyParaBackGround->m_width - (x % MyParaBackGround->m_width);
//				}
//				else
//				{
//					x %= MyParaBackGround->m_width;
//				}
//
//				x -= MyParaBackGround->m_width;	// move one back
//			}
//			else
//			{
//
//				//////// Calc parallax position
//				x = (int)( MyParaBackGround->m_parallax * (float)( +gamestate.m_parallax ) );  
//				if( x < 0 )	// neg -> pos
//				{
//					x *= -1;	// invert sign, because % only works with positive integers
//					x = MyParaBackGround->m_width - (x % MyParaBackGround->m_width);
//				}
//				else
//				{
//					x %= MyParaBackGround->m_width;
//				}
//
//				x -= MyParaBackGround->m_width;	// move one back
//			}
//
//			for( int i = 0; i < 2; i++ )
//			{
//				SDL_Rect sourceRect = { 0 + x, MyParaBackGround->m_surfaceYOffset,
//										MyParaBackGround->m_width, MyParaBackGround->m_height };
//
//				SDL_Rect destinationRect = {	MyParaBackGround->DX, MyParaBackGround->DY, 
//												MyParaBackGround->DW, MyParaBackGround->DH };
//
//				SDL_BlitSurface( Gfx.GetSurface( MyParaBackGround->m_surface ), &sourceRect, gamestate.BackBuffer, &destinationRect ); 			
//				//SDL_BlitSurface( m_surfaceList[ MyParaBackGround->m_surface ], &sourceRect, gamestate.BackBuffer, &destinationRect ); 
//				
//				
//				x += MyParaBackGround->m_width;
//			}
//
//
//			MyParaBackGround->HowFarGone = MyParaBackGround->Xpos - MyParaBackGround->m_width;
//
//		}
		//gamestate.m_parallax += 500 * gamestate.DeltaTime;
//}

void Gamestate::CreateAll()
{
	AnimalController.Create_Animals();
	EnemyController.Create_Enemies();
	ObjectController.CreateObjects();
}

// draws MyFellow on the screen and changes animations
//void Gamestate::DrawSprite()
//{
//		//SDL_Rect demonDest = { demon.xPos, demon.yPos, demon.demon_Width, demon.demon_Height };
//		//demonDest = demon.GetPosition();
//		if( demon.isImmortal )
//		{
//			if( demon.AlphaImmortal < SDL_ALPHA_OPAQUE - 100 )
//			{
//				demon.AlphaImmortal += 50;
//			}
//			else
//			{
//				demon.AlphaImmortal -= 50;
//			}
//			SDL_SetAlpha( Gfx.GetSurface( demon.demonSurface ), SDL_SRCALPHA | SDL_RLEACCEL, demon.AlphaImmortal );
//		}
//		else
//		{
//			SDL_SetAlpha( Gfx.GetSurface( demon.demonSurface ), SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE );
//		}
//
//		
//			//Current_Frame = demon.Updatedemon();
//			//demon.Updatedemon();
//			demon.Update();
//		
//			// Rendering demon Character
//			/*
//			SDL_BlitSurface(	m_surfaceList[ demon.demonSurface ], 
//								&demon.AnimationArrays[ Current_AnimArray ][ Current_Frame ],
//								gamestate.BackBuffer, &demonDest );*/
//			if( CurrentFrame == 3 )
//			{
//				CurrentFrame = 0;
//			}
//			
//			SDL_BlitSurface(	Gfx.GetSurface( demon.demonSurface ), 
//				&demon.AnimationArrays[ demon.GetState() ][ ++CurrentFrame ],
//				gamestate.BackBuffer, &demon.GetPosition() );
//			/*
//			SDL_BlitSurface(	m_surfaceList[ emon.demonSurface ], 
//				&demon.AnimationArrays[ demon.GetState() ][ ++CurrentFrame ],
//								gamestate.BackBuffer, &demonDest );*/
//
//
//			PreviousFrame = CurrentFrame;
//}


void Gamestate::AddTick()
{
	 //float Speed = 1000.0f * ( gamestate.DeltaTime / 1000.0f );
	 //UpdateAnimationSpeed += Speed;
	UpdateAnimationSpeed += ( 1000.0f * ( gamestate.DeltaTime / 1000.0f ) );
}

//void Gamestate::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
//{
//    //Holds offsets
//    SDL_Rect offset;
//    
//    //Get offsets
//    offset.x = x;
//    offset.y = y;
//    
//    //Blit
//    SDL_BlitSurface( source, clip, destination, &offset );
//}

//void Gamestate::stretchPicToBackBuffer( ParallaxLayer * layer, SDL_Rect srcRect, SDL_Rect destRect )	
//{			
//
//	int srcWidth = srcRect.w - srcRect.x;
//	int srcHeight = srcRect.h - srcRect.y,
//		DestWidth = destRect.w - destRect.x,
//		DestHeight = destRect.h - destRect.y;
//
//	SDL_LockSurface( gamestate.BackBuffer );
//	SDL_LockSurface( m_surfaceList[ layer->m_surface ] );
//
//	int dstPitch = gamestate.BackBuffer->pitch;
//	int pitch = m_surfaceList[ layer->m_surface ]->pitch;
//
//	DWORD * dst = ( DWORD * )gamestate.BackBuffer->pixels;
//
//	DWORD * src = ( DWORD * )m_surfaceList[ layer->m_surface ]->pixels;
//
//	float scaleWidth = srcWidth / ( float )DestWidth;
//	float scaleHeight = srcHeight / ( float )DestHeight; 
//
//
//	float fSrcX = srcRect.x,
//		  fSrcY = srcRect.y;
//
//	for(int y = destRect.y;  y < destRect.y + destRect.h; y++ )
//	{
//		fSrcX = 0;
//
//		for(int x = destRect.x ;  x < destRect.x + destRect.w ; x++)
//		{
//			dst[ ( y * dstPitch / 4) + ( x ) ] = src[ int( ( fSrcY ) * ( pitch / 4 ) + int( fSrcX ) )];
//
//			fSrcX += scaleWidth;
//		}
//
//		fSrcY += scaleHeight;	
//	}
//
//	SDL_UnlockSurface( gamestate.BackBuffer );
//	SDL_UnlockSurface( m_surfaceList[ layer->m_surface ] );
//
//}

//void Gamestate::stretchBlit( ParallaxLayer * layer, SDL_Rect srcRect, SDL_Rect destRect )	
//{			
//	SDL_LockSurface( gamestate.BackBuffer );
//	SDL_LockSurface( m_surfaceList[ layer->m_surface ] );
//
//	int dstPitch = gamestate.BackBuffer->pitch;
//	int pitch = m_surfaceList[ layer->m_surface ]->pitch;
//
//	DWORD * dst = ( DWORD * )gamestate.BackBuffer->pixels;
//
//	DWORD * src = ( DWORD * )m_surfaceList[ layer->m_surface ]->pixels;
//
//	float scaleWidth = srcRect.w / ( float )destRect.w;
//	float scaleHeight = srcRect.h / ( float )destRect.h; 
//
//
//	float fSrcX = srcRect.x,
//		  fSrcY = srcRect.y;
//
//	for(int y = destRect.y;  y < destRect.y + destRect.h; y++ )
//	{
//		fSrcX = srcRect.x;
//
//		for(int x = destRect.x ;  x < destRect.x + destRect.w ; x++)
//		{
//			dst[ (y * dstPitch / 4) + (x) ] = src[ int((fSrcY) * (pitch / 4) + int(fSrcX) )];
//
//			fSrcX += scaleWidth;
//		}
//		fSrcY += scaleHeight;	
//	}
//
//	SDL_UnlockSurface( gamestate.BackBuffer );
//	SDL_UnlockSurface( m_surfaceList[ layer->m_surface ] );
//
//}

//void Gamestate::PasteScreenToAnother( SDL_Rect srcRect, SDL_Rect destRect )	
//{
//	SDL_LockSurface( gamestate.screen );
//	SDL_LockSurface( gamestate.BackBuffer );
//
//	int dstPitch = gamestate.screen->pitch;
//	int pitch = gamestate.BackBuffer->pitch;
//
//	DWORD * dst = ( DWORD * )gamestate.screen->pixels;
//	DWORD * src = ( DWORD * )gamestate.BackBuffer->pixels;
//
//	float scaleWidth = gamestate.BackBuffer->w / ( float )destRect.w;
//	float scaleHeight = gamestate.BackBuffer->h / ( float )destRect.h; 
//
//
//	float fSrcX = 0.0f,
//		  fSrcY = 0.0f;
//
//	for(int y = destRect.y;  y < destRect.y + destRect.h; y++ )
//	{
//		fSrcX = 0.0f;
//
//		for(int x = destRect.x ;  x < destRect.x + destRect.w ; x++)
//		{
//			dst[ (y * dstPitch / 4) + (x) ] = src[ int(fSrcY) * (pitch / 4) + int(fSrcX) ];
//
//			fSrcX += scaleWidth;
//		}
//
//		fSrcY += scaleHeight;	
//	}
//
//	SDL_UnlockSurface( gamestate.screen );
//	SDL_UnlockSurface( gamestate.BackBuffer );
//}

// ----------------------------------------------------------------------------
// setUpParallaxLayers() - initializes parallax-struct
// ----------------------------------------------------------------------------
void Gamestate::setUpParallaxLayers()
{
	// Create background
	ParallaxBG = new ParallaxBackground();
	ParallaxBG->createLayers( 10 );

	//Firstlayer
	ParallaxBG->setLayer( 0, 0.0f, m_srfBlack, 
						0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h, 0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h );

	////sky
	ParallaxBG->setLayer( 1, 0.0f, m_srfSky, 
						0, SDL_GetVideoSurface()->w, 400, 0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h );

	// trees must be here otherwise division by zero currently
	ParallaxBG->setLayer( 2, 0.7f, m_srfSky, 
						0, 1172, 170, 0, 370, SDL_GetVideoSurface()->w, 170 ); 

	//clouds
	ParallaxBG->setLayer(	3, 0.5f, m_srfClouds, 
						0, SDL_GetVideoSurface()->w, 38, 0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h );

	ParallaxBG->setLayer(	4, 0.4f, m_srfClouds, 
						38, SDL_GetVideoSurface()->w, 87, 0, 38, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h );

	ParallaxBG->setLayer(	5, 0.3f, m_srfClouds, 
						126, SDL_GetVideoSurface()->w, 46, 0, 126, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h );

	ParallaxBG->setLayer(	6, 0.2f, m_srfClouds, 
						172, SDL_GetVideoSurface()->w, 21, 0, 172, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h );

	ParallaxBG->setLayer(	7, 0.1f, m_srfClouds, 
						193, SDL_GetVideoSurface()->w, 12, 0, 193, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h );

	ParallaxBG->setLayer( 8, 0.7f, m_srfCity, 
						0, 5100, 535, 0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h );

	ParallaxBG->setLayer(	9, 1.0f, m_srfCity, 
						540, 5100, 60, 0, 535, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h );
}


