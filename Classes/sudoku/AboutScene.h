#ifndef _ABOUT_SCENE_H
#define _ABOUT_SCENE_H

#include "cocos2d.h"

USING_NS_CC;

class AboutScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(AboutScene);

	void returnMenuPage(Ref *ref);
};

#endif // _HELP_SCENE_H
