/*�Ի��򳡾���ľ���ʵ��*/
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

		//���һ�����ɫ�Ĳ�
		LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
		this->addChild(backLayerColor);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		auto closeButton = Sprite::create("image/close.png");
		closeButton->setPosition(Vec2(winSize.width - closeButton->getContentSize().width / 2, winSize.height - closeButton->getContentSize().height / 2));
		this->addChild(closeButton, 10);

		//���������ı���ͼƬ������������λ��Ϊ������Ļ���е�
		CCSprite * background = CCSprite::create("image/popup-bg.jpg");
		m_bgSprite = background;
		background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(background);

		//��ñ���ͼƬ�Ĵ�С
		CCSize contentSize = background->getContentSize();
		m_size = contentSize;

		//��������˵����������
		CCMenuItemImage * item1 = CCMenuItemImage::create("image/O.png",
			"btn-play-selected.png", "",
			this, menu_selector(PopScene::yesButton));

		CCMenuItemImage * item2 = CCMenuItemImage::create("image/X.png",
			"btn-highscores-selected.png", "",
			this, menu_selector(PopScene::noButton));

		CCMenu * menu = CCMenu::create(item1, item2, NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		menu->setPosition(ccp(contentSize.width / 2, contentSize.height / 3));
		//kCCMenuHandlerPriority��ֵΪ-128��������ǲ˵���ť�Ĵ������ȼ�
		//����menu���ȼ�����������Ϊ��ͨmenu�Ķ�����һ��ԭ���±�
		//menu->setTouchPriority(kCCMenuHandlerPriority * 2 - 1);

		background->addChild(menu);

		//������Ŀ���ı�����
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


//����˵���ť��ʱ����õ��¼�������
void PopScene::yesButton(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}

void PopScene::noButton(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}

//������������Ŀ
void PopScene::setTitle()
{
	//CCLabelTTF * title = CCLabelTTF::create("Tips","",24);
	CCLabelBMFont * title = CCLabelBMFont::create("Tips", "fonts/futura-48.fnt");
	title->setPosition(ccp(m_size.width / 2, m_size.height - title->getContentSize().height / 2));
	m_bgSprite->addChild(title);
}

//���ò������
void PopScene::setContent()
{
	CCLabelTTF * content = CCLabelTTF::create("hello! everyone,welcome to www.jb51.net",
		"", 24);
	content->setPosition(ccp(m_size.width / 2, m_size.height / 2));
	//����ttf���ı���
	content->setDimensions(CCSize(this->m_size.width - 60, this->m_size.height - 100));
	//����ttf��ˮƽ���뷽ʽ
	content->setHorizontalAlignment(kCCTextAlignmentLeft);

	m_bgSprite->addChild(content);
}