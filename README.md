[![C/C++ CI](https://github.com/db2-2021-1/proyecto-1/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/db2-2021-1/proyecto-1/actions/workflows/c-cpp.yml)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/db2-2021-1/proyecto-1?logo=github)
![GitHub](https://img.shields.io/github/license/db2-2021-1/proyecto-1?logo=gnu)

# Proyecto 1

[Informe](informe.md)

## Dependencias

* `bison`
* `cmake`
* `flex`
* `rapidjson`
* `readline`

## Compilación
``` bash
sudo apt install build-essential cmake libreadline-dev bison libfl-dev rapidjson-dev

mkdir -p build && cd build

cmake ..
make

./proyecto-1
```

