#ifndef __ANIMATIONSYSTEM_H__
#define __ANIMATIONSYSTEM_H__

class AnimationSystem : public CCObject
{
public:
	static AnimationSystem* ptr()
	{
		static AnimationSystem data;
		return &data;
	}


};

#endif