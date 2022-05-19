#ifndef INCOPIABLE_H
#define INCOPIABLE_H

//
// https://h-deb.clg.qc.ca/Sujets/Divers--cplusplus/Incopiable.html
//


class Incopiable {
public:
    Incopiable(const Incopiable&) = delete;
    Incopiable& operator=(const Incopiable&) = delete;
protected:
    constexpr Incopiable() = default;
    ~Incopiable() = default;
};

// https://h-deb.clg.qc.ca/Sujets/Divers--cplusplus/CPP--Singletons.html => Variante sur l'implémentation du schéma de conception Singleton
template <class S>
struct Singleton : Incopiable {
    static S& get() {
        static S singleton;
        return singleton;
    }
};



// /////////// Example ///////////
/*
#include "Incopiable.h"
//
// Ceci est un exemple pour démontrer la technique
//
class Service : Singleton<Service> { // magie!
    // sans ceci, le parent ne peut instancier son enfant
    friend class Singleton<Service>;
    Service() : cur{} {
        // bla bla
    }
    ~Service() noexcept {
        // bla bla
    }
    int cur;
public:
    // vos services vont ici
    int prochain() noexcept {
        return ++cur;
    }
};
//
// Ta-daaa: ça marche!
//
#include <iostream>
int main() {
    using std::cout;
    auto &service = Singleton<Service>::get();
    for (int i = 0; i < 10; ++i)
        cout << service.prochain() << ' ';
}
*/
#endif //INCOPIABLE_H
