#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <thread>
#include <chrono>
#include "Hello/include/Hello.h"
#include "CommunicationProtocols/SerialProtocol/SpiInterface/include/SpiInterface.h"


void MyRxHandler(const std::vector<uint8_t>& packetReceived)
{
    printf("\nPacket Received: ");

    for (auto& byteInPacket : packetReceived)
    {
        printf("0x%x ", byteInPacket);
    }

    printf("\n");
}

int main()
{
    try {
    SpiInterface::Instance().SetProperties(SpiInterface::SPI_0, static_cast<uint32_t>(SpiInterface::SPISpeed::SPEED_4_MHZ), 0, 8, MyRxHandler);
    printf("Before Rx compelted set\n");
    // SpiInterface::Instance().SetRxCompletedHandler(MyRxHandler);
    }
    catch(const std::exception& ex){
        fprintf(stderr, "excepction %s\n", ex.what());
    }
    while(true)
    {
        std::vector<uint8_t> tx = {0xa, 0xb};
        std::vector<uint8_t> lenTx = {2};
        // SpiInterface::Instance().Transmit(lenTx);
        // SpiInterface::Instance().Transmit(tx);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}