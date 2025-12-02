#include "Player.h"
#include <glm/glm.hpp>

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
			setPlayerState(PlayerState::Walking);
			pushWalkingActionDirection(action);
		}
		
		processPlayerState();
		action = popStackSafely(mPlayerActionsStack);
	}
}

void Player::processCamera() {
	if(auto pCharacter = getCharacter()) {
		std::cout << "doing cam\n";
		if(pCharacter->getPos() == mLastCharacterPos) // I think a compare op is cheaper than two copy-sets with single addition
			return;
		std::cout << "setting pos!\n";
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

void Player::processPlayerState() {
	switch(getPlayerState()) {
	case PlayerState::Walking:
		invokeWalking();
	}
}
