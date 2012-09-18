//
//  CCValue.h
//  A*Path
//
//  Created by Dave on 12-9-12.
//
//

#ifndef __A_Path__CCValue__
#define __A_Path__CCValue__

#include "Base.h"

class CCValue : public CCObject{
    CCPoint p;
public:
    static CCValue*  valueWithCCPoint(CCPoint p);
    static CCPoint   ccpointFromCCValue(CCValue *p);
};


#endif /* defined(__A_Path__CCValue__) */
