#pragma once
#include "WorldObject.h"
#include <stack>
#include "Instance.h"
#include <memory>
#include <set>
#include <map>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.hpp"

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
		Idle,
		Walking, // any horizontal movement
		Jumping,
		Falling,
		Landing,
		Flying, // with character
		FlyingDetached // no character, just camera
	};

	Player(std::shared_ptr<Camera> pClientCamera) :
		mHeight(0.f),
		mPlayerState(Player::PlayerState::Idle),
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
		mwkpCharacterInstance{},
		mWalkingActionDirections{},
		mpCamera(pClientCamera),
		mLastCharacterPos(0.f),
		mIsFlyingDetached(false)
	{};

	// Push an action enum to the input stack
	void pushAction(PlayerAction action);

	// Evaluate whether @vAction is considered to trigger walking state
	bool isThisActionWalking(Player::PlayerAction vAction) const;

	void invokeWalking();

	void setCharacter(std::weak_ptr<Instance> pInstance);
	std::shared_ptr<Instance> getCharacter() const;
	inline PlayerState getPlayerState() const { return mPlayerState; };
	inline const auto& getKeybindsToActions() const { return mKeybindsToActions; };
	inline auto getCamera() { return mpCamera; };
	void setFlyingDetached(bool bOption);

	void processWalkingDirsToDetachedCamera();

	float mHeight;
	bool mIsFlyingDetached;

	// Game loop functions vv

	void processInput();

	// Pushes updates to the player camera, which typically is positional updates
	void processCamera();
private:
	PlayerState mPlayerState;
	const std::set<PlayerAction> mHorizontalMovementActions;
	std::stack<PlayerAction> mPlayerActionsStack;

	typedef int GLFW_KEY;
	std::map<GLFW_KEY, PlayerAction> mKeybindsToActions;

	// The physical thing of this Player
	std::weak_ptr<Instance> mwkpCharacterInstance;
	std::stack<PlayerAction> mWalkingActionDirections;
	std::shared_ptr<Camera> mpCamera;
	glm::vec3 mLastCharacterPos;

	inline void setPlayerState(PlayerState newState) { mPlayerState = newState; };
	inline void pushWalkingActionDirection(PlayerAction action) { mWalkingActionDirections.push(action); };

	template<typename tStackType>
	static tStackType popStackSafely(std::stack<tStackType>& stack);

	void processPlayerState();
};

template<typename tStackType>
inline tStackType Player::popStackSafely(std::stack<tStackType>& stack) {
	if(stack.size() == 0)
		return tStackType{};
	else {
		tStackType element = stack.top();
		stack.pop();
		return element;
	}
}
