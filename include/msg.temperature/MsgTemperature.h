#ifndef MSG_TEMPERATURE_MSGTEMPERATURE_H_
#define MSG_TEMPERATURE_MSGTEMPERATURE_H_

#include "msg.common/MsgBase.h"

namespace Msg {
namespace Temperature {
const uint32_t sizeOfMessage = 14;
class MsgTemperature: public Msg::Common::MsgBase<sizeOfMessage> {
public:
    MsgTemperature(float tempInCelcius);
    virtual ~MsgTemperature();

protected:
    char* getTemperatureBitStream(float temp) const;
};

} /* namespace Temperature */
} /* namespace Msg */

#endif /* MSG_TEMPERATURE_MSGTEMPERATURE_H_ */
