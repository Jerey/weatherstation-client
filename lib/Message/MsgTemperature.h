#ifndef MSG_TEMPERATURE_MSGTEMPERATURE_H_
#define MSG_TEMPERATURE_MSGTEMPERATURE_H_

#include <MsgBase.h>

namespace Msg {
namespace Temperature {

const uint32_t sizeOfMessage = 2;
class MsgTemperature final : public Msg::Common::MsgBase<sizeOfMessage> {
public:
    explicit MsgTemperature(float tempInCelcius);
    virtual ~MsgTemperature() = default;

protected:
    char* getTemperatureBitStream(float temp) const;
};

} /* namespace Temperature */
} /* namespace Msg */

#endif /* MSG_TEMPERATURE_MSGTEMPERATURE_H_ */
