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
#include "Parser/include/Parser.h"
#include "Commands/include/PowerOffPiCommand.h"


void MyRxHandler(const std::vector<uint8_t>& packetReceived)
{
    printf("Length = %d\n", packetReceived.size());
    printf("\nPacket Received: ");

    for (auto& byteInPacket : packetReceived)
    {
        printf("0x%x ", byteInPacket);
    }

    printf("\n");

    Parser::Instance().ParseCmd(packetReceived.data(), packetReceived.size());
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
    Parser::Instance().SetCommandsType({(uint8_t)Parser::CmdType::CMD_HELLO,
        (uint8_t)Parser::CmdType::CMD_SENSOR_DATA,
        (uint8_t)Parser::CmdType::CMD_ERROR,
        (uint8_t)Parser::CmdType::CMD_CONTROL,
        (uint8_t)Parser::CmdType::CMD_HCI,
        (uint8_t)Parser::CmdType::CMD_ACK,
        (uint8_t)Parser::CmdType::CMD_POWEROFF,
        (uint8_t)Parser::CmdType::CMD_POWERON});

// Parser::Instance().RegisterHandler((uint8_t)CMD_HCI, std::make_unique<HciCommand>());

Parser::Instance().RegisterHandler((uint8_t)Parser::CmdType::CMD_POWEROFF, std::make_unique<PowerOffPiCommand>());
// Parser::Instance().RegisterHandler((uint8_t)CMD_POWERON_PI, std::make_unique<PowerOnPiCommand>());

    std::vector<uint8_t> tx;

    // std::vector<uint8_t> tx = {'H', 'e', 'l', 'l', 'o', '\r', '\n'};


    // for (auto counter = 0; counter < 2; counter++)
    // {

    //     // tx.at(counter) = counter;
    //     tx.push_back(0xaa);
    // }

    // tx.push_back(0x02);


    for (auto counter = 0; counter < UINT8_MAX; counter++)
    {

        // tx.at(counter) = counter;
        tx.push_back(counter);
    }

    


    while(true)
    {
        
        
        // UartInterface::Instance().Transmit(tx);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    return 0;
}