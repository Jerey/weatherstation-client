#ifndef MSG_HUMIDITY_MSGHUMIDITY_H_
#define MSG_HUMIDITY_MSGHUMIDITY_H_

#include <MsgBase.h>

namespace Msg {
namespace Humidity {

const uint32_t sizeOfMessage = 2;
class MsgHumidity final: public Msg::Common::MsgBase<sizeOfMessage> {
public:
    explicit MsgHumidity(float humInPercent);
    virtual ~MsgHumidity() = default;

protected:
    char* getHumidityBitStream(float humInPercent) const;
};

} /* namespace Humidity */
} /* namespace Msg */

#endif /* MSG_HUMIDITY_MSGHUMIDITY_H_ */
