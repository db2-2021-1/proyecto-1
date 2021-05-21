#include "ExtendedHash.h"

int main(){
    auto hash = new ExtendedHash("C:\\Users\\alost\\OneDrive\\Escritorio\\healthcare-dataset-stroke-data.csv");

    hash->scanAllCSV();

    //hash->print();

    //hash->find(17295).print();

    hash->insert(Register(23027,"Male",23,0,0,"No","Self-Employed","Rural","64.7","34.2","never",0));

    auto vec = hash->find(23000,23100);
    for(auto it:vec){
        it.print();
    }

    hash->erase(23027);

}