#include "BPlusTree.h"

using namespace std;
int main()
{
    int basekey = 10000;
    int topkey = 20000;
    BPlusTree bplustree; //UNCLUSTERED B+ TREE

    Lector lector;

    auto registros = lector.scanAllCSV();

    for (auto &item : registros)
    {
        bplustree.insert(item);
    }
    /*
    //INSERCION DE UN ELEMENTO
    //bplustree.display(bplustree.getRoot());
    auto register_found = bplustree.find(72915);
    register_found.print();
    */

    /*
    //BUSQUEDA DE UN SOLO VALOR
    auto register_found = bplustree.find(9);
    register_found.print();

    //REMOVE -> MEJORAR CASO EN EL QUE EL ROOT SE ELIMINE..
    //bplustree.remove(9);
    register_found = bplustree.find(9);
    register_found.print();
*/
    //BUSQUEDA POR RANGO
    std::cout << "\n\n\n\n\n\n";
    auto registros_ = bplustree.find(basekey, topkey);
    for (auto &item : registros_)
    {
        std::cout<<"item.id: "<<item.id<<"\n";
        //item.print();
    }

    //bplustree.print_leaf(bplustree.getRoot());
    //bplustree.display(bplustree.getRoot());
    return 0;
}
