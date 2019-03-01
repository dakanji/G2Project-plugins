
    -- This file was automatically generated from an XSL template, which is
    -- why it looks so ugly.  Editing it by hand would be a bad idea.
    --

    CREATE TABLE DB_TABLE_PREFIXDupeDetectHistogramsMap (
  DB_COLUMN_PREFIXitemId
      int(11)
    
    NOT NULL
  
      ,
    DB_COLUMN_PREFIXred0_63
      int(11)
    
      ,
    DB_COLUMN_PREFIXred64_127
      int(11)
    
      ,
    DB_COLUMN_PREFIXred128_191
      int(11)
    
      ,
    DB_COLUMN_PREFIXred192_255
      int(11)
    
      ,
    DB_COLUMN_PREFIXgreen0_63
      int(11)
    
      ,
    DB_COLUMN_PREFIXgreen64_127
      int(11)
    
      ,
    DB_COLUMN_PREFIXgreen128_191
      int(11)
    
      ,
    DB_COLUMN_PREFIXgreen192_255
      int(11)
    
      ,
    DB_COLUMN_PREFIXblue0_63
      int(11)
    
      ,
    DB_COLUMN_PREFIXblue64_127
      int(11)
    
      ,
    DB_COLUMN_PREFIXblue128_191
      int(11)
    
      ,
    DB_COLUMN_PREFIXblue192_255
      int(11)
    
    ,
  
      PRIMARY KEY (DB_COLUMN_PREFIXitemId)
    
    ,
  
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_99543 (DB_COLUMN_PREFIXred0_63)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_94031 (DB_COLUMN_PREFIXred64_127)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_74637 (DB_COLUMN_PREFIXred128_191)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_90307 (DB_COLUMN_PREFIXred192_255)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_54342 (DB_COLUMN_PREFIXgreen0_63)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_54821 (DB_COLUMN_PREFIXgreen64_127)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_4216 (DB_COLUMN_PREFIXgreen128_191)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_81878 (DB_COLUMN_PREFIXgreen192_255)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_41726 (DB_COLUMN_PREFIXblue0_63)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_43142 (DB_COLUMN_PREFIXblue64_127)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_22170 (DB_COLUMN_PREFIXblue128_191)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectHistogramsMap_27092 (DB_COLUMN_PREFIXblue192_255)
    

    ) TYPE=DB_TABLE_TYPE;

    INSERT INTO DB_TABLE_PREFIXSchema (
      DB_COLUMN_PREFIXname,
      DB_COLUMN_PREFIXmajor,
      DB_COLUMN_PREFIXminor
      ) VALUES (
      'DupeDetectHistogramsMap',
      1,
      0
      );

  