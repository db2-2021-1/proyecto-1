#include "BPlusTree.h"

using namespace std;
int main()
{
    BPlusTree bplustree;

    bplustree.insert(Register(2, "Esteban", "Principe", 20));
    bplustree.insert(Register(7, "Andres", "Losta", 20));
    bplustree.insert(Register(14, "Jose", "De Lama", 21));
    bplustree.insert(Register(32, "Monita", "China", 18));
    bplustree.insert(Register(8, "Claudia", "Noche", 21));


    //bplustree.print_leaf(bplustree.getRoot());
    bplustree.display(bplustree.getRoot());
    return 0;
}
