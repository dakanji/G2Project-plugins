
    -- This file was automatically generated from an XSL template, which is
    -- why it looks so ugly.  Editing it by hand would be a bad idea.
    --

    CREATE TABLE DB_TABLE_PREFIXDupeDetectHistogramsMap (
  DB_COLUMN_PREFIXitemId
      INTEGER
    
      NOT NULL
    
      ,
    DB_COLUMN_PREFIXred0_63
      INTEGER
    
      ,
    DB_COLUMN_PREFIXred64_127
      INTEGER
    
      ,
    DB_COLUMN_PREFIXred128_191
      INTEGER
    
      ,
    DB_COLUMN_PREFIXred192_255
      INTEGER
    
      ,
    DB_COLUMN_PREFIXgreen0_63
      INTEGER
    
      ,
    DB_COLUMN_PREFIXgreen64_127
      INTEGER
    
      ,
    DB_COLUMN_PREFIXgreen128_191
      INTEGER
    
      ,
    DB_COLUMN_PREFIXgreen192_255
      INTEGER
    
      ,
    DB_COLUMN_PREFIXblue0_63
      INTEGER
    
      ,
    DB_COLUMN_PREFIXblue64_127
      INTEGER
    
      ,
    DB_COLUMN_PREFIXblue128_191
      INTEGER
    
      ,
    DB_COLUMN_PREFIXblue192_255
      INTEGER
    
    );

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_99543
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXred0_63);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_94031
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXred64_127);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_74637
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXred128_191);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_90307
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXred192_255);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_54342
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXgreen0_63);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_54821
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXgreen64_127);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_4216
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXgreen128_191);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_81878
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXgreen192_255);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_41726
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXblue0_63);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_43142
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXblue64_127);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_22170
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXblue128_191);

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_27092
    ON DB_TABLE_PREFIXDupeDetectHistogramsMap
    (DB_COLUMN_PREFIXblue192_255);

  
    ALTER TABLE DB_TABLE_PREFIXDupeDetectHistogramsMap
    ADD PRIMARY KEY (DB_COLUMN_PREFIXitemId);
    

    INSERT INTO DB_TABLE_PREFIXSchema (
      DB_COLUMN_PREFIXname,
      DB_COLUMN_PREFIXmajor,
      DB_COLUMN_PREFIXminor
      ) VALUES (
      'DupeDetectHistogramsMap',
      1,
      0
      );

  