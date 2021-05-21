#include "BPlusTree.h"

using namespace std;
int main()
{
    BPlusTree bplustree; //UNCLUSTERED B+ TREE

    bplustree.insert(Register(25, "Esteban", "Principe", 20));
    bplustree.insert(Register(16, "Andres", "Losta", 20));
    bplustree.insert(Register(1, "Jose", "De Lama", 21));
    bplustree.insert(Register(4, "Monita", "China", 18));
    bplustree.insert(Register(9, "Claudia", "Noche", 21));
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
    auto registros = bplustree.find(6,7);
    for(auto& item : registros){
        item.print();
    }
    

    //bplustree.print_leaf(bplustree.getRoot());
    //bplustree.display(bplustree.getRoot());
    return 0;
}
