#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

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
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

	auto startItem = MenuItemImage::create(
		"button1.jpg",
		"button2.jpg",
		CC_CALLBACK_1(HelloWorld::startCallback, this));
	if (startItem == nullptr ||
		startItem->getContentSize().width <= 0 ||
		startItem->getContentSize().height <= 0)
	{
		problemLoading("'button1.jpg' and 'button2.jpg'");
	}
	else
	{
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + startItem->getContentSize().height / 2 + 55;
		startItem->setPosition(Vec2(x, y));
	}

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	auto menu2 = Menu::create(startItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 1);
	auto butLabel = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 12);
	butLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + startItem->getContentSize().height / 2 + 55);
	butLabel->setName("butLabel");
	this->addChild(butLabel, 2);
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("16340020\nChen Jifan", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height -10));

        // add the label as a child to this layer
		label->setName("show");
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("yeye.png");
    if (sprite == nullptr)
    {
        problemLoading("'yeye.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
	auto back = Sprite::create("background.jpg");
	if (back == nullptr)
	{
		problemLoading("'background.jpg'");
	}
	else
	{
		Size winsize = Director::getInstance()->getWinSize();
		float winwid = winsize.width;
		float winheig = winsize.height;
		float imgwid = back->getTextureRect().getMaxX();
		float imgheig = back->getTextureRect().getMaxY();
		back->setScaleX(winwid / imgwid);
		back->setScaleY(winheig / imgheig);
		back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(back, -1);
	}
    return true;
}

void HelloWorld::startCallback(Ref* psender) {
	Label* label = static_cast<Label*>(this -> getChildByName("show"));
	Label* butLabel = static_cast<Label*>(this->getChildByName("butLabel"));
	if (label->getString()[0] != 'H') {
		label->setString("Hello Cocos");
		butLabel->setString("Back");
	}
	else {
		label->setString("16340020\nChen Jifan");
		butLabel->setString("Start");
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
