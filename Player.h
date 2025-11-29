#pragma once
#include "WorldObject.h"
#include <memory>
#include <vector>

class Player : public WorldObject {
public:
	enum class PlayerAction {
		Forward,
		Backward,
		Left,
		Right,
		Jump,
		Crouch,

		_total_enums
	};

	// Push an action enum to the input stack
	void pushAction();

	// Pops an enum action from the input stack
	PlayerAction popAction();
private:
	std::unique_ptr<std::vector<PlayerAction>> mPlayerActionsStack;
};
