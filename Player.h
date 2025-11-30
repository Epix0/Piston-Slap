#pragma once
#include "WorldObject.h"
#include <stack>
#include "Instance.h"
#include <memory>

// The Player is 

class Player : public WorldObject {
public:
	Player() : mHeight(0.f) {};

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

	float mHeight;
private:
	std::stack<PlayerAction> mPlayerActionsStack;

	// The physical thing of this Player
	std::shared_ptr<Instance> mCharacterInstance;
};
