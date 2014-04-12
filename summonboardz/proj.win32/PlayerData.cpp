#include "PlayerData.h"
#include "Utils.h"
#include "BattleSystem.h"

//////////////////////////////////////////////////////////////////////////

void calculateXYWithDirectionAndXY(XY xy, attackDirection direciton, attackType type, CCArray* array)
{
	if (type == eAttackNone)
	{
		return;
	}

	int mapLen = mapWidth - 1;
	switch (direciton)
	{
	case eAttackDirectionL:
		{
			if (xy.x <= 0)
			{
				return;
			}

			int i = xy.x - 1;
			int j = xy.y;
			do 
			{
				XY* coord = new XY(i, j);
				--i;
				if (PlayerData::ptr()->beenTakenPlace(*coord))
				{
					if (type == eAttackOnce)
					{
						break;
					}
					continue;
				}
				array->addObject(coord);

				if (type == eAttackOnce)
				{
					break;
				}
			} 
			while (i >= 0);
		}
		break;
	case eAttackDirectionR:
		{
			if (xy.x >= mapLen)
			{
				return;
			}
			
			int i = xy.x + 1;
			int j = xy.y;
			do 
			{
				XY* coord = new XY(i, j);
				++i;
				if (PlayerData::ptr()->beenTakenPlace(*coord))
				{
					if (type == eAttackOnce)
					{
						break;
					}
					continue;
				}
				array->addObject(coord);

				if (type == eAttackOnce)
				{
					break;
				}
			} 
			while (i < mapWidth);
		}
		break;
	case eAttackDirectionU:
		{
			if (xy.y >= mapLen)
			{
				return;
			}

			int i = xy.x;
			int j = xy.y + 1;
			do 
			{
				XY* coord = new XY(i, j);
				++j;
				if (PlayerData::ptr()->beenTakenPlace(*coord))
				{
					if (type == eAttackOnce)
					{
						break;
					}
					continue;
				}
				array->addObject(coord);

				if (type == eAttackOnce)
				{
					break;
				}
			} 
			while (j < mapWidth);
		}
		break;
	case eAttackDirectionD:
		{
			if (xy.y <= 0)
			{
				return;
			}

			int i = xy.x;
			int j = xy.y - 1;
			do 
			{
				XY* coord = new XY(i, j);
				--j;
				if (PlayerData::ptr()->beenTakenPlace(*coord))
				{
					if (type == eAttackOnce)
					{
						break;
					}
					continue;
				}
				array->addObject(coord);

				if (type == eAttackOnce)
				{
					break;
				}
			} 
			while (j >= 0);
		}
		break;
	case eAttackDirectionLU:
		{
			if (xy.x <= 0 || xy.y >= mapLen)
			{
				return;
			}

			int i = xy.x - 1;
			int j = xy.y + 1;

			do 
			{
				XY* coord = new XY(i, j);
				--i;
				++j;

				if (PlayerData::ptr()->beenTakenPlace(*coord))
				{
					if (type == eAttackOnce)
					{
						break;
					}
					continue;
				}
				array->addObject(coord);

				if (type == eAttackOnce)
				{
					break;
				}
			} 
			while (i >= 0 && j < mapWidth);
		}
		break;
	case eAttackDirectionRU:
		{
			if (xy.x >= mapLen || xy.y >= mapLen)
			{
				return;
			}

			int i = xy.x + 1;
			int j = xy.y + 1;

			do 
			{
				XY* coord = new XY(i, j);

				++i;
				++j;

				if (PlayerData::ptr()->beenTakenPlace(*coord))
				{
					if (type == eAttackOnce)
					{
						break;
					}
					continue;
				}
				array->addObject(coord);

				if (type == eAttackOnce)
				{
					break;
				}
			} 
			while (i < mapWidth && j < mapWidth);
		}
		break;
	case eAttackDirectionLD:
		{
			if (xy.x <= 0 || xy.y <= 0)
			{
				return;
			}

			int i = xy.x - 1;
			int j = xy.y - 1;

			do 
			{
				XY* coord = new XY(i, j);

				--i;
				--j;

				if (PlayerData::ptr()->beenTakenPlace(*coord))
				{
					if (type == eAttackOnce)
					{
						break;
					}
					continue;
				}
				array->addObject(coord);

				if (type == eAttackOnce)
				{
					break;
				}
			} 
			while (i >= 0 && j >= 0);
		}
		break;
	case eAttackDirectionRD:
		{
			if (xy.x >= mapLen || xy.y <= 0)
			{
				return;
			}

			int i = xy.x + 1;
			int j = xy.y - 1;

			do 
			{
				XY* coord = new XY(i, j);

				--i;
				--j;

				if (PlayerData::ptr()->beenTakenPlace(*coord))
				{
					if (type == eAttackOnce)
					{
						break;
					}
					continue;
				}
				array->addObject(coord);

				if (type == eAttackOnce)
				{
					break;
				}
			} 
			while (i < mapWidth && j >= 0);
		}
		break;
	}
}

float getRotationWithAttackDir(attackDirection dir)
{
	switch (dir)
	{
	case eAttackDirectionL:
		{
			return 270.0f;
		}
		break;
	case eAttackDirectionR:
		{
			return 90.0f;
		}
		break;
	case eAttackDirectionU:
		{
			return 0.0f;
		}
		break;
	case eAttackDirectionD:
		{
			return 180.0f;
		}
		break;
	case eAttackDirectionLU:
		{
			return 315.0f;
		}
		break;
	case eAttackDirectionRU:
		{
			return 45.0f;
		}
		break;
	case eAttackDirectionLD:
		{
			return 225.0f;
		}
		break;
	case eAttackDirectionRD:
		{
			return 135.0f;
		}
		break;
	}

	return 0.0f;
}


CCPoint getPositionWithAttackDir(attackDirection dir)
{
	switch (dir)
	{
	case eAttackDirectionL:
		{
			return ccp(20, 80);
		}
		break;
	case eAttackDirectionR:
		{
			return ccp(140, 80);
		}
		break;
	case eAttackDirectionU:
		{
			return ccp(80, 140);
		}
		break;
	case eAttackDirectionD:
		{
			return ccp(80, 20);
		}
		break;
	case eAttackDirectionLU:
		{
			return ccp(20, 140);
		}
		break;
	case eAttackDirectionRU:
		{
			return ccp(140, 140);
		}
		break;
	case eAttackDirectionLD:
		{
			return ccp(20, 20);
		}
		break;
	case eAttackDirectionRD:
		{
			return ccp(140, 20);
		}
		break;
	}

	return ccp(0, 0);
}

CCPoint getPointVectorWithAttackDir(attackDirection dir)
{
	switch (dir)
	{
	case eAttackDirectionL:
		{
			return ccp(-1, 0);
		}
		break;
	case eAttackDirectionR:
		{
			return ccp(1, 0);
		}
		break;
	case eAttackDirectionU:
		{
			return ccp(0, 1);
		}
		break;
	case eAttackDirectionD:
		{
			return ccp(0, -1);
		}
		break;
	case eAttackDirectionLU:
		{
			return ccp(-1, 1);
		}
		break;
	case eAttackDirectionRU:
		{
			return ccp(1, 1);
		}
		break;
	case eAttackDirectionLD:
		{
			return ccp(-1, -1);
		}
		break;
	case eAttackDirectionRD:
		{
			return ccp(1, -1);
		}
		break;
	}

	return ccp(0, 0);
}

//////////////////////////////////////////////////////////////////////////

MonsterData::MonsterData()
{
	
}

MonsterData::~MonsterData()
{

}

void MonsterData::initLikeMonster()
{
	attack[eAttackDirectionMax];
	for (int i = 0; i < eAttackDirectionMax; ++i)
	{
		attack[i] = eAttackNone;
	}
	attack[eAttackDirectionU] = eAttackOnce;
	attack[eAttackDirectionD] = eAttackOnce;
	attack[eAttackDirectionL] = eAttackOnce;
	attack[eAttackDirectionR] = eAttackOnce;

	damage = 100;
	hp = 1000;
	maxhp = 1000;
	xy.x = 0;
	xy.y = 0;
	name = std::string("barret.png");

	type = enemyType;
	death = false;
}

void MonsterData::initLikeMember()
{
	attack[eAttackDirectionMax];
	for (int i = 0; i < eAttackDirectionMax; ++i)
	{
		attack[i] = eAttackNone;
	}
	attack[eAttackDirectionU] = eAttackOnce;
	attack[eAttackDirectionD] = eAttackOnce;
	attack[eAttackDirectionL] = eAttackOnce;
	attack[eAttackDirectionR] = eAttackTwice;
	attack[eAttackDirectionLU] = eAttackTwice;
	attack[eAttackDirectionLD] = eAttackTwice;

	damage = 300;
	hp = 1000;
	maxhp = 1000;
	xy.x = 0;
	xy.y = 0;
	name = std::string("lich.png");
	header = std::string("lich.png");

	type = memberType;
	death = false;
}

PlayerData::PlayerData()
{
	for (int i = 0; i < playerUnitsCount; ++i)
	{
		mMonster[i].initLikeMember();
		mMonster[i].id = i;
	}

	mMonster[0].setXY(3, 2);
	mMonster[0].name = std::string("cloud.png");
	mMonster[0].header = std::string("cloud_head.png");
	mMonster[1].setXY(3,1);
	mMonster[1].name = std::string("aries.png");
	mMonster[1].header = std::string("aries_head.png");
	mMonster[2].setXY(3,0);
	mMonster[2].name = std::string("tifa.png");
	mMonster[2].header = std::string("tifa_head.png");
	mMonster[3].setXY(2,0);
	mMonster[3].name = std::string("red13.png");
	mMonster[3].header = std::string("red13_head.png");
}

PlayerData::~PlayerData()
{

}

void PlayerData::generateMovableXY(int index)
{
	MonsterData* pData = &(mMonster[index]);

	for (int i = 0; i < eAttackDirectionMax; ++i)
	{
		calculateXYWithDirectionAndXY(pData->xy, (attackDirection)i, pData->attack[i], &mMovableXY);
	}
}

bool PlayerData::isMovableXY(XY xy)
{
	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&mMovableXY, pObject)
	{
		XY* pChild = dynamic_cast<XY*>(pObject);
		if ((*pChild) == xy)
		{
			return true;
		}
	}

	return false;
}

void PlayerData::clearPickedInfo()
{
	mPickedIndex = -1;
	mOldPositon = ccp(0, 0);

	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&mMovableXY, pObject)
	{
		XY* pChild = dynamic_cast<XY*>(pObject);
		if (pChild)
		{
			//pChild->release();
			safe_release(pChild)
		}
	}
	mMovableXY.removeAllObjects();
}

void PlayerData::updateMonstersXY(XY xy)
{
	mMonster[mPickedIndex].xy = xy;
}

bool PlayerData::beenTakenPlace(XY xy)
{
	if (DungeonData::ptr()->hasMonsterInXY(xy))
	{
		return true;
	}

	return false;
}

bool PlayerData::allDied()
{
	bool death = true;
	for (int i = 0; i < playerUnitsCount; ++i)
	{
		if (!(mMonster[i].death))
		{
			death = false;
			break;
		}
	}

	return death;
}

//////////////////////////////////////////////////////////////////////////

RoundData::~RoundData()
{
	if (mMonsters.count() == 0)
	{
		return;
	}

	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&mMonsters, pObject)
	{
		MonsterData* pChild = dynamic_cast<MonsterData*>(pObject);
		if (pChild)
		{
			//pChild->release();	//maybe problem.
			safe_release(pChild)
		}
	}
};

void RoundData::init()
{
	MonsterData* dataA = new MonsterData;
	dataA->initLikeMonster();
	dataA->id = 1;
	dataA->xy = XY(1, 1);

	MonsterData* dataB = new MonsterData;
	dataB->initLikeMonster();
	dataB->id = 2;

	mMonsters.addObject(dataA);
	mMonsters.addObject(dataB);
}

bool RoundData::allDied()
{
	bool death = true;

	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&mMonsters, pObject)
	{
		MonsterData* pChild = dynamic_cast<MonsterData*>(pObject);
		if (pChild->hp >0)
		{
			death = false;
			break;
		}
	}

	return death;
}

void RoundData::tick()
{
// 	CCObject* pObject = NULL;
// 	CCARRAY_FOREACH(&(pRound->mMonsters), pObject)
// 	{
// 		MonsterData* pChild = dynamic_cast<MonsterData*>(pObject);
// 		if (pChild->death)
// 		{
// 			mpDun->mpGameScene->
// 		}
// 	}
}

bool RoundData::generateMoveData(MonsterData** pMob, XY* xy)
{
	CCArray movableArr;
	CCArray mobArr;

	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&mMonsters, pObject)
	{
		MonsterData* pChild = dynamic_cast<MonsterData*>(pObject);
		if (!(pChild->death))
		{	
			CCArray* movable = new CCArray;
			generateMovableXY(pChild, movable);
			if (movable->count() <= 0)
			{
				continue;
			}
			
			pChild->retain();
			mobArr.addObject(pChild);
			movableArr.addObject(movable);	
		}
	}

	if (mobArr.count() <= 0)
	{
		return false;
	}

	int mobIndex = (movableArr.count() > 1) ? (rand() % (movableArr.count() - 1)) : (0);
	CCArray* pArr = dynamic_cast<CCArray*>(movableArr.objectAtIndex(mobIndex));
	(*pMob) = dynamic_cast<MonsterData*>(mobArr.objectAtIndex(mobIndex));

	int moveIndex = (pArr->count() > 1) ? (rand() % (pArr->count() - 1)) : (0);
	XY* pXY = dynamic_cast<XY*>(pArr->objectAtIndex(moveIndex));
	(*xy) = *(pXY);

	for (int i = 0; i < movableArr.count(); ++i)
	{
		CCArray* pA = dynamic_cast<CCArray*>(movableArr.objectAtIndex(mobIndex));
		pA->removeAllObjects();
	}
	mobArr.removeAllObjects();
	movableArr.removeAllObjects();

	if (pMob)
	{
		return true;
	}

	return false;
}

bool RoundData::generateAttackData(CCEnemyAIInfo** info)
{
	CCObject* pObject = NULL;
	CCArray wholeInfo;
	CCARRAY_FOREACH(&mMonsters, pObject)
	{
		MonsterData* pChild = dynamic_cast<MonsterData*>(pObject);
		if (!(pChild->death))
		{
			CCArray movable;
			CCEnemyAIInfo* singleInfo = NULL;

			generateMovableXY(pChild, &movable);
			if (movable.count() <= 0)
			{
				continue;
			}

			CCObject* pO = NULL;
			CCARRAY_FOREACH(&movable, pO)
			{
				XY* pNextXY = dynamic_cast<XY*>(pO);
				for (int i = 0; i < eAttackDirectionMax; ++i)
				{
					if (pChild->attack[i] == eAttackNone)
					{
						continue;
					}

					XY tarXY;
					if (getXYwithTargetXYAndDir(*pNextXY, (attackDirection)i, &tarXY))
					{
						if (DungeonData::ptr()->hasMonsterInXY(tarXY) == memberType)
						{
							if (!singleInfo)
							{
								singleInfo = new CCEnemyAIInfo;
								singleInfo->mpEnemy = pChild;
								singleInfo->nextXY = (*pNextXY);
								singleInfo->mpAttackers = new CCArray;
							}
							
							attacker* er = new attacker;
							er->mpMob = DungeonData::ptr()->mpDish[tarXY.x][tarXY.y];
							er->mDir = (attackDirection)i;
							singleInfo->mpAttackers->addObject(er);
						}
					}
				}

				if (singleInfo)
				{
					wholeInfo.addObject(singleInfo);
					singleInfo = NULL;
				}
			}
		}
	}

	if (wholeInfo.count() > 0)
	{
		int max = 0;
		int maxIndex = 0;
		
		for (int i = 0; i < wholeInfo.count(); ++i)
		{
			CCEnemyAIInfo* pAI = dynamic_cast<CCEnemyAIInfo*>(wholeInfo.objectAtIndex(i));
			if (pAI->mpAttackers->count() > max)
			{
				max = pAI->mpAttackers->count();
				maxIndex = i;
			}
		}

		if (maxIndex >= 0)
		{
			(*info) = dynamic_cast<CCEnemyAIInfo*>(wholeInfo.objectAtIndex(maxIndex));
			wholeInfo.removeObject(wholeInfo.objectAtIndex(maxIndex), false);

			for (int i = 0; i < wholeInfo.count(); ++i)
			{
				CCEnemyAIInfo* pAI = dynamic_cast<CCEnemyAIInfo*>(wholeInfo.objectAtIndex(i));
				for (int j = 0; j < pAI->mpAttackers->count(); ++j)
				{
					attacker* p = dynamic_cast<attacker*>(pAI->mpAttackers->objectAtIndex(j));
					safe_release(p)
				}
				pAI->mpAttackers->removeAllObjects();
				safe_release(pAI->mpAttackers)
				safe_release(pAI)
			}

			return true;
		}
	}

	return false;
}

void RoundData::generateMovableXY(MonsterData* mob, CCArray* array)
{
	for (int i = 0; i < eAttackDirectionMax; ++i)
	{
		calculateXYWithDirectionAndXY(mob->xy, (attackDirection)i, mob->attack[i], array);
	}
	array->addObject(new XY(mob->xy.x, mob->xy.y));
}

//////////////////////////////////////////////////////////////////////////

DungeonData::DungeonData()
{
	init();
}

DungeonData::~DungeonData()
{
	clearRounds();
}

void DungeonData::clearRounds()
{
	if (mRounds.count() == 0)
	{
		return;
	}

	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&mRounds, pObject)
	{
		RoundData* pChild = dynamic_cast<RoundData*>(pObject);
		if (pChild)
		{
			//pChild->release();		//maybe problem.
			safe_release(pChild)
		}
	}
}

void DungeonData::init()
{
	for (int i = 0; i < 4; ++i)
	{
		RoundData* data = new RoundData;
		data->init();
		data->mpDun = this;

		mRounds.addObject(data);
	}

	mCurRound = 0;

	PlayerData::ptr()->init();

	updateDish();
}

void DungeonData::tick()
{
	RoundData* pRound = dynamic_cast<RoundData*>(mRounds.objectAtIndex(mCurRound));
	if (pRound && pRound->allDied())
	{
		++mCurRound;
		if (mCurRound >= mRounds.count())
		{
			//dungeon clear.
			//brunch ok.
			int i = 0;
			return;
		}
		else
		{
			//set scene with new round.
			updateRoundMonstersXY();
			updateDish();
			BattleSystem::ptr()->goNextRound();
		}
	}
	else if (PlayerData::ptr()->allDied())
	{
		//dungeon failed.
		int i = 0;
	}
	else
	{
		//CCAssert(0, "Override me");
	}
}
XY DungeonData::getEmptyXY()
{
	for (int i = 0; i < mapWidth; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (mpDish[i][j] == NULL)
			{
				return XY(i, j);
			}
		}
	}
}

void DungeonData::updateRoundMonstersXY()
{
	RoundData* pRound = dynamic_cast<RoundData*>(mRounds.objectAtIndex(mCurRound));
	if (pRound)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(&(pRound->mMonsters), pObject)
		{
			MonsterData* pChild = dynamic_cast<MonsterData*>(pObject);
			if (mpDish[pChild->xy.x][pChild->xy.y] != NULL)
			{
				pChild->xy = getEmptyXY();
				mpDish[pChild->xy.x][pChild->xy.y] = pChild;
			}
		}
	}
}

void DungeonData::updateDish()
{
	for (int i = 0; i < mapWidth; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			mpDish[i][j] = NULL;
		}
	}

	RoundData* pRound = dynamic_cast<RoundData*>(mRounds.objectAtIndex(mCurRound));
	if (pRound)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(&(pRound->mMonsters), pObject)
		{
			MonsterData* pChild = dynamic_cast<MonsterData*>(pObject);
			if (!pChild->death)
			{
				mpDish[pChild->xy.x][pChild->xy.y] = pChild;
			}
		}
	}

	for (int i = 0; i < playerUnitsCount; ++i)
	{
		MonsterData* p = &(PlayerData::ptr()->mMonster[i]);
		if (!p->death)
		{
			mpDish[p->xy.x][p->xy.y] = p;
		}
	}
}

int DungeonData::hasMonsterInXY(XY xy)
{
	if (mpDish[xy.x][xy.y])
	{
		return mpDish[xy.x][xy.y]->type;
	}

	return 0;
}

RoundData* DungeonData::curRound()
{
	if (mCurRound >= 0 && mCurRound < mRounds.count())
	{
		RoundData* pRound = dynamic_cast<RoundData*>(mRounds.objectAtIndex(mCurRound));
		if (pRound)
		{
			return pRound;
		}
	}

	return NULL;
}