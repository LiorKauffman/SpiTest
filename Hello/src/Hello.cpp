#include "Hello/include/Hello.h"


void Hello::SetName(std::string newName)
{
    _name = newName;
}

void Hello::DisplayName()
{
    if (_name.length())
    {
        printf("Hello from %s\n", _name.c_str());
    }
    else
    {
        printf("Name is empty!\n");
    }
        
}