#ifndef MSG_HUMIDITY_MSGHUMIDITY_H_
#define MSG_HUMIDITY_MSGHUMIDITY_H_

#include <MsgBase.h>

namespace Msg {
namespace Humidity {

const uint32_t sizeOfMessage = 14;
class MsgHumidity final: public Msg::Common::MsgBase<sizeOfMessage> {
public:
    MsgHumidity(float humInPercent);
    virtual ~MsgHumidity() = default;

protected:
    char* getHumidityBitStream(float humInPercent) const;
};

} /* namespace Humidity */
} /* namespace Msg */

#endif /* MSG_HUMIDITY_MSGHUMIDITY_H_ */
