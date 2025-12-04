#include "Player.h"
#include <glm/glm.hpp>
#include "Camera.hpp"
#include "World.h"
#include <iostream>

void Player::pushAction(PlayerAction action) {
	mPlayerActionsStack.push(action);
}

bool Player::isThisActionWalking (Player::PlayerAction vAction) const {
	return mHorizontalMovementActions.contains(vAction);
}

void Player::invokeWalking() {
	//switch(popStackSafely(mWalkingActionDirections)) {
	//case PlayerAction::Forward:

	//}
}

void Player::processInput() {
	auto action = popStackSafely(mPlayerActionsStack);
	while(action != PlayerAction::None) {
		if(isThisActionWalking(action)) {
			pushWalkingActionDirection(action);

		}
		
		processPlayerState();
		action = popStackSafely(mPlayerActionsStack);
	}
}

void Player::processCamera() {
	if(mIsFlyingDetached) {
		// any new offsets should go here. For now, it's in the state case machine
		return;
	}

	if(auto pCharacter = getCharacter()) {
		if(pCharacter->getPos() == mLastCharacterPos) // I think a compare op is cheaper than two copy-sets with single addition
			return;

		mLastCharacterPos = pCharacter->getPos();
		mpCamera->Position = mLastCharacterPos + glm::vec3(0.f, mHeight, 0.f);
	}
}

void Player::setCharacter(std::weak_ptr<Instance> wkpInstance) {
	mwkpCharacterInstance = wkpInstance;
	if(auto pCharacter = mwkpCharacterInstance.lock()) {
		mpCamera->Position = pCharacter->getPos() + glm::vec3(0.f, mHeight, 0.f);
	}
}

std::shared_ptr<Instance> Player::getCharacter() const {
	return mwkpCharacterInstance.lock();
}

void Player::setFlyingDetached(bool bOption) {
	mIsFlyingDetached = bOption;
	setPlayerState(bOption ? PlayerState::FlyingDetached : PlayerState::Falling);
}

void Player::processWalkingDirsToDetachedCamera() {
	auto direction = popStackSafely(mWalkingActionDirections);
	float delta = World::getWorld()->mDeltaTime;
	using CamDir = Camera::Camera_Movement;

	while(direction != PlayerAction::None) {
		switch(direction) {
		case PlayerAction::Forward:
			mpCamera->ProcessKeyboard(CamDir::FORWARD, delta);
			break;
		case PlayerAction::Right:
			mpCamera->ProcessKeyboard(CamDir::RIGHT, delta);
			break;
		case PlayerAction::Left:
			mpCamera->ProcessKeyboard(CamDir::LEFT, delta);
			break;
		case PlayerAction::Backward:
			mpCamera->ProcessKeyboard(CamDir::BACKWARD, delta);
			break;
		}

		direction = popStackSafely(mWalkingActionDirections);
	}
}

void Player::processPlayerState() {
	switch(getPlayerState()) {
	case PlayerState::Walking:
		invokeWalking();
		break;
	case PlayerState::Idle:
		invokeWalking();
		break;
	case PlayerState::FlyingDetached:
		processWalkingDirsToDetachedCamera();
		break;
	}
}
