#include "MenuScene.h"
#include "AboutScene.h"
#pragma execution_character_set("utf-8") 

USING_NS_CC;

Scene* AboutScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AboutScene::create();

	scene->addChild(layer);

	return scene;
}

bool AboutScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add bg
	auto bgsprite = Sprite::create("image/about_bg.png");
	float odds;
	float oddsY;
	oddsY = bgsprite->getContentSize().height / visibleSize.height;
	odds = bgsprite->getContentSize().width / visibleSize.width;
	bgsprite->setScaleY(1 / oddsY);
	bgsprite->setScaleX(1 / odds);
	bgsprite->setPosition(Vec2(visibleSize / 2) + origin);
	this->addChild(bgsprite, 0);

	// add button to go back menu page
	auto backItem = MenuItemImage::create("image/back.png", "image/back.png", CC_CALLBACK_1(AboutScene::returnMenuPage, this));
	backItem->setPosition(backItem->getContentSize().width * 2, visibleSize.height - backItem->getContentSize().height * 2);
	auto menuback = Menu::create(backItem, NULL);
	menuback->setPosition(Vec2::ZERO);
	this->addChild(menuback, 10);

	return true;
}

void AboutScene::returnMenuPage(Ref *ref)
{
	this->stopAllActions();
	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

