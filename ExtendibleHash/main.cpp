#include "ExtendedHash.h"

int main(){
    auto hash = new ExtendedHash();

    std::cout<<"------------------(Prueba inserts)-------------------\n";
    hash->insert(Register(1,"Juan","Sanchez",19));
    hash->insert(Register(2,"Pedro","Guzman",19));
    hash->insert(Register(31,"Enrique","Toribio",19));
    hash->insert(Register(43,"Jorge","Riquelme",19));
    hash->insert(Register(53,"Renato","Diaz",19));
    hash->insert(Register(83,"Gladys","Sanchez",19));
    hash->insert(Register(123,"Ramona","Fernandez",19));
    hash->insert(Register(133,"Valeria","Carmona",19));
    hash->insert(Register(149,"Lucia","de la Torre",19));
    hash->insert(Register(165,"Ernesto","Carmelo",19));
    hash->insert(Register(181,"Jose","Hidalgo",19));
    hash->insert(Register(197,"Tadeo","Solano",19));
    hash->print();

    std::cout<<"\n------------------(Prueba find(43)-------------------\n";
    auto testReg = hash->find(43);
    std::cout<<testReg.firstName<<" "<<testReg.lastName<<std::endl;

    std::cout<<"\n------------------(Prueba find(100,200))-------------------\n";
    auto testReg2 = hash->find(100,200);
    for(auto it:testReg2){
        std::cout<<it.firstName<<" "<<it.lastName<<std::endl;
    }

    std::cout<<"\n------------------(Prueba remove(1))-------------------\n";
    hash->erase(1);
}