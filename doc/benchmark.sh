#!/usr/bin/env bash

function run-sql()
{
	proyecto-1 -b -f"$@" 2>&1 1>/dev/null
}

# https://www.kaggle.com/smithsonian/volcanic-eruptions
function create-volcano()
{
	rm -rf volcano
	run-sql volcano.sql
}

# https://www.kaggle.com/jessemostipak/hotel-booking-demand
function create-hotel()
{
	rm -rf hotel
	run-sql hotel.sql
}

function create-volcano-hash-index()
{
	run-sql <(echo "CREATE INDEX ON volcano USING hash(Country);")
}

function create-volcano-bptree-index()
{
	run-sql <(echo "CREATE INDEX ON volcano(Country);")
}

function create-hotel-hash-index()
{
	run-sql <(echo "CREATE INDEX ON hotel USING hash(country);")
}

function create-hotel-bptree-index()
{
	run-sql <(echo "CREATE INDEX ON hotel(country);")
}

function select-sql-n()
{
	run-sql <(yes "$1" | head -n "$2")
}

function volcano-benchmark()
{
	select-sql-n 'SELECT * FROM volcano WHERE Country IS "Peru";' 100
}

function hotel-benchmark()
{
	select-sql-n 'SELECT * FROM hotel WHERE country IS "PER";' 100
}

function to-csv()
{
	jq ".[]" |\
	jq -rs '(.[0] | keys_unsorted) as $keys | $keys, map([.[ $keys[] ]])[] | @csv'
}

create-volcano > /dev/null

volcano-benchmark | to-csv > volcano-no-index.csv

create-volcano-hash-index > /dev/null
volcano-benchmark | to-csv > volcano-hash-index.csv

#create-volcano-bptree-index > /dev/null
#volcano-benchmark | to-csv > volcano-bptree-index.csv



create-hotel > /dev/null

hotel-benchmark | to-csv > hotel-no-index.csv

create-hotel-hash-index > /dev/null
hotel-benchmark | to-csv > hotel-hash-index.csv

#create-hotel-bptree-index > /dev/null
#hotel-benchmark | to-csv > hotel-bptree-index.csv
