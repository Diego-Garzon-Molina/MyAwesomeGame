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
#include "MyMenuScene.h"

USING_NS_CC;

 int x;

Scene *HelloWorld::createScene(int i) {
    auto scene = Scene::createWithPhysics();
    x=i;
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    //_difficulty = i;
    layer->init(i);
    return scene;

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first

    return true;
}

bool HelloWorld::onContactBegin(cocos2d::PhysicsContact &contact) {
    PhysicsShape *a = contact.getShapeA();
    PhysicsShape *b = contact.getShapeB();
    Node *player = NULL;
    Node *ground = NULL;
    Node *enemy = NULL;
    if (1 == a->getCollisionBitmask()) player = a->getBody()->getNode();
    if (2 == a->getCollisionBitmask()) ground = a->getBody()->getNode();
    if (3 == a->getCollisionBitmask()) enemy = a->getBody()->getNode();
    if (1 == b->getCollisionBitmask()) player = b->getBody()->getNode();
    if (2 == b->getCollisionBitmask()) ground = b->getBody()->getNode();
    if (3 == b->getCollisionBitmask()) enemy = b->getBody()->getNode();
    if (player != NULL && ground != NULL) {
        if (player->getPosition().y >
            ground->getPosition().y + ground->getContentSize().height)
            _jumping = true;
    }
    if (player != NULL && enemy != NULL) {
        if (player->getPosition().y > enemy->getPosition().y + enemy->getContentSize().height)
            enemy->removeFromParentAndCleanup(true);
        else {
            Scene *scene = MyMenu::createScene(2);
            Director::getInstance()->runWithScene(scene);
        }

    }

    return true;
}

void HelloWorld::initPhysics(cocos2d::Sprite *sprite) {
    auto cuerpo = PhysicsBody::createBox(sprite->getContentSize());
    cuerpo->setContactTestBitmask(true);
    cuerpo->setRotationEnable(false);
    cuerpo->setDynamic(true);
    sprite->setPhysicsBody(cuerpo);
    cocos2d::log("player w %f", sprite->getContentSize().height);

}

void HelloWorld::initPhysicsGround(cocos2d::Sprite *sprite) {
    auto cuerpo = PhysicsBody::createBox(sprite->getContentSize());
    cuerpo->setContactTestBitmask(true);
    cuerpo->setGravityEnable(false);
    cuerpo->setDynamic(false);
    sprite->setPhysicsBody(cuerpo);

}

void HelloWorld::initTouch() {
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool HelloWorld::onTouchesMoved(const std::vector<cocos2d::Touch *> &touches,
                                cocos2d::Event *event) {
    for (cocos2d::Touch *touch : touches) {
        auto positionTouch = touch->getLocation();
        if (positionTouch.x < _width / 2) {
            auto velocity = _player->getPhysicsBody()->getVelocity();
            _player->getPhysicsBody()->setVelocity(velocity);
        }
    }
    return true;
}

bool
HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event) {
    for (cocos2d::Touch *touch : touches) {
        auto positionTouch = touch->getLocation();
        if (positionTouch.x < _width / 2) {
            movePlayer();
        } else {
            if (_jumping) {
                jumpPlayer();
            }
        }
    }
    return true;

}

bool
HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event) {
    for (cocos2d::Touch *touch : touches) {
        auto positionTouch = touch->getLocation();
        if (positionTouch.x < _width / 2) {
            auto velocity = _player->getPhysicsBody()->getVelocity();
            velocity.setPoint(0, velocity.y);
            _player->getPhysicsBody()->setVelocity(velocity);
        }
    }
    return true;

}

void HelloWorld::movePlayer() {
    auto velocity = _player->getPhysicsBody()->getVelocity();
    velocity.add(Vec2(200, 0));
    _player->getPhysicsBody()->setVelocity(velocity);
}

void HelloWorld::jumpPlayer() {

    _jumping = false;
    _player->getPhysicsBody()->applyImpulse(Vect(0, 22500));

}

void HelloWorld::moveEnemy(int i) {
    int direction = -1;
    if (_player->getPosition().x > _enemies.at(i)->getPosition().x)
        direction = 1;
    _enemies.at(i)->getPhysicsBody()->setVelocity(Vec2(100 * direction, 0));
}

void HelloWorld::update(float delta) {
    auto positionPlayer = _player->getPosition();
    auto cam = Camera::getDefaultCamera();

    if (positionPlayer.x > cam->getPosition().x + _cam_init_x / 2) {

        _moveAll = cocos2d::MoveBy::create(0, cocos2d::Vec2(
                static_cast<float>(_width * 0.75 * _grounds_size - cam->getPosition().x), 0));
        _sprite_fondo->runAction(_moveAll);
        cam->setPositionX(static_cast<float>(_width * 0.75 * _grounds_size));
        _grounds_size += 1;
    }
    for (int i = 0; i < _enemies.size(); i++) {
        if (_enemies.at(i)->getPosition().x < cam->getPosition().x + _width / 2 &&
            _enemies.at(i)->getPosition().x > cam->getPosition().x - _width) {
            moveEnemy(i);
        }
    }
    if (positionPlayer.x >= _finish->getPosition().x) {
        Scene *scene = MyMenu::createScene(1);
        Director::getInstance()->runWithScene(scene);
    }
    if (positionPlayer.y <= 0) {
        Scene *scene = MyMenu::createScene(2);
        Director::getInstance()->runWithScene(scene);
    }
}

void HelloWorld::menuCloseCallback(Ref *pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

bool HelloWorld::init(int z) {
    if (!Scene::init()) {
        return false;
    }

    cocos2d::log("scene tag %d", this->getTag());
    _width = Director::getInstance()->getWinSize().width;
    _height = Director::getInstance()->getWinSize().height;
    _sprite_fondo = Sprite::create("images/fondo.png");
    _sprite_fondo->setAnchorPoint(Vec2::ZERO);
    _sprite_fondo->setPosition(Vec2::ZERO);
    addChild(_sprite_fondo, 0);
    auto _groundAux = Sprite::create("images/ground.png");
    _groundAux->setAnchorPoint(Vec2::ZERO);
    initPhysicsGround(_groundAux);
    _groundAux->setPosition(cocos2d::Vec2::ZERO);

    addChild(_groundAux, 1);
    detachChild(_groundAux, 0, true);
    for (int i = 0; i < z; i++) {
        auto _ground = Sprite::create("images/ground.png");
        _ground->setAnchorPoint(Vec2::ZERO);
        initPhysicsGround(_ground);
        _ground->getPhysicsBody()->setCollisionBitmask(2);
        if (i == 0) _ground->setPosition(cocos2d::Vec2::ZERO);
        else
            _ground->setPosition(cocos2d::Vec2(
                    static_cast<float>(_grounds.at(i - 1)->getPosition().x +
                                       _ground->getContentSize().width +
                                       _width * 0.05 * cocos2d::RandomHelper::random_int(0, 4)),
                    static_cast<float>(_ground->getContentSize().height *
                                       cocos2d::RandomHelper::random_real(0.0, 0.4))));
        addChild(_ground, 1);
        _grounds.pushBack(_ground);
    }
    _player = Sprite::create("images/player.png");
    _player->setAnchorPoint(Vec2::ZERO);
    initPhysics(_player);
    _player->getPhysicsBody()->setCollisionBitmask(1);

    _player->setPosition(10, 100);

    addChild(_player, 1);
    _finish = Sprite::create("images/finish.png");
    _finish->setAnchorPoint(Vec2::ZERO);
    _finish->setPosition(
            static_cast<float>(_grounds.at(_grounds.size() - 1)->getPosition().x + _width * 0.8),
            _grounds.at(_grounds.size() - 1)->getPosition().y + 100);
    addChild(_finish, 1);
    for (int i = 0; i < _grounds.size(); i++) {
        _enemy = Sprite::create("images/enemy.png");
        initPhysics(_enemy);
        _enemy->getPhysicsBody()->setCollisionBitmask(3);
        _enemy->setAnchorPoint(Vec2::ZERO);
        _enemy->setPosition(static_cast<float>(_grounds.at(i)->getPosition().x + _width * 0.8),
                            _grounds.at(i)->getPosition().y + 100);
        addChild(_enemy, 1);
        _enemy->setTag(i);
        _enemies.pushBack(_enemy);
    }

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    auto cam = getDefaultCamera();
    _cam_init_x = cam->getPosition().x;
    scheduleUpdate();

    initTouch();
    return true;
}
