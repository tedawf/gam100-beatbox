#include "StateMachine.h"
#include "Console/Console.h"
#include "Game.h"

static GameState currentState = State_Default;
static GameState reqState = State_Default;

void StateMachine_Start()
{
	if (currentState != reqState)
	{
		switch (currentState)
		{
		case State_MainMenu: MainMenu_ExitState(); break;
		case State_Game: Game_ExitState(); break;
		case State_GameOver: GameOver_ExitState(); break;
		}

		currentState = reqState;

		switch (currentState)
		{
		case State_MainMenu: MainMenu_EnterState(); break;
		case State_Game: Game_EnterState(); break;
		case State_GameOver: GameOver_EnterState(); break;
		}
	}
}

void StateMachine_changeState(GameState newState)
{
	reqState = newState;
}

void StateMachine_ProcessInput()
{
	switch (currentState)
	{
	case State_MainMenu: MainMenu_ProcessInput(); break;
	case State_Game: Game_ProcessInput(); break;
	case State_GameOver: GameOver_ProcessInput(); break;
	}
}

void StateMachine_Update()
{
	switch (currentState)
	{
	case State_MainMenu: MainMenu_Update(); break;
	case State_Game: Game_Update(); break;
	case State_GameOver: GameOver_Update(); break;
	}
}

void StateMachine_Render()
{
	Console_ClearRenderBuffer();

	switch (currentState)
	{
	case State_MainMenu: MainMenu_Render(); break;
	case State_Game: Game_Render(); break;
	case State_GameOver: GameOver_Render(); break;
	}

	Console_SwapRenderBuffer();
}