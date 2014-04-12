#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"
#include "Total.h"

USING_NS_CC;

inline CCPoint getPositionByXY(int x, int y)
{
	return ccp(x * blockWidth, y * blockWidth);
}

inline CCPoint getPositionByXYEx(XY xy)
{
	return ccp(xy.x * blockWidth, xy.y * blockWidth);
}

inline XY getXYByPosition(CCPoint pt)
{
	return XY(pt.x / blockWidth, pt.y / blockWidth);
}

#endif