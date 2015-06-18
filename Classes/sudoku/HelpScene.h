#ifndef _HELP_SCENE_H
#define _HELP_SCENE_H

#include "cocos2d.h"

USING_NS_CC;

class HelpScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(HelpScene);

	void returnMenuPage(Ref *ref);
};

#endif // _HELP_SCENE_H
