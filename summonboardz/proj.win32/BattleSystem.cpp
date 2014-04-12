#include "BattleSystem.h"
#include "PlayerData.h"

//////////////////////////////////////////////////////////////////////////

bool getXYwithTargetXYAndDir(XY tar, attackDirection dir, XY* destXY)
{
	switch (dir)
	{
	case eAttackDirectionL:
		{
			(*destXY) = XY(tar.x - 1, tar.y);
			if (isValidXY(*destXY))
			{
				return true;
			}
		}
		break;
	case eAttackDirectionR:
		{
			(*destXY) = XY(tar.x + 1, tar.y);
			if (isValidXY(*destXY))
			{
				return true;
			}
		}
		break;
	case eAttackDirectionU:
		{
			(*destXY) = XY(tar.x, tar.y + 1);
			if (isValidXY(*destXY))
			{
				return true;
			}
		}
		break;
	case eAttackDirectionD:
		{
			(*destXY) = XY(tar.x, tar.y - 1);
			if (isValidXY(*destXY))
			{
				return true;
			}
		}
		break;
	case eAttackDirectionLU:
		{
			(*destXY) = XY(tar.x - 1, tar.y + 1);
			if (isValidXY(*destXY))
			{
				return true;
			}
		}
		break;
	case eAttackDirectionRU:
		{
			(*destXY) = XY(tar.x + 1, tar.y + 1);
			if (isValidXY(*destXY))
			{
				return true;
			}
		}
		break;
	case eAttackDirectionLD:
		{
			(*destXY) = XY(tar.x - 1, tar.y - 1);
			if (isValidXY(*destXY))
			{
				return true;
			}
		}
		break;
	case eAttackDirectionRD:
		{

			(*destXY) = XY(tar.x + 1, tar.y - 1);
			if (isValidXY(*destXY))
			{
				return true;
			}
		}
		break;
	}

	return false;
}

attackDirection getDirByTwoTargets(XY attacker, XY defender)
{
	defender = defender - attacker;
	if (defender.x == -1 && defender.y == 1)
	{
		return eAttackDirectionLU;
	}
	else if (defender.x == 0 && defender.y == 1)
	{
		return eAttackDirectionU;
	}
	else if (defender.x == 1 && defender.y == 1)
	{
		return eAttackDirectionRU;
	}
	else if (defender.x == -1 && defender.y == 0)
	{
		return eAttackDirectionL;
	}
	else if (defender.x == 1 && defender.y == 0)
	{
		return eAttackDirectionR;
	}
	else if (defender.x == -1 && defender.y == -1)
	{
		return eAttackDirectionLD;
	}
	else if (defender.x == 0 && defender.y == -1)
	{
		return eAttackDirectionD;
	}
	else if (defender.x == 1 && defender.y == -1)
	{
		return eAttackDirectionRD;
	}
	else
	{
		return eAttackDirectionMax;
	}
}

//////////////////////////////////////////////////////////////////////////

int BattleSystem::getIndexIfMemberInXY(XY xy)
{
	for (int i = 0; i < playerUnitsCount; ++i)
	{
		if (PlayerData::ptr()->mMonster[i].xy == xy &&
			PlayerData::ptr()->mMonster[i].death == false)
		{
			return i;
		}
	}

	return -1;
}

bool BattleSystem::playerAttackEnemyData(CCArray* array)
{
	RoundData* pRound = dynamic_cast<RoundData*>(DungeonData::ptr()->mRounds.objectAtIndex(DungeonData::ptr()->mCurRound));

	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&(pRound->mMonsters), pObject)
	{
		MonsterData* pMob = dynamic_cast<MonsterData*>(pObject);
		if (pMob->death)
		{
			continue;
		}
		calculateMembersWithTouchableEnemy(pMob, array);
	}

	if (array->count())
	{
		return true;
	}

	return false;
}

void BattleSystem::clearPlayerAttackInfo()
{
	if (PlayerClear)
	{
		return;
	}

	mCurPlayerAttackRound = -1;

	CCObject* pObject = NULL;
	CCARRAY_FOREACH(&(mAttackInfo), pObject)
	{
		CCAttackInfo* pAI = dynamic_cast<CCAttackInfo*>(pObject);

		CCObject* pOb = NULL;
		CCARRAY_FOREACH(pAI->mpAttackers, pOb)
		{
			attacker* pA = dynamic_cast<attacker*>(pOb);
			pA->release();
		}
		
		pAI->mpEnemy = NULL;
		pAI->mpAttackers->removeAllObjects();
		pAI->release();
	}

	mAttackInfo.removeAllObjects();
	PlayerClear = true;
}

void BattleSystem::calculateMembersWithTouchableEnemy(MonsterData* pMob, CCArray* array)
{
	XY destXY;
	CCAttackInfo* arr = NULL;
	for (int i = 0; i < eAttackDirectionMax; ++i)
	{
		if (getXYwithTargetXYAndDir(pMob->xy, (attackDirection)i, &destXY))
		{
			int index = getIndexIfMemberInXY(destXY);
			if (index >= 0)
			{
				MonsterData* pMember = &(PlayerData::ptr()->mMonster[index]);
				if (pMember->death)
				{
					continue;
				}
				attackDirection dir = getDirByTwoTargets(pMember->xy, pMob->xy);
				if (dir != eAttackDirectionMax)
				{
					if (pMember->attack[dir] != eAttackNone)
					{
						if (!arr)
						{
							arr = new CCAttackInfo;
							arr->mpAttackers = new CCArray;
							arr->mpEnemy = pMob;
						}

						attacker* pAttacker = new attacker;
						pAttacker->mpMob = pMember;
						pAttacker->mDir = dir;
						arr->mpAttackers->addObject(pAttacker);
						arr->mTotalDmg += pAttacker->mpMob->damage;
					}
				}
			}
		}
	}

	if (arr)
	{
		array->addObject(arr);
	}
}

bool BattleSystem::playerCanAttack()
{
	clearPlayerAttackInfo();
	if (playerAttackEnemyData(&mAttackInfo))
	{
		PlayerClear = false;
		return true;
	}

	return false;
}

void BattleSystem::playerFirstAttack()
{
	mCurPlayerAttackRound = 0;
	CCAttackInfo* p = dynamic_cast<CCAttackInfo*>(mAttackInfo.objectAtIndex(mCurPlayerAttackRound));
	mpGameScene->mUnitsLayer.playerAttackAct();
}

void BattleSystem::playerNextAttack()
{
	++mCurPlayerAttackRound;

	if (mCurPlayerAttackRound < mAttackInfo.count())
	{
		CCAttackInfo* p = dynamic_cast<CCAttackInfo*>(mAttackInfo.objectAtIndex(mCurPlayerAttackRound));
		mpGameScene->mUnitsLayer.playerAttackAct();
	}
	else
	{
		clearPlayerAttackInfo();
		//round change.

		changeRound(eRoundAi);
	}
}

bool BattleSystem::aiAttackPlayerData()
{
	if (DungeonData::ptr()->curRound()->generateAttackData(&mpAiAttackInfo))
	{
		return true;
	}

	return false;
}

bool BattleSystem::aiMove()
{
	MonsterData* pMob;
	XY xy;
	if (DungeonData::ptr()->curRound()->generateMoveData(&pMob, &xy))
	{
		mpGameScene->mUnitsLayer.aiMoveAct(pMob, xy);
		return true;
	}

	return false;
}

void BattleSystem::aiMoveEnd()
{
	changeRound(eRoundPlayer);
}

void BattleSystem::clearAiAttackInfo()
{
	if (AiClear)
	{
		return;
	}

	for (int i = 0; i < mpAiAttackInfo->mpAttackers->count(); ++i)
	{
		attacker* p = dynamic_cast<attacker*>(mpAiAttackInfo->mpAttackers->objectAtIndex(i));
		safe_release(p)
	}

	safe_release(mpAiAttackInfo->mpAttackers);
	safe_release(mpAiAttackInfo)
	mpAiAttackInfo = NULL;
	AiClear = true;
}

bool BattleSystem::aiCanAttack()
{
	clearAiAttackInfo();
	if (aiAttackPlayerData())
	{
		AiClear = false;
		return true;
	}

	return false;
}

void BattleSystem::aiFirstAttack()
{
	mCurAiAttackRound = 0;
	mpGameScene->mUnitsLayer.aiAttackAct();
}

void BattleSystem::aiNextAttack()
{
	++mCurAiAttackRound;

	if (mCurAiAttackRound < mpAiAttackInfo->mpAttackers->count())
	{
		mpGameScene->mUnitsLayer.aiAttackAct();
	}
	else
	{
		clearAiAttackInfo();
		//round change.
		changeRound(eRoundPlayer);
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void BattleSystem::goNextRound()
{
	mpGameScene->mGameState = eGoNextRound;
	mpGameScene->goNextRoundAct();
}

void BattleSystem::goNextRoundEnd()
{
	mpGameScene->mGameState = eWaitPlayerAct;
}

void BattleSystem::tick()
{
	if (mpGameScene->mGameState == ePlayerAttack)
	{
		BattleSystem::ptr()->playerNextAttack();
	} 
	else if(mpGameScene->mGameState == eMobAttack)
	{
		BattleSystem::ptr()->aiNextAttack();
	}
}

void BattleSystem::changeRound(attackRound round)
{
	mpGameScene->changeRound(round);

	turn = round;
}

void BattleSystem::changRoundEnd()
{
	if (turn == eRoundPlayer)
	{
		mpGameScene->mGameState = eWaitPlayerAct;
	}
	else if (turn == eRoundAi)
	{
		if (aiCanAttack())
		{
			aiFirstAttack();
			mpGameScene->mGameState = eMobAttack;
		}
		else if (aiMove())
		{
			mpGameScene->mGameState = eMobAttack;
		}
		else
		{
			mpGameScene->mGameState = eWaitPlayerAct;
		}
	}
}

void BattleSystem::attackFlash()
{
	mpGameScene->attachFlash();
}