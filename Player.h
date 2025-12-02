#pragma once
#include "WorldObject.h"
#include <stack>
#include "Instance.h"
#include <memory>
#include <set>
#include <map>
#include <GLFW/glfw3.h>

class Player : public WorldObject {
public:
	typedef std::shared_ptr<Player> PlayerPtr;

	enum class PlayerAction : unsigned char {
		None = 0,
		Forward,
		Backward,
		Left,
		Right,
		Jump,
		Crouch
	};

	enum class PlayerState : unsigned char {
		None = 0,
		Walking, // any horizontal movement
		Jumping,
		Falling,
		Landing,
		Flying,
	};

	Player() :
		mHeight(0.f),
		mPlayerState(Player::PlayerState::None),
		mHorizontalMovementActions{
			PlayerAction::Forward, PlayerAction::Backward, PlayerAction::Right, PlayerAction::Left
		},
		mPlayerActionsStack{},
		mKeybindsToActions{
			{GLFW_KEY_W, PlayerAction::Forward},
			{GLFW_KEY_A, PlayerAction::Left},
			{GLFW_KEY_D, PlayerAction::Right},
			{GLFW_KEY_S, PlayerAction::Backward},
		},
		mpCharacterInstance(nullptr)
	{};

	// Push an action enum to the input stack
	void pushAction(PlayerAction action);

	// Pops an enum action from the input stack
	PlayerAction popAction();

	// Evaluate whether @vAction is considered to trigger walking state
	bool isThisActionWalking(Player::PlayerAction vAction) const;

	inline PlayerState getPlayerState() const { return mPlayerState; };

	float mHeight;
private:
	PlayerState mPlayerState;

	const std::set<PlayerAction> mHorizontalMovementActions;

	std::stack<PlayerAction> mPlayerActionsStack;

	typedef int GLFW_KEY;
	std::map<GLFW_KEY, PlayerAction> mKeybindsToActions;

	// The physical thing of this Player
	std::shared_ptr<Instance> mpCharacterInstance;

};
