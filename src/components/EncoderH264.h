#ifndef ENCODERH264_H
#define ENCODERH264_H

#include "src/core/Component.h"

class EncoderH264 : public Component
{
public:
    EncoderH264();

    static const int InputPort = 200;
    static const int OutputPort = 201;

    bool SetVideoParameters();
    bool SetVideoParametersForCamera( const OMX_PARAM_PORTDEFINITIONTYPE &cameraPortdef );
};

#endif // ENCODERH264_H
