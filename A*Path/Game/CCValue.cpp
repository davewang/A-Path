//
//  CCValue.cpp
//  A*Path
//
//  Created by Dave on 12-9-12.
//
//

#include "CCValue.h"
CCValue* CCValue::valueWithCCPoint(CCPoint p)
{
    CCValue *value = new CCValue();
    value->p = p;
    value->autorelease();
    return value;
}
CCPoint CCValue::ccpointFromCCValue(CCValue *p)
{
    return p->p;
}