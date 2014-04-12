#include "GameScene.h"
#include "GameSceneTouchDelegate.h"
#include "Utils.h"
#include "BattleSystem.h"
#include <math.h>

using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////

FloorLayer::FloorLayer()
{
	this->initWithFile("block.png");
	this->setAnchorPoint(ccp(0, 0));
};

FloorLayer::~FloorLayer()
{
	//this->removeAllChildrenWithCleanup(false);
};

bool FloorLayer::init()
{
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			CCSprite* sprite = CCSprite::create();
			sprite->initWithFile("block.png");
			sprite->setAnchorPoint(ccp(0, 0));
			sprite->setPosition(ccp(i * blockWidth, j * blockWidth));
			this->addChild(sprite);
		}
	}

	this->setPosition(ccp(mapWidth * blockWidth - width, 0));

	return true;
}

void FloorLayer::initWithRounds(int i)
{
	if (i <= 0)
	{
		return;
	}
	
	x = mapWidth * i;
	y = mapHeight;

	width = x * blockWidth;
	height = y * blockWidth;

	init();
}

RouteLayer::~RouteLayer()
{
	//this->removeAllChildrenWithCleanup(false);
}

bool RouteLayer::init()
{
	for (int i = 0;  i < mapWidth; ++i)
	{
		for (int j = 0; j < mapHeight; ++j)
		{
			mpSprites[i][j] = CCSprite::create();
			mpSprites[i][j]->initWithFile("route.png");
			mpSprites[i][j]->setAnchorPoint(ccp(0, 0));
			mpSprites[i][j]->setPosition(getPositionByXY(i, j));
			this->addChild(mpSprites[i][j]);

			mpSprites[i][j]->setVisible(false);
		}
	}

	this->setPosition(ccp(0, 160));

	return true;
}

void RouteLayer::setBlockVisible(int x, int y)
{
	if (x < 0 || x > mapWidth || y < 0 || y > mapHeight)
	{
		return;
	}
	
	mpSprites[x][y]->setVisible(true);
}

void RouteLayer::updateAllVisible(CCArray* array)
{
	CCObject* pObject = NULL;
	CCARRAY_FOREACH(array, pObject)
	{
		XY* pChild = dynamic_cast<XY*>(pObject);
		(mpSprites[pChild->x][pChild->y])->setVisible(true);
	}
}

void RouteLayer::invisibleAll()
{
	for (int i = 0;  i < mapWidth; ++i)
	{
		for (int j = 0; j < mapHeight; ++j)
		{
			mpSprites[i][j]->setVisible(false);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

Unit::~Unit()
{
	if (mpSelf)
	{
		mpSelf->removeAllChildrenWithCleanup(false);

		if (mAttackDir.count())
		{
			CCObject* pObject = NULL;
			CCARRAY_FOREACH(&mAttackDir, pObject)
			{
				//mAttackDir.removeObject(pObject);
				CCSprite* pChild = dynamic_cast<CCSprite*>(pObject);
				//pChild->release();		//maybe problem.
				safe_release(pChild)
			}
		}

		safe_release(mpHpBG)
		safe_release(mpHp)
	}
	
	//mpSelf->release();		//maybe problem.
	safe_release(mpSelf)
}

void Unit::initWithMonsterData(MonsterData* pData)
{
	mId = pData->id;

	//mpSelf = new CCSprite;		//this caused heap and rtti problems.
	mpSelf = CCSprite::create();
	mpSelf->retain();
	mpSelf->initWithFile(pData->name.c_str());
	mpSelf->setAnchorPoint(ccp(0, 0));
	mpSelf->setPosition(getPositionByXY(pData->xy.x, pData->xy.y));

	for (int i = 0; i < eAttackDirectionMax; ++i)
	{
		if (pData->attack[i] == eAttackTwice)
		{
			CCSprite* p = new CCSprite;
			//CCSprite* p = CCSprite::create();
			p->initWithFile("daa.png");

			mAttackDir.addObject(p);
			mpSelf->addChild(p, 2);

			p->setPosition(getPositionWithAttackDir((attackDirection)i));
			p->setRotation(getRotationWithAttackDir((attackDirection)i));
		}
		else if (pData->attack[i] == eAttackOnce)
		{
			CCSprite* p = new CCSprite;
			//CCSprite* p = CCSprite::create();
			p->initWithFile("aa.png");

			mAttackDir.addObject(p);
			mpSelf->addChild(p, 2);

			p->setPosition(getPositionWithAttackDir((attackDirection)i));
			p->setRotation(getRotationWithAttackDir((attackDirection)i));
		}	
	}

	mpHpBG = CCSprite::create("hpbg.png");
	mpHpBG->retain();
	mpHpBG->setAnchorPoint(ccp(0, 0));
	mpHpBG->setPosition(ccp(40, 40));
	mpSelf->addChild(mpHpBG, 0);
	
	mpHp = CCSprite::create("hp.png");
	mpHp->retain();
	mpHp->setAnchorPoint(ccp(0, 0));
	mpHp->setPosition(ccp(40 + 3, 40 + 3));
	mpSelf->addChild(mpHp, 1);

	mHpLen = 80 - 6;
}

void Unit::setHpBar(float rate)
{
	if (rate <= 0)
	{
		mpHp->setVisible(false);
	}
	else
	{
		float oldRate = mpHp->getScaleX();
		mpHp->setScaleX(rate / oldRate);
	}
}

void Unit::setVisible(bool b)
{
	mpSelf->setVisible(b);
	mpHpBG->setVisible(b);
	mpHp->setVisible(b);

	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&mAttackDir, pObject)
	{
		//mAttackDir.removeObject(pObject);
		CCSprite* pChild = dynamic_cast<CCSprite*>(pObject);
		if (pChild)
		{
			pChild->setVisible(b);
		}
		
	}
	//CCArray mAttackArrow;

	
}

//////////////////////////////////////////////////////////////////////////

UnitsLayer::~UnitsLayer()
{
	//this->removeAllChildrenWithCleanup(false);
	for (int i = 0; i < playerUnitsCount; ++i)
	{
		//mpPlayerUnits[i]->release();		//maybe problem.
		safe_release(mpPlayerUnits[i])
	}

	if (mMonsterArray.count() == 0)
	{
		return;
	}
	
	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&mMonsterArray, pObject)
	{
		mMonsterArray.removeObject(pObject);
		MonsterData* pChild = dynamic_cast<MonsterData*>(pObject);
		//pChild->release();		//maybe problem.
		safe_release(pChild)
	}
}

bool UnitsLayer::init()
{	
	initPlayerUnits();
	initEnemyUnits();

	this->setPosition(ccp(0, 160));

	return true;
}

void UnitsLayer::initPlayerUnits()
{
	for (int i = 0; i < playerUnitsCount; ++i)
	{
		MonsterData* pData = &(PlayerData::ptr()->mMonster[i]);
		mpPlayerUnits[i] = new Unit;
		mpPlayerUnits[i]->initWithMonsterData(pData);

 		this->addChild(mpPlayerUnits[i]->mpSelf);
	}

	mpPlayerUnits[0]->mpSelf->setPosition(getPositionByXY(3, 2));
	mpPlayerUnits[1]->mpSelf->setPosition(getPositionByXY(3, 1));
	mpPlayerUnits[2]->mpSelf->setPosition(getPositionByXY(3, 0));
	mpPlayerUnits[3]->mpSelf->setPosition(getPositionByXY(2, 0));
}

void UnitsLayer::initEnemyUnits()
{
	int curRound = DungeonData::ptr()->mCurRound;
	if (curRound < 0 || curRound >= DungeonData::ptr()->mRounds.count())
	{
		return;
	}
	
	RoundData* pData = dynamic_cast<RoundData*>(DungeonData::ptr()->mRounds.objectAtIndex(curRound));
	if (!pData || pData->mMonsters.count() <= 0)
	{
		return;
	}
	
	for (int i = 0; i < pData->mMonsters.count(); ++i)
	{
		MonsterData* pMob = dynamic_cast<MonsterData*>(pData->mMonsters.objectAtIndex(i));

		Unit* pUnit = new Unit;
		pUnit->initWithMonsterData(pMob);
		mMonsterArray.addObject(pUnit);

		this->addChild(pUnit->mpSelf);
	}
	
}

Unit* UnitsLayer::getEnemyById(int id)
{
	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&mMonsterArray, pObject)
	{
		Unit* pChild = dynamic_cast<Unit*>(pObject);
		if (pChild && pChild->mId == id)
		{
			return pChild;
		}
	}

	return NULL;
}

int UnitsLayer::getTouchedSpriteIndex(CCTouch* touch)
{
	CCSprite* p = NULL;
	for (int i = 0; i < playerUnitsCount; ++i)
	{
		p = mpPlayerUnits[i]->mpSelf;
		CCRect rect = p->boundingBox();  
		if (rect.containsPoint(p->getParent()->convertTouchToNodeSpace(touch)))
		{  
			return i;
		}
	}

	return -1;
}

void UnitsLayer::setPickedInfo(int index)
{
	CCSprite* p = mpPlayerUnits[index]->mpSelf;
	PlayerData::ptr()->mOldPositon = p->getPosition();

	//calculate movable XY
	PlayerData::ptr()->mPickedIndex = index;
	PlayerData::ptr()->generateMovableXY(index);

	//update routeLayer display.
	
}

void UnitsLayer::updatePickedSpritePosition(CCTouch* touch)
{
	CCSprite* sprite = mpPlayerUnits[PlayerData::ptr()->mPickedIndex]->mpSelf;
	CCPoint pt = sprite->getParent()->convertTouchToNodeSpace(touch);
	pt.x -= blockWidth / 2;
	pt.y -= blockWidth / 2;
	sprite->setPosition(pt);
}

bool UnitsLayer::dropSprite(CCTouch* touch)
{
	CCSprite* sprite = mpPlayerUnits[PlayerData::ptr()->mPickedIndex]->mpSelf;
	CCPoint pt = sprite->getParent()->convertTouchToNodeSpace(touch);
	XY xy = getXYByPosition(pt);

	if (PlayerData::ptr()->isMovableXY(xy))
	{
		sprite->setPosition(getPositionByXY(xy.x, xy.y));
		PlayerData::ptr()->updateMonstersXY(xy);
		DungeonData::ptr()->updateDish();

		return true;
	}
	else
	{
		sprite->setPosition(PlayerData::ptr()->mOldPositon);
		return false;
	}
}

void UnitsLayer::addTip(int n, ccColor3B color, CCSprite* p)
{
	char tip[32];
	sprintf(tip, "%d!", n);
	CCLabelTTF* label = CCLabelTTF::create(tip, "Arial", 40);
	label->setColor(color);

	CCPoint pt = p->getPosition();
	pt.x += blockWidth / 2.0f;
	pt.y += blockWidth;

	label->setPosition(pt);
	this->addChild(label, 4);
	mDamageTips.addObject(label);

	CCFiniteTimeAction* pActionA = CCDelayTime::create(singleActionTime * 2);
	CCFiniteTimeAction* pActionB = CCFadeOut::create(singleActionTime * 2);
	CCFiniteTimeAction* pList = CCSequence::create(pActionA, pActionB, NULL);
	label->runAction(pList);
}

void UnitsLayer::clearTipList()
{
	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&(mDamageTips), pObject)
	{
		if (pObject)
		{
			CCLabelTTF* pChild = dynamic_cast<CCLabelTTF*>(pObject);
			pChild->removeFromParentAndCleanup(true);
		}
	}
	mDamageTips.removeAllObjects();
}

void UnitsLayer::attachEffects()
{
	BattleSystem::ptr()->attackFlash();
}

void UnitsLayer::playerAttackAct()
{
	int nRound = BattleSystem::ptr()->mCurPlayerAttackRound;
	CCAttackInfo* pInfo = dynamic_cast<CCAttackInfo*>(BattleSystem::ptr()->mAttackInfo.objectAtIndex(nRound));

	if (pInfo->mpEnemy->death)
	{
		DungeonData::ptr()->tick();
		return;
	}
	

	int attackersNum = pInfo->mpAttackers->count();
	syncActNum = attackersNum;		//!!!
	syncActNumA = syncActNum;
	attacker* pAttacker = NULL;	

	//create action.
	mpActionArr = new CCArray[attackersNum];

	for (int i = 0; i <= attackersNum + 1; ++i)
	{
		for (int j = 0; j < attackersNum; ++j)
		{
			pAttacker = dynamic_cast<attacker*>(pInfo->mpAttackers->objectAtIndex(j));

			if (i == attackersNum)		//attack to direction's move action.
			{
				CCPoint pt = getPointVectorWithAttackDir(pAttacker->mDir);

				CCFiniteTimeAction* pAction = CCMoveBy::create(singleActionTime / 2.0f, ccp(attackDirWidth * pt.x, attackDirWidth * pt.y));
				mpActionArr[j].addObject(pAction);
				
				pAction = CCCallFunc::create(this, callfunc_selector(UnitsLayer::attachEffects));
				mpActionArr[j].addObject(pAction);

				pAction = CCMoveBy::create(singleActionTime / 2.0f, ccp(-attackDirWidth * pt.x, -attackDirWidth * pt.y));
				mpActionArr[j].addObject(pAction);
			}
			else if (i == attackersNum + 1)		//end.
			{
				CCFiniteTimeAction* pAction = CCCallFunc::create(this, callfunc_selector(UnitsLayer::playerAttackActEnd));
				mpActionArr[j].addObject(pAction);
			}
			else if (i == j)				//jump action.
			{
				CCFiniteTimeAction* pAction = CCMoveBy::create(singleActionTime / 2.0f, ccp(0, attackDirWidth));
				mpActionArr[j].addObject(pAction);
				
				int dmg = pAttacker->mpMob->damage;
				pAction = CCCallFuncND::create(this, callfuncND_selector(UnitsLayer::showDamageTip), (void*)dmg);
				mpActionArr[j].addObject(pAction);
				
				pAction = CCMoveBy::create(singleActionTime / 2.0f, ccp(0, -attackDirWidth));
				mpActionArr[j].addObject(pAction);
			}
			else
			{
				CCFiniteTimeAction* pAction = CCDelayTime::create(singleActionTime);
				mpActionArr[j].addObject(pAction);
			}
		}
	}

	CCAction* anime = NULL;
	for (int i = 0; i < attackersNum; ++i)
	{
		anime = CCSequence::create(&(mpActionArr[i]));

		pAttacker = dynamic_cast<attacker*>(pInfo->mpAttackers->objectAtIndex(i));
		mpPlayerUnits[pAttacker->mpMob->id]->mpSelf->runAction(anime);
	}
}

void UnitsLayer::showDamageTip(CCNode* sender, void* data)
{
	CCSprite* p = dynamic_cast<CCSprite*>(sender);
	int dmg = (int)data;

	addTip(dmg, ccYELLOW, p);
}

void UnitsLayer::playerAttackActEnd()
{
	--syncActNum;
	if (syncActNum == 0)
	{
		for (int i = 0; i < syncActNumA; ++i)
		{
			mpActionArr[i].removeAllObjects();
		}
		delete[] mpActionArr;

		clearTipList();
		enemyBeAttackedAct();
	}
}

void UnitsLayer::enemyBeAttackedAct()
{
	int nRound = BattleSystem::ptr()->mCurPlayerAttackRound;
	CCAttackInfo* pInfo = dynamic_cast<CCAttackInfo*>(BattleSystem::ptr()->mAttackInfo.objectAtIndex(nRound));

	if (!pInfo)
	{
		enemyBeAttackedActEnd();
		return;
	}
	
	Unit* pU = getEnemyById(pInfo->mpEnemy->id);
	if (pU)
	{
		pInfo->mpEnemy->hp -= pInfo->mTotalDmg;
		bool death = false;
		if (pInfo->mpEnemy->hp <= 0)
		{
			pInfo->mpEnemy->death = true;
			death = true;
		}

		CCArray actionList;
		CCFiniteTimeAction* action = CCMoveBy::create(singleActionTime / 4.0f, ccp(-10, 0));
		actionList.addObject(action);

		action = CCMoveBy::create(singleActionTime / 4.0f, ccp(10, 0));
		actionList.addObject(action);

		action = CCMoveBy::create(singleActionTime / 4.0f, ccp(-10, 0));
		actionList.addObject(action);

		action = CCMoveBy::create(singleActionTime / 4.0f, ccp(10, 0));
		actionList.addObject(action);

		if (death)
		{
			action = CCFadeOut::create(singleActionTime * 2);
			actionList.addObject(action);
		}

		action = CCCallFunc::create(this, callfunc_selector(UnitsLayer::enemyBeAttackedActEnd));
		actionList.addObject(action);

		
		action = CCSequence::create(&actionList);
		pU->mpSelf->runAction(action);

		//////////////////////////////////////////////////////////////////////////
		addTip(pInfo->mTotalDmg, ccRED, pU->mpSelf);

		//////////////////////////////////////////////////////////////////////////
		float rate = (float)pInfo->mpEnemy->hp / (float)pInfo->mpEnemy->maxhp;
		pU->setHpBar(rate);
	}
}

void UnitsLayer::enemyBeAttackedActEnd()
{
	clearTipList();

	int nRound = BattleSystem::ptr()->mCurPlayerAttackRound;
	CCAttackInfo* pInfo = dynamic_cast<CCAttackInfo*>(BattleSystem::ptr()->mAttackInfo.objectAtIndex(nRound));
	MonsterData* p = dynamic_cast<MonsterData*>(pInfo->mpEnemy);

	if (p && p->death)
	{
		DungeonData::ptr()->mpDish[p->xy.x][p->xy.y] = NULL;

		Unit* pU = getEnemyById(p->id);

		pU->setVisible(false);
		mMonsterArray.removeObject(pU);
		safe_release(pU)
	}

	DungeonData::ptr()->tick();
	BattleSystem::ptr()->tick();
}

void UnitsLayer::aiMoveAct(MonsterData* pMob, XY xy)
{
	CCPoint pt = getPositionByXYEx(xy);

	if (!pMob)
	{
		//
		CCAssert(0, "Override me");
		return;
	}

	pMob->xy = xy;
	DungeonData::ptr()->updateDish();
	
	Unit* pU = getEnemyById(pMob->id);
	if (pU)
	{
		CCFiniteTimeAction* action = CCSequence::create(
			CCMoveTo::create(singleActionTime, pt),
			CCCallFunc::create(this, callfunc_selector(UnitsLayer::aiMoveActEnd)),
			NULL);
		pU->mpSelf->runAction(action);
	}
}
void UnitsLayer::aiMoveActEnd()
{
	BattleSystem::ptr()->aiMoveEnd();
}

void UnitsLayer::aiAttackAct()
{
	CCEnemyAIInfo* info = BattleSystem::ptr()->mpAiAttackInfo;
	int nRound = BattleSystem::ptr()->mCurAiAttackRound;

	Unit* pU = getEnemyById(info->mpEnemy->id);
	if (!pU)
	{
		return;
	}

	CCArray actionList;
	CCFiniteTimeAction* pAction = NULL;
	attacker* pMem = dynamic_cast<attacker*>(info->mpAttackers->objectAtIndex(nRound));
	if (nRound == 0)
	{
		//first time. need move to target position;
		CCPoint srcPt = getPositionByXYEx(info->mpEnemy->xy);
		CCPoint destPt = getPositionByXYEx(info->nextXY);
		CCPoint dis = ccp(destPt.x - srcPt.x, destPt.y - srcPt.y);

		pAction = CCMoveBy::create(singleActionTime, dis);
		actionList.addObject(pAction);

		info->mpEnemy->xy = info->nextXY;
		DungeonData::ptr()->updateDish();
	}

	pAction = CCCallFuncND::create(this, callfuncND_selector(UnitsLayer::showDamageTip), (void*)(info->mpEnemy->damage));
	actionList.addObject(pAction);

	CCPoint pt = getPointVectorWithAttackDir(pMem->mDir);
	pAction = CCMoveBy::create(singleActionTime / 2.0f, ccp(attackDirWidth * pt.x, attackDirWidth * pt.y));
	actionList.addObject(pAction);

	pAction = CCCallFunc::create(this, callfunc_selector(UnitsLayer::attachEffects));
	actionList.addObject(pAction);

	pAction = CCMoveBy::create(singleActionTime / 2.0f, ccp(-attackDirWidth * pt.x, -attackDirWidth * pt.y));
	actionList.addObject(pAction);

	pAction = CCCallFunc::create(this, callfunc_selector(UnitsLayer::aiAttackActEnd));
	actionList.addObject(pAction);
	
	pAction = CCSequence::create(&actionList);

	pU->mpSelf->runAction(pAction);
}

void UnitsLayer::aiAttackActEnd()
{
	memberBeAttackedAct();
}

void UnitsLayer::memberBeAttackedAct()
{
	CCEnemyAIInfo* info = BattleSystem::ptr()->mpAiAttackInfo;
	if (!info)
	{
		memberBeAttackedActEnd();
		return;
	}

	int nRound = BattleSystem::ptr()->mCurAiAttackRound;
	attacker* pA = dynamic_cast<attacker*>(info->mpAttackers->objectAtIndex(nRound));
	MonsterData* pMob = pA->mpMob;


	Unit* pU = mpPlayerUnits[pMob->id];
	if (pU)
	{
		pMob->hp -= info->mpEnemy->damage;
		bool death = false;
		if (pMob->hp <= 0)
		{
			pMob->death = true;
			death = true;
		}

		CCArray actionList;
		CCFiniteTimeAction* action = CCMoveBy::create(singleActionTime / 4.0f, ccp(-10, 0));
		actionList.addObject(action);

		action = CCMoveBy::create(singleActionTime / 4.0f, ccp(10, 0));
		actionList.addObject(action);

		action = CCMoveBy::create(singleActionTime / 4.0f, ccp(-10, 0));
		actionList.addObject(action);

		action = CCMoveBy::create(singleActionTime / 4.0f, ccp(10, 0));
		actionList.addObject(action);

		if (death)
		{
			action = CCFadeOut::create(singleActionTime * 2);
			actionList.addObject(action);
		}

		action = CCCallFunc::create(this, callfunc_selector(UnitsLayer::memberBeAttackedActEnd));
		actionList.addObject(action);


		action = CCSequence::create(&actionList);
		pU->mpSelf->runAction(action);

		//////////////////////////////////////////////////////////////////////////
		addTip(info->mpEnemy->damage, ccRED, pU->mpSelf);

		//////////////////////////////////////////////////////////////////////////
		float rate = (float)pMob->hp / (float)pMob->maxhp;
		pU->setHpBar(rate);
	}
}

void UnitsLayer::memberBeAttackedActEnd()
{
	clearTipList();

	CCEnemyAIInfo* info = BattleSystem::ptr()->mpAiAttackInfo;
	int nRound = BattleSystem::ptr()->mCurAiAttackRound;
	MonsterData* p = dynamic_cast<MonsterData*>(info->mpAttackers->objectAtIndex(nRound));

	if (p && p->death)
	{
		DungeonData::ptr()->mpDish[p->xy.x][p->xy.y] = NULL;

		Unit* pU = mpPlayerUnits[p->id];

		pU->setVisible(false);
	}

	DungeonData::ptr()->tick();
	BattleSystem::ptr()->tick();
}

void UnitsLayer::goNextRoundAct()
{
	syncActNum = 0;
	for (int i = 0; i < playerUnitsCount; ++i)
	{
		if (!(PlayerData::ptr()->mMonster[i].death))
		{
			++syncActNum;

			CCFiniteTimeAction* action = CCSequence::create(
				CCMoveBy::create(singleActionTime / 2.0, ccp(0, attackDirWidth)),
				CCMoveBy::create(singleActionTime / 2.0, ccp(0, -attackDirWidth)),
				CCMoveBy::create(singleActionTime / 2.0, ccp(0, attackDirWidth)),
				CCMoveBy::create(singleActionTime / 2.0, ccp(0, -attackDirWidth)),
				CCMoveBy::create(singleActionTime / 2.0, ccp(0, attackDirWidth)),
				CCMoveBy::create(singleActionTime / 2.0, ccp(0, -attackDirWidth)),
				CCMoveBy::create(singleActionTime / 2.0, ccp(0, attackDirWidth)),
				CCMoveBy::create(singleActionTime / 2.0, ccp(0, -attackDirWidth)),
				CCCallFunc::create(this, callfunc_selector(UnitsLayer::goNextRoundActEnd)),
				NULL);
			mpPlayerUnits[PlayerData::ptr()->mMonster[i].id]->mpSelf->runAction(action);
		}
		
	}
	
}

void UnitsLayer::goNextRoundActEnd()
{
	--syncActNum;
	if (syncActNum == 0)
	{
		initEnemyUnits();
		BattleSystem::ptr()->goNextRoundEnd();
	}
}

//////////////////////////////////////////////////////////////////////////

MapLayer::~MapLayer()
{
	//this->removeAllChildrenWithCleanup(false);
}

bool MapLayer::init()
{
	mFloorLayer.initWithRounds(DungeonData::ptr()->mRounds.count());
	this->addChild(&mFloorLayer, 1);

	this->setPosition(ccp(0, 160));

	return true;
}

//////////////////////////////////////////////////////////////////////////

MemberHeader::~MemberHeader()
{
	mpHeader->release();
	mpHeadRect->release();

	mpLayer->removeAllChildrenWithCleanup(true);
	mpLayer->release();
}

void MemberHeader::init()
{
	mpLayer = new CCSprite;
	mpLayer->retain();
	mpLayer->initWithFile("transparent.png");
	mpLayer->setAnchorPoint(ccp(0, 0));
	//mpLayer->setSize(CCSize(160, 160));


	mpHeader = new CCSprite;
	mpHeader->retain();
	mpHeader->initWithFile("lich.png");
	mpHeader->setAnchorPoint(ccp(0, 0));
	mpLayer->addChild(mpHeader, 0);

	mpHeadRect = new CCSprite;
	mpHeadRect->retain();
	mpHeadRect->initWithFile("headrect.png");
	mpHeadRect->setAnchorPoint(ccp(0, 0));
	mpLayer->addChild(mpHeadRect, 1);
}

void MemberHeader::setHeader(const char* name)
{
	CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(name);
	if (pTexture)
	{
		mpHeader->setTexture(pTexture);
	}
}

DungeonUILayer::~DungeonUILayer()
{
	mpTopBG->release();
	mpMiddleBG->release();
	mpDownBG->release();
}

bool DungeonUILayer::init()
{
	mpTopBG = CCSprite::create();
	mpTopBG->retain();
	mpTopBG->initWithFile("upbg.png");
	mpTopBG->setAnchorPoint(ccp(0, 0));
	mpTopBG->setPosition(ccp(0, 960 - 160));
	this->addChild(mpTopBG, 0);

	mpMiddleBG = CCSprite::create();
	mpMiddleBG->retain();
	mpMiddleBG->initWithFile("mbg.png");
	mpMiddleBG->setAnchorPoint(ccp(0, 0));
	mpMiddleBG->setPosition(ccp(0, 160));
	this->addChild(mpMiddleBG, 0);

	mpDownBG = CCSprite::create();
	mpDownBG->retain();
	mpDownBG->initWithFile("downbg.png");
	mpDownBG->setAnchorPoint(ccp(0, 0));
	mpDownBG->setPosition(ccp(0, 0));
	this->addChild(mpDownBG, 0);

	this->setPosition(ccp(0, 0));

	//////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < playerUnitsCount; ++i)
	{
		mpHeader[i] = new MemberHeader;
		mpHeader[i]->retain();
		mpHeader[i]->init();

		mpHeader[i]->setHeader(PlayerData::ptr()->mMonster[i].header.c_str());

		mpHeader[i]->mpLayer->setAnchorPoint(ccp(0, 0));
		mpHeader[i]->mpLayer->setPosition(ccp(blockWidth * i, 0));
		this->addChild(mpHeader[i]->mpLayer, 1);
	}
	
	return true;
}

void DungeonUILayer::setHeader(MonsterData* pData)
{
	
}

//////////////////////////////////////////////////////////////////////////

GameScene::~GameScene()
{
	//this->removeAllChildrenWithCleanup(false);
	GameSceneTouchDelegate::ptr()->mpGameScene = NULL;
	BattleSystem::ptr()->mpGameScene = NULL;
}

CCScene* GameScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GameScene *layer = GameScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

void GameScene::initBG()
{
	CCSprite* pMiddleBG = CCSprite::create();
	pMiddleBG->initWithFile("bg.png");
	pMiddleBG->setAnchorPoint(ccp(0, 0));
	pMiddleBG->setPosition(ccp(0, 0));
	this->addChild(pMiddleBG, 0);
}

void GameScene::initSprites()
{
	mpPlayerTurn = CCSprite::create("playerturn.png");
	mpPlayerTurn->setAnchorPoint(ccp(0, 0));
	mpPlayerTurn->setPosition(ccp(0, 0));
	mpPlayerTurn->setOpacity(0.0);
	//mpPlayerTurn->setVisible(false);
	this->addChild(mpPlayerTurn, 4);

	mpAiTurn = CCSprite::create("mobturn.png");
	mpAiTurn->setAnchorPoint(ccp(0, 0));
	mpAiTurn->setPosition(ccp(0, 0));
	mpAiTurn->setOpacity(0.0);
	//mpAiTurn->setVisible(false);
	this->addChild(mpAiTurn, 5);

	mpAttackFlash = CCSprite::create("attackflash.png");
	mpAttackFlash->setAnchorPoint(ccp(0, 0));
	mpAttackFlash->setPosition(ccp(0, 0));
	mpAttackFlash->setVisible(false);
	this->addChild(mpAttackFlash, 6);
}

void GameScene::goNextRoundAct()
{
	CCFiniteTimeAction* s = CCSequence::create(
		CCMoveBy::create(singleActionTime / 2.0, ccp(blockWidth, 0)),
		CCDelayTime::create(singleActionTime / 2.0),
		CCMoveBy::create(singleActionTime / 2.0, ccp(blockWidth, 0)),
		CCDelayTime::create(singleActionTime / 2.0),
		CCMoveBy::create(singleActionTime / 2.0, ccp(blockWidth, 0)),
		CCDelayTime::create(singleActionTime / 2.0),
		CCMoveBy::create(singleActionTime / 2.0, ccp(blockWidth, 0)),
		CCDelayTime::create(singleActionTime / 2.0),
		NULL);

	mMapLayer.mFloorLayer.runAction(s);

	//////////////////////////////////////////////////////////////////////////
	mUnitsLayer.goNextRoundAct();
}

void GameScene::changeRound(attackRound round)
{
	if (round == eRoundPlayer)
	{
		//mpPlayerTurn->setVisible(true);
		//mpAiTurn->setVisible(false);

		CCFiniteTimeAction* action = CCSequence::create(
			CCFadeIn::create(singleActionTime),
			CCDelayTime::create(singleActionTime),
			CCFadeOut::create(singleActionTime),
			CCCallFunc::create(this, callfunc_selector(GameScene::changeRoundEnd)),
			NULL);
		mpPlayerTurn->runAction(action);
	}
	else if (round == eRoundAi)
	{
		//mpPlayerTurn->setVisible(false);
		//mpAiTurn->setVisible(true);

		CCFiniteTimeAction* action = CCSequence::create(
			CCFadeIn::create(singleActionTime),
			CCDelayTime::create(singleActionTime),
			CCFadeOut::create(singleActionTime),
			CCCallFunc::create(this, callfunc_selector(GameScene::changeRoundEnd)),
			NULL);
		mpAiTurn->runAction(action);
	}
}

void GameScene::changeRoundEnd()
{
	BattleSystem::ptr()->changRoundEnd();
}

void GameScene::attachFlash()
{
	mpAttackFlash->stopAllActions();
	mpAttackFlash->setVisible(true);
	CCFiniteTimeAction* action = CCSequence::create(
		CCBlink::create(singleActionTime / 2.0, 2),
		CCCallFunc::create(this, callfunc_selector(GameScene::attachFlashEnd)),
		NULL);
	mpAttackFlash->runAction(action);
}

void GameScene::attachFlashEnd()
{
	mpAttackFlash->setVisible(false);
}

bool GameScene::init()
{
	this->setTouchEnabled(true);
	GameSceneTouchDelegate::ptr()->mpGameScene = this;
	BattleSystem::ptr()->mpGameScene = this;

	DungeonData::ptr()->init();

	initBG();
	initSprites();
	

	mMapLayer.init();
	this->addChild(&mMapLayer, 1);

	mRouteLayer.init();
	this->addChild(&mRouteLayer, 2);

	mUnitsLayer.init();
	this->addChild(&mUnitsLayer, 3);

	mDungeonUILayer.init();
	this->addChild(&mDungeonUILayer, 0);

	mGameState = eWaitPlayerAct;

	//////////////////////////////////////////////////////////////////////////
// 	CCMenuItemImage* pCloseItem = CCMenuItemImage::create(
// 		"CloseNormal.png",
// 		"CloseSelected.png",
// 		this,
// 		menu_selector(GameScene::menuCloseCallback));
// 	pCloseItem->setAnchorPoint(ccp(0, 40));
// 	pCloseItem->setPosition(ccp(0, 960));
// 	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
// 	pMenu->setPosition(CCPointZero);
// 	this->addChild(pMenu, 4);


	//this->schedule(schedule_selector(DungeonData::tick));

	return true;
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

void GameScene::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	GameSceneTouchDelegate::ptr()->ccTouchesBegan(pTouches, event);
}

void GameScene::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
	GameSceneTouchDelegate::ptr()->ccTouchesMoved(pTouches, event);
}

void GameScene::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
	GameSceneTouchDelegate::ptr()->ccTouchesEnded(pTouches, event);
}