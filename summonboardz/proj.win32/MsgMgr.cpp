#include "MsgMgr.h"

void MsgMgr::dealMsg(CCTouch* touch, eMsgType type)
{
	mCurBeganTime = mTimer.getInterval();
	if (type == eMsgType_Began)
	{
		
	} 
	else if (type == eMsgType_Move)
	{
		mLastType = eMsgType_Move;

		//todo:
	}
	else if (type == eMsgType_End)
	{
		if (mCurBeganTime - mLastBeganTime <= DBClickJudgement &&
			mLastType == eMsgType_Click)
		{
			mLastType = eMsgType_DClick;

			//todo:
		}
		else if (mLastType == eMsgType_Began)
		{
			mLastType == eMsgType_Click;

			//todo:
		}
	}
}