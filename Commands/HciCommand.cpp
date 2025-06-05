#include "include/HciCommand.h"

void HciCommand::execute(const uint8_t *data, uint32_t length)
{
    // printf("Hci Data length: %d, Data content: ", (int)length);

    // for (auto index = 0; index < length; index++)
    // {
    //     printf("0x%x ", data[index]);
    // }

    // std::vector<uint8_t> buffer;

    // // check if the data and the length
    // if (!data || !length)
    //     return OS_ERR_MALFORMED;

    // buffer.insert(buffer.begin(), data, data + length);

    // pthread_mutex_lock(controller->mutex);

    // controller->inbound.push(buffer);
    // pthread_cond_broadcast(controller->recv_condition);

    // pthread_mutex_unlock(controller->mutex);

    // return OS_ERR_NO_ERROR;

    // printf("\n");
    // std::vector<uint8_t> packet(data, data + length - 10);
    // SerialInterface::Transmit("SPI3", data, length - 10);
}
