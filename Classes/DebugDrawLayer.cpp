#include "DebugDrawLayer.h"

using namespace cocos2d;

DebugDrawLayer* DebugDrawLayer::create(b2World* world, float ptmRatio)
{
	DebugDrawLayer *pRet = new DebugDrawLayer(world, ptmRatio);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
DebugDrawLayer::DebugDrawLayer(b2World* world, float ptmRatio)
: _world(world), _ptmRatio(ptmRatio)
{
}

DebugDrawLayer::~DebugDrawLayer()
{
	delete _debugDraw;
}


bool DebugDrawLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	_debugDraw = new GLESDebugDraw(_ptmRatio);
	_world->SetDebugDraw(_debugDraw);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	_debugDraw->SetFlags(flags);

	return true;
}

void DebugDrawLayer::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder);
	_customCmd.func = CC_CALLBACK_0(DebugDrawLayer::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}


void DebugDrawLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	_world->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
