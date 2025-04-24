#ifndef _SINGLETON_H
#define _SINGLETON_H

template<class SingletonClassType>
class Singleton
{
public:
    static inline SingletonClassType& Instance()
    {
        static SingletonClassType classTypeInstance;

        return classTypeInstance;
    }
    
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(const Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif // _SINGLETON_H