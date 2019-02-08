#include <math.h>
#include "msg.temperature/MsgTemperature.h"

namespace Msg {
namespace Temperature {

MsgTemperature::MsgTemperature(float tempInCelcius) :
        Msg::Common::MsgBase<sizeOfMessage>(1, getTemperatureBitStream(tempInCelcius)) {
}

MsgTemperature::~MsgTemperature() {
}

char* Msg::Temperature::MsgTemperature::getTemperatureBitStream(float temp) const {
    if ((temp >= -64.24) && (temp <= 63.74)) { // The biggest number representable is 63.74 --> Therefore anything bigger will be set to 63.5
    } else if (temp > 63.74) {
        temp = 63.5;
    } else {
        temp = -64;
    }
    temp = temp * 2;
    temp = round(temp);
    const char tempInuint8_t = static_cast<char>(temp);
    return new char[sizeOfMessage - 1] { tempInuint8_t };
}

} /* namespace Temperature */
} /* namespace Msg */
