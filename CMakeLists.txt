CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (Sigma)

# Set the include directory
<<<<<<< HEAD
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")
=======
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}")
>>>>>>> Web GUI now uses Chromium Embedded Framework.

# Set the cmake search paths
SET(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)

# change output directory of executable so that relative paths in the code are correct
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)

# Set the directory of Find<Library>.cmake modules
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

# find all source files in the src directory
FILE(GLOB Sigma_SRC "src/*.cpp")

# find all the files in the includes directory
FILE(GLOB Sigma_INCLUDES "include/*.h" "include/*.hpp")

## Filter the source/header files for various groups of files.

# Put all component files into a filter.
FILE(GLOB Sigma_SRC_COMPONENT_H "include/components/*.h")
SOURCE_GROUP("Header Files\\components" FILES ${Sigma_SRC_COMPONENT_H})
FILE(GLOB Sigma_SRC_COMPONENT_CPP "src/components/*.cpp")
SOURCE_GROUP("Source Files\\components" FILES ${Sigma_SRC_COMPONENT_CPP})

# Put all Systems files into a filter.
FILE(GLOB Sigma_SRC_SYSTEM_H "include/systems/*.h")
SOURCE_GROUP("Header Files\\systems" FILES ${Sigma_SRC_SYSTEM_H})
FILE(GLOB Sigma_SRC_SYSTEM_CPP "src/systems/*.cpp")
SOURCE_GROUP("Source Files\\systems" FILES ${Sigma_SRC_SYSTEM_CPP})

# Put all Controller files into a filter.
FILE(GLOB Sigma_SRC_CONTROLLERS_H "include/controllers/*.h")
SOURCE_GROUP("Header Files\\controllers" FILES ${Sigma_SRC_CONTROLLERS_H})
FILE(GLOB Sigma_SRC_CONTROLLERS_CPP "src/controllers/*.cpp")
SOURCE_GROUP("Source Files\\controllers" FILES ${Sigma_SRC_CONTROLLERS_CPP})

# Put all Resource files into a filter.
FILE(GLOB Sigma_SRC_RESOURCES_H "include/resources/*.h")
SOURCE_GROUP("Header Files\\resources" FILES ${Sigma_SRC_RESOURCES_H})

# Put the Test files into a filter
FILE(GLOB Sigma_SRC_TESTS_CPP "src/tests/*.cpp")
SOURCE_GROUP("Source Files\\tests" FILES ${Sigma_SRC_TESTS_CPP})

# Output the compiler ID.
MESSAGE("Compiler ID: " ${CMAKE_CXX_COMPILER_ID})

## Check that the compiler is compatible and new enough to build everything.

# check for gcc version to set c++11 or c++0x.
# thanks to http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake .
IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	IF (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
		# c++11 std only appears in gcc 4.7 or higher. c++0x will work for lesser versions.
		MESSAGE("gcc ${GCC_VERSION} does not support c++11, only c++0x. This may cause some problems in the future. We recommend upgrading to gcc-4.7 or greater.")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	ELSE (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
		# compiler is gcc 4.7 or higher. Force c++11.
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	ENDIF (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	# Clang 3.3 and up support c++11.  
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE CLANG_VERSION)
	IF (NOT (CLANG_VERSION VERSION_GREATER 3.3 OR CLANG_VERSION VERSION_EQUAL 3.3))
		# c++11 std only appears in clang 3.3 or higher. c++0x will work for lesser versions.
		MESSAGE("clang ${CLANG_VERSION} does not support c++11, only c++0x. This may cause some problems in the future. We recommend upgrading to clang-3.3 or greater.")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	ELSE (NOT (CLANG_VERSION VERSION_GREATER 3.3 OR CLANG_VERSION VERSION_EQUAL 3.3))
		# compiler is clang 3.3 or higher. Force c++11 and use libc++.
		IF (XCODE_VERSION)
			SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD c++11)
			SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY libc++)
		ELSE (XCODE_VERSION)
			SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
		ENDIF (XCODE_VERSION)
	ENDIF (NOT (CLANG_VERSION VERSION_GREATER 3.3 OR CLANG_VERSION VERSION_EQUAL 3.3))
ELSEIF (MSVC AND (MSVC10 OR MSVC11))
	MESSAGE("Supported Visual Studio!")
ELSE ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	MESSAGE(FATAL_ERROR "Your C++ compiler does not support C++11.")
ENDIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")

## Set the libraries

<<<<<<< HEAD
# If we are on windows add in the local search directories as well.
IF (WIN32 AND NOT MINGW) # Windows
	SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include/libraries)
	INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/libraries")
	IF (CMAKE_CL_64)
	LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/x64/debug" "${CMAKE_SOURCE_DIR}/lib/x64/release")
	SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x64/debug ${CMAKE_SOURCE_DIR}/lib/x64/release)
	ELSE (CMAKE_CL_64)
	LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/x86/debug" "${CMAKE_SOURCE_DIR}/lib/x86/release")
	SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x86/debug ${CMAKE_SOURCE_DIR}/lib/x86/release)
	ENDIF (CMAKE_CL_64)
ENDIF (WIN32 AND NOT MINGW)

# Find all the libraries except GLEW and platform specific.
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLFW3 REQUIRED)
FIND_PACKAGE(OggVorbis REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(Bullet REQUIRED)
FIND_PACKAGE(SOIL REQUIRED)
FIND_PACKAGE(Awesomium REQUIRED)

# Give these some dummy values and if the platform is LINUX or OSX they will be set accordingly.
SET(X11_LIBRARIES "")
SET(OSX_LIBRARIES "")

IF (NOT APPLE) # X11 and GLEW are not needed on OSX.
	FIND_PACKAGE(X11)
	set(USE_STATIC_GLEW FALSE CACHE BOOL "Build against GLEW static (default no)")

	FIND_PACKAGE(GLEW REQUIRED) # We find GLEW here as OSX doesn't need it.
	IF (NOT GLEW_FOUND)
		MESSAGE(FATAL_ERROR "GLEW not found! Aborting!")
	ENDIF (NOT GLEW_FOUND)
ENDIF (NOT APPLE)

<<<<<<< HEAD
=======
IF (UNIX AND NOT APPLE)
	FIND_PACKAGE(GTK2 REQUIRED)
	INCLUDE_DIRECTORIES("${GTK2_INCLUDE_DIRS}")
ENDIF (UNIX AND NOT APPLE)

>>>>>>> Changed link dir for CEF.
IF (APPLE) # Mac OSX
	SET(GLEW_LIBRARY "") # Set a dummy value for GLEW.

	# Awesomium does not support 64-bit.
	SET(CMAKE_OSX_ARCHITECTURES i386)
	SET(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "i386")
	SET(CMAKE_XCODE_ATTRIBUTE_SUPPORTED_PLATFORMS macosx)
=======
# Link aganst OpenGL and GLEW
# We can use the find OpenGL package on windows
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL not found! Aborting!")
endif(NOT OPENGL_FOUND)

# Figure out what Operating System to compile against
if(WIN32 AND NOT MINGW) # Windows
	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include/libraries)
	include_directories("${CMAKE_SOURCE_DIR}/include/libraries")
	set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x86)
	link_directories("${CMAKE_SOURCE_DIR}/lib/x86")

	SET(GLFW3_LIBRARY glfw3)
	set(GLEW_LIBRARY glew32s glew32)
	set(BULLET_LINEARMATH_LIBRARIES LinearMath)
	set(BULLET_COLLISION_LIBRARIES BulletCollision)
	set(BULLET_DYNAMICS_LIBRARIES BulletDynamics)
	SET(SOIL_LIBRARY SOIL)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux") # *nix compliant
	if (NOT APPLE) # X11 and GLEW are not needed on OS X.
		find_package(GLEW REQUIRED)
		if(NOT GLEW_FOUND)
			message(FATAL_ERROR "GLEW not found! Aborting!")
		endif(NOT GLEW_FOUND)
	endif (NOT APPLE)

	find_package(GLFW3 REQUIRED)
	if(NOT GLFW3_FOUND)
		message(FATAL_ERROR "GLFW3 not found! Aborting!")
	endif(NOT GLFW3_FOUND)

	find_package(Bullet REQUIRED)
	if(NOT (BULLET_DYNAMICS_FOUND AND BULLET_COLLISION_FOUND AND BULLET_LINEARMATH_FOUND))
		message(FATAL_ERROR "Bullet Physics was not found. Aborting!")
	endif(NOT (BULLET_DYNAMICS_FOUND AND BULLET_COLLISION_FOUND AND BULLET_LINEARMATH_FOUND))

	find_package(SOIL REQUIRED)
	if(NOT ${SOIL_FOUND})
		message(FATAL_ERROR "SOIL wasn't found!")
		message(ERROR "Please install SOIL, or if you have it installed, make sure that modules/FindSOIL.cmake looks for it")
	endif(NOT ${SOIL_FOUND})
endif(MINGW OR UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(UNIX AND NOT APPLE)
	find_package(GTK2 REQUIRED)
	include_directories("${GTK2_INCLUDE_DIRS}")
endif(UNIX AND NOT APPLE)

link_directories("${CMAKE_SOURCE_DIR}/build/bin")
SET(OSX_CF_LIBRARY "")

if(APPLE) # Mac OSX
	SET(GLEW_LIBRARY "")

	set(CMAKE_OSX_ARCHITECTURES i386;x86_64)
	set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "i386 x86_64")
	set(CMAKE_XCODE_ATTRIBUTE_SUPPORTED_PLATFORMS macosx)
>>>>>>> Web GUI now uses Chromium Embedded Framework.

	# Need the 10.7 SDK or later.
	EXECUTE_PROCESS(COMMAND xcodebuild -sdk macosx -version SDKVersion OUTPUT_VARIABLE OSX_SDK_VERSION)
	IF (NOT (OSX_SDK_VERSION VERSION_GREATER 10.7 OR OSX_SDK_VERSION VERSION_EQUAL 10.7))
		MESSAGE(FATAL_ERROR "The installed version of Xcode does not support the 10.7 SDK or later.  Please upgrade Xcode and try again.")
	ENDIF (NOT (OSX_SDK_VERSION VERSION_GREATER 10.7 OR OSX_SDK_VERSION VERSION_EQUAL 10.7))

	# Configure the project to use the correct SDK.
	IF (XCODE_VERSION)
		SET(CMAKE_OSX_SYSROOT macosx)
	ELSE (XCODE_VERSION)
		# Non-Xcode generators need the full path.
		EXECUTE_PROCESS(COMMAND xcodebuild -sdk macosx -version Path | head -n 1 OUTPUT_VARIABLE CMAKE_OSX_SYSROOT)
		STRING(REGEX REPLACE "(\r?\n)+$" "" CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}")
	ENDIF (XCODE_VERSION)

	# Can deploy back to 10.7, the first OS X to support the GL Core.
	SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)

<<<<<<< HEAD
	# Need Core Foundation and libobjc.
	SET(OSX_LIBRARIES "-framework CoreFoundation /usr/lib/libobjc.dylib")

	IF (NOT XCODE_VERSION)
		# Yuck. The Makefile generator is not emitting a -F/Path/To/Awesomium flag.
		# As a workaround, add the necessary flags ourself assuming the Awesomium was installed in its default location.
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -F/Library/Frameworks")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -F/Library/Frameworks")
	ENDIF (NOT XCODE_VERSION)
ENDIF (APPLE)
=======
	# Need Core Foundation
	set(OSX_CF_LIBRARY "-framework CoreFoundation")
endif(APPLE)
>>>>>>> Web GUI now uses Chromium Embedded Framework.

# We like having options
set(BUILD_EXE_Sigma TRUE CACHE BOOL "Build the Sigma test executable")
set(BUILD_STATIC_Sigma FALSE CACHE BOOL "Build Sigma as a static library")
set(BUILD_SHARED_Sigma TRUE CACHE BOOL "Build Sigma as a shared library")

# define all required external libraries
set(Sigma_ALL_LIBS
	${OPENGL_LIBRARIES}
	${GLFW3_LIBRARY}
	${OGGVORBIS_LIBRARIES}
	${OPENAL_LIBRARY}
	${X11_LIBRARIES}
	${OSX_LIBRARIES}
	${GLEW_LIBRARY}
	${SOIL_LIBRARY}
	${BULLET_LINEARMATH_LIBRARIES}
	${BULLET_COLLISION_LIBRARIES}
	${BULLET_DYNAMICS_LIBRARIES}
	${AWESOMIUM_LIBRARY}
	)

# all source common to the library and test exe
set(Sigma_ALL_SOURCE
	${Sigma_SRC}
	${Sigma_SRC_COMPONENT_CPP}
	${Sigma_SRC_SYSTEM_CPP}
	${Sigma_SRC_CONTROLLERS_CPP}
	)

# all source common to the library and test exe
set(Sigma_ALL_INCLUDES
	${Sigma_INCLUDES}
	${Sigma_SRC_COMPONENT_H}
	${Sigma_SRC_SYSTEM_H}
	${Sigma_SRC_CONTROLLERS_H}
	${Sigma_SRC_RESOURCES_H}
	)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/cef")

if(BUILD_STATIC_Sigma)
	add_library(libSigmas STATIC
		${Sigma_ALL_SOURCE}
		${Sigma_ALL_INCLUDES}
		)
	# Link the Sigma lib to all required external libraries
	TARGET_LINK_LIBRARIES(libSigmas ${Sigma_ALL_LIBS})
	if(WIN32)
	set_target_properties(libSigmas PROPERTIES
		OUTPUT_NAME Sigma_static
		)
	else(WIN32)
	set_target_properties(libSigmas PROPERTIES
		OUTPUT_NAME Sigma
		)
	endif(WIN32)
endif(BUILD_STATIC_Sigma)

if(BUILD_SHARED_Sigma)
	add_library(libSigma SHARED
		${Sigma_ALL_SOURCE}
		${Sigma_ALL_INCLUDES}
		)
	# Link the Sigma lib to all required external libraries
	TARGET_LINK_LIBRARIES(libSigma ${Sigma_ALL_LIBS})
	set_target_properties(libSigma PROPERTIES
		OUTPUT_NAME Sigma
		)
endif(BUILD_SHARED_Sigma)

if(BUILD_EXE_Sigma)
	# add the test executable, include the test app source containing main()

	# if we are building a library of Sigma just link that and the includes
	if(BUILD_STATIC_Sigma)
		ADD_EXECUTABLE(Sigma
			${Sigma_ALL_INCLUDES}
			${Sigma_SRC_TESTS_CPP}
			)
		TARGET_LINK_LIBRARIES(Sigma libSigmas)
	elseif(BUILD_SHARED_Sigma)
		ADD_EXECUTABLE(Sigma
			${Sigma_ALL_INCLUDES}
			${Sigma_SRC_TESTS_CPP}
			)
		TARGET_LINK_LIBRARIES(Sigma libSigma)
	else(BUILD_STATIC_Sigma)
		# if just building an exe use all the source and libraries
		ADD_EXECUTABLE(Sigma
			${Sigma_ALL_SOURCE}
			${Sigma_ALL_INCLUDES}
			${Sigma_SRC_TESTS_CPP}
			)
		# Link the executable to all required libraries
		TARGET_LINK_LIBRARIES(Sigma ${Sigma_ALL_LIBS})
	endif(BUILD_STATIC_Sigma)
	
endif(BUILD_EXE_Sigma)

<<<<<<< HEAD
# Link the executable to all required libraries
<<<<<<< HEAD
TARGET_LINK_LIBRARIES(Sigma ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} ${OGGVORBIS_LIBRARIES} ${OPENAL_LIBRARY} ${X11_LIBRARIES} ${OSX_LIBRARIES} ${GLEW_LIBRARY} ${SOIL_LIBRARY} ${BULLET_LINEARMATH_LIBRARIES} ${BULLET_COLLISION_LIBRARIES} ${BULLET_DYNAMICS_LIBRARIES} ${AWESOMIUM_LIBRARY})
=======
target_link_libraries (Sigma ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} ${OSX_CF_LIBRARY} ${GLEW_LIBRARY} ${SOIL_LIBRARY} ${BULLET_LINEARMATH_LIBRARIES} ${BULLET_COLLISION_LIBRARIES} ${BULLET_DYNAMICS_LIBRARIES} cef_dll_wrapper cef)

# build the Tests subproject
#ADD_SUBDIRECTORY("${CMAKE_SOURCE_DIR}/tests")
>>>>>>> Web GUI now uses Chromium Embedded Framework.
=======
>>>>>>> Adjusted CMakeLists and added options to build as a library
