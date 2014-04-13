#ifndef __SKILL_MGR_H__
#define __SKILL_MGR_H__

#include "cocos2d.h"
#include "Total.h"

USING_NS_CC;

enum skillType
{
	eSkillNone = 0,
	eSkillImm,
	eSkillNoImm,
};

class ISkill : public CCObject
{
public:
	virtual void act();

public:
	skillType mType;
};

class SkillMgr
{

public:
	CCArray mImmSkillList;
	CCArray mLastsSkillList;
};

#endif