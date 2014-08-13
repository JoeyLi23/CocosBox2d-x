#include "HelloWorldScene.h"

#include "Box2D/Box2D.h"
#include "cocos-ext.h"

#include "VisibleRect.h"
#include "GLES-Render.h"
#include "DebugDrawLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PTM_RATIO 32

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
	if (!Layer::init())
	{
		return false;
	}

	this->_initPhysicsWorld();
	

	// Touch Event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->addNewSpriteAtPosition(VisibleRect::center());
	this->scheduleUpdate();
	return true;
}

void HelloWorld::_initPhysicsWorld()
{
	_world = new b2World(b2Vec2(0, -10));

	// Do we want to let bodies sleep?
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	auto debugLayer = DebugDrawLayer::create(_world, PTM_RATIO);
	this->addChild(debugLayer, 9999);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = _world->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2EdgeShape groundBox;

	// bottom
	groundBox.Set(b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, VisibleRect::leftBottom().y / PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, VisibleRect::rightBottom().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// top
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, VisibleRect::leftTop().y / PTM_RATIO), b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, VisibleRect::rightTop().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// left
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, VisibleRect::leftTop().y / PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, VisibleRect::leftBottom().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// right
	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, VisibleRect::rightBottom().y / PTM_RATIO), b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, VisibleRect::rightTop().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
	this->addNewSpriteAtPosition(this->convertTouchToNodeSpace(touch));
}

void HelloWorld::addNewSpriteAtPosition(Vec2 p)
{
	CCLOG("Add sprite %0.2f x %02.f", p.x, p.y);

	// Define the dynamic body.
	//Set up a 1m squared box in the physics world
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x / PTM_RATIO, p.y / PTM_RATIO);

	b2Body *body = _world->CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);


	//We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
	//just randomly picking one of the images
	int idx = (CCRANDOM_0_1() > .5 ? 0 : 1);
	int idy = (CCRANDOM_0_1() > .5 ? 0 : 1);

	auto sprite = PhysicsSprite::create("CloseSelected.png");
	
	this->addChild(sprite);
	sprite->setB2Body(body);
	sprite->setPTMRatio(PTM_RATIO);
	sprite->setPosition(Vec2(p.x, p.y));
}

void HelloWorld::update(float dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/

	int velocityIterations = 8;
	int positionIterations = 1;

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	_world->Step(dt, velocityIterations, positionIterations);
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	return true;
}
