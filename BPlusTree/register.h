#include <string>
#include <map>

const int MAX = 3;

typedef int keyType;
struct Register
{
    keyType id;
    char firstName[16];
    char lastName[16];
    int age;

    Register() {}

    Register(keyType _id, std::string _firstName, std::string _lastName, int _age) : id(_id), age(_age)
    {
        for (int i = 0; i < 16; i++)
        {
            if (i < _firstName.size())
            {
                firstName[i] = _firstName[i];
            }
            else
            {
                firstName[i] = '\0';
            }
        }
        for (int i = 0; i < 16; i++)
        {
            if (i < _lastName.size())
            {
                lastName[i] = _lastName[i];
            }
            else
            {
                lastName[i] = '\0';
            }
        }
    }

    void print()
    {
        std::cout << "\n";
        std::cout << "id: " << id << "\n";
        std::cout << "firstName: ";
        for (int i = 0; i < 16; i++)
        {
            std::cout << firstName[i];
        }
        std::cout << "\n";
        std::cout << "lastName: ";
        for (int i = 0; i < 16; i++)
        {
            std::cout << lastName[i];
        }
        std::cout << "\n";
        std::cout << "age: " << age << "\n\n";
    }
};

struct Node
{
    bool IS_LEAF;
    int *key, size;
    Node **ptr;
    std::map<int, long> values; //int -> id, long -> posicion del registro
    friend class BPTree;
    Node()
    {
        key = new int[MAX];
        ptr = new Node *[MAX + 1];
    }
    int pos_registro = 0;
};
