// #include "spriteMgr.h"
// 
// SpriteMgr::~SpriteMgr()
// {
// 	CCObject* pObject = NULL;
// 	CCSprite* pS = NULL;
// 	CCARRAY_FOREACH(array, pObject)
// 	{
// 		pS = dynamic_cast<CCSprite*>(pObject);
// 		if (pS)
// 		{
// 			pS->release();
// 		}
// 		
//	}
// }
// 
// CCSprite* SpriteMgr::createSprite()
// {
// 	CCSprite* pS = NULL;
// 	pS = new CCSprite;
// 	if (pS)
// 	{
// 		pS->setTag(mAcc);
// 		++mAcc;
// 
// 		pS->retain();
// 		mSpritesCon.addObject(pS);
// 
// 		return pS;
// 	}
// }