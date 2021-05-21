#include "ExtendedHash.h"

int main(){
    auto hash = new ExtendedHash();
    hash->scanAllCSV("C:\\Users\\alost\\OneDrive\\Escritorio\\healthcare-dataset-stroke-data.csv");

    //hash->print();

    //hash->find(17295).print();

    auto vec = hash->find(23000,23100);
    for(auto it:vec){
        it.print();
    }

    //hash->insert(Register(32200,"Male",23,0,0,"No","Company","Alone",23,"34.2","never",0));
}