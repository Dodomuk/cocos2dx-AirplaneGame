/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "HelloMenu.h"

USING_NS_CC;
using namespace CocosDenshion;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}
Size winSize;
Sprite* Dragon;
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	MusicStart();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	initData();
	initBG();

	initPlayer();
	initScore();
	initHP();
	this->scheduleUpdate();
	this->schedule(schedule_selector(HelloWorld::setMissile), 0.2);
	this->schedule(schedule_selector(HelloWorld::setItem), 5.0+rand()%4);
	this->schedule(schedule_selector(HelloWorld::setEnemy), 1);
    return true;
}


//처음 실행시 리스트와 모든것 기본상태로 만듬//
void HelloWorld::initData()
{
	winSize = Director::getInstance()->getWinSize();
	items.clear();
	enemies.clear();
	enemiesnamed.clear();
	missiles.clear();
	isGetItem = false;
	PlayerHp = 3;

	nScore = 0;
	nScoreHigh = UserDefault::getInstance()->getIntegerForKey("HIGH_SCORE", 0);
}


//게임 배경//
void HelloWorld::initBG()
{
	auto bgLayer = Layer::create();
	this->addChild(bgLayer);

	auto spr_1_0 = Sprite::create("res/spaces.png");
	spr_1_0->setAnchorPoint(Point::ZERO);
	spr_1_0->setPosition(Point::ZERO);
	bgLayer->addChild(spr_1_0);

	auto spr_1_1 = Sprite::create("res/spaces.png", Rect(0, 0, 550, 780));
	spr_1_1->setAnchorPoint(Point::ZERO);
	spr_1_1->setPosition(Point(0, spr_1_0->getContentSize().height));
	bgLayer->addChild(spr_1_1);

	auto action_0 = MoveBy::create(5.0, Point(0, -780));
	auto action_1 = Place::create(Point::ZERO);
	auto action_2 = Sequence::create(action_0, action_1, NULL);
	auto action_3 = RepeatForever::create(action_2);

	bgLayer->runAction(action_3);
}

//플레이어 드래곤 정보//
void HelloWorld::initPlayer()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dragon.plist");
	Dragon = Sprite::createWithSpriteFrameName("dragon_1.png");
	Dragon->setPosition(Vec2(240, 50));
	Dragon->setTag(TAG_SPRITE_PLAYER);
	this->addChild(Dragon);
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.2);
	for (int i = 1; i < 4; i++) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("dragon_%d.png", i));
		animation->addSpriteFrame(frame);
	}
	auto animate = Animate::create(animation);
	Dragon->runAction(RepeatForever::create(animate));
}

//미사일 정보//
void HelloWorld::setMissile(float delta)
{
	auto sprPlayer = Dragon;// (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);
	Sprite *sprMissile;
	auto animation_shot = Animation::create();
	if (isGetItem) {// 아이템을 먹으면 총알이 변경//
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("powershot.plist");
		sprMissile = Sprite::createWithSpriteFrameName("powershot_1.png");
		sprMissile->setPosition(Vec2(Dragon->getPositionX(), Dragon->getPositionY() + 30));
		sprMissile->setTag(5);

		animation_shot->setDelayPerUnit(0.5);
		for (int i = 1; i < 4; i++) {
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("powershot_%d.png", i));
			animation_shot->addSpriteFrame(frame);

		}

	}

	else {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shot.plist");
		sprMissile = Sprite::createWithSpriteFrameName("shot_1.png");
		sprMissile->setPosition(Vec2(Dragon->getPositionX(), Dragon->getPositionY() + 30));
		sprMissile->setTag(1);


		animation_shot->setDelayPerUnit(0.5);
		for (int i = 1; i < 4; i++) {
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("shot_%d.png", i));
			animation_shot->addSpriteFrame(frame);

		}

	}

	sprMissile->setPosition(sprPlayer->getPosition() + Vec2(-1, 20));
	this->addChild(sprMissile);
	missiles.pushBack(sprMissile);

	auto animate_shot = Animate::create(animation_shot);
	auto action = Sequence::create(
		MoveBy::create(1.0, Vec2(0, winSize.height)),
		CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetMissile, this)),
		NULL);
	sprMissile->runAction(animate_shot);
	sprMissile->runAction(action);
}

//미사일이 MoveBy로 이동이 다되면 미사일을 삭제하는 함수//
void HelloWorld::resetMissile(Ref *sender)
{
	auto sprMissile = (Sprite*)sender;
	missiles.eraseObject(sprMissile);
	this->removeChild(sprMissile);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	posStartTouch = touch->getLocation();
	auto sprPlayer = Dragon;//(Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);
	posStartPlayer = sprPlayer->getPosition();
	return true;
}
void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{

	auto location = touch->getLocation();
	Dragon->setPosition(location.x, 50);
}

//총알을 강하게 만드는 아이템 함수//
void HelloWorld::setItem(float delta)
{
	//PADDING_SCREEN = 아이템이 끝부분에 안나오게 하기위해 10정도 간격을 둠
	int x = PADDING_SCREEN + rand() % ((int)winSize.width - PADDING_SCREEN * 2);
	auto sprItem = Sprite::create("game/item.png");
	sprItem->setPosition(Vec2(x, winSize.height));
	this->addChild(sprItem);

	items.pushBack(sprItem);
	auto action = Sequence::create(
		MoveBy::create(3.0, Vec2(0, -winSize.height)),
		CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetItem, this)),
		NULL);
	sprItem->runAction(action);
}

//아이템이 화면상 사라지면 아이템이랑 리스트둘다 삭제//
void HelloWorld::resetItem(Ref *sender)
{
	auto sprItem = (Sprite*)sender;

	items.eraseObject(sprItem);

	this->removeChild(sprItem);
}

void HelloWorld::update(float delta)
{
	Rect rectPlayer = Rect(Dragon->getPositionX() - 10, Dragon->getPositionY() - 10, 20, 20); // 플레이어의 피격 범위를 낮춤
	auto removeSpr = Sprite::create();
	for (Sprite* sprItem : items) {
		Rect rectItem = sprItem->getBoundingBox();
		if (rectPlayer.intersectsRect(rectItem)) { // 아이템과 플레이어가 부딪힌지 확인
			removeSpr = sprItem; // 부딪히면 removeSpr에 sprItem 넣음
			//아이템 먹을시 소리
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance(); 
			audio->preloadEffect("Powerup.wav");
			audio->playEffect("Powerup.wav");
		}
	}
	if (items.contains(removeSpr)) { //아이템 Vector에 해당 Sprite가 있는지 확인 있으면  resetItem를 통해 데이터 제거
		resetItem(removeSpr);
		isGetItem = true; // 미사일 파워업 함
		this->scheduleOnce(schedule_selector(HelloWorld::resetGetItem), 5.0); // 5초뒤 종료//
	}

	auto removeMissile = Sprite::create();
	auto removeEnemy = Sprite::create();
		for(int i=0; i<missiles.size(); i++){
			Sprite* sprMissile = missiles.at(i);
		Rect rectMissile = sprMissile->getBoundingBox();
		for (Sprite* sprEnemys : enemies) {
			Rect rectEnemy = Rect(sprEnemys->getPositionX() - 10, sprEnemys->getPositionY() - 10, 20, 20);

			//일반 몬스터가 죽을때 코드//
			if (rectMissile.intersectsRect(rectEnemy)) {
				int attack = sprMissile->getTag();
				int hp = sprEnemys->getTag();
				removeMissile = sprMissile;
				if (hp - attack > 0) {
					sprEnemys->setTag(hp - attack);
				}
				else {
					removeEnemy = sprEnemys;
					addScore(1); // 몬스터가 죽으면 addScore에 1점 추가//
					//몬스터 죽는 소리//
					auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					audio->preloadEffect("Hit_Hurt.wav");
					audio->playEffect("Hit_Hurt.wav");
				}
			}

		}
		//네임드 몬스터가 죽을때 코드//
		for (Sprite* sprEnemyNamed : enemiesnamed)
		{
			Rect rectEnemyNamed = Rect(sprEnemyNamed->getPositionX() - 10, sprEnemyNamed->getPositionY() - 10, 20, 20);

			if (rectMissile.intersectsRect(rectEnemyNamed))
			{
				int attack = sprMissile->getTag();
				int hp = sprEnemyNamed->getTag();
				removeMissile = sprMissile;
				if (hp - attack > 0)
				{
					sprEnemyNamed->setTag(hp - attack); //네임드 몬스터는 hp가 5이기 때문에 hp가 0이 될때까지 피가 담

				}
				else {
					removeEnemy = sprEnemyNamed;
					addScore(10); // 네임드몬스터 피가 0이되면 스코어 10점 획득
					auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					audio->preloadEffect("Hit_Hurt.wav");
					audio->playEffect("Hit_Hurt.wav");
				}
			}
		}

		for (int i = 0; i < enemies.size(); i++)
		{
			Rect Mobs = enemies.at(i)->getBoundingBox();

			if (rectPlayer.intersectsRect(Mobs))
			{
				int Hit = 1;
				 
				if (PlayerHp - Hit >= 0)
				{
					PlayerATK(); // 플레이어가 몬스터랑 부딪힐시 파티클과 소리 나오는 함수
					resetMonster();//플레이어랑 몬스터 부딪히면 화면상의 몬스터, 미사일을의 정보를 다 삭제
					addPlayerHP(1);// 플레이어의 Life를 1감소시킴
					
				
				}
				if (PlayerHp == 0)
				{
					PlayerATK();
					resetMonster();
					GameOver(); // 화면 중앙에 Game Over 가 생성
					doStop(); //플레이어의 Life가 몬스터,미사일,아이템 등 생성을 중단함
				}
				
			}
			
		}
		for (int i = 0; i < enemiesnamed.size(); i++)
		{
			Rect MobsNamed = enemiesnamed.at(i)->getBoundingBox();
			if (rectPlayer.intersectsRect(MobsNamed))
			{
				int Hit = 1;
				if (PlayerHp - Hit >= 0)
				{
					PlayerATK();
					resetMonster();
					addPlayerHP(1);
					

				}
				if (PlayerHp == 0)
				{
					PlayerATK();
					resetMonster();
					GameOver();
					doStop();
				}
			}
		}

	}

	//Vector 안에 몬스터랑 미사일이 부딪힌 Sprite가 있는지 확인
	if (missiles.contains(removeMissile)) {
		resetMissile(removeMissile);
	}
	//Vector 안에 피가 0이된 몬스터가 있는지 확인
	//있으면 그 몬스터한태 파티클 실행 및 제거
	if (enemies.contains(removeEnemy)) {
		auto particle = ParticleSystemQuad::create("game/explosion.plist");
		particle->setPosition(removeEnemy->getPosition());
		particle->setScale(0.5);
		this->addChild(particle);
		auto action = Sequence::create(
			DelayTime::create(1.0),
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetBoom, this)),
			NULL);
		particle->runAction(action);
		resetEnemy(removeEnemy);
	}
	if (enemiesnamed.contains(removeEnemy)) {
		auto particle = ParticleSystemQuad::create("game/explosion.plist");
		particle->setPosition(removeEnemy->getPosition());
		particle->setScale(0.5);
		this->addChild(particle);
		auto action = Sequence::create(
			DelayTime::create(1.0),
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetBoom, this)),
			NULL);
		particle->runAction(action);
		resetEnemyNamed(removeEnemy);
	}


}


//플레이어 피격 함수
void HelloWorld::PlayerATK()
{
	auto particle = ParticleSystemQuad::create("game/explosion.plist");
	particle->setPosition(Dragon->getPosition());
	particle->setScale(0.5);
	this->addChild(particle);
	auto action = Sequence::create(
		DelayTime::create(1.0),
		CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetBoom, this)),
		NULL);

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect("Hit_Hurt.wav");
	audio->playEffect("Hit_Hurt.wav");
}

//몬스터의 Sprite와 벡터리스트 전부 제거
void HelloWorld::resetMonster()
{
	for (int i = 0; i < missiles.size(); i++)
	{
		removeChild(missiles.at(i));
		
	}
	missiles.clear();
	for (int i = 0; i < enemies.size(); i++)
	{
		removeChild(enemies.at(i));
	}
	enemies.clear();
	for (int i = 0; i < enemiesnamed.size(); i++)
	{
		removeChild(enemiesnamed.at(i));

	}

	enemiesnamed.clear();

}
//아이템을 먹고난 후 5초후 다시 원래 미사일로 바꿀때 사용
void HelloWorld::resetGetItem(float delta)
{
	isGetItem = false;
}

//몬스터 생성 함수
void HelloWorld::setEnemy(float detla)
{
	int x = PADDING_SCREEN + rand() % ((int)winSize.width - PADDING_SCREEN * 2);

	auto animation_mob = Animation::create();

	if (rand() % 100 < 20) { // 확률적으로 네임드 몬스터가 나오게함


		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("named.plist");
		sprEnemyNamed = Sprite::createWithSpriteFrameName("named_1.png");
		sprEnemyNamed->setTag(5); // 네임드 몬스터의 HP = 5

		animation_mob->setDelayPerUnit(0.2);
		for (int i = 1; i < 4; i++) {
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("named_%d.png", i));
			animation_mob->addSpriteFrame(frame);
		}

		sprEnemyNamed->setPosition(Vec2(x, winSize.height));
		this->addChild(sprEnemyNamed);
		enemiesnamed.pushBack(sprEnemyNamed);

		auto animate_mob = Animate::create(animation_mob);
		auto RepeateF = RepeatForever::create(animate_mob);
		auto action = Sequence::create(
			MoveBy::create(4.0, Vec2(0, -winSize.height)),
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetEnemy, this)),
			NULL);
		sprEnemyNamed->runAction(action);
		sprEnemyNamed->runAction(RepeateF);
	}

	else {

		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mob.plist");
		sprEnemy = Sprite::createWithSpriteFrameName("mob_1.png");
		sprEnemy->setTag(1);

		animation_mob->setDelayPerUnit(0.2);
		for (int i = 1; i < 4; i++) {
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("mob_%d.png", i));
			animation_mob->addSpriteFrame(frame);
		}

		sprEnemy->setPosition(Vec2(x, winSize.height));
		this->addChild(sprEnemy);
		enemies.pushBack(sprEnemy);

		auto animate_mob = Animate::create(animation_mob);
		auto RepeateF = RepeatForever::create(animate_mob);
		auto action = Sequence::create(
			MoveBy::create(5.0, Vec2(0, -winSize.height)),
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetEnemy, this)),
			NULL);
		sprEnemy->runAction(action);
		sprEnemy->runAction(RepeateF);
	}
	

}
//MoveBy를 끝낸 몬스터를 제거하는 함수
void HelloWorld::resetEnemy(Ref *sender)
{
	auto sprEnemys = (Sprite*)sender;
	enemies.eraseObject(sprEnemys);
	this->removeChild(sprEnemys);
}
//MoveBy를 끝낸 네임드 몬스터를 제거하는 함수
void HelloWorld::resetEnemyNamed(Ref *sender)
{
	auto sprEnemyss = (Sprite*)sender;
	enemiesnamed.eraseObject(sprEnemyss);
	this->removeChild(sprEnemyss);
}

//파티클 생성후 삭제하는 함수
void HelloWorld::resetBoom(Ref *sender)
{
	auto particle = (ParticleSystemQuad*)sender;
	this->removeChild(particle);
}

//스코어의 정보값을 나타내는 함수
void HelloWorld::initScore()
{
	auto labelHigh = Label::createWithSystemFont("", "", 14);
	labelHigh->setAnchorPoint(Vec2(1, 1));
	labelHigh->setPosition(Vec2(winSize.width - 10, winSize.height - 10));
	labelHigh->setColor(Color3B::WHITE);
	labelHigh->setTag(TAG_LABEL_HIGHSCORE);
	this->addChild(labelHigh, 100);
	addScore(0);
}


void HelloWorld::addScore(int add)
{
	nScore += add;

	if (nScore > nScoreHigh) // 현재 점수가 최고 점수보다 높을 경우 최고 점수값을 갱신하는 조건
	{
		nScoreHigh = nScore;
		UserDefault::getInstance()->setIntegerForKey("HIGH_SCORE", nScoreHigh);
		UserDefault::getInstance()->flush(); // UserDefault에 저장된 내용을 xml 파일로 저장, flush 안하면 게임 종료시 내용 삭제
	}
	auto labelHigh = (Label*)this->getChildByTag(TAG_LABEL_HIGHSCORE);
	labelHigh->setString(StringUtils::format("SCORE : %d / %d", nScore, nScoreHigh));
}

//플레이어의 Life를 나타내는 함수
void HelloWorld::initHP()
{
	auto labelScore = Label::createWithSystemFont("", "", 14);
	labelScore->setAnchorPoint(Vec2(0, 1));
	labelScore->setPosition(Vec2(10, winSize.height - 10));
	labelScore->setColor(Color3B::WHITE);
	labelScore->setTag(TAG_LABEL_PLAYERHP);
	this->addChild(labelScore, 100);
	addPlayerHP(0);
}

//입력받은 Life 감소를 받는 함수
void HelloWorld::addPlayerHP(int add)
{
	PlayerHp -= add;
	UserDefault::getInstance()->setIntegerForKey("Life", PlayerHp);
	UserDefault::getInstance()->flush();
	auto labelScore = (Label*)this->getChildByTag(TAG_LABEL_PLAYERHP);
	labelScore->setString(StringUtils::format("Life : %d", PlayerHp));
}

//////////////////////////게임오버 등장////////////////////////////
void HelloWorld::GameOver()
{


	auto label1 = Label::createWithSystemFont("GAME OVER", "Algerian", 20);
	auto GameOver = MenuItemLabel::create(label1, CC_CALLBACK_1(HelloWorld::changeScenes, this));
	GameOver->setColor(Color3B::RED); // 색상 지정

	auto menu = Menu::create(GameOver, NULL);
	menu->alignItemsVertically();
	this->addChild(menu);

}

//미사일,몬스터 생성 함수 중단
void HelloWorld::doStop()
{
	
	this->unschedule(schedule_selector(HelloWorld::setMissile));
	this->unschedule(schedule_selector(HelloWorld::setItem));
	this->unschedule(schedule_selector(HelloWorld::setEnemy));
}

//게임오버 클릭시 메인화면 가는함수
void HelloWorld::changeScenes(Ref *sender)
{
	MusicStop();//나오고있던 노래 중단//
	auto scene = TransitionPageTurn::create(1.0, HelloMenu::createScene(), true);
	Director::getInstance()->replaceScene(scene);
}
//처음 게임음악 실행
void HelloWorld::MusicStart()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("gameBGM.mp3");
}
//노래 중단
void HelloWorld::MusicStop()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
