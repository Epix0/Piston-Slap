#include "Player.h"

void Player::pushAction(PlayerAction action) {
	mPlayerActionsStack.push(action);
}

bool Player::isThisActionWalking (Player::PlayerAction vAction) const {
	return mHorizontalMovementActions.contains(vAction);
}

void Player::invokeWalking() {
	switch(popStackSafely(mWalkingActionDirections)) {
	case PlayerAction::Forward:

	}
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

void Player::processPlayerState() {
	switch(getPlayerState()) {
	case PlayerState::Walking:
		invokeWalking();
	}
}
