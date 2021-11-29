#include "../include/game.h"

Game::Game() {
    this->state = GameState::MENU;
    this->registerDb = new Database<Register>("register.txt", 2);
    this->registerDb->read();
    Level level;
}

Game::~Game(){}

GameState Game::getState()
{
    return state;
}

void Game::setState(GameState state_)
{
	this->state = state_;
}

void Game::clearScreen() 
{
    std::cout << "\033[2J\033[1;1H";
}

void Game::delay(int a)
{
  int add=0;
  int time;
  
  time = a * 10000000;

  for (int i = 0; i < time; i++)
  {
    add *= i;
    add++;
    add++;
  }
}

void Game::input(){
  if(kbhit()){
    switch(getch()){
      case 'a':
        // state = MENU;
        // std::cout << "*********************" << std::endl;
        return;
      default: 
        break;
    }
  }
}

void Game::run() {
  Menu *const mainMenu = new MainMenu();
  auto menuState = mainMenu;
  while (state != END) {
    delay(1);
    switch (this->state) {
      case GameState::MENU: {
        clearScreen();
        while (menuState->getMenuState() != OUT)
        {
          switch (menuState->getMenuState())
          {
            case MenuState::MAIN_MENU: {
              menuState = mainMenu;
              menuState->setMenuState(MenuState::MAIN_MENU);
              menuState->run();
              break;
            }
            case MenuState::PLAY: {
              this->state = GameState::RUN;
              menuState->setMenuState(MenuState::OUT);
              break;
            }
            case MenuState::EXIT: {
              menuState->setMenuState(MenuState::OUT);
              this->state = GameState::END;
              break;
            }
            default: {
              break;
            }
          }
        }
        break;
      }
      case GameState::RUN: {
        // input();
        clearScreen();
        if (level.draw())
          this->state = GameState::GAMEOVER;
        break;
      }
      case GameState::GAMEOVER:{
        clearScreen();
        menuState = new GameOver;
        this->state = GameState::GAMEOVER;
        menuState->setMenuState(MenuState::GAME_OVER);
        menuState->run();
        if (menuState->getMenuState() == MenuState::SAVE)
        {
          this->save();
          this->state = GameState::END;
        }
        else if (menuState->getMenuState() == MenuState::EXIT)
          this->state = GameState::END;
        break;
      }
      default:{
          break;
      }
    }
  }
}

void Game::save()
{
  std::cout << "----" << std::endl;
  Register *reg = new Register("ROOT", "1");
  this->registerDb->add(reg);
  this->registerDb->write();
}
