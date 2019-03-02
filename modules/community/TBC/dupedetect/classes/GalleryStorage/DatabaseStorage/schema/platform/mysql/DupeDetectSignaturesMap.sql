
    -- This file was automatically generated from an XSL template, which is
    -- why it looks so ugly.  Editing it by hand would be a bad idea.
    --

    CREATE TABLE DB_TABLE_PREFIXDupeDetectSignaturesMap (
  DB_COLUMN_PREFIXitemId
      int(11)
    
    NOT NULL
  
      ,
    DB_COLUMN_PREFIXcrc32
      int(11)
    
    ,
  
      PRIMARY KEY (DB_COLUMN_PREFIXitemId)
    
    ,
  
    INDEX DB_TABLE_PREFIXDupeDetectSignaturesMap_73566 (DB_COLUMN_PREFIXcrc32)
    

    ) TYPE=DB_TABLE_TYPE;

    INSERT INTO DB_TABLE_PREFIXSchema (
      DB_COLUMN_PREFIXname,
      DB_COLUMN_PREFIXmajor,
      DB_COLUMN_PREFIXminor
      ) VALUES (
      'DupeDetectSignaturesMap',
      1,
      0
      );

  