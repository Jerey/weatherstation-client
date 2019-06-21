#include <math.h>
#include <MsgHumidity.h>

namespace Msg {
namespace Humidity {

MsgHumidity::MsgHumidity(float humInPercent) :
        Msg::Common::MsgBase<sizeOfMessage>(2, getHumidityBitStream(humInPercent)) {
}

char* Msg::Humidity::MsgHumidity::getHumidityBitStream(float humInPercent) const {
    double humInPercentAndRounded;
    if (humInPercent > 100) {
        humInPercentAndRounded = 100;
    } else if (humInPercent < 0) {
        humInPercentAndRounded = 0;
    } else {
        humInPercentAndRounded = round(humInPercent);
    }
    const char humInChar = static_cast<unsigned char>(humInPercentAndRounded);
    return new char[sizeOfMessage - 1] { humInChar };
}

} /* namespace Humidity */
} /* namespace Msg */
