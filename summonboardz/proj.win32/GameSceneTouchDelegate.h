#ifndef __GAMESCENE_TOUCH_DELEGATE_H__
#define __GAMESCENE_TOUCH_DELEGATE_H__

#include "cocos2d.h"
#include "GameScene.h"

USING_NS_CC;

extern class GameScene;

class GameSceneTouchDelegate
{
public:
	static GameSceneTouchDelegate* ptr()
	{
		static GameSceneTouchDelegate data;
		return &data;
	}

	void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
	void ccTouchesMoved(CCSet* pTouches, CCEvent* event);
	void ccTouchesEnded(CCSet* pTouches, CCEvent* event);

public:
	GameScene* mpGameScene;
};

#endif