
    -- This file was automatically generated from an XSL template, which is
    -- why it looks so ugly.  Editing it by hand would be a bad idea.
    --

    CREATE TABLE DB_TABLE_PREFIXDupeDetectDupesListMap (
  DB_COLUMN_PREFIXsrcItemId
      int(11)
    
      ,
    DB_COLUMN_PREFIXdupItemId
      int(11)
    
      ,
    DB_COLUMN_PREFIXdupType
      int(11)
    
      ,
    DB_COLUMN_PREFIXuserStatus
      int(11)
    
    ,
  
    INDEX DB_TABLE_PREFIXDupeDetectDupesListMap_73007 (DB_COLUMN_PREFIXsrcItemId)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectDupesListMap_69149 (DB_COLUMN_PREFIXdupItemId)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectDupesListMap_6110 (DB_COLUMN_PREFIXdupType)
    
      ,
    
    INDEX DB_TABLE_PREFIXDupeDetectDupesListMap_11118 (DB_COLUMN_PREFIXuserStatus)
    

    ) TYPE=DB_TABLE_TYPE;

    INSERT INTO DB_TABLE_PREFIXSchema (
      DB_COLUMN_PREFIXname,
      DB_COLUMN_PREFIXmajor,
      DB_COLUMN_PREFIXminor
      ) VALUES (
      'DupeDetectDupesListMap',
      1,
      0
      );

  