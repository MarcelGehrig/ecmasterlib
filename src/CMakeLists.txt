# ## EtherCATCmake
# ## ////////////////////////////////////////////////////////////////////////
# cmake_minimum_required(VERSION 2.6)
# project(myecmasterlib)
# 
# 
# set(EC_MASTER "/home/mgehrig2/projects/020_EtherCAT/EC-Master-V2.9-Linux_x64-Eval")
# list(APPEND ADDITIONAL_LINK_DIRS "${EC_MASTER}/SDK/LIB/Linux/x64")
# list(APPEND ADDITIONAL_INCLUDE_DIRS "${EC_MASTER}/SDK/INC/Linux")
# list(APPEND ADDITIONAL_INCLUDE_DIRS "${EC_MASTER}/SDK/INC")
# 
# 
# 
# include_directories(${ADDITIONAL_INCLUDE_DIRS})
# link_directories(${ADDITIONAL_LINK_DIRS})
# 
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# 
# ADD_LIBRARY(myecmasterlib SHARED
# 							EtherCATStack.cpp
# 							EtherCATMain.cpp
# 							ecatDemoCommon.cpp
# 							ecatNotification.cpp
# 							Logging.cpp
# 							selectLinkLayer.cpp )
# 							
# target_link_libraries(myecmasterlib AtemRasSrv EcMaster pthread dl rt)
# 
# # install(TARGETS myecmasterlib RUNTIME DESTINATION bin)
# # install(FILES DCDemoMain.hpp
# # 	
# # INSTALL_FILES(FILES DCDemoMain.hpp
# #   DESTINATION include
# # )
# 
# INSTALL(FILES EtherCATMain.hpp DESTINATION include)
# 
# INSTALL(TARGETS myecmasterlib
# 	LIBRARY DESTINATION lib
# )
# 	