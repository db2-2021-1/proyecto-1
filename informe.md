# Informe

## Introducción

https://www.kaggle.com/jessemostipak/hotel-booking-demand
https://www.kaggle.com/smithsonian/volcanic-eruptions

## Técnicas

- Hash extendible.
- B+ tree.

## Benchmark

### INSERT

`COPY volcano FROM "database.csv" CSV HEADER;`
|Índice    |Lecturas|Escrituras|Tiempo    |
|----------|--------|----------|----------|
|Sin índice|69      |71        |20.337    |
|Hash      |14800   |8569      |48.059    |

`COPY hotel FROM "hotel_bookings.csv" CSV HEADER;`
|Índice    |Lecturas|Escrituras|Tiempo    |
|----------|--------|----------|----------|
|Sin índice|4121    |3396      |646.712   |
|Hash      |88032810|261432    |112510.605|


### SELECT
El promedio de 100 SELECTs.

`SELECT * FROM volcano WHERE Country IS "Peru";`
|Índice         |Lecturas|Escrituras|Tiempo   |
|---------------|--------|----------|---------|
|Sin índice     |3402    |1.07      |201.36222|
|Hash extendible|795     |1.07      |2.01123  |

`SELECT * FROM hotel WHERE country IS "PER";`
|Índice         |Lecturas|Escrituras|Tiempo   |
|---------------|--------|----------|---------|
|Sin índice     |76      |0.69      |1.76824  |
|Hash extendible|24      |0.69      |0.18064  |

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
