# Informe

## Introducción

El hash extendible y el indexed sequential access method ISAM son técnicas
de indexación muy conocidas y mediante las muestras de  tiempo vamos a poder notar la diferencia entre ambas.
El ISAM combina las grandes facultades de los métodos de acceso secuencial y random. Usualmente se implementa usando un Index File y un Data File.

El Index File está compuesto de 2 columnas, la primera es la llave a la que 
se aplica el método de indexación, y la otra es la dirección o posición donde se encuentra el registra con la llave correspondiente.
Por otra parte, el Data File va a contener los datos con los registros, la cantidad de datos en cada columna puedes ser variante por lo que el tamaño de cada registro también puede cambiar. 

![Getting Started](./ISAM.JPG)


se caracteiza por mantener las forma de un B tree con la diferencia de pode



## Técnicas

- Hash extendible.
- ISAM

- https://www.kaggle.com/jessemostipak/hotel-booking-demand
- https://www.kaggle.com/smithsonian/volcanic-eruptions

## Benchmark

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
