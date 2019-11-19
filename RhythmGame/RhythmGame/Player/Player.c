#include "Player.h"
#include "../Global.h"
#include "../Console/Console.h"
#include "../Map/Map.h"
#include "../Enemy/Enemy.h"
#include "../States/StateMachine.h"
#include "../Clock/Clock.h"

Player player;
CONSOLECOLOR color;
static double factor;
static double velocity;

// CHECKS
static int EaseCheck;
static int EaseBool;
static bool canAttack;

// TIMERS
static double dt; // Euler Movement
static double EaseTimer; // Ease
static double dashCooldownTimer; // Cooldown of Dash
static double speedUpTimer; // Duration of Dash
static double invulTimer; // Invulnerable timer

/* Internal functions */

void _MovePlayer();
// Checks if player is out of border
void _CheckBorder();
// Updates Timer
void _UpdateState();


void Player_Init()
{
	player = (Player){
		.direction = STAY,
		.position.x = 100, .position.y = 100,
		.position.eulerX = 100.0, .position.eulerY = 100.0,
		.health = 10,
		.PlayerSprite = Text_CreateSprite(),
		.PlayerSprite.printColor = bRED,
		.state = Normal,
	};

	Text_Init(&player.PlayerSprite, "..//RhythmGame//$Resources//skull.txt");

	factor = 0.0;
	EaseBool = false;
	EaseCheck = SlowDown;
	EaseCheck = true;
	canAttack = true;
	velocity = 0.04;

	// Timers
	EaseTimer = 0.0; // Ease
	dashCooldownTimer = 0.0;
	speedUpTimer = 0.0;
	invulTimer = 0.0;
}

void Player_Update()
{
	_MovePlayer();
	_CheckBorder();
	_UpdateState();
}

void Player_Render()
{
	switch (player.state)
	{
	case Normal:
		color = BLUE;
		break;
	case Invul:
		color = RED;
		break;
	default:
		color = CYAN;
		break;
	}

	for (int i = 0; i < SPRITE_SIZE; i++)
		player.PlayerSprite.printColor[i] = color;

	Text_Render(&player.PlayerSprite, Map_GetShakeFactor(RIGHT)/2, 0);

	// Debug origin
	Console_SetRenderBuffer_CharColor(player.position.x, player.position.y, '+', CYAN);
}

void Player_SetVel(DIRECTION dir, EASEMOVEMENT EaseC)
{
	player.direction = dir;
	EaseCheck = EaseC;
}

int Player_GetHealth()
{
	return player.health;
}

double Player_GetEaseFactor()
{
	return factor;
}

int Player_GetDirection()
{
	return player.direction;
}

void Player_Dash()
{
	// If cdTimer is running
	if (dashCooldownTimer > 0.0)
		return;

	factor = 1;
	speedUpTimer = 100.0;
	invulTimer = 200.0;
	player.state = Dash;
	dashCooldownTimer = 1000.0;
}

void Player_ExtendDash()
{
	if (!canAttack) return;
	canAttack = false;
	factor = 1;
	speedUpTimer += 20.0;
	player.state = ExDash;
	invulTimer += 100.0;
	Map_Shake(UP, 50.0, 2);
}

void Player_Damage()
{
	if (invulTimer > 0.0) return;

	player.health--;
	player.state = Invul;
	invulTimer = 2000.0;
	Map_Shake(RIGHT, 50.0, 5);
}

sprite *Player_GetSprite()
{
	return &player.PlayerSprite;
}

PLAYERSTATE Player_GetState()
{
	return player.state;
}

void _UpdateState()
{
	dt = Clock_GetDeltaTime();
	EaseTimer += Clock_GetDeltaTime();

	if (invulTimer > 0.0)
		invulTimer -= Clock_GetDeltaTime();
	if (speedUpTimer > 0.0)
		speedUpTimer -= Clock_GetDeltaTime();
	if (dashCooldownTimer > 0.0)
		dashCooldownTimer -= Clock_GetDeltaTime();

	switch (player.state)
	{
	case Invul:
		if (invulTimer < 0.0)
			player.state = Normal;
		break;
	case Dash:
		if (speedUpTimer < 0.0)
			player.state = Normal;
		break;
	case ExDash:
		if (speedUpTimer < 0.0)
			player.state = Normal;
		break;
	default:
		if (dashCooldownTimer < 0.0)
			canAttack = true;
		break;
	}

	if (EaseTimer >= 25.0)
	{
		if (EaseCheck == SpeedUp)
			factor += 0.1;
		else if (EaseCheck == SlowDown)
			factor -= 0.1;

		if (factor < 0)
			factor = 0.0;
		else if (factor > 1.0)
			factor = 1.0;
		EaseTimer -= EaseTimer;
	}
}

void _CheckBorder()
{
	if (player.position.eulerX < (MAP_OFFSET + 1)) player.position.eulerX = MAP_OFFSET + 1;
	if (player.position.eulerY < (MAP_OFFSET + 1)) player.position.eulerY = MAP_OFFSET + 1;
	if (player.position.eulerX > (GAME_WIDTH - MAP_OFFSET - BOXSIZE)) player.position.eulerX = GAME_WIDTH - MAP_OFFSET - BOXSIZE;
	if (player.position.eulerY > (GAME_HEIGHT - MAP_OFFSET - BOXSIZE)) player.position.eulerY = GAME_HEIGHT - MAP_OFFSET - BOXSIZE;
}

void _MovePlayer()
{
	switch (player.state)
	{
	case ExDash:
		velocity = 0.15;
		break;
	case Dash:
		velocity = 0.3;
		break;
	default:
		velocity = 0.02;
		break;
	}
	switch (player.direction)
	{
	case TOPLEFT:
		player.position.eulerX += -1 * dt * velocity * factor;
		player.position.eulerY += -1 * dt * velocity * factor;
		break;
	case TOPRIGHT:
		player.position.eulerX += 1 * dt * velocity * factor;
		player.position.eulerY += -1 * dt * velocity * factor;
		break;
	case BOTTOMRIGHT:
		player.position.eulerX += 1 * dt * velocity * factor;
		player.position.eulerY += 1 * dt * velocity * factor;
		break;
	case BOTTOMLEFT:
		player.position.eulerX += -1 * dt * velocity * factor;
		player.position.eulerY += 1 * dt * velocity * factor;
		break;
	case UP: player.position.eulerY += -1 * dt * velocity * factor; break;
	case RIGHT: player.position.eulerX += 1 * dt * velocity * factor; break;
	case DOWN: player.position.eulerY += 1 * dt * velocity * factor; break;
	case LEFT: player.position.eulerX += -1 * dt * velocity * factor; break;
	default: break;
	}

	player.position.x = player.position.eulerX;
	player.position.y = player.position.eulerY;

	Text_Move(&player.PlayerSprite, player.position.x, player.position.y);
}