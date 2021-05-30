-- cols(
--   Number = col_double(),
--   Name = col_character(),
--   Country = col_character(),
--   Region = col_character(),
--   Type = col_character(),
--   `Activity Evidence` = col_character(),
--   `Last Known Eruption` = col_character(),
--   Latitude = col_double(),
--   Longitude = col_double(),
--   Elevation = col_double(),
--   `Dominant Rock Type` = col_character(),
--   `Tectonic Setting` = col_character()
-- )

CREATE TABLE volcano(
	"Number"              INT,
	Name                  VARCHAR(63),
	Country               VARCHAR(63),
	Region                VARCHAR(31),
	"Type"                VARCHAR(31),
	"Activity Evidence"   VARCHAR(31),
	"Last Known Eruption" VARCHAR(15),
	Latitude              REAL,
	Longitude             REAL,
	Elevation             REAL,
	"Dominant Rock Type"  VARCHAR(63),
	"Tectonic Setting"    VARCHAR(63)
);

-- SELECT * FROM volcano WHERE Country IS "Peru";

-- CREATE INDEX ON volcano USING hash(Country);

-- SELECT * FROM volcano WHERE Country IS "Peru";
