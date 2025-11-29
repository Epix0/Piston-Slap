#pragma once
#include "WorldObject.h"
#include <stack>

// The Player is 

class Player : public WorldObject {
public:
	Player() : mHeight(0) {};

	enum class PlayerAction {
		None = 1,
		Forward,
		Backward,
		Left,
		Right,
		Jump,
		Crouch,

		_total_enums
	};

	// Push an action enum to the input stack
	void pushAction(PlayerAction action);

	// Pops an enum action from the input stack
	PlayerAction popAction();

	size_t mHeight;
private:
	std::stack<PlayerAction> mPlayerActionsStack;
};
