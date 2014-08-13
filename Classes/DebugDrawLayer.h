#ifndef __B2_DEBUG_DRAW_LAYER_H__
#define __B2_DEBUG_DRAW_LAYER_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"

class DebugDrawLayer : public cocos2d::Layer
{
	b2World* _world;
	GLESDebugDraw* _debugDraw;
	const float _ptmRatio;
public:
	DebugDrawLayer(b2World* World, float ptmRatio);
	~DebugDrawLayer();
	static DebugDrawLayer* create(b2World* world, float ptmRatio);
	
	virtual bool init();
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags);

protected:
	void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
	cocos2d::CustomCommand _customCmd;
};

#endif