#!/usr/bin/env Rscript

library(readr)

# TODO Add B+ tree benchmark

volcano_no_index <- read_csv("volcano-no-index-select.csv")
volcano_hash_index <- read_csv("volcano-hash-index-select.csv")

hotel_no_index <- read_csv("hotel-no-index-select.csv")
hotel_hash_index <- read_csv("hotel-hash-index-select.csv")

add_row <- function(df, index, table) {
    new_row <- data.frame(
        "Índice"     = index,
        "Lecturas"   = mean(table$reads),
        "Escrituras" = mean(table$writes),
        "Tiempo"     = mean(table$duration)
    )

    return(rbind(df, new_row))
}

volcano_table <- data.frame(
    "Índice"     = character(),
    "Lecturas"   = integer(),
    "Escrituras" = integer(),
    "Tiempo"     = double()
)

hotel_table <- data.frame(
    "Índice"     = character(),
    "Lecturas"   = integer(),
    "Escrituras" = integer(),
    "Tiempo"     = double()
)

volcano_table <- add_row(volcano_table, "Sin índice", volcano_no_index)
volcano_table <- add_row(volcano_table, "Hash extendible", volcano_hash_index)

write_csv(volcano_table, "volcano-select-benchmark.csv")

hotel_table <- add_row(hotel_table, "Sin índice", hotel_no_index)
hotel_table <- add_row(hotel_table, "Hash extendible", hotel_hash_index)

write_csv(hotel_table, "hotel-select-benchmark.csv")

# vim: set expandtab:
