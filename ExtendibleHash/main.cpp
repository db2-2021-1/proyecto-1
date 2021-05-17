#include "ExtendedHash.h"

int main(){
    auto hash = new ExtendedHash();


    hash->insert(1);
    hash->insert(2);
    hash->insert(31);
    hash->insert(43);
    hash->insert(53);
    hash->insert(83);
    hash->insert(123);
    hash->insert(133);
    hash->insert(149);
    hash->insert(165);
    hash->insert(181);
    hash->insert(197);
    hash->print();
}