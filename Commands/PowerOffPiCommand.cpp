#include "include/PowerOffPiCommand.h"
#include "CommunicationProtocols/SerialProtocol/UartInterface/include/UartInterface.h"

void PowerOffPiCommand::execute(const uint8_t *data, uint32_t length)
{

    // // need to send halt comand to raspberry pi for gracefully shutdown
    // // uint8_t byte = 0x01; // command id that recognized by the raspberry pi - maybe need to change it
    // std::vector<uint8_t> byte(0x01);
    // UartInterface::Instance().Transmit(byte);
    // printf("%d\n", byte);

    // vTaskDelay(pdMS_TO_TICKS(5000)); // delay time untill cut the power of the raspberry
    // std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // need to talk with esp c6 to cut the power of the rapberry

    printf("should halting!\n");
    auto ret = std::system("sudo shutdown -h now");
    if (ret != 0)
    {
        printf("Halt Failed!\n");
    }
}

