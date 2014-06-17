#include "ParallaxNodeExtras.h"

class PointObject : public Ref
{
    CC_SYNTHESIZE(Point, _ratio, Ratio)
    CC_SYNTHESIZE(Point, _offset, Offset)
    CC_SYNTHESIZE(Node*, _child, Child)

public:
    static PointObject *create(Point ratio, Point offset)
    {
        PointObject *ret = new PointObject();
        ret->initWithPoint(ratio, offset);
        ret->autorelease();
        return ret;
    }

    bool initWithPoint(Point ratio, Point offset)
    {
        _ratio = ratio;
        _offset = offset;
        _child = nullptr;
        return true;
    }
};

ParallaxNodeExtras::ParallaxNodeExtras()
{
    ParallaxNode::create();
}

ParallaxNodeExtras *ParallaxNodeExtras::create()
{
    return new ParallaxNodeExtras();
}

void ParallaxNodeExtras::incrementOffset(Point offset, Node *node)
{
    for (int i = 0; i < _parallaxArray->num; i++) {
        PointObject *point = (PointObject *) _parallaxArray->arr[i];
        Node *curNode = point->getChild();
        if (curNode == node) {
            point->setOffset(point->getOffset() + offset);
            break;
        }
    }
}
