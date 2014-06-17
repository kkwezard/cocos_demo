#include "HelloWorldScene.h"
#include "ParallaxNodeExtras.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
    
    closeItem->setPosition(origin + Point(visibleSize) - Point(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    _batchNode = SpriteBatchNode::create("Sprites.pvr.ccz");
    this->addChild(_batchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");

    _ship = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    _ship->setPosition(Point(visibleSize.width * 0.1, visibleSize.height * 0.5));
    _batchNode->addChild(_ship, 1);

    // Parallax Node
    _backgroundNode = ParallaxNodeExtras::create();
    this->addChild(_backgroundNode, -1);

    // Sprites
    _spacedust1 = Sprite::create("bg_front_spacedust.png");
    _spacedust2 = Sprite::create("bg_front_spacedust.png");
    _planetsunrise = Sprite::create("bg_planetsunrise.png");
    _galaxy = Sprite::create("bg_galaxy.png");
    _spacialanomaly = Sprite::create("bg_spacialanomaly.png");
    _spacialanomaly2 = Sprite::create("bg_spacialanomaly2.png");

    auto dustSpeed = Point(0.1, 0.1);
    auto bgSpeed = Point(0.05, 0.05);

    _backgroundNode->addChild(_spacedust1, 0, dustSpeed, Point(0, visibleSize.height * 0.5));
    _backgroundNode->addChild(_spacedust2, 0, dustSpeed, Point(_spacedust1->getContentSize().width, visibleSize.height * 0.5));
    _backgroundNode->addChild(_galaxy, -1, bgSpeed, Point(0, visibleSize.height * 0.7));
    _backgroundNode->addChild(_planetsunrise, -1, bgSpeed, Point(600, visibleSize.height * 0));
    _backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, Point(900, visibleSize.height * 0.3));
    _backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, Point(1500, visibleSize.height * 0.9));


    HelloWorld::addChild(ParticleSystemQuad::create("Stars1.plist"));
    HelloWorld::addChild(ParticleSystemQuad::create("Stars2.plist"));
    HelloWorld::addChild(ParticleSystemQuad::create("Stars3.plist"));

#define KNUMASTEROIDS 15
    _asteroids = new Vector<Sprite*>(KNUMASTEROIDS);
    for (int i = 0; i < KNUMASTEROIDS; i++) {
        auto asteroid = Sprite::createWithSpriteFrameName("asteroid.png");
        asteroid->setVisible(false);
        _batchNode->addChild(asteroid);
        _asteroids->pushBack(asteroid);
    }

#define KNUMLASERS 5
    _shipLasers = new Vector<Sprite*>(KNUMLASERS);
    for (int i = 0; i < KNUMLASERS; i++) {
        auto shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");
        shipLaser->setVisible(false);
        _batchNode->addChild(shipLaser);
        _shipLasers->pushBack(shipLaser);
    }

    Device::setAccelerometerEnabled(true);
    auto accelerationListener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(accelerationListener, this);

    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    _lives = 3;
    _gameOver = false;
    double curTime = getTimeTick();
    _gameOverTime = curTime + 30000;

    this->scheduleUpdate();

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float dt)
{
    //Point backgroundScrollVert = Point(-1000, 0);
    //_backgroundNode->setPosition(_backgroundNode->getPosition() + backgroundScrollVert * dt);

    //auto spaceDusts = new Vector<Sprite *>(2);
    //spaceDusts->pushBack(_spacedust1);
    //spaceDusts->pushBack(_spacedust2);
    //for (auto spaceDust : *spaceDusts) {
        //float xPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
        //float size = spaceDust->getContentSize().width;
        //if (xPosition < -size/2) {
            //_backgroundNode->incrementOffset(Point(spaceDust->getContentSize().width * 2, 0), spaceDust);
        //}
    //}

    //auto backGrounds = new Vector<Sprite *>(4);
    //backGrounds->pushBack(_galaxy);
    //backGrounds->pushBack(_planetsunrise);
    //backGrounds->pushBack(_spacialanomaly);
    //backGrounds->pushBack(_spacialanomaly2);
    //for (auto background : *backGrounds) {
        //float xPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).x;
        //float size = background->getContentSize().width;
        //if (xPosition < -size) {
            //_backgroundNode->incrementOffset(Point(2000, 0), background);
        //}
    //}

    Size winSize = Director::getInstance()->getWinSize();
    float maxY= winSize.height - _ship->getContentSize().height / 2;
    float minY = _ship->getContentSize().height / 2;
    float newY = _ship->getPosition().y + _shipPointPerSecY * dt;
    newY = MIN(MAX(newY, minY), maxY);
    _ship->setPosition(Point(_ship->getPosition().x, newY));

    float curTimeMillis = getTimeTick();
    if (curTimeMillis > _nextAsteroidSpawn) {
        float randMillisecs = randomValueBetween(0.20, 1.0) * 1000;
        _nextAsteroidSpawn = randMillisecs + curTimeMillis;

        float randY = randomValueBetween(0.0, winSize.height);
        float randDuration = randomValueBetween(2.0, 10.0);

        Sprite *asteroid = _asteroids->at(_nextAsteroid++);
        if (_nextAsteroid == _asteroids->size()) {
            _nextAsteroid = 0;
        }

        asteroid->stopAllActions();
        asteroid->setPosition(Point(winSize.width + asteroid->getContentSize().width / 2, randY));
        asteroid->setVisible(true);
        asteroid->runAction(Sequence::create(MoveBy::create(randDuration, Point(-winSize.width - asteroid->getContentSize().width, 0)), 
                    CallFuncN::create([] (Node *node) { node->setVisible(false);}),
                    NULL));
    }

    for (auto asteroid : *_asteroids) { 
        if (!asteroid->isVisible())
            continue;
        for (auto shipLaser : *_shipLasers) { 
            if (!shipLaser->isVisible())
                continue;
            if (shipLaser->boundingBox().intersectsRect(asteroid->boundingBox())) {
                shipLaser->setVisible(false);
                asteroid->setVisible(false);
            }
        }
        if (_ship->boundingBox().intersectsRect(asteroid->boundingBox())) {
            asteroid->setVisible(false);
            _ship->runAction(Blink::create(1.0, 9));
            _lives--;
        }
    }

    if (_lives <= 0) {
        _ship->stopAllActions();
        _ship->setVisible(false);
        this->endScene(KENDREASONLOSE);
    } else if (curTimeMillis >= _gameOverTime) {
        this->endScene(KENDREASONWIN);
    }
}

void HelloWorld::onAcceleration(Acceleration *acc, Event *ev)
{
#define KFILTERFACTOR 0.05
#define KRESTACCLEX -0.6
#define KSHIPMAXPOINTSPERSEC (winSize.height * 0.5)
#define KMAXDIFFX 0.2

    double rollingX = 0.0;

    acc->x = acc->y;
    rollingX = (acc->x * KFILTERFACTOR) + (rollingX * (1.0 - KFILTERFACTOR));
    float accelX = acc->x - rollingX;
    Size winSize = Director::getInstance()->getWinSize();
    float accelDiff = accelX - KRESTACCLEX;
    float accelFraction = accelDiff /KMAXDIFFX;
    _shipPointPerSecY = KSHIPMAXPOINTSPERSEC * accelFraction;

}

float HelloWorld::randomValueBetween(float low, float high)
{
    return (((float) arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
}

float HelloWorld::getTimeTick()
{
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
    return (float) millisecs;
}

void HelloWorld::onTouchesBegan(const std::vector<Touch *> &touches, Event *ev)
{
    auto winSize = Director::getInstance()->getWinSize();
    auto shipLaser = _shipLasers->at(_nextShipLaser++);
    if (_nextShipLaser = _shipLasers->size()) {
        _nextShipLaser = 0;
    }

    shipLaser->setPosition(_ship->getPosition() + Point(shipLaser->getContentSize().width / 2, 0));
    shipLaser->setVisible(true);
    shipLaser->stopAllActions();
    shipLaser->runAction(Sequence::create(MoveBy::create(0.5, Point(winSize.width, 0)), 
                CallFuncN::create([] (Node *node) {node->setVisible(false);}),
                NULL));

}

void HelloWorld::endScene(EndReason endReason)
{
    if (_gameOver)
        return;
    _gameOver = true;

    auto winSize = Director::getInstance()->getWinSize();
    std::string message = "You Win";
    if (endReason == KENDREASONLOSE)
        message = "You Lose";

    auto label = LabelBMFont::create(message, "Arial.fnt");
    label->setScale(0.1);
    label->setPosition(Point(winSize.width / 2, winSize.height * 0.6));
    this->addChild(label);

    message = "Restart";
    auto restartLabel = LabelBMFont::create(message, "Arial.fnt");
    auto restartItem = MenuItemLabel::create(restartLabel, [&] (Ref* ref) {
                Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, this->createScene()));
                this->scheduleUpdate();
            });
    restartItem->setScale(0.1);
    restartItem->setPosition(Point(winSize.width / 2, winSize.height * 0.4));

    auto menu = Menu::create(restartItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    restartItem->runAction(ScaleTo::create(0.5, 1.0));
    label->runAction(ScaleTo::create(0.5, 1.0));

    this->unscheduleUpdate();
}

