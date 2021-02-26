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

#include "HelloMenu.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

USING_NS_CC;

using namespace CocosDenshion;
Scene* HelloMenu::createScene()
{
	return HelloMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance

bool HelloMenu::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	MusicStart();
	

	////////////////////////////////////////배경 이미지 삽입///////////////////////////////////////////
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto sprite = Sprite::create("res/BackGR.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(sprite);
	///////////////////////////////////////////////////////////////////////////////////////////////////

	auto label1 = Label::createWithSystemFont("PLAY", "Algerian", 30);
	auto label2 = Label::createWithSystemFont("QUIT", "Algerian", 30);

	auto play = MenuItemLabel::create(label1, CC_CALLBACK_1(HelloMenu::changeScene, this));
	play->setColor(Color3B::WHITE); // 색상 지정

	auto quit = MenuItemLabel::create(label2, CC_CALLBACK_1(HelloMenu::menuCloseCallback, this));
	quit->setColor(Color3B::WHITE);


	auto menu = Menu::create(play,quit, NULL);
	menu->alignItemsVertically();  // 간격을 자동으로 수직정렬 시킴
	menu->setPosition(Vec2(100, 630)); // 위에 메뉴의 위치 지정
	this->addChild(menu);
	return true;
}

void HelloMenu::MusicStart()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("FF3-Forbidden Land Eureka.mp3");
}

void HelloMenu::MusicStop()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

}

void HelloMenu::changeScene(Ref *sender)
{

	MusicStop();
	auto scene = TransitionPageTurn::create(1.0, HelloWorld::createScene(),false);
	
	Director::getInstance()->replaceScene(scene);


}


void HelloMenu::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end(); //화면 종료
}

//충돌처리가 안되면 거리 벡터값끼리 같아지면 삭제함 ㄱ
//https://genieker.tistory.com/70