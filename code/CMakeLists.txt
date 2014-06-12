CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT( "UvwFrame" )

SET ( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE )

SET ( UNICODE_FLAGS -D_UNICODE )
SET ( DEPRECATION_FLAGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE )
SET ( RTTI_FLAGS /GR )

SET ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Lib" )
SET ( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Lib" )
SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Bin" )

# 3 d s   m a x

# 3ds Max 9    | MAXSDK9  | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2008 | MAXSDK10 | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2009 | MAXSDK11 | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2010 | MAXSDK12 | Visual Studio 9.0 | 32 Bits | 64 Bits 
# 3ds Max 2011 | MAXSDK13 | Visual Studio 9.0 | 32 Bits | 64 Bits 
                                                      
# Define the Project Name.
#  Projects that are built with the same IDE (ie. Visual Studio 2005) will be
#  added to the same Solution. For this reason all project need to have 
#  unique names.

SET ( UVW_FRAME_MAX_2010    "UvwFrameMax2010" )
SET ( UVW_FRAME_MAX_2011    "UvwFrameMax2011" )
SET ( UVW_FRAME_MAX_2012    "UvwFrameMax2012" )

# Define the header libraries for each SDK

SET ( MAXSDK_2010_HEADERS_DIR "c:/Libraries/maxsdk/12/include" )
SET ( MAXSDK_2011_HEADERS_DIR "c:/Libraries/maxsdk/13/include" )
SET ( MAXSDK_2012_HEADERS_DIR "c:/Libraries/maxsdk/14/include" )

# Define the library files for each SDK, and platform. 64 bits lib path will replace
# 32 bits lib path.

# 32 BITS 

SET ( MAXSDK_2010_LIBRARY_DIR "c:/Libraries/maxsdk/12/lib" )
SET ( MAXSDK_2011_LIBRARY_DIR "c:/Libraries/maxsdk/13/lib" )
SET ( MAXSDK_2012_LIBRARY_DIR "c:/Libraries/maxsdk/14/lib" )

SET ( PROGRAM_FILES "c:\\Program Files (x86)" )

IF ( CMAKE_CL_64 ) # 64 BITS

	SET ( MAXSDK_2010_LIBRARY_DIR "c:/Libraries/maxsdk/12/x64/lib" )
	SET ( MAXSDK_2011_LIBRARY_DIR "c:/Libraries/maxsdk/13/x64/lib" )
	SET ( MAXSDK_2012_LIBRARY_DIR "c:/Libraries/maxsdk/14/x64/lib" )
	
	SET ( PROGRAM_FILES "c:\\Program Files" )

ENDIF ()

# Add the source files.

SET ( SOURCE_FILES
	"../Plugin.cpp"
	"../frame_mod.cpp"
	"../frame_mod_edit.cpp"	
	"../uvwframe.cpp"
	"../uvwframe_mesh.cpp"
)

# Add the header files.

SET ( HEADER_FILES
	"../frame_mod.h"
	"../plugin.h"
	"../resource.h"
	"../uvwframe.h"
)

# Add the resource and definition file.

SET ( RESOURCE_FILES
  "../Helper.rc"
  "../Plugin.def"
)

# Define the max libraries.

SET( MAX_LIBS  "mnmath.lib"
			   "poly.lib"
			   "paramblk2.lib"
			   "core.lib"
			   "geom.lib"
			   "mesh.lib"
			   "maxutil.lib"
			   "bmm.lib"
			   "maxscrpt.lib"
			   "comctl32.lib"
			   "gfx.lib"
)

IF ( MSVC80 )

	#######################################################
	# P L U G I N   F O R   3 D S M A X   9               #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MAXSDK9_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MAXSDK9_LIBRARY_DIR} )

	ADD_LIBRARY ( ${UVW_FRAME_MAX_9} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
	TARGET_LINK_LIBRARIES( ${UVW_FRAME_MAX_9} ${MAX_LIBS} )

	SET_TARGET_PROPERTIES( ${UVW_FRAME_MAX_9} PROPERTIES OUTPUT_NAME "Max9/UvwFrame" )
	SET_TARGET_PROPERTIES( ${UVW_FRAME_MAX_9} PROPERTIES SUFFIX ".dlt" )
	SET_TARGET_PROPERTIES( ${UVW_FRAME_MAX_9} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )

	#######################################################
	# P L U G I N   F O R   3 D S M A X   2 0 0 8         #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MAXSDK10_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MAXSDK10_LIBRARY_DIR} )

	ADD_LIBRARY ( ${UVW_FRAME_MAX_10} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
	TARGET_LINK_LIBRARIES( ${UVW_FRAME_MAX_10} ${MAX_LIBS} )

	SET_TARGET_PROPERTIES( ${UVW_FRAME_MAX_10} PROPERTIES OUTPUT_NAME "Max2008/UvwFrame" )
	SET_TARGET_PROPERTIES( ${UVW_FRAME_MAX_10} PROPERTIES SUFFIX ".dlt" )
	SET_TARGET_PROPERTIES( ${UVW_FRAME_MAX_10} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )
	
ENDIF ( MSVC80 )

IF ( MSVC90 )

	ADD_SUBDIRECTORY ( Max2010 )
	ADD_SUBDIRECTORY ( Max2012 )
	
ENDIF ( MSVC90 )