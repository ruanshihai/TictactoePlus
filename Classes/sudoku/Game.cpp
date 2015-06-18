#include "Game.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#pragma execution_character_set("utf-8") 

#define GRID_SIZE 64
#define PLAYER_O  1
#define PLAYER_X  2

using namespace std;
using namespace CocosDenshion;

Scene* Game::createScene(int n)
{
	auto scene = Scene::create();
	auto layer = Game::create();
	layer->setNumPlayer(n);

	scene->addChild(layer);

	return scene;
}

void Game::setNumPlayer(int n)
{
	nPlayer = n;
}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	dispatcher = Director::getInstance()->getEventDispatcher();

	// add bg
	auto bgsprite = Sprite::create("image/bg.jpg");
	float odds;
	float oddsY;
	oddsY = bgsprite->getContentSize().height / visibleSize.height;
	odds = bgsprite->getContentSize().width / visibleSize.width;
	bgsprite->setScaleY(1 / oddsY);
	bgsprite->setScaleX(1 / odds);
	bgsprite->setPosition(Vec2(visibleSize / 2));
	this->addChild(bgsprite, 0);
	
	// add button to go back menu page
	auto backItem = MenuItemImage::create("image/back.png", "image/back.png", CC_CALLBACK_1(Game::returnMenuPage, this));
	backItem->setPosition(visibleSize.width - backItem->getContentSize().width * 2, visibleSize.height - backItem->getContentSize().height * 2);
	// add button to restart the game
	auto restartItem = MenuItemLabel::create(Label::createWithTTF("Replay", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_1(Game::restartGame, this));
	restartItem->setPosition(visibleSize.width / 4 * 3, visibleSize.height / 2);
	auto menuback = Menu::create(backItem, restartItem, NULL);
	menuback->setPosition(Vec2::ZERO);
	this->addChild(menuback, 10);

	// add the map background
	TMXTiledMap* tmx = TMXTiledMap::create("sudoku/chessboard.tmx");
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setPosition(tmx->getContentSize().width / 2 + 50, visibleSize.height / 2);
	DIS_X = tmx->getPositionX() - tmx->getContentSize().width / 2;
	DIS_Y = tmx->getPositionY() - tmx->getContentSize().height / 2;
	this->addChild(tmx, 1);

	// add cursor
	cursor = Sprite::create("image/cursor.png");
	cursor->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	cursor->setPosition(Vec2(DIS_X + GRID_SIZE * (9 / 2), DIS_Y + GRID_SIZE * (9 / 2)));
	this->addChild(cursor, 5);

	// add reminder label and player's logo
	auto reminder = Label::createWithSystemFont("Íæ¼Ò          ÇëÏÂÆå", "Arial", 40);
	reminder->setPosition(visibleSize.width / 4 * 3 + 15, visibleSize.height / 4 *3);
	this->addChild(reminder, 1);
	nextPlayer = Sprite::create("image/O.png");
	nextPlayer->setPosition(visibleSize.width / 4 * 3, visibleSize.height / 4 * 3);
	this->addChild(nextPlayer, 1);

	// backgroud music setting
	auto bgMusicOnItem = MenuItemFont::create("±³¾°ÒôÀÖ  ¿ª");
	bgMusicOnItem->setUserData((void *)"ON");
	auto bgMusicOffItem = MenuItemFont::create("±³¾°ÒôÀÖ  ¹Ø");
	bgMusicOffItem->setUserData((void *)"OFF");
	auto bgMusicMenuToggle = MenuItemToggle::createWithCallback([&](Ref* obj){
		MenuItemFont *item = (MenuItemFont*)((MenuItemToggle *)obj)->getSelectedItem();
		char* musicState = (char*)item->getUserData();
		if (musicState == "ON")
		{
			playBgm();
		}
		else{
			pauseBgm();
		}
	}, bgMusicOnItem, bgMusicOffItem, NULL);
	bgMusicMenuToggle->setPosition(visibleSize.width / 4 * 3, visibleSize.height / 8 + bgMusicMenuToggle->getContentSize().height * 1.2);

	// music effect setting
	auto musicEffectOnItem = MenuItemFont::create("ÒôÐ§      ¿ª");
	musicEffectOnItem->setUserData((void *)"ON");
	auto musicEffectOffItem = MenuItemFont::create("ÒôÐ§      ¹Ø");
	musicEffectOffItem->setUserData((void *)"OFF");
	auto musicEffectMenuToggle = MenuItemToggle::createWithCallback([&](Ref* obj){
		MenuItemFont *item = (MenuItemFont*)((MenuItemToggle *)obj)->getSelectedItem();
		char* musicState = (char*)item->getUserData();
		if (musicState == "ON")
		{
			musicEffectEnable = true;
		}
		else{
			musicEffectEnable = false;
		}
	}, musicEffectOnItem, musicEffectOffItem, NULL);
	musicEffectMenuToggle->setPosition(visibleSize.width / 4 * 3, visibleSize.height / 8);

	auto menu = Menu::create(bgMusicMenuToggle, musicEffectMenuToggle, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 5);

	isRunning = true;
	musicEffectEnable = true;
	whoSTurn = PLAYER_O;
	memset(grid, NULL, sizeof(grid));

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	preloadMusic();
	playBgm();

	return true;
}

void Game::returnMenuPage(Ref *ref)
{
	this->stopAllActions();
	this->pauseBgm();
	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void Game::restartGame(Ref *ref)
{
	this->stopAllActions();
	auto scene = Game::createScene(nPlayer);
	Director::getInstance()->replaceScene(scene);
}

void Game::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mid");
	SimpleAudioEngine::getInstance()->preloadEffect("music/step.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/chess.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/win.mp3");
}

void Game::playBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mid", true);
}

void Game::pauseBgm()
{
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void Game::playStepEffect()
{
	SimpleAudioEngine::getInstance()->playEffect("music/step.mp3");
}

void Game::playChessEffect()
{
	SimpleAudioEngine::getInstance()->playEffect("music/chess.mp3");
}

void Game::playWinEffect()
{
	SimpleAudioEngine::getInstance()->playEffect("music/win.mp3");
}

void Game::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (!isRunning)
		return;

	Vec2 posIndex = getIndexFromPos(cursor->getPosition());

	if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		Vec2 nextPosIndex = posIndex + Vec2(0, 1);
		moveOnIndex(posIndex, nextPosIndex);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		Vec2 nextPosIndex = posIndex + Vec2(-1, 0);
		moveOnIndex(posIndex, nextPosIndex);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		Vec2 nextPosIndex = posIndex + Vec2(0, -1);
		moveOnIndex(posIndex, nextPosIndex);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		Vec2 nextPosIndex = posIndex + Vec2(1, 0);
		moveOnIndex(posIndex, nextPosIndex);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE || keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
		fallChessToIndex(getIndexFromPos(cursor->getPosition()));
	}
}

void Game::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	
}

Vec2 Game::getIndexFromPos(Vec2 pos)
{
	return Vec2((pos.x - DIS_X) / GRID_SIZE, (pos.y - DIS_Y) / GRID_SIZE);
}

Vec2 Game::getPosFromIndex(Vec2 index)
{
	return Vec2(DIS_X + index.x * GRID_SIZE, DIS_Y + index.y * GRID_SIZE);
}

void Game::moveOnIndex(Vec2 from, Vec2 to)
{
	if (to.x >= 0 && to.y >= 0 && int(from.x + 0.1) / 3 == int(to.x + 0.1) / 3 && int(from.y + 0.1) / 3 == int(to.y + 0.1) / 3) {
		if (musicEffectEnable)
			playStepEffect();
		cursor->setPosition(getPosFromIndex(to));
	}
}

Vec2 Game::getRandomIndex(Vec2 index)
{
	int r = int(index.x + 0.1) - 1;
	int c = int(index.y + 0.1) - 1;
	
	while (true) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (!grid[r + i][c + j] && random() % 100 < 20)
					return Vec2(r + i, c + j);
			}
		}
	}
}

Vec2 Game::getGoodIndex(Vec2 index)
{
	int eval[3][3] = { 0 };
	int count = 0;
	int r = int(index.x + 0.1) - 1;
	int c = int(index.y + 0.1) - 1;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (grid[r + i][c + j]) {
				eval[i][j] = -1;
			}
			else {
				if (checkGoodLuck(Vec2(r + i, c + j), PLAYER_X)) {
					return Vec2(r + i, c + j);
				}
				else if (checkBadLuck(Vec2(r + i, c + j), PLAYER_X)) {
					eval[i][j] = 1;
				}
				else {
					eval[i][j] = 2;
					count++;
				}
			}
		}
	}

	while (true) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (!count && eval[i][j] == 1) {
					return Vec2(r + i, c + j);
				}
				else if (count && eval[i][j] == 2){
					if (random() % 100 < 100.0 / count)
						return Vec2(r + i, c + j);
				}
			}
		}
	}
}

Vec2 Game::getCorrespondingIndex(Vec2 index)
{
	int r = int(index.x + 0.1);
	int c = int(index.y + 0.1);
	int nr = r % 3 * 3 + 1;
	int nc = c % 3 * 3 + 1;

	return Vec2(nr, nc);
}

bool Game::checkGoodLuck(Vec2 index, int player)
{
	int r = int(index.x + 0.1);
	int c = int(index.y + 0.1);
	int rCount = 0, cCount = 0;

	for (int i = 0; i < 3; i++) {
		if (r / 3 * 3 + i != r && grid[r / 3 * 3 + i][c] && grid[r / 3 * 3 + i][c]->getTag() == player)
			cCount++;
		if (c / 3 * 3 + i != c && grid[r][c / 3 * 3 + i] && grid[r][c / 3 * 3 + i]->getTag() == player)
			rCount++;
	}
	if (rCount == 2 || cCount == 2)
		return true;

	if (r % 3 == 1 && c % 3 == 1
		&& grid[r - 1][c + 1] && grid[r - 1][c + 1]->getTag() == player
		&& grid[r + 1][c - 1] && grid[r + 1][c - 1]->getTag() == player)
		return true;
	if (r % 3 == 1 && c % 3 == 1
		&& grid[r - 1][c - 1] && grid[r - 1][c - 1]->getTag() == player
		&& grid[r + 1][c + 1] && grid[r + 1][c + 1]->getTag() == player)
		return true;
	if (r % 3 == 0 && c % 3 == 2
		&& grid[r + 1][c - 1] && grid[r + 1][c - 1]->getTag() == player
		&& grid[r + 2][c - 2] && grid[r + 2][c - 2]->getTag() == player)
		return true;
	if (r % 3 == 2 && c % 3 == 2
		&& grid[r - 1][c - 1] && grid[r - 1][c - 1]->getTag() == player
		&& grid[r - 2][c - 2] && grid[r - 2][c - 2]->getTag() == player)
		return true;
	if (r % 3 == 0 && c % 3 == 0
		&& grid[r + 1][c + 1] && grid[r + 1][c + 1]->getTag() == player
		&& grid[r + 2][c + 2] && grid[r + 2][c + 2]->getTag() == player)
		return true;
	if (r % 3 == 2 && c % 3 == 0
		&& grid[r - 1][c + 1] && grid[r - 1][c + 1]->getTag() == player
		&& grid[r - 2][c + 2] && grid[r - 2][c + 2]->getTag() == player)
		return true;

	return false;
}

bool Game::checkBadLuck(Vec2 index, int player)
{
	Vec2 cospondingIndex = getCorrespondingIndex(index);
	int r = int(cospondingIndex.x + 0.1) - 1;
	int c = int(cospondingIndex.y + 0.1) - 1;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (!grid[r + i][c + j] && checkGoodLuck(Vec2(r + i, c + j), PLAYER_O))
				return true;
		}
	}

	return false;
}

void Game::fallChessToIndex(Vec2 index) {
	int row = int(index.x + 0.1);
	int col = int(index.y + 0.1);

	if (!grid[row][col]) {
		if (!(nPlayer == 1 && whoSTurn == PLAYER_X)) {
			if (musicEffectEnable)
				playChessEffect();
		}

		if (whoSTurn == PLAYER_O) {
			auto oIcon = Sprite::create("image/O.png");
			oIcon->setTag(PLAYER_O);
			oIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			oIcon->setPosition(getPosFromIndex(index));
			this->addChild(oIcon, 2);
			grid[row][col] = oIcon;
		}
		else if (whoSTurn == PLAYER_X) {
			auto xIcon = Sprite::create("image/X.png");
			xIcon->setTag(PLAYER_X);
			xIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			xIcon->setPosition(getPosFromIndex(index));
			this->addChild(xIcon, 2);
			grid[row][col] = xIcon;
		}

		updateGameStatus();
	}
}

void Game::updateGameStatus()
{
	if (hasThreePointLine() || !canFallChess()) {
		win();
		return;
	}

	if (whoSTurn == PLAYER_O) {
		whoSTurn = PLAYER_X;
		nextPlayer->setTexture("image/X.png");
	}
	else if (whoSTurn == PLAYER_X) {
		whoSTurn = PLAYER_O;
		nextPlayer->setTexture("image/O.png");
	}

	Vec2 index = getIndexFromPos(cursor->getPosition());
	int r = int(index.x + 0.1);
	int c = int(index.y + 0.1);
	int nr = r % 3 * 3 + 1;
	int nc = c % 3 * 3 + 1;

	if (nPlayer == 1 && whoSTurn == PLAYER_X) {
		//Vec2 nextIndex = getRandomIndex(Vec2(nr, nc));
		Vec2 nextIndex = getGoodIndex(Vec2(nr, nc));
		cursor->setPosition(getPosFromIndex(nextIndex));
		fallChessToIndex(nextIndex);
	}
	else {
		cursor->setPosition(getPosFromIndex(Vec2(nr, nc)));
	}
}

bool Game::hasThreePointLine()
{
	Vec2 index = getIndexFromPos(cursor->getPosition());
	int r = int(index.x + 0.1);
	int c = int(index.y + 0.1);

	if (grid[r / 3 * 3][c] && grid[r / 3 * 3 + 1][c] && grid[r / 3 * 3 + 2][c]
		&& grid[r / 3 * 3][c]->getTag() == grid[r / 3 * 3 + 1][c]->getTag()
		&& grid[r / 3 * 3][c]->getTag() == grid[r / 3 * 3 + 2][c]->getTag())
		return true;
	if (grid[r][c / 3 * 3] && grid[r][c / 3 * 3 + 1] && grid[r][c / 3 * 3 + 2]
		&& grid[r][c / 3 * 3]->getTag() == grid[r][c / 3 * 3 + 1]->getTag()
		&& grid[r][c / 3 * 3]->getTag() == grid[r][c / 3 * 3 + 2]->getTag())
		return true;

	r = r / 3 * 3 + 1;
	c = c / 3 * 3 + 1;
	if (grid[r][c] && grid[r - 1][c + 1] && grid[r + 1][c - 1]
		&& grid[r][c]->getTag() == grid[r - 1][c + 1]->getTag()
		&& grid[r][c]->getTag() == grid[r + 1][c - 1]->getTag())
		return true;
	if (grid[r][c] && grid[r - 1][c - 1] && grid[r + 1][c + 1]
		&& grid[r][c]->getTag() == grid[r - 1][c - 1]->getTag()
		&& grid[r][c]->getTag() == grid[r + 1][c + 1]->getTag())
		return true;

	return false;
}

bool Game::canFallChess()
{
	Vec2 index = getIndexFromPos(cursor->getPosition());
	int r = int(index.x + 0.1);
	int c = int(index.y + 0.1);
	int nr = r % 3 * 3;
	int nc = c % 3 * 3;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (!grid[nr + i][nc + j])
				return true;
		}
	}

	return false;
}

void Game::win()
{
	if (musicEffectEnable)
		playWinEffect();
	if (whoSTurn == PLAYER_O) {
		MessageBox("Player O win!", "Congratulation");
	}
	else if (whoSTurn == PLAYER_X) {
		MessageBox("Player X win!", "Congratulation");
	}

	isRunning = false;
}
