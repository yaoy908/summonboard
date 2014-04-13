#ifndef __MSG_MGR_H__
#define __MSG_MGR_H__

#include "cocos2d.h"
#include "Total.h"

USING_NS_CC;

enum eMsgType
{
	eMsgType_None = 0,
	eMsgType_Began,
	eMsgType_Move,
	eMsgType_End,
	eMsgType_Click,
	eMsgType_DClick,
	eMsgType_Press,
	eMsgType_Num,
};

const float DBClickJudgement = 1;
const float PressJudgement = 2;

class MsgMgr
{
public:
	MsgMgr() :mLastPos(ccp(0, 0)), mLastBeganTime(0), mCurBeganTime(0), mLastType(eMsgType_None) {}
	void dealMsg(CCTouch* touch, eMsgType type);

public:
	CCPoint mLastPos;
	CCTimer mTimer;
	eMsgType mLastType;
	float mLastBeganTime;
	float mCurBeganTime;
};

#endif