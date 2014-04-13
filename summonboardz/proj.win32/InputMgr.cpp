#include "InputMgr.h"

InputMgr::InputMgr()
: mLastStatus(eInputNone)
, mCurTime(0)
, mLastTime(0)
, mLastPos(ccp(0, 0))
{

}

void InputMgr::dealInput(CCTouch* pTouch, inputStatus status)
{
	switch (status)
	{
	case eInputBegan:
		{
			if (mLastStatus != eInputClick)
			{
				mLastStatus = eInputBegan;
			}
		}
		break;
	case eInputMove:
		{
			mLastStatus =  eInputMove;
		}
		break;
	case eInputEnd:
		{
			if (mLastStatus == eInputBegan /*|| mLastStatus == eInputPress*/)
			{
				mLastStatus = eInputClick;
			}
			else if (mLastStatus == eInputClick)
			{
				mCurTime = mTimer.getInterval();
				if (mCurTime - mLastTime < DBClickJudgement)
				{
					mLastStatus = eInputDBClick;
					mLastTime = mCurTime;
				}
			}
		}
		break;
	case default:
		break;
	}

	//
}

void InputMgr::update(float dt)
{
	if (mLastStatus == eInputBegan)
	{
		mCurTime = mTimer.getInterval();
		if (mCurTime - mLastTime > PressClickJudgement)
		{
			mLastStatus = eInputPress;

			//
		}
	}
}