#include "Player.h"

void Player::pushAction(PlayerAction action) {
	mPlayerActionsStack.push(action);
}

Player::PlayerAction Player::popAction() {
	if(mPlayerActionsStack.size() == 0)
		return PlayerAction::None;
	else {
		auto action = mPlayerActionsStack.top();
		mPlayerActionsStack.pop();
		return action;
	}
}

bool Player::isThisActionWalking (Player::PlayerAction vAction) const {
	return mHorizontalMovementActions.contains(vAction);
}
