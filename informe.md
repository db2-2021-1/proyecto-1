# Informe

## Introducción

El hash extendible y el indexed sequential access method ISAM son técnicas
de indexación muy conocidas y mediante las muestras de  tiempo vamos a poder notar la diferencia entre ambas.

>Indexed sequential access method (ISAM)

El ISAM combina las grandes facultades de los métodos de acceso secuencial y random. Usualmente se implementa usando un Index File y un Data File. 

El Index File está compuesto de 2 columnas, la primera es la llave a la que 
se aplica el método de indexación, y la otra es la dirección o posición donde se encuentra el registra con la llave correspondiente. Además, existen 
2 tipos de index file dentro de los ISAM, estos son los dense y los sparse.
La diferencia más notoria es la de la poca cantidad de entradas en el Sparse Index File, debido a que cada entrada del Index File va a apuntar a un grupo de registros en el Data File, cuyos registros en el Data File tienen que estar ordenados según su llave. Por otro, lado em el Dense Index File, cada registro en el Data File tiene su respectivo mapeo en el Dense Index File, por lo que el dense index file tiende a tener más filas o tuplas. Sin embargo, la búsqueda de un registro es más lento en un dense index file.

Por otra parte, el Data File va a contener los datos con los registros, la cantidad de datos en cada columna puedes ser variante por lo que el tamaño 
de cada registro también puede cambiar. 

![ISAM](./ISAM.JPG)


Así como se puede tener un index file con un data file, se puede incluso mapear llaves con otro index file, con el fin de aligerar la búsqueda de un registro o los registros cuyos valores de llave están dentro de un rango. A este concepto se la llama multilevel index file;

![Multi level index file](./multilevelindexfile.JPG)


>Hash extendible

Es un tipo de hash que crece y reduce su tamaño en el tiempo. Como todo hash,
hace uso de una función hash con el detalle de generar una secuencia de bits.
Solo se usa un prefijo o sufijo del valor binario para indexar los registros en una tabla de direcciones de buckets.

Las ventaja de el extendible hashing es que la eficiencia no se disminuye con el tamaño del archivos de datos (Data file).

Por otro lado, el índice podría llegar a ser muy grande por lo que ocupa 
espacio de memoria. Además no soporta las búsquedas por rango.

![Extendible hashing](./extendible.JPG)

## Técnicas

- Hash extendible.
- ISAM

- https://www.kaggle.com/jessemostipak/hotel-booking-demand
- https://www.kaggle.com/smithsonian/volcanic-eruptions

## Benchmark
Ahora presentamos los tiempos que tomaron las diversas operaciones usando los 2 métodos de indexación y hacemos una comparación entre ambos.
### INSERT

`COPY volcano FROM "database.csv" CSV HEADER;`
|Índice    |Lecturas|Escrituras|Tiempo|
|----------|--------|----------|------|
|Sin índice|69      |71        |19.466|
|Hash      |14799   |8570      |56.276|
|ISAM      |69      |85        |21.673|


`COPY hotel FROM "hotel_bookings.csv" CSV HEADER;`
|Índice    |Lecturas|Escrituras|Tiempo|
|----------|--------|----------|------|
|Sin índice|4121    |3396      |1025.771|
|Hash      |88032809|261433    |130883.331|
|ISAM      |4121    |3630      |909.145|


### SELECT
El promedio de 100 SELECTs.

`SELECT * FROM volcano WHERE Country IS "Peru";`
|Índice    |Lecturas|Escrituras|Tiempo|
|----------|--------|----------|------|
|Sin índice|76      |0.69      |3.07605|
|Hash      |24      |0.69      |0.21495|
|ISAM      |35      |14.69     |4.4432|


`SELECT * FROM hotel WHERE country IS "PER";`
|Índice    |Lecturas|Escrituras|Tiempo|
|----------|--------|----------|------|
|Sin índice|3402    |1.07      |373.66265|
|Hash      |795     |1.07      |1.79106|
|ISAM      |268     |235.07    |107.27113|


## Uso y video
`proyecto-1` soporta los siguientes comandos SQL.
- `CREATE TABLE table_name (name type, ...);`
- `CREATE INDEX ON table_name(column_name);`
- `CREATE INDEX ON table_name USING hash(column_name);`
- `SELECT * FROM table_name;`
- `SELECT * FROM table_name WHERE column IS value;`
- `SELECT * FROM table_name WHERE column BETWEEN value AND value;`
- `INSERT INTO table_name VALUES (value,...), ...;`
- `DELETE FROM table_name WHERE column IS value;`
- `DELETE FROM table_name WHERE column BETWEEN value AND value;`
- `COPY table_name FROM file.csv CSV HEADER;`

https://user-images.githubusercontent.com/39320840/120132166-773e7c80-c18f-11eb-879d-d1d8ebf63934.mp4
