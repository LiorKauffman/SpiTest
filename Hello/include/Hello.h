#ifndef _HELLO_H
#define _HELLO_H

#include <string>
#include "Utils/Singleton/include/Singleton.h"

class Hello : public Singleton<Hello>
{
    friend class Singleton<Hello>;

public:
    void SetName(std::string newName);
    void DisplayName();

private:
    Hello() = default;
    ~Hello() = default;
    
    std::string _name;

};

#endif //_HELLO_H