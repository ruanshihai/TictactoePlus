#ifndef _GAME_H_
#define _GAME_H_
#include "cocos2d.h"

USING_NS_CC;

class Game :public Layer
{
public:
	static cocos2d::Scene* createScene(int n = 1);
	virtual bool init();

	CREATE_FUNC(Game);

	void setNumPlayer(int n);
	void preloadMusic();
	void playBgm();
	void pauseBgm();
	void playStepEffect();
	void playChessEffect();
	void playWinEffect();
	void returnMenuPage(Ref *ref);
	void restartGame(Ref *ref);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	Vec2 getIndexFromPos(Vec2 pos);
	Vec2 getPosFromIndex(Vec2 index);
	Vec2 getRandomIndex(Vec2 index);
	Vec2 getGoodIndex(Vec2 index);
	Vec2 getCorrespondingIndex(Vec2 index);
	bool checkGoodLuck(Vec2 index, int player);
	bool checkBadLuck(Vec2 index, int player);
	void moveOnIndex(Vec2 from, Vec2 to);
	void fallChessToIndex(Vec2 index);
	void updateGameStatus();
	bool hasThreePointLine();
	bool canFallChess();
	void win();
private:
	EventDispatcher* dispatcher;
	Sprite* grid[9][9];
	Sprite* cursor;
	Sprite* nextPlayer;
	Size visibleSize;
	int DIS_X, DIS_Y;
	int whoSTurn;
	int nPlayer;
	bool isRunning;
	bool musicEffectEnable;
};
#endif
