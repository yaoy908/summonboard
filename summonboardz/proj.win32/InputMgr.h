#ifndef __INPUT_MGR_H__
#define __INPUT_MGR_H__

#include "cocos2d.h"
#include "Total.h"

USING_NS_CC;

const float DBClickJudgement = 1;
const float PressClickJudgement = 2;

class InputMgr
{
public:
	InputMgr();
	~InputMgr() {}

	static InputMgr* ptr()
	{
		static InputMgr mgr;
		return &(mgr);
	}

	void dealInput(CCTouch* pTouch, inputStatus status);
	void update(float dt);	//press not implied.

public:
	inputStatus mLastStatus;
	float mCurTime;
	float mLastTime;
	CCPoint mLastPos;
	CCTimer mTimer;
};

#endif