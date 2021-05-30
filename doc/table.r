#!/usr/bin/env Rscript

library(readr)

# TODO Add B+ tree benchmark

volcano_select_no_index <- read_csv("volcano-no-index-select.csv")
volcano_select_hash <- read_csv("volcano-hash-index-select.csv")

hotel_select_no_index <- read_csv("hotel-no-index-select.csv")
hotel_select_hash <- read_csv("hotel-hash-index-select.csv")


volcano_insert_no_index <- read_csv("volcano-no-index-insert.csv")
volcano_insert_hash <- read_csv("volcano-hash-index-insert.csv")

hotel_insert_no_index <- read_csv("hotel-no-index-insert.csv")
hotel_insert_hash <- read_csv("hotel-hash-index-insert.csv")

add_row <- function(df, index, table) {
    new_row <- data.frame(
        "Índice"     = index,
        "Lecturas"   = mean(table$reads),
        "Escrituras" = mean(table$writes),
        "Tiempo"     = mean(table$duration)
    )

    return(rbind(df, new_row))
}

volcano_select <- data.frame(
    "Índice"     = character(),
    "Lecturas"   = integer(),
    "Escrituras" = integer(),
    "Tiempo"     = double()
)

hotel_select <- volcano_select

volcano_insert <- volcano_select
hotel_insert <- volcano_select

volcano_select <- add_row(volcano_select, "Sin índice", volcano_select_no_index)
volcano_select <- add_row(volcano_select, "Hash", volcano_select_hash)

hotel_select <- add_row(hotel_select, "Sin índice", hotel_select_no_index)
hotel_select <- add_row(hotel_select, "Hash", hotel_select_hash)


volcano_insert <- add_row(volcano_insert, "Sin índice", volcano_insert_no_index)
volcano_insert <- add_row(volcano_insert, "Hash", volcano_insert_hash)

hotel_insert <- add_row(hotel_insert, "Sin índice", hotel_insert_no_index)
hotel_insert <- add_row(hotel_insert, "Hash", hotel_insert_hash)

write_csv(volcano_select, "volcano-select-benchmark.csv")
write_csv(hotel_select, "hotel-select-benchmark.csv")

write_csv(volcano_insert, "volcano-insert-benchmark.csv")
write_csv(hotel_insert, "hotel-insert-benchmark.csv")

# vim: set expandtab:
