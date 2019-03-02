
    -- This file was automatically generated from an XSL template, which is
    -- why it looks so ugly.  Editing it by hand would be a bad idea.
    --

    CREATE TABLE DB_TABLE_PREFIXDupeDetectDupesListMap (
  DB_COLUMN_PREFIXsrcItemId
      INTEGER
    
      ,
    DB_COLUMN_PREFIXdupItemId
      INTEGER
    
      ,
    DB_COLUMN_PREFIXdupType
      INTEGER
    
      ,
    DB_COLUMN_PREFIXuserStatus
      INTEGER
    
    );

  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectDupesListMap_73007
    ON DB_TABLE_PREFIXDupeDetectDupesListMap
    (DB_COLUMN_PREFIXsrcItemId);
  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectDupesListMap_69149
    ON DB_TABLE_PREFIXDupeDetectDupesListMap
    (DB_COLUMN_PREFIXdupItemId);
  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectDupesListMap_6110
    ON DB_TABLE_PREFIXDupeDetectDupesListMap
    (DB_COLUMN_PREFIXdupType);
  
    CREATE INDEX DB_TABLE_PREFIXDupeDetectDupesListMap_11118
    ON DB_TABLE_PREFIXDupeDetectDupesListMap
    (DB_COLUMN_PREFIXuserStatus);
  

    INSERT INTO DB_TABLE_PREFIXSchema (
      DB_COLUMN_PREFIXname,
      DB_COLUMN_PREFIXmajor,
      DB_COLUMN_PREFIXminor
      ) VALUES (
      'DupeDetectDupesListMap',
      1,
      0
      );

  