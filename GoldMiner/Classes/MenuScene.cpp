#include "MenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
	return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg_sky = Sprite::create("menu-background-sky.jpg");
	bg_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
	this->addChild(bg_sky, 0);

	auto bg = Sprite::create("menu-background.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(bg, 0);

	auto miner = Sprite::create("menu-miner.png");
	miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(miner, 1);

	auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	leg->runAction(RepeatForever::create(legAnimate));
	leg->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(leg, 1);

	auto gold_text = Sprite::create("gold-miner-text.png");
	gold_text->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 550));
	this->addChild(gold_text, 1);

	auto start_button_gold = Sprite::create("menu-start-gold.png");
	start_button_gold->setPosition(Vec2(origin.x + visibleSize.width - 20 - start_button_gold->getContentSize().width / 2 - 10, origin.y + start_button_gold->getContentSize().height / 2 - 10));
	this->addChild(start_button_gold, 1);

	auto start_button = MenuItemImage::create("start-0.png", "start-1.png",
		CC_CALLBACK_1(MenuScene::start_callback, this));
	start_button->setPosition(Vec2(origin.x + visibleSize.width - 20 - start_button_gold->getContentSize().width / 2 - 10, origin.y + start_button_gold->getContentSize().height / 2 + 48 - 10));
	auto start_menu = Menu::create(start_button, NULL);
	start_menu->setPosition(Vec2::ZERO);
	this->addChild(start_menu, 2);

	return true;
}

void MenuScene::start_callback(cocos2d::Ref *pSender) {
	log("Start");
	Director::getInstance()->replaceScene(GameScene::create());//create()和createScene()有什么区别呢（目前只知道前者触发init()）
}