#include "GameScene.h"
#include <Math.h>
#include <string>
#include <sstream>
USING_NS_CC;

Scene* GameScene::createScene()
{
	log("Game Create");
	return Scene::create();
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	log("Game Init");
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//                                      
	auto bg = Sprite::create("level-background-0.jpg");
	Size winsize = Director::getInstance()->getWinSize();
	bg->setScaleX(winsize.width / bg->getTextureRect().getMaxX());
	bg->setScaleY(winsize.height / bg->getTextureRect().getMaxY());
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(bg, 0);

	mouseLayer = Layer::create();
	mouseLayer->setPosition(Vec2(0, visibleSize.height / 2));
	this->addChild(mouseLayer);
	mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate *mouseAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimation));
	mouse->setPosition(Vec2(visibleSize.width / 2, 0));
	mouseLayer->addChild(mouse, 0);

	stoneLayer = Layer::create();
	this->addChild(stoneLayer);
	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(origin.x + visibleSize.width / 2 + 200, origin.y + visibleSize.height - 140));
	stoneLayer->addChild(stone, 1);

	auto shootLabel = Label::create("Shoot", "fonts/Marker Felt.ttf", 46);
	auto shootMenuItem = MenuItemLabel::create(shootLabel, CC_CALLBACK_1(GameScene::shootCallback, this));// this->shootCallback 和 GameScene::shootCallback
	shootMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + 320, origin.y + visibleSize.height - 140));
	auto shootBut = Menu::create(shootMenuItem, NULL);
	shootBut->setPosition(Vec2::ZERO);
	this->addChild(shootBut, 1);

	return true;
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event) {
	auto location = touch->getLocation();
	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(location);
	this->addChild(cheese, 1);

	float distance_sq = (mouse->getPosition().x - location.x)*(mouse->getPosition().x - location.x) +
		(mouse->getPosition().y - location.y + mouseLayer->getPosition().y) * (mouse->getPosition().y - location.y + mouseLayer->getPosition().y);
	float time = sqrt(distance_sq) / 200;
	ActionInterval *cheese_fade = FadeOut::create(1.5f); //ActionInterval 和 Action 区别？
	cheese->runAction(Sequence::create(DelayTime::create(time), cheese_fade, NULL));

	ActionInterval *mouse_move = MoveBy::create(time, mouse->convertToNodeSpaceAR(location));
	mouse->runAction(mouse_move);
	return true;
}

bool GameScene::shootCallback(Ref *pSender) {
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto newStone = Sprite::create("stone.png");
	newStone->setPosition(Vec2(origin.x + visibleSize.width / 2 + 200, origin.y + visibleSize.height - 140));
	stoneLayer->addChild(newStone, 1);

	float distance_sq1 = (stone->getPosition().x - mouse->getPosition().x)*(stone->getPosition().x - mouse->getPosition().x) + 
		(stone->getPosition().y - mouse->getPosition().y - visibleSize.height / 2) * (stone->getPosition().y - mouse->getPosition().y - visibleSize.height / 2);
	float time1 = sqrt(distance_sq1) / 580;
	ActionInterval *stone_shoot = MoveTo::create(time1, Vec2(mouse->getPosition().x, mouse->getPosition().y + visibleSize.height / 2));
	ActionInterval *stone_disapp = FadeOut::create(0.5f);
	stone->runAction(Sequence::create(stone_shoot, stone_disapp, NULL));
	stone = newStone;

	float rand = CCRANDOM_0_1();
	float randX = visibleSize.width * rand;
	float rand2 = CCRANDOM_0_1();
	float randY = visibleSize.height * rand2;
	float distance_sq2 = (mouse->getPosition().x - randX)*(mouse->getPosition().x - randX) +
		(mouse->getPosition().y - randY + mouseLayer->getPosition().y) * (mouse->getPosition().y - randY + mouseLayer->getPosition().y);
	float time2 = sqrt(distance_sq2) / 260;
	ActionInterval *mouse_leave = MoveTo::create(time2, Vec2(randX, randY - mouseLayer->getPosition().y));

	CallFunc *diamond_app = CallFunc::create([&]() {
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Sprite *diamond = Sprite::create("diamond.png");
		diamond->setPosition(Vec2(mouse->getPosition().x + origin.x, mouse->getPosition().y + visibleSize.height / 2 + origin.y));
		this->addChild(diamond, 1);
	});
	mouse->runAction(Sequence::create(DelayTime::create(time1), diamond_app, mouse_leave, NULL));
    //很奇怪，回调函数钻石位置不正常？
	//原因：CallFunc::Create 里的匿名函数域问题，
	//      CallFunc调用机制：使用&仍可接受到visibleSize的地址，但是其已经被释放了
	//解决：将visibleSize声明为成员变量或者不使用外部的visibleSize而是自己寻找visibleSize

	return true;
}