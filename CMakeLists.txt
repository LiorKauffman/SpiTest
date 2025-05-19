cmake_minimum_required(VERSION 3.10)
project(SpiTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/Utils/Singleton/include
    ${CMAKE_SOURCE_DIR}/Hello/include
    ${CMAKE_SOURCE_DIR}/CommunicationProtocols/SerialProtocol/SpiInterface/include
    ${CMAKE_SOURCE_DIR}/CommunicationProtocols/SerialProtocol/ISerial/include
    ${CMAKE_SOURCE_DIR}/CommunicationProtocols/SerialProtocol/UartInterface/include
    ${CMAKE_SOURCE_DIR}/DataStruct/Include
    ${CMAKE_SOURCE_DIR}
)

# Source files
set(SOURCES
    main.cpp
    Hello/src/Hello.cpp
    CommunicationProtocols/SerialProtocol/SpiInterface/src/SpiInterface.cpp
    CommunicationProtocols/SerialProtocol/UartInterface/src/UartInterface.cpp
    CommunicationProtocols/SerialProtocol/ISerial/src/ISerial.cpp
    # Singleton/Singleton.cpp
)

# Create executable
add_executable(main ${SOURCES})

# Link pthread if using multithreading
target_link_libraries(main pthread)