#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <thread>
#include <chrono>
#include "Hello/include/Hello.h"
#include "CommunicationProtocols/SerialProtocol/SpiInterface/include/SpiInterface.h"
#include "CommunicationProtocols/SerialProtocol/UartInterface/include/UartInterface.h"


void MyRxHandler(const std::vector<uint8_t>& packetReceived)
{
    printf("Length = %d\n", packetReceived.size());
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
    // SpiInterface::Instance().SetProperties(SpiInterface::SPI_0, static_cast<uint32_t>(SpiInterface::SPISpeed::SPEED_4_MHZ), 0, 8, MyRxHandler);
    UartInterface::Instance().SetProperties(UartInterface::UART_0, 115200, 0, 0, MyRxHandler);
    printf("Before Rx compelted set\n");
    // SpiInterface::Instance().SetRxCompletedHandler(MyRxHandler);
    }
    catch(const std::exception& ex){
        fprintf(stderr, "excepction %s\n", ex.what());
    }

    std::vector<uint8_t> tx (100);

    // std::vector<uint8_t> tx = {'H', 'e', 'l', 'l', 'o', '\r', '\n'};

    for (auto counter = 0; counter < 100; counter++)
    {
        tx.at(counter) = counter;
    }

    while(true)
    {
        
        // UartInterface::Instance().Transmit(tx);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    return 0;
}