#pragma once

#include "includes.h"

// declare our own class based on CCLayer
class CustomLayer : public CCLayer {
public:
    static CustomLayer* create();

    // button callback function. if we were robtop this would be in MenuLayer, however we are not
    void twitch(CCObject*);
    void discord(CCObject*);
};

// continue reading on `custom-layer.cpp`
