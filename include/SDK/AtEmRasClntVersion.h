/*-----------------------------------------------------------------------------    
 * AtEmRasClntVersion.h     file                                                   
 * Copyright                acontis technologies GmbH, Weingarten, Germany         
 * Description              EC-Master RAS Client Version information.              
 *---------------------------------------------------------------------------*/    
#ifndef __ATEMRASCLNTVERSION_H__                                                   
#define __ATEMRASCLNTVERSION_H__     1                                             
/*-DEFINES-------------------------------------------------------------------*/    
#define ATEMRASCLNT_VERS_MAJ         1   /* major version */    
#define ATEMRASCLNT_VERS_MIN         9   /* minor version */    
#define ATEMRASCLNT_VERS_SERVICEPACK 1   /* service pack */  
#define ATEMRASCLNT_VERS_BUILD       6   /* build number */     
#define ATEMRASCLNT_VERSION (                    \
      (ATEMRASCLNT_VERS_MAJ << 3*8)            \
    | (ATEMRASCLNT_VERS_MIN << 2*8)            \
    | (ATEMRASCLNT_VERS_SERVICEPACK << 1*8)    \
    | (ATEMRASCLNT_VERS_BUILD << 0*8)          \
                        )                                                          
#define ATEMRASCLNT_FILEVERSION     1,9,1,6 
#define ATEMRASCLNT_FILEVERSIONSTR  "1.9.1.06\0" 
#endif /* __ATEMRASCLNTVERSION_H__ */                                              
/*-END OF SOURCE FILE--------------------------------------------------------*/    
