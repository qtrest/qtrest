#ifndef USINGLETON_H
#define USINGLETON_H

#include <QtGlobal>
//#pragma warning(disable : 4355)

template <class T>
class uSingleton {
public:
    uSingleton(T& rObject) {

        Q_ASSERT_X(!s_pInstance, "constructor", "Only one instance of this class is permitted.");
        s_pInstance = &rObject;
    }
    ~uSingleton() {
        Q_ASSERT_X(s_pInstance, "destructor", "The singleton instance is invalid.");
        s_pInstance = 0;
    }
    static T& instance() {
        if(!s_pInstance) {
                   s_pInstance = new T();
        }
        //Q_ASSERT_X(s_pInstance, "instancing", "The singleton has not yet been created.");
        return (*s_pInstance);
    }
private:
    static T* s_pInstance;
    uSingleton(const uSingleton& Src);
    uSingleton& operator=(const uSingleton& Src);
};
template <class T> T* uSingleton<T>::s_pInstance = 0;

#endif // USINGLETON_H
