#ifndef __BATTLESYSTEM_H__
#define __BATTLESYSTEM_H__

#include "cocos2d.h"
#include "GameScene.h"

USING_NS_CC;

inline bool isValidXY(XY xy)
{
	if (xy.x >= 0 && xy.x <mapWidth &&
		xy.y >= 0 && xy.y <mapWidth)
	{
		return true;
	}

	return false;
}

bool getXYwithTargetXYAndDir(XY tar, attackDirection dir, XY* destXY);
attackDirection getDirByTwoTargets(XY attacker, XY defender);

class BattleSystem : public CCObject
{
public:
	BattleSystem() : AiClear(true), PlayerClear(true), turn(eRoundPlayer) {}

	static BattleSystem* ptr()
	{
		static BattleSystem data;
		return &data;
	}

	int getIndexIfMemberInXY(XY xy);

	bool playerAttackEnemyData(CCArray* array);
	void clearPlayerAttackInfo();
	void calculateMembersWithTouchableEnemy(MonsterData* pMob, CCArray* array);

	bool playerCanAttack();

	void playerFirstAttack();
	void playerNextAttack();

	//////////////////////////////////////////////////////////////////////////
	bool aiAttackPlayerData();
	void clearAiAttackInfo();

	bool aiCanAttack();
	bool aiMove();
	void aiMoveEnd();

	void aiFirstAttack();
	void aiNextAttack();

	//////////////////////////////////////////////////////////////////////////
	void goNextRound();
	void goNextRoundEnd();

	void tick();

	void changeRound(attackRound round);
	void changRoundEnd();
	void attackFlash();

public:
	CCArray mAttackInfo;
	int mCurPlayerAttackRound;

	CCEnemyAIInfo* mpAiAttackInfo;
	int mCurAiAttackRound;

	GameScene* mpGameScene;

	bool AiClear;
	bool PlayerClear;

	attackRound turn;
};

#endif