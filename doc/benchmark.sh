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

function volcano-insert()
{
	run-sql <(echo 'COPY volcano FROM "database.csv" CSV HEADER;')
}

function hotel-insert()
{
	run-sql <(echo 'COPY hotel FROM "hotel_bookings.csv" CSV HEADER;')
}

function create-volcano-hash-index()
{
	run-sql <(echo "CREATE INDEX ON volcano USING hash(Country);")
}

function create-volcano-isam-index()
{
	run-sql <(echo "CREATE INDEX ON volcano(Country);")
}

function create-hotel-hash-index()
{
	run-sql <(echo "CREATE INDEX ON hotel USING hash(country);")
}

function create-hotel-isam-index()
{
	run-sql <(echo "CREATE INDEX ON hotel(country);")
}

function select-sql-n()
{
	run-sql <(yes "$1" | head -n "$2")
}

function volcano-select()
{
	select-sql-n 'SELECT * FROM volcano WHERE Country IS "Peru";' 100
}

function hotel-select()
{
	select-sql-n 'SELECT * FROM hotel WHERE country IS "PER";' 100
}

function to-csv()
{
	jq ".[]" |\
	jq -rs '(.[0] | keys_unsorted) as $keys | $keys, map([.[ $keys[] ]])[] | @csv'
}

create-volcano > /dev/null
volcano-insert | to-csv > volcano-no-index-insert.csv
volcano-select | to-csv > volcano-no-index-select.csv

create-volcano > /dev/null
create-volcano-hash-index > /dev/null
volcano-insert | to-csv > volcano-hash-index-insert.csv
volcano-select | to-csv > volcano-hash-index-select.csv

create-volcano > /dev/null
create-volcano-isam-index > /dev/null
volcano-insert | to-csv > volcano-isam-index-insert.csv
volcano-select | to-csv > volcano-isam-index-select.csv


create-hotel > /dev/null
hotel-insert | to-csv > hotel-no-index-insert.csv
hotel-select | to-csv > hotel-no-index-select.csv

create-hotel > /dev/null
create-hotel-hash-index > /dev/null
hotel-insert | to-csv > hotel-hash-index-insert.csv
hotel-select | to-csv > hotel-hash-index-select.csv

create-hotel > /dev/null
create-hotel-isam-index > /dev/null
hotel-insert | to-csv > hotel-isam-index-insert.csv
hotel-select | to-csv > hotel-isam-index-select.csv

./table.r
