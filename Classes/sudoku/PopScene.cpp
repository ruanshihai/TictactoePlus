/*对话框场景类的具体实现*/
#include "PopScene.h"

CCScene * PopScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		scene = CCScene::create();
		PopScene * layer = PopScene::create();
		scene->addChild(layer);
	} while (0);

	return scene;
}

bool PopScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		//添加一个半灰色的层
		LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
		this->addChild(backLayerColor);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		auto closeButton = Sprite::create("image/close.png");
		closeButton->setPosition(Vec2(winSize.width - closeButton->getContentSize().width / 2, winSize.height - closeButton->getContentSize().height / 2));
		this->addChild(closeButton, 10);

		//设置这个层的背景图片，并且设置其位置为整个屏幕的中点
		CCSprite * background = CCSprite::create("image/popup-bg.jpg");
		m_bgSprite = background;
		background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(background);

		//获得背景图片的大小
		CCSize contentSize = background->getContentSize();
		m_size = contentSize;

		//添加俩个菜单在这个层中
		CCMenuItemImage * item1 = CCMenuItemImage::create("image/O.png",
			"btn-play-selected.png", "",
			this, menu_selector(PopScene::yesButton));

		CCMenuItemImage * item2 = CCMenuItemImage::create("image/X.png",
			"btn-highscores-selected.png", "",
			this, menu_selector(PopScene::noButton));

		CCMenu * menu = CCMenu::create(item1, item2, NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		menu->setPosition(ccp(contentSize.width / 2, contentSize.height / 3));
		//kCCMenuHandlerPriority的值为-128，代表的是菜单按钮的触摸优先级
		//设置menu优先级，这里设置为普通menu的二倍减一，原因看下边
		//menu->setTouchPriority(kCCMenuHandlerPriority * 2 - 1);

		background->addChild(menu);

		//设置题目和文本内容
		this->setTitle();
		this->setContent();

		this->setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

bool PopScene::TouchBegan(Touch * touch, Event * pevent)
{
	return true;
}


//点击菜单按钮的时候调用的事件处理函数
void PopScene::yesButton(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}

void PopScene::noButton(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}

//设置这个层的题目
void PopScene::setTitle()
{
	//CCLabelTTF * title = CCLabelTTF::create("Tips","",24);
	CCLabelBMFont * title = CCLabelBMFont::create("Tips", "fonts/futura-48.fnt");
	title->setPosition(ccp(m_size.width / 2, m_size.height - title->getContentSize().height / 2));
	m_bgSprite->addChild(title);
}

//设置层的内容
void PopScene::setContent()
{
	CCLabelTTF * content = CCLabelTTF::create("hello! everyone,welcome to www.jb51.net",
		"", 24);
	content->setPosition(ccp(m_size.width / 2, m_size.height / 2));
	//设置ttf的文本域
	content->setDimensions(CCSize(this->m_size.width - 60, this->m_size.height - 100));
	//设置ttf的水平对齐方式
	content->setHorizontalAlignment(kCCTextAlignmentLeft);

	m_bgSprite->addChild(content);
}