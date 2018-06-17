#include "Thunder.h"
#include <algorithm>

USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Thunder::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Thunder::init() {
	if (!Layer::init()) {
		return false;
	}
	stoneType = 0;
	isMove = false;  // �Ƿ����ɴ�
	visibleSize = Director::getInstance()->getVisibleSize();

	// ��������
	auto bgsprite = Sprite::create("bg.jpg");
	bgsprite->setPosition(visibleSize / 2);
	bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
		visibleSize.height / bgsprite->getContentSize().height);
	this->addChild(bgsprite, 0);

	// �����ɴ�
	player = Sprite::create("player.png");
	player->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPosition(visibleSize.width / 2, player->getContentSize().height);
	player->setName("player");
	this->addChild(player, 1);

	// ��ʾ��ʯ���ӵ�����
	enemysNum = Label::createWithTTF("enemys: 0", "fonts/arial.TTF", 20);
	enemysNum->setColor(Color3B(255, 255, 255));
	enemysNum->setPosition(50, 60);
	this->addChild(enemysNum, 3);
	bulletsNum = Label::createWithTTF("bullets: 0", "fonts/arial.TTF", 20);
	bulletsNum->setColor(Color3B(255, 255, 255));
	bulletsNum->setPosition(50, 30);
	this->addChild(bulletsNum, 3);

	addEnemy(5);        // ��ʼ����ʯ
	preloadMusic();     // Ԥ��������
	playBgm();          // ���ű�������
	explosion();        // ������ը֡����

						// ��Ӽ�����
	addTouchListener();
	addKeyboardListener();
	addCustomListener();

	// ������
	schedule(schedule_selector(Thunder::update), 0.04f, kRepeatForever, 0);

	return true;
}

//Ԥ���������ļ�
void Thunder::preloadMusic() {
	// Todo
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/explore.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/fire.wav");
}

//���ű�������
void Thunder::playBgm() {
	// Todo
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
}

//��ʼ����ʯ
void Thunder::addEnemy(int n) {
	enemys.clear();
	for (unsigned i = 0; i < 3; ++i) {
		char enemyPath[20];
		sprintf(enemyPath, "stone%d.png", 3 - i);
		double width = visibleSize.width / (n + 1.0),
			height = visibleSize.height - (50 * (3 - i));
		for (int j = 0; j < n; ++j) {
			auto enemy = Sprite::create(enemyPath);
			enemy->setAnchorPoint(Vec2(0.5, 0.5));
			enemy->setScale(0.5, 0.5);
			enemy->setPosition(width * (j + 1), height);
			enemys.push_back(enemy);
			addChild(enemy, 1);
		}
	}
}

// ��ʯ�����ƶ��������µ�һ��(�ӷ���)
void Thunder::newEnemy() {
	// Todo
	for (auto it = enemys.begin(); it != enemys.end(); ++it) {
		(*it)->setPosition((*it)->getPosition() + Vec2(0, -50));
	}
	char enemyPath[20];
	float n = random(-1, 2);
	n = n == -1 ? -0.99 : n;
	sprintf(enemyPath, "stone%d.png", int(3 - n));
	double width = visibleSize.width / (5 + 1.0),
		height = visibleSize.height - 50;
	for (int i = 0; i < 5; ++i) {
		auto enemy = Sprite::create(enemyPath);
 		enemy->setAnchorPoint(Vec2(0.5, 0.5));
		enemy->setScale(0.5, 0.5);
		enemy->setPosition(width * (i + 1) - 80, height);
		enemys.push_back(enemy);
		addChild(enemy, 1);
	}
	if (enemys.front()->getPosition().y < 80) {
		stopAc();
	}
}

// �ƶ��ɴ�
void Thunder::movePlane(char c) {
	// Todo
	auto position = player->getPosition();
	if (c == 'A') {
		if (position.x > 40) {
			player->setPosition(position + Vec2(-8, 0));
		}
	}
	else {
		if (position.x < visibleSize.width - 40) {
			player->setPosition(position + Vec2(8, 0));
		}
	}
}

//�����ӵ�
void Thunder::fire() {
	auto bullet = Sprite::create("bullet.png");
	bullet->setAnchorPoint(Vec2(0.5, 0.5));
	bullets.push_back(bullet);
	bullet->setPosition(player->getPosition());
	addChild(bullet, 1);
	SimpleAudioEngine::getInstance()->playEffect("music/fire.wav", false);

	// �Ƴ��ɳ���Ļ����ӵ�
	// Todo
	auto move = MoveBy::create(0.9f, Vec2(0, visibleSize.height - bullet->getPosition().y + bullet->getContentSize().height / 2));
	auto getout = CallFunc::create([&] {
		auto bullet = bullets.front();
		bullets.pop_front();
		bullet->removeFromParentAndCleanup(true);
	});
	bullet->runAction(Sequence::create(move, getout, nullptr));
}

// �иը����֡
void Thunder::explosion() {
	// Todo
	auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
	explore.reserve(8);
	for (int i = 0; i < 8; i++) {
		if (i < 5) {
			auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(193 * i + 1, 0, 142, 142)));
			explore.pushBack(frame);
		}
		else {
			auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(193 * (i - 5) + 1, 193, 142, 142)));
			explore.pushBack(frame);
		}
	}
}

void Thunder::update(float f) {
	// ʵʱ����ҳ������ʯ���ӵ�����(����ɾ��)
	// Ҫ��������ʾ��ȷ(�ӷ���)
	char str[15];
	sprintf(str, "enemys: %d", enemys.size());
	enemysNum->setString(str);
	sprintf(str, "bullets: %d", bullets.size());
	bulletsNum->setString(str);

	// �ɴ��ƶ�
	if (isMove)
		this->movePlane(movekey);

	static int ct = 0;
	static int dir = 4;
	++ct;
	if (ct == 120) {
		ct = 40, dir = -dir;
	}
	else if (ct == 80) {
		dir = -dir;
		newEnemy();  // ��ʯ�����ƶ��������µ�һ��(�ӷ���)
	}
	else if (ct == 20) {
		ct = 40, dir = -dir;
	}

	//��ʯ�����ƶ�
	for (Sprite* s : enemys) {
		if (s != NULL) {
			s->setPosition(s->getPosition() + Vec2(dir, 0));
		}
	}

	// �ַ��Զ����¼�
	EventCustom e("meet");
	_eventDispatcher->dispatchEvent(&e);
}

// �Զ�����ײ�¼�
void Thunder::meet(EventCustom * event) {
	// �ж��ӵ��Ƿ������ʯ��ִ�ж�Ӧ����
	// Todo
	for (auto it1 = bullets.begin(); it1 != bullets.end(); ++it1) {
		for (auto it2 = enemys.begin(); it2 != enemys.end(); ++it2) {
			if ((*it1)->getPosition().getDistance((*it2)->getPosition()) < 20) {
				SimpleAudioEngine::getInstance()->playEffect("music/explore.wav", false);
				(*it1)->runAction(Sequence::create(CallFunc::create([=] {
					(*it1)->removeFromParentAndCleanup(true);
					bullets.erase(it1);
				}), nullptr));
				auto exploreAnimation = Animation::createWithSpriteFrames(explore, 0.1f);
				auto exploreAnimate = Animate::create(exploreAnimation);
				auto enemy = (*it2);
				auto seq = Sequence::create(CallFunc::create([=] {
					enemys.erase(it2);
				}), exploreAnimate, CallFunc::create([=] {
					enemy->removeFromParentAndCleanup(true); 
				}), nullptr);
				(*it2)->runAction(seq);
				return;
			}
		}
	}
}

void Thunder::stopAc() {
	// Todo
	auto animation = Animation::createWithSpriteFrames(explore, 0.1f);
	auto animate = Animate::create(animation);
	SimpleAudioEngine::getInstance()->playEffect("music/explore.wav", false);
	player->runAction(Sequence::create(animate, CallFunc::create([=] {
		this->unscheduleAllCallbacks();
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		auto over = Sprite::create("gameOver.png");
		over->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(over, 1);
		player->removeFromParentAndCleanup(true);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}), nullptr));
}

// ����Զ��������
void Thunder::addCustomListener() {
	// Todo
	auto customEventListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(customEventListener, this);
}

// ��Ӽ����¼�������
void Thunder::addKeyboardListener() {
	// Todo
	auto KeyboardListener = EventListenerKeyboard::create();
	KeyboardListener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	KeyboardListener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KeyboardListener, this);
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		movekey = 'A';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		movekey = 'D';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		fire();
		break;
	}
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
		isMove = false;
		break;
	}
}

// ��Ӵ����¼�������
void Thunder::addTouchListener() {
	// Todo
	auto touchEventListener = EventListenerTouchOneByOne::create();
	touchEventListener->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
	touchEventListener->onTouchEnded = CC_CALLBACK_2(Thunder::onTouchEnded, this);
	touchEventListener->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
}

// ����������ڵ�
bool Thunder::onTouchBegan(Touch *touch, Event *event) {
	if (touch->getLocation().getDistance(player->getPosition()) <= 30)
		isClick = true;
	// Todo
	fire();
	return true;
}

void Thunder::onTouchEnded(Touch *touch, Event *event) {
	isClick = false;
}

// ����갴ס�ɴ���ɿ��Ʒɴ��ƶ� (�ӷ���)
void Thunder::onTouchMoved(Touch *touch, Event *event) {
	// Todo
	Vec2 pos = touch->getDelta();
	if (isClick) {
		player->setPosition(Vec2(player->getPosition().x + pos.x, player->getPosition().y));
	}
}
