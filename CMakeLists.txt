cmake_minimum_required(VERSION 2.6)
project(ecmasterlib)


include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/SDK)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/SDK/Linux)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/externalLib)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# ## Doxygen documentation target (make doc)
# find_package(Doxygen)
# if(DOXYGEN_FOUND)
# 	configure_file(doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
# 	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
# 		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
# 		COMMENT "Generating API documentation with Doxygen" VERBATIM )
# endif(DOXYGEN_FOUND)


## Install CMake configuration file
set(INCLUDE_INSTALL_DIR include/)
set(LIB_INSTALL_DIR lib/)

include(CMakePackageConfigHelpers)

configure_package_config_file(
	cmake/ecmasterlibConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/ecmasterlibConfig.cmake
	INSTALL_DESTINATION ${LIB_INSTALL_DIR}/ecmasterlib/cmake
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)


ADD_LIBRARY(ecmasterlib SHARED
							src/EtherCATStack.cpp
							src/EtherCATMain.cpp
							src/ecatDemoCommon.cpp
							src/ecatNotification.cpp
							src/Logging.cpp
							src/selectLinkLayer.cpp )
							
target_link_libraries(ecmasterlib AtemRasSrv EcMaster pthread dl rt)

# files required for 'find_package'
install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/ecmasterlibConfig.cmake
	DESTINATION
		${LIB_INSTALL_DIR}/ecmasterlib/cmake
		)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libecmasterlib.so DESTINATION lib)
INSTALL(FILES 	${CMAKE_CURRENT_SOURCE_DIR}/include/EtherCATMain.hpp
				DESTINATION include)
INSTALL(DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR}/include/SDK/
					${CMAKE_CURRENT_SOURCE_DIR}/include/SDK/Linux/
					DESTINATION include)

add_subdirectory(src)