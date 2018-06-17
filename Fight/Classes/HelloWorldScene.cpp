#include "HelloWorldScene.h"
#include "Monster.h"
#include "SimpleAudioEngine.h"
#pragma execution_character_set("utf-8")

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//添加TMX地图文件
	TMXTiledMap* map = TMXTiledMap::create("map.tmx");
	map->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	map->setAnchorPoint(Vec2(0.5f, 0.5f));
	map->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(map, 0);

	//按钮
	auto w_label = Label::create("W", "fonts/arial.ttf", 36);
	auto w_item = MenuItemLabel::create(w_label, CC_CALLBACK_1(HelloWorld::w_run_callback, this));
	w_item->setPosition(Vec2(origin.x + 65, origin.y + 75));
	w_but = Menu::create(w_item, NULL);
	w_but->setPosition(Vec2::ZERO);
	this->addChild(w_but);
	auto a_label = Label::create("A", "fonts/arial.ttf", 36);
	auto a_item = MenuItemLabel::create(a_label, CC_CALLBACK_1(HelloWorld::a_run_callback, this));
	a_item->setPosition(Vec2(origin.x + 25, origin.y + 30));
	a_but = Menu::create(a_item, NULL);
	a_but->setPosition(Vec2::ZERO);
	this->addChild(a_but);
	auto s_label = Label::create("S", "fonts/arial.ttf", 36);
	auto s_item = MenuItemLabel::create(s_label, CC_CALLBACK_1(HelloWorld::s_run_callback, this));
	s_item->setPosition(Vec2(origin.x + 65, origin.y + 30));
	s_but = Menu::create(s_item, NULL);
	s_but->setPosition(Vec2::ZERO);
	this->addChild(s_but);
	auto d_label = Label::create("D", "fonts/arial.ttf", 36);
	auto d_item = MenuItemLabel::create(d_label, CC_CALLBACK_1(HelloWorld::d_run_callback, this));
	d_item->setPosition(Vec2(origin.x + 105, origin.y + 30));
	d_but = Menu::create(d_item, NULL);
	d_but->setPosition(Vec2::ZERO);
	this->addChild(d_but);
	auto x_label = Label::create("X", "fonts/arial.ttf", 36);
	auto x_item = MenuItemLabel::create(x_label, CC_CALLBACK_1(HelloWorld::attack_callback, this));
	x_item->setPosition(Vec2(origin.x + visibleSize.width - 40, origin.y + 75));
	x_but = Menu::create(x_item, NULL);
	x_but->setPosition(Vec2::ZERO);
	this->addChild(x_but);
	auto y_label = Label::create("Y", "fonts/arial.ttf", 36);
	auto y_item = MenuItemLabel::create(y_label, CC_CALLBACK_1(HelloWorld::dead_callback, this));
	y_item->setPosition(Vec2(origin.x + visibleSize.width - 40, origin.y + 30));
	y_but = Menu::create(y_item, NULL);
	y_but->setPosition(Vec2::ZERO);
	this->addChild(y_but);

	//怪物调度器
	schedule(schedule_selector(HelloWorld::updateMonster), 2.0f);
	//时间及其调度器

	dtime = 180;
	std::string timestr = StringUtils::toString(dtime);
	time = Label::create(timestr, "fonts/arial.ttf", 36);
	time->setPosition(origin.x + visibleSize.width / 2 - time->getWidth() / 2, origin.y + visibleSize.height - 50);
	this->addChild(time);
	schedule(schedule_selector(HelloWorld::updateTime), 1.0f);

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	addChild(player, 3);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x + 14 * pT->getContentSize().width, origin.y + visibleSize.height - 2 * pT->getContentSize().height));
	addChild(pT, 1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0, 0);

	// 静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	// 攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113 * i, 0, 113, 113)));
		attack.pushBack(frame);
	}
	// 可以仿照攻击动画
	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}

	// 运动动画(帧数：8帧，高：101，宽：68）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	run.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}

	return true;
}
//攻击回调（按下X键）
void HelloWorld::attack_callback(Ref *psender) {
	auto ani = Animation::createWithSpriteFrames(attack, 0.1f);
	ani->setRestoreOriginalFrame(true);
	auto anima = Animate::create(ani);
	anima->setTag(1);
	if (player->getNumberOfRunningActionsByTag(1) == 0) {
		player->runAction(anima);

		//判断是否攻击到怪物
		Rect playerRect = player->getBoundingBox();
		Rect attackRect = Rect(playerRect.getMinX() - 40, playerRect.getMinY(), playerRect.getMaxX() - playerRect.getMinX() + 80, playerRect.getMaxY() - playerRect.getMinY());
		auto fac = Factory::getInstance();
		auto coll = fac->collider(attackRect);
		if (coll != NULL) {
			hpchange(1);
		}
		while (coll != NULL) {
			fac->removeMonster(coll);
			coll = fac->collider(attackRect);
		}
	}
}
//死亡回调（按下Y键）
void HelloWorld::dead_callback(Ref *psender) {
	auto ani = Animation::createWithSpriteFrames(dead, 0.1f);
	ani->setRestoreOriginalFrame(true);
	auto anima = Animate::create(ani);
	anima->setTag(1);
	if (player->getNumberOfRunningActionsByTag(1) == 0) {
		player->runAction(anima);
		hpchange(0);
	}
}
//以下为向不同方向移动的回调函数（分别按下WASD键）
void HelloWorld::w_run_callback(Ref *psender) {
	int y = 0;
	auto location = player->getPosition();
	if (location.y + 50 <= origin.y + visibleSize.height - player->getContentSize().height / 2) {
		y = 50;
	}
	move(0, y);
}

void HelloWorld::a_run_callback(Ref *psender) {
	player->setFlippedX(true);
	int x = 0;
	auto location = player->getPosition();
	if (location.x - 50 >= origin.x + player->getContentSize().width / 2) {
		x = -50;
	}
	move(x, 0);
}

void HelloWorld::s_run_callback(Ref *psender) {
	int y = 0;
	auto location = player->getPosition();
	if (location.y - 50 >= origin.y + player->getContentSize().height / 2) {
		y = -50;
	}
	move(0, y);
}

void HelloWorld::d_run_callback(Ref *psender) {
	player->setFlippedX(false);
	int x = 0;
	auto location = player->getPosition();
	if (location.x + 50 <= origin.x + visibleSize.width - player->getContentSize().width / 2) {
		x = 50;
	}
	move(x, 0);
}
//同时调用动画并移动到目标位置
void HelloWorld::move(int x, int y) {
	auto ani = Animation::createWithSpriteFrames(run, 0.11f);
	ani->setRestoreOriginalFrame(true);
	auto anima = Animate::create(ani);
	auto move = MoveBy::create(0.77, Vec2(x, y));
	auto spawn = Spawn::createWithTwoActions(anima, move);
	auto seq1 = Sequence::create(spawn, CallFunc::create(this, callfunc_selector(HelloWorld::hitByMonster)), nullptr);
	seq1->setTag(3);
	auto seq2 = Sequence::create(move, CallFunc::create(this, callfunc_selector(HelloWorld::hitByMonster)), nullptr);
	seq2->setTag(2);
	//实现移动的同时可以攻击或死亡
	if (player->getNumberOfRunningActions() != 0) {
		if (player->getNumberOfRunningActionsByTag(3) == 0 && player->getNumberOfRunningActionsByTag(2) == 0) {
			player->runAction(seq2);
		}
	}
	else
	{
		player->runAction(seq1);
	}
	hitByMonster();
}
//实现时间调度器的update
void HelloWorld::updateTime(float dt) {
	if (dtime > 0) {
		dtime--;
		std::string timestr = StringUtils::toString(dtime);
		time->setString(timestr);
	}
	else {
		Director::getInstance()->end();
	}
}
//血条变化
void HelloWorld::hpchange(int state) {
	int per;
	if (state) {
		per = pT->getPercentage() + 20 > 100 ? 100 : pT->getPercentage() + 20;
	}
	else {
		per = pT->getPercentage() - 20 < 0 ? 0 : pT->getPercentage() - 20;
	}
	pT->setPercentage(per);
	if (per == 0) {
		gameOver();
	}
}
//实现怪物调度器的update
void HelloWorld::updateMonster(float dt) {
	auto fac = Factory::getInstance();
	auto mon = fac->createMonster();
	auto ranX = CCRANDOM_0_1() * visibleSize.width;
	auto ranY = CCRANDOM_0_1() * visibleSize.height;
	mon->setPosition(origin.x + ranX, origin.y + ranY);
	this->addChild(mon);
	fac->moveMonster(player->getPosition(), 0.6f);
	hitByMonster();
}
//判断是否被怪物碰撞，是则掉血
void HelloWorld::hitByMonster() {
	auto fac = Factory::getInstance();
	auto coll = fac->collider(player->getBoundingBox());
	while (coll != NULL) {
		fac->removeMonster(coll);
		hpchange(0);
		coll = fac->collider(player->getBoundingBox());
	}
}
//游戏结束，取消所有绑定
void HelloWorld::gameOver() {
	time->setString("GameOver~");
	this->unschedule(schedule_selector(HelloWorld::updateTime));
	this->unschedule(schedule_selector(HelloWorld::updateMonster));
	auto ani = Animation::createWithSpriteFrames(dead, 0.1f);
	auto anima = Animate::create(ani);
	player->runAction(anima);
	w_but->setEnabled(false);
	a_but->setEnabled(false);
	s_but->setEnabled(false);
	d_but->setEnabled(false);
	x_but->setEnabled(false);
	y_but->setEnabled(false);
}