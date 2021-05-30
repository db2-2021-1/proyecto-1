# Extendible Hash
## General
Un extendible hash es una estructura de datos utilizable para la indexación de bases de datos, el cual garantiza inserciones y búsquedas singulares ágiles. En este caso se está trabajando con un archivo **.csv** de 5110 registros para la experimentación. La llave en este caso es un elemento entero **ID** que representa el número de identificación de la persona examinada.
>La función hash es toma un entero y devuelve en un string el módulo de n-binario de este, donde n = hashSize.
>Ejemplo: 918 = 0110 cuando n = 4 y  918 = 10110 cuando n=5.
## Find (key)
La función find(key) está hecha para realizar una búsqueda precisa. Se recibe como parámetro la **llave** del registro que se planea encontrar y se retorna un objeto **registro**.
### Algoritmo
El algoritmo utilizado para la búsqueda singular se puede dividir en dos partes: Búsqueda en el índice, y búsqueda y lectura del archivo.
1. Búsqueda en el índice: La llave pasa por una función hash con la cuál ubica el bucket respectivo y luego itera los buckets hasta encontrar el índice.
2. Con el índice se puede navegar en el archivo con solo un seekg() y de ahí se lee el registro y devuelve la lectura.
### Costo
El algoritmo es de O(n) y $\Omega$(1), siendo n la cantidad máxima de buckets que se deben iterar. El algoritmo es bien eficiente y tiende más hacia el mejor caso mientras más grande sea el blockSize y el hashSize.
### Lecturas /Escrituras
El algoritmo solo realiza una lectura en memoria secundaria.

## Find (key, key)
La función find(key, key) es de búsqueda pero por rango.  Se recibe como parámetro dos  llaves , la **beginKey** y la **endKey** y devuelve como resultado un **vector** de registros de modo que $$beginKey <= registro.key <=endKey$$
### Algoritmo
Si bien un hash es muy útil para las lecturas precisas no lo es mucho para la búsqueda por rango. Como ningún valor dentro del hash está ordenado se tiene que revisar todo el índice para poder completar la búsqueda. El algoritmo itera por todos los buckets y todos los índices dentro de ellos, y cada vez que encuentra un índice que cae en el rango realiza una lectura singular.
### Costo
El costo es proporcional al número de registros que hay en el archivo.
### Lecturas / Escrituras
El algoritmo realiza k lecturas en memoria secundaria, donde k es igual a todos los registros con llave dentro del rango establecido.
## Insert (register)
La función insert(register) recibe como parámetro un **registro** y no retorna ningún valor. Se encarga de insertar nuevos registros en el archivo e indexarlos.
### Algoritmo
El algoritmo se puede dividir en dos partes: Inserción en el archivo y la indexación del nuevo registro.
1. Inserción: Debido a que la estructura no necesita de orden del archivo la inserción se vuelve muy sencilla y se puede dar al final del archivo. Previamente a la inserción se guarda el valor del final del archivo con tellg().
2. Indexación: Usando la llave del registro proporcionado se inserta el índice en el bucket apropiado y luego se da el manejo de posibles overflows.
### Costo
El costo de la inserción es de O(1)  al igual que el de inserción. 
### Lecturas /Escrituras
El algoritmo realiza una única inserción a memoria secundaria
## Remove (key)
La función remove() toma como parámetro la **llave** de un registro y no retorna ningún valor. Se encarga de remover del archivo y del hash al registro y al índice sin afectar en la integridad de los anteriores.
### Algoritmo
Remover no es una tarea fácil, pero se puede dividir en dos subtareas: Remover el registro y remover la indexación. En ambos casos es importante no alterar la integridad del archivo.
1. Debido a que el registro puede encontrarse en cualquier parte del archivo y al mismo tiempo no pueden quedar espacios en blanco se optó por escribir todos los registros excepto los destinados a ser eliminados en un archivo auxiliar, eliminar el archivo previo y renombrar el archivo auxiliar al nombre correspondiente.
2. Al moverse el archivo los índices de posición también son afectados, así que se optó por eliminar los índices previos y volver a escanear el archivo nuevo.
### Costo 
El costo es proporcional al número de archivos en el registro.
### Lecturas / Escrituras
El algoritmo realiza una lectura y una escritura completa por registro del archivo.
 
