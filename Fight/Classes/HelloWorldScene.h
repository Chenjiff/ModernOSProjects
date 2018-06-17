#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	void w_run_callback(Ref* psender);

	void a_run_callback(Ref* psender);

	void s_run_callback(Ref* psender);

	void d_run_callback(Ref* psender);

	void move(int x, int y);

	void attack_callback(Ref* psender);

	void dead_callback(Ref* psender);

	void updateTime(float dt);

	void hpchange(int state);

	void updateMonster(float dt);

	void hitByMonster();

	void gameOver();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	cocos2d::Menu* w_but;
	cocos2d::Menu* a_but;
	cocos2d::Menu* s_but;
	cocos2d::Menu* d_but;
	cocos2d::Menu* x_but;
	cocos2d::Menu* y_but;
	int dtime;
	cocos2d::ProgressTimer* pT;
};
