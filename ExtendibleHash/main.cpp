#include "ExtendibleHash.h"

void eraseTest(ExtendibleHash *hash){
    std::cout<<"\n-------------------------\n";
    hash->insert(Register(23028,"Male","23",0,0,"No","Self-Employed","Rural","64.7","34.2","never",1));

    //hash->find(23027).print();
    auto vec = hash->find(23000,23100);
    for(auto it:vec){
        it.print();
    }

    hash->erase(23028);
}

int main(){
    auto hash = new ExtendibleHash("healthcare-dataset-stroke-data.csv");

    hash->scanAllCSV();

    //hash->print();

    eraseTest(hash);

    std::cout<<"\n-------------------------\n";
    //std::cout<<hash->find(9046).dataToCSV();
    hash->find(9046).print();
}

