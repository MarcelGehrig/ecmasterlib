/*-----------------------------------------------------------------------------    
 * AtEmRasSrvVersion.h      file                                                   
 * Copyright                acontis technologies GmbH, Weingarten, Germany         
 * Description              EC-Master RAS Server Version information.              
 *---------------------------------------------------------------------------*/    
#ifndef __ATEMRASSRVVERSION_H__                                                    
#define __ATEMRASSRVVERSION_H__     1                                              
/*-DEFINES-------------------------------------------------------------------*/    
#define ATEMRASSRV_VERS_MAJ             1   /* major version */  
#define ATEMRASSRV_VERS_MIN             9          /* minor version */  
#define ATEMRASSRV_VERS_SERVICEPACK     1       /* service pack  */  
#define ATEMRASSRV_VERS_BUILD           6          /* build number  */  
#define ATEMRASSRV_VERSION (                     \
      (ATEMRASSRV_VERS_MAJ   << 3*8)           \
    | (ATEMRASSRV_VERS_MIN   << 2*8)           \
    | (ATEMRASSRV_VERS_SERVICEPACK << 1*8)     \
    | (ATEMRASSRV_VERS_BUILD << 0*8)           \
                        )                                                          
#define ATEMRASSRV_FILEVERSION     1,9,1,6 
#define ATEMRASSRV_FILEVERSIONSTR  "1.9.1.06\0" 
#endif /* __ATEMRASSRVVERSION_H__ */                                               
/*-END OF SOURCE FILE--------------------------------------------------------*/    
