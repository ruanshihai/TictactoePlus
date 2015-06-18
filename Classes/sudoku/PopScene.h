/*对话框场景类的头文件*/
#ifndef _POP_SCENE_H_
#define _POP_SCENE_H_
#include "cocos2d.h"

USING_NS_CC;

class PopScene : public Layer
{
public:
	static Scene * scene();
	bool init();
	CREATE_FUNC(PopScene);
	//void registerWithTouchDispatcher();
private:
	bool TouchBegan(Touch *pTouch, Event *pEvent);
	void yesButton(CCObject * object);
	void noButton(CCObject * object);
	void setTitle();
	void setContent();
	Size m_size;
	Sprite * m_bgSprite;
};
#endif