#ifndef __PLAYER_DATA_H__
#define __PLAYER_DATA_H__

#include "cocos2d.h"
#include "Total.h"
#include <string>

USING_NS_CC;

void calculateXYWithDirectionAndXY(XY xy, attackDirection direciton, attackType type, CCArray* array);
float getRotationWithAttackDir(attackDirection dir);
CCPoint getPositionWithAttackDir(attackDirection dir);
CCPoint getPointVectorWithAttackDir(attackDirection dir);

class MonsterData : public CCObject
{
public:
	MonsterData();
	~MonsterData();
	
	void initLikeMonster();
	void initLikeMember();

	inline void setXY(int nx, int ny)
	{
		xy.x = nx;
		xy.y = ny;
	}

public:
	int type;
	int id;

	attackType attack[eAttackDirectionMax];
	int damage;
	int hp;
	int maxhp;
	XY xy;
	std::string name;
	std::string header;

	bool death;
};

class attacker : public CCObject
{
public:
	attacker() : mDir(eAttackDirectionMax) {}

	MonsterData* mpMob;
	attackDirection mDir;
};

class CCAttackInfo : public CCObject
{
public:
	CCAttackInfo() : mTotalDmg(0) {}

	MonsterData* mpEnemy;
	CCArray* mpAttackers;
	int mTotalDmg;
};

class CCEnemyAIInfo : public CCObject
{
public:
	MonsterData* mpEnemy;
	CCArray* mpAttackers;
	XY nextXY;
};

class PlayerData : public CCObject
{
public:
	PlayerData();
	~PlayerData();

	void init() {}
	static PlayerData* ptr()
	{
		static PlayerData data;
		return &data;
	}

	void generateMovableXY(int index);
	bool isMovableXY(XY xy);
	void clearPickedInfo();
	void updateMonstersXY(XY xy);
	bool beenTakenPlace(XY xy);

	bool allDied();

public:
	MonsterData mMonster[playerUnitsCount];
	CCArray mMovableXY;
	int mPickedIndex;
	CCPoint mOldPositon;
};

class DungeonData;

class RoundData : public CCObject
{
public:
	RoundData() {}
	~RoundData();

	void init();
	void tick();

	bool generateMoveData(MonsterData** pMob, XY* xy);
	bool generateAttackData(CCEnemyAIInfo** info);
	void generateMovableXY(MonsterData* mob, CCArray* array);

	bool allDied();

public:
	CCArray mMonsters;
	DungeonData* mpDun;
};

class DungeonData
{
public:
	DungeonData();
	~DungeonData();

	void clearRounds();

	void init();
	void tick();

	void calculate() {}

	XY getEmptyXY();
	void updateRoundMonstersXY();

	void updateDish();
	int hasMonsterInXY(XY xy);

	RoundData* curRound();

	static DungeonData* ptr()
	{
		static DungeonData data;
		return &data;
	}

public:
	CCArray mRounds;
	int mCurRound;

	MonsterData* mpDish[mapWidth][mapWidth];
};

#endif