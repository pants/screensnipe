#ifndef SCREEN_SNIPE_UPLOAD_STRATEGY_H
#define SCREEN_SNIPE_UPLOAD_STRATEGY_H

#include <QString>

class UploadStrategy {
public:
    virtual QString sendUploadRequest() = 0;
};

#endif //SCREEN_SNIPE_UPLOAD_STRATEGY_H
