#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();  
	void addNewSpriteAtPosition(cocos2d::Vec2 p);

    CREATE_FUNC(HelloWorld);

protected:
	void _initPhysicsWorld();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	
	void update(float dt);
	b2World* _world;
};

#endif // __HELLOWORLD_SCENE_H__
