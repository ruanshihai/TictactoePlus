#include "MenuScene.h"
#include "HelpScene.h"
#include "AboutScene.h"
#include "Game.h"
#pragma execution_character_set("utf-8") 

USING_NS_CC;

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();

	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add logo
	auto logo = Sprite::create("image/logo.png");
	logo->setPosition(visibleSize.width / 4, visibleSize.height / 2 + logo->getContentSize().height);
	this->addChild(logo, 1);

	// add label
	auto label = Label::createWithSystemFont("9¡Á9", "Arial", 60);
	label->setColor(Color3B::BLACK);
	label->setPosition(logo->getPositionX() + logo->getContentSize().width / 2, logo->getPositionY() - logo->getContentSize().height);
	this->addChild(label, 1);

	// create menu
	auto singlePlayer = MenuItemImage::create("image/single_player_normal.png", "image/single_player_clicked.png", CC_CALLBACK_1(MenuScene::SinglePlayer, this));
	auto twoPlayer = MenuItemImage::create("image/two_player_normal.png", "image/two_player_clicked.png", CC_CALLBACK_1(MenuScene::TwoPlayer, this));
	auto help = MenuItemImage::create("image/help_normal.png", "image/help_clicked.png", CC_CALLBACK_1(MenuScene::Help, this));
	auto about = MenuItemImage::create("image/about_normal.png", "image/about_clicked.png", CC_CALLBACK_1(MenuScene::About, this));
	auto menuLineOne = Menu::create(singlePlayer, twoPlayer, NULL);
	menuLineOne->setPosition(visibleSize.width / 4 * 3, visibleSize.height / 2 + 100);
	menuLineOne->alignItemsHorizontallyWithPadding(0);
	this->addChild(menuLineOne, 1);
	auto menuLineTwo = Menu::create(help, about, NULL);
	menuLineTwo->setPosition(visibleSize.width / 4 * 3, visibleSize.height / 2 - 100);
	menuLineTwo->alignItemsHorizontallyWithPadding(0);
	this->addChild(menuLineTwo, 1);

	// add bg
	auto bgsprite = Sprite::create("image/bg.jpg");
	float odds;
	float oddsY;
	oddsY = bgsprite->getContentSize().height / visibleSize.height;
	odds = bgsprite->getContentSize().width / visibleSize.width;
	bgsprite->setScaleY(1 / oddsY);
	bgsprite->setScaleX(1 / odds);
	bgsprite->setPosition(Vec2(visibleSize / 2) + origin);
	this->addChild(bgsprite, 0);

	return true;
}

void MenuScene::SinglePlayer(Ref *ref){
	this->stopAllActions();
	auto scene = Game::createScene(1);
	Director::getInstance()->replaceScene(scene);
}

void MenuScene::TwoPlayer(Ref *ref){
	this->stopAllActions();
	auto scene = Game::createScene(2);
	Director::getInstance()->replaceScene(scene);
}

void MenuScene::Help(Ref *ref) {
	this->stopAllActions();
	auto scene = HelpScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MenuScene::About(Ref *ref) {
	this->stopAllActions();
	auto scene = AboutScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
