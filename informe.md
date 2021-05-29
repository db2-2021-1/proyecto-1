# Informe

## Introducción

https://www.kaggle.com/jessemostipak/hotel-booking-demand
https://www.kaggle.com/smithsonian/volcanic-eruptions

## Técnicas

- Hash extendible.
- B+ tree.

## Benchmark

El promedio de 100 SELECTs.

### Hoteles
`SELECT * FROM volcano WHERE Country IS "Peru";`
|Índice         |Lecturas|Escrituras|Tiempo   |
|---------------|--------|----------|---------|
|Sin índice     |3402    |1.07      |201.36222|
|Hash extendible|795     |1.07      |2.01123  |

### Volcanes
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
