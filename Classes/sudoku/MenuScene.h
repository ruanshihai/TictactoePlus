#ifndef _MENU_SCENE_H
#define _MENU_SCENE_H

#include "cocos2d.h"

USING_NS_CC;

class MenuScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(MenuScene);

	void SinglePlayer(Ref *ref);
	void TwoPlayer(Ref *ref);
	void Help(Ref *ref);
	void About(Ref *ref);
};

#endif // _MENU_SCENE_H
