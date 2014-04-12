#ifndef __TOTAL_H__
#define __TOTAL_H__

#include "cocos2d.h"

USING_NS_CC;

#define screenWidth 640
#define screenHeight 960

#define mapWidth 4
#define mapHeight 4

#define blockWidth 160
#define attackDirWidth 40

#define playerUnitsCount 4

#define attackArrowWidth 40

const float singleActionTime = 0.5f;

#define memberType 1
#define enemyType 2

enum monsterAttribute
{
	eMonsterAttributeFire = 0,
	eMonsterAttributeWater,
	eMonsterAttributeWood,
	eMonsterAttributeLight,
	eMonsterAttributeDark,
	eMonsterAttributeMax,
};

enum attackDirection
{
	eAttackDirectionL = 0,
	eAttackDirectionR,
	eAttackDirectionU,
	eAttackDirectionD,
	eAttackDirectionLU,
	eAttackDirectionRU,
	eAttackDirectionLD,
	eAttackDirectionRD,
	eAttackDirectionMax,
};

enum attackType
{
	eAttackNone = 0,
	eAttackOnce,
	eAttackTwice,
	eAttackMax,
};

enum gameState
{
	eIdle = 0,
	eSpriteAnimate,

	eWaitPlayerAct,
	eMemberPick,
	ePlayerAttack,

	eMobAttack,
	
	eGoNextRound,
};

enum attackRound
{
	eRoundNone = 0,
	eRoundPlayer,
	eRoundAi,
	eRoundMax,
};

//////////////////////////////////////////////////////////////////////////

class XY : public CCObject
{
public:
	XY(int nx = 0, int ny = 0)
	{
		x = nx;
		y = ny;
	}

	XY& operator = (const XY xy)
	{
		this->x = xy.x;
		this->y = xy.y;

		return (*this);
	}

	XY& operator - (const XY xy)
	{
		this->x -= xy.x;
		this->y -= xy.y;

		return (*this);
	}

	bool operator == (const XY xy)
	{
		return (this->x == xy.x && this->y == xy.y);
	}

	int x;
	int y;
};

#define safe_delete(p)\
	if (p)\
{\
	delete (p);\
	p = NULL;\
}

#define safe_release(p)\
	if (p)\
{\
	(p)->release();\
	p = NULL;\
}

#endif