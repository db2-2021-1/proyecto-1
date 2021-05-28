# Informe

## Introducción

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

## Video
