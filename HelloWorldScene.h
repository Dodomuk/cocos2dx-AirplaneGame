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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__



#include "cocos2d.h"
USING_NS_CC;

#define TAG_SPRITE_PLAYER		1000
#define PADDING_SCREEN			10

#define TAG_DRAGON	1

#define TAG_LABEL_SCORE 1001
#define TAG_LABEL_HIGHSCORE 1002
#define TAG_LABEL_PLAYERHP	1003
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
	Sprite *sprEnemy;
	Sprite *sprEnemyNamed;
    virtual bool init();

	void initHP();
	void addPlayerHP(int add);
    // a selector callback
	void initBG();

	Vec2 posStartTouch, posStartPlayer;

	Vector<Sprite*> items, enemies, enemiesnamed, missiles;

	Vector<Sprite*>::iterator it;
	bool isGetItem;

	void VectorReSet();

	int nScore, nScoreHigh, PlayerHp;
	void resetEnemyNamed(Ref *sender);
	void initScore();
	void addScore(int add);

	void update(float delta);
	void resetGetItem(float delta);
	
	void resetMonster();
	void initData();

	void initPlayer();

	void setMissile(float delta);
	void resetMissile(Ref *sender);

	void setItem(float delta);
	void resetItem(Ref *sender);

	void setEnemy(float delta);
	void resetEnemy(Ref *sender);
	void resetBoom(Ref *sender);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);

	void GameOver();
	void doStop();

	void MusicStart();
	void MusicStop();
	void changeScenes(Ref *sender);

	void PlayerATK();
	bool missilesbool = true;
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
