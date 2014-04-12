#include "GameSceneTouchDelegate.h"
#include "PlayerData.h"
#include "BattleSystem.h"

void GameSceneTouchDelegate::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	CCTouch* touch = (CCTouch *)pTouches->anyObject();
	//CCPoint pt = touch->getLocation();

	if (mpGameScene->mGameState == eWaitPlayerAct)
	{
		int index = mpGameScene->mUnitsLayer.getTouchedSpriteIndex(touch);
		if (index >= 0)
		{
			mpGameScene->mGameState = eMemberPick;
			mpGameScene->mUnitsLayer.setPickedInfo(index);
			mpGameScene->mRouteLayer.updateAllVisible(&(PlayerData::ptr()->mMovableXY));
		}
	}
}

void GameSceneTouchDelegate::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
	CCTouch* touch = (CCTouch *)pTouches->anyObject();

	if (mpGameScene->mGameState == eMemberPick)
	{
		mpGameScene->mUnitsLayer.updatePickedSpritePosition(touch);
	}
}

void GameSceneTouchDelegate::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
	CCTouch* touch = (CCTouch *)pTouches->anyObject();

	if (mpGameScene->mGameState == eMemberPick)
	{
		bool canDrop = mpGameScene->mUnitsLayer.dropSprite(touch);
		if (canDrop)
		{
			//attack.
			if (BattleSystem::ptr()->playerCanAttack())
			{
				BattleSystem::ptr()->playerFirstAttack();
				mpGameScene->mGameState = ePlayerAttack;
			}
			else
			{
				//round change.
				BattleSystem::ptr()->changeRound(eRoundAi);
			}
		}
		else
		{
			mpGameScene->mGameState = eWaitPlayerAct;
		}
		
		PlayerData::ptr()->clearPickedInfo();
		mpGameScene->mRouteLayer.invisibleAll();
	}
}