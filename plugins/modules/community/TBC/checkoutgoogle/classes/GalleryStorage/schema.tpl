## mysql
# CheckoutGoogleOrderMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleOrderMap(
 DB_COLUMN_PREFIXtransactionId int(11) NOT NULL,
 DB_COLUMN_PREFIXgoogleOrderNumber varchar(128) NOT NULL,
 DB_COLUMN_PREFIXmainURL text NOT NULL,
 INDEX DB_TABLE_PREFIXCheckoutGoogleOrderMap_83252(DB_COLUMN_PREFIXtransactionId),
 INDEX DB_TABLE_PREFIXCheckoutGoogleOrderMap_9847(DB_COLUMN_PREFIXgoogleOrderNumber)
) DB_TABLE_TYPE
/*!40100 DEFAULT CHARACTER SET utf8 */;

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleOrderMap', 1, 0);

# CheckoutGoogleResponseMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleResponseMap(
 DB_COLUMN_PREFIXgoogleOrderNumber varchar(128) NOT NULL,
 DB_COLUMN_PREFIXresponse text NOT NULL,
 INDEX DB_TABLE_PREFIXCheckoutGoogleResponseMap_9847(DB_COLUMN_PREFIXgoogleOrderNumber)
) DB_TABLE_TYPE
/*!40100 DEFAULT CHARACTER SET utf8 */;

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleResponseMap', 1, 0);

## postgres
# CheckoutGoogleOrderMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleOrderMap(
 DB_COLUMN_PREFIXtransactionId INTEGER NOT NULL,
 DB_COLUMN_PREFIXgoogleOrderNumber VARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXmainURL text NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleOrderMap_83252 ON DB_TABLE_PREFIXCheckoutGoogleOrderMap(DB_COLUMN_PREFIXtransactionId);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleOrderMap_9847 ON DB_TABLE_PREFIXCheckoutGoogleOrderMap(DB_COLUMN_PREFIXgoogleOrderNumber);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleOrderMap', 1, 0);

# CheckoutGoogleResponseMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleResponseMap(
 DB_COLUMN_PREFIXgoogleOrderNumber VARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXresponse text NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleResponseMap_9847 ON DB_TABLE_PREFIXCheckoutGoogleResponseMap(DB_COLUMN_PREFIXgoogleOrderNumber);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleResponseMap', 1, 0);

## oracle
# CheckoutGoogleOrderMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleOrderMap(
 DB_COLUMN_PREFIXtransactionId INTEGER NOT NULL,
 DB_COLUMN_PREFIXgoogleOrderNumber VARCHAR2(128) NOT NULL,
 DB_COLUMN_PREFIXmainURL VARCHAR2(4000) NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleOrderMap_83252
   ON DB_TABLE_PREFIXCheckoutGoogleOrderMap(DB_COLUMN_PREFIXtransactionId);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleOrderMap_9847
   ON DB_TABLE_PREFIXCheckoutGoogleOrderMap(DB_COLUMN_PREFIXgoogleOrderNumber);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleOrderMap', 1, 0);

# CheckoutGoogleResponseMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleResponseMap(
 DB_COLUMN_PREFIXgoogleOrderNumber VARCHAR2(128) NOT NULL,
 DB_COLUMN_PREFIXresponse VARCHAR2(4000) NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleResponseMap_9847
   ON DB_TABLE_PREFIXCheckoutGoogleResponseMap(DB_COLUMN_PREFIXgoogleOrderNumber);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleResponseMap', 1, 0);

## db2
# CheckoutGoogleOrderMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleOrderMap(
 DB_COLUMN_PREFIXtransactionId INTEGER NOT NULL,
 DB_COLUMN_PREFIXgoogleOrderNumber VARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXmainURL VARCHAR(10000) NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheck5a_83252
   ON DB_TABLE_PREFIXCheckoutGoogleOrderMap(DB_COLUMN_PREFIXtransactionId);

CREATE INDEX DB_TABLE_PREFIXCheck5a_9847
   ON DB_TABLE_PREFIXCheckoutGoogleOrderMap(DB_COLUMN_PREFIXgoogleOrderNumber);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleOrderMap', 1, 0);

# CheckoutGoogleResponseMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleResponseMap(
 DB_COLUMN_PREFIXgoogleOrderNumber VARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXresponse VARCHAR(10000) NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheck37_9847
   ON DB_TABLE_PREFIXCheckoutGoogleResponseMap(DB_COLUMN_PREFIXgoogleOrderNumber);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleResponseMap', 1, 0);

## mssql
# CheckoutGoogleOrderMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleOrderMap(
 DB_COLUMN_PREFIXtransactionId INT NOT NULL,
 DB_COLUMN_PREFIXgoogleOrderNumber NVARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXmainURL NVARCHAR(MAX) NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleOrderMap_83252 ON DB_TABLE_PREFIXCheckoutGoogleOrderMap(DB_COLUMN_PREFIXtransactionId);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleOrderMap_9847 ON DB_TABLE_PREFIXCheckoutGoogleOrderMap(DB_COLUMN_PREFIXgoogleOrderNumber);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleOrderMap', 1, 0);

# CheckoutGoogleResponseMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleResponseMap(
 DB_COLUMN_PREFIXgoogleOrderNumber NVARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXresponse NVARCHAR(MAX) NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleResponseMap_9847 ON DB_TABLE_PREFIXCheckoutGoogleResponseMap(DB_COLUMN_PREFIXgoogleOrderNumber);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleResponseMap', 1, 0);

## sqlite
# CheckoutGoogleOrderMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleOrderMap(
 DB_COLUMN_PREFIXtransactionId INTEGER NOT NULL,
 DB_COLUMN_PREFIXgoogleOrderNumber TEXT NOT NULL,
 DB_COLUMN_PREFIXmainURL TEXT NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleOrderMap_83252 ON DB_TABLE_PREFIXCheckoutGoogleOrderMap(DB_COLUMN_PREFIXtransactionId);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleOrderMap', 1, 0);

# CheckoutGoogleResponseMap
CREATE TABLE DB_TABLE_PREFIXCheckoutGoogleResponseMap(
 DB_COLUMN_PREFIXgoogleOrderNumber TEXT NOT NULL,
 DB_COLUMN_PREFIXresponse TEXT NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXCheckoutGoogleResponseMap_9847 ON DB_TABLE_PREFIXCheckoutGoogleResponseMap(DB_COLUMN_PREFIXgoogleOrderNumber);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CheckoutGoogleResponseMap', 1, 0);

