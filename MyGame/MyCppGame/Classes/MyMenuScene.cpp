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
#include "MyMenuScene.h"

USING_NS_CC;

int a;
Scene *MyMenu::createScene(int i) {
    auto scene = Scene::create();
    auto layer = MyMenu::create();
    cocos2d::log("creating %d",i);
    a = i;
    scene->addChild(layer);
    layer->init(i);

    return scene;

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MenuScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MyMenu::init() {
    //////////////////////////////
    // 1. super init first

    return true;
}


void MyMenu::goToGameScene1(Ref *pSender) {
    Scene *scene = HelloWorld::createScene(5);
    Director::getInstance()->replaceScene(scene);
}

void MyMenu::goToGameScene2(Ref *pSender) {
    Scene *scene = HelloWorld::createScene(20);
    Director::getInstance()->replaceScene(scene);
}

void MyMenu::goToGameScene3(Ref *pSender) {
    Scene *scene = HelloWorld::createScene(30);
    Director::getInstance()->replaceScene(scene);
}

bool MyMenu::init(int i) {
    if (!Scene::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto sprite_fondo = Sprite::create("images/fondo.png");
    sprite_fondo->setAnchorPoint(Vec2::ZERO);
    sprite_fondo->setPosition(Vec2::ZERO);
    addChild(sprite_fondo, -1);
    auto *title = Label::create();
    if (i == 0)
        title->setString("My awesome Game");
    if (i == 1)
        title->setString("You WIN");
    if (i == 2)
        title->setString("Game Over");
    title->setSystemFontSize(20.0);
    auto menuTitle = MenuItemLabel::create(title);

    auto playItem1 = MenuItemImage::create("images/easy.png", "images/easy.png",
                                           CC_CALLBACK_1(MyMenu::goToGameScene1, this));
    auto playItem2 = MenuItemImage::create("images/medium.png", "images/medium.png",
                                           CC_CALLBACK_1(MyMenu::goToGameScene2, this));
    auto playItem3 = MenuItemImage::create("images/hard.png", "images/hard.png",
                                           CC_CALLBACK_1(MyMenu::goToGameScene3, this));
    auto menu = Menu::create(menuTitle, playItem1, playItem2, playItem3, NULL);
    menu->alignItemsVerticallyWithPadding(visibleSize.height / 5);
    addChild(menu, 1);
    return true;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
exit(0);
#endif

/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

//EventCustom customEndEvent("game_scene_close_event");
//_eventDispatcher->dispatchEvent(&customEndEvent);



