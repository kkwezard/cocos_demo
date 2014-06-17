#ifndef __PARALLAX_NODE_EXTRAS_H__
#define __PARALLAX_NODE_EXTRAS_H__

#include "cocos2d.h"

USING_NS_CC;

class ParallaxNodeExtras : public ParallaxNode {

public:
    ParallaxNodeExtras();
    static ParallaxNodeExtras *create();
    void incrementOffset(Point offset, Node *node);
};

#endif
