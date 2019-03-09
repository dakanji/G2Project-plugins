
    -- This file was automatically generated from an XSL template, which is
    -- why it looks so ugly.  Editing it by hand would be a bad idea.
    --

    CREATE TABLE DB_TABLE_PREFIXDupeDetectSignaturesMap (
  DB_COLUMN_PREFIXitemId
      INTEGER
    
      NOT NULL
    
      ,
    DB_COLUMN_PREFIXcrc32
      INTEGER
    
    );

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectSignaturesMap_73566
    ON DB_TABLE_PREFIXDupeDetectSignaturesMap
    (DB_COLUMN_PREFIXcrc32);

  
    ALTER TABLE DB_TABLE_PREFIXDupeDetectSignaturesMap
    ADD PRIMARY KEY (DB_COLUMN_PREFIXitemId);
    

    INSERT INTO DB_TABLE_PREFIXSchema (
      DB_COLUMN_PREFIXname,
      DB_COLUMN_PREFIXmajor,
      DB_COLUMN_PREFIXminor
      ) VALUES (
      'DupeDetectSignaturesMap',
      1,
      0
      );

  