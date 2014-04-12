#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
//#include "Total.h"
#include "PlayerData.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////

class GameScene;

class FloorLayer : public CCSprite
{
public:
	FloorLayer();
	~FloorLayer();

	virtual bool init();

	void initWithRounds(int i);

public:
	int x;
	int y;

	int width;
	int height;
};

class RouteLayer : public CCLayer
{
public:
	RouteLayer() {}
	~RouteLayer();

	virtual bool init();

	void setBlockVisible(int x, int y);
	void updateAllVisible(CCArray* array);
	void invisibleAll();

public:
	CCSprite* mpSprites[mapWidth][mapHeight];
};

class Unit : public CCObject
{
public:
	Unit() {}
	~Unit();

	void initWithMonsterData(MonsterData* pData);
	void setHpBar(float rate);
	void setVisible(bool b = true);

public:
	CCSprite* mpSelf;
	CCArray mAttackDir;
	CCArray mAttackArrow;

	CCSprite* mpHpBG;
	CCSprite* mpHp;

	int mHpLen;

	int mId;
};

class UnitsLayer : public CCLayer
{
public:
	UnitsLayer() {}
	~UnitsLayer();

	virtual bool init();

	void initPlayerUnits();
	void initEnemyUnits();

	Unit* getEnemyById(int id);

	int getTouchedSpriteIndex(CCTouch* touch);
	void setPickedInfo(int index);
	void updatePickedSpritePosition(CCTouch* touch);
	bool dropSprite(CCTouch* touch);

	void addTip(int n, ccColor3B color, CCSprite* p);
	void clearTipList();

	void attachEffects();

	//////////////////////////////////////////////////////////////////////////
	void playerAttackAct();
	void showDamageTip(CCNode* sender, void* data);
	void playerAttackActEnd();

	void enemyBeAttackedAct();
	void enemyBeAttackedActEnd();

	//////////////////////////////////////////////////////////////////////////
	//mark
	void aiMoveAct(MonsterData* pMob, XY xy);
	void aiMoveActEnd();

	void aiAttackAct();
	void aiAttackActEnd();

	void memberBeAttackedAct();
	void memberBeAttackedActEnd();
	
	//////////////////////////////////////////////////////////////////////////
	void goNextRoundAct();
	void goNextRoundActEnd();

public:
	CCArray mMonsterArray;
	Unit* mpPlayerUnits[playerUnitsCount];

	int syncActNum;
	int syncActNumA;
	CCArray* mpActionArr;
	CCArray mDamageTips;
};

class MapLayer : public CCLayer
{
public:
	MapLayer() {}
	~MapLayer();

public:
	virtual bool init();

public:
	FloorLayer mFloorLayer;
};

//////////////////////////////////////////////////////////////////////////

// class HeaderLayer : public CCLayer
// {
// public:
// 	ccs
// };

class MemberHeader : public CCObject
{
public:
	MemberHeader() {}
	~MemberHeader();

	void init();
	void setHeader(const char* name);

public:
	CCSprite* mpHeader;
	CCSprite* mpHeadRect;
	CCSprite* mpLayer;
};

class DungeonUILayer : public CCLayer
{
public:
	DungeonUILayer() {}
	~DungeonUILayer();
	virtual bool init();
	void setHeader(MonsterData* pData);

public:
	CCSprite* mpTopBG;
	CCSprite* mpMiddleBG;
	CCSprite* mpDownBG;

	MemberHeader* mpHeader[playerUnitsCount];
};

//////////////////////////////////////////////////////////////////////////

class GameScene : public cocos2d::CCLayer
{
public:
	GameScene() {}
	~GameScene();

    virtual bool init();  

    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);

public:
	void initBG();
	void initSprites();

	void goNextRoundAct();
	void changeRound(attackRound round);
	void changeRoundEnd();
	void attachFlash();
	void attachFlashEnd();

	void menuCloseCallback(CCObject* pSender);

	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);

public:
	MapLayer mMapLayer;
	UnitsLayer mUnitsLayer;
	RouteLayer mRouteLayer;
	DungeonUILayer mDungeonUILayer;

	gameState mGameState;

	CCSprite* mpPlayerTurn;
	CCSprite* mpAiTurn;
	CCSprite* mpAttackFlash;
};

//////////////////////////////////////////////////////////////////////////

#endif  // __HELLOWORLD_SCENE_H__