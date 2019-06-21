#ifndef MSG_COMMON_MSGBASE_H_
#define MSG_COMMON_MSGBASE_H_

#include <stdint.h>
#include <vector>

namespace Msg {
namespace Common {

template<uint32_t SizeOfMessage>
class MsgBase {
public:
    MsgBase(const char msgID, const char msgData[SizeOfMessage - 1]) {
        m_msgData[0] = msgID;
        for (unsigned int i = 0; i < SizeOfMessage; i++) {
            m_msgData[i + 1] = msgData[i];
        }
        m_msgData[SizeOfMessage] = '\0';
    }

    virtual ~MsgBase() {
    }

    char getMsgID() const {
        return m_msgData[0];
    }

    /**
     * Function to retrieve an entire msg stream This includes the msg id and data.
     * @return A stream of chars, representing the uint8_ts of a message. This includes the id and the data.
     */
    const char* getMsgStream() const {
        return m_msgData;
    }

protected:
    char m_msgData[SizeOfMessage + 1];
};

}
}
#endif /* MSG_COMMON_MSGBASE_H_ */
