# The project's name is Pokebot it uses C and C++ code
PROJECT(Pokebot C CXX)

cmake_minimum_required( VERSION 2.6.0 )
if( COMMAND cmake_policy )
    cmake_policy( SET CMP0003 NEW )
    cmake_policy( SET CMP0005 OLD )
endif( COMMAND cmake_policy )
SET( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeScripts )

option( ENABLE_SDL "Build the SDL port" ON )
option( ENABLE_GTK "Build the GTK+ GUI" ON )
option( ENABLE_WX "Build the wxWidgets port" OFF )
option( ENABLE_DEBUGGER "Enable the debugger" ON )
option( ENABLE_NLS "Enable translations" ON )
option( ENABLE_ASM_CORE "Enable x86 ASM CPU cores" OFF )
option( ENABLE_ASM_SCALERS "Enable x86 ASM graphic filters" OFF )
option( ENABLE_LINK "Enable GBA linking functionality" OFF )
option( ENABLE_LIRC "Enable LIRC support" OFF )
option( ENABLE_FFMPEG "Enable ffmpeg A/V recording" OFF )
if(ENABLE_ASM_SCALERS)
    option( ENABLE_MMX "Enable MMX" OFF )
endif(ENABLE_ASM_SCALERS)
option( ENABLE_GBA_LOGGING "Enable extended GBA logging" ON )
if( ENABLE_GBA_LOGGING )
   ADD_DEFINITIONS (-DGBA_LOGGING )
endif( ENABLE_GBA_LOGGING )
if(ENABLE_MMX)
  ADD_DEFINITIONS (-DMMX)
endif(ENABLE_MMX)

# The SDL port can't be built without debugging support
if( NOT ENABLE_DEBUGGER AND ENABLE_SDL )
        message( SEND_ERROR "The SDL port can't be built without debugging support" )
endif( NOT ENABLE_DEBUGGER AND ENABLE_SDL )

# Set the version number with -DVERSION=X.X.X-uber
IF( NOT VERSION )
    SET( VERSION "1.8.0-SVN" )

    IF(EXISTS "${CMAKE_SOURCE_DIR}/.svn")
        FIND_PACKAGE(Subversion)
        IF(SUBVERSION_FOUND)
            Subversion_WC_INFO(${CMAKE_SOURCE_DIR} SVN_INFO)
            SET( VERSION "1.8.0-SVN${SVN_INFO_WC_REVISION}" )
        ENDIF(SUBVERSION_FOUND)
    ENDIF(EXISTS "${CMAKE_SOURCE_DIR}/.svn")
ENDIF( NOT VERSION )

# Fill in SDLMAIN_LIBRARY on OS X manually to avoid using SDLMain.m
# OS X users will have to compile and install SDL from source.
if( APPLE )
    SET(SDLMAIN_LIBRARY "-lSDLmain")
endif( APPLE )

# Check for nasm
if( ENABLE_ASM_SCALERS )
    ENABLE_LANGUAGE( ASM_NASM )
endif( ENABLE_ASM_SCALERS )

# Look for some dependencies using CMake scripts
FIND_PACKAGE ( ZLIB REQUIRED )
FIND_PACKAGE ( PNG REQUIRED )
FIND_PACKAGE ( OpenGL REQUIRED )
FIND_PACKAGE ( SDL REQUIRED )

if( ENABLE_LINK )
    FIND_PACKAGE ( SFML REQUIRED )
endif( ENABLE_LINK )
# set the standard libraries all ports use
SET(VBAMCORE_LIBS
    lua5.1
    -Llib
    vbamcore
    fex
    ${SDL_LIBRARY}
    ${SFML_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${ZLIB_LIBRARY}
    ${PNG_LIBRARY})

IF( ENABLE_LIRC )
        SET( WITHLIRC 1 )
ELSE( ENABLE_LIRC )
        SET( WITHLIRC 0 )
ENDIF( ENABLE_LIRC )

# Set the default install dir
IF( NOT DATA_INSTALL_DIR )
    SET( DATA_INSTALL_DIR "share/pokebot" )
ENDIF( NOT DATA_INSTALL_DIR )

SET( PKGDATADIR ${CMAKE_INSTALL_PREFIX}/${DATA_INSTALL_DIR} )

# Set the configuration file location
IF( NOT SYSCONFDIR )
    SET( SYSCONFDIR "/etc" )
ENDIF( NOT SYSCONFDIR )

# C defines
ADD_DEFINITIONS (-DHAVE_NETINET_IN_H -DHAVE_ARPA_INET_H -DHAVE_ZLIB_H -DFINAL_VERSION -DSDL -DUSE_OPENGL -DSYSCONFDIR='"${SYSCONFDIR}"' -DWITH_LIRC='${WITHLIRC}')
ADD_DEFINITIONS (-DVERSION='"${VERSION}"' -DPKGDATADIR='"${PKGDATADIR}"' -DPACKAGE='')

if( ENABLE_LINK )
    # IPC linking code needs sem_timedwait which can be either in librt or pthreads
    FIND_LIBRARY(RT_LIB rt)
    IF(RT_LIB)
       SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${RT_LIB})
       SET(VBAMCORE_LIBS ${VBAMCORE_LIBS} ${RT_LIB})
    ENDIF(RT_LIB)
    
    FIND_LIBRARY(PTHREAD_LIB pthread)
    IF(PTHREAD_LIB)
       SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${PTHREAD_LIB})
       SET(VBAMCORE_LIBS ${VBAMCORE_LIBS} ${PTHREAD_LIB})
    ENDIF(PTHREAD_LIB)
    
    INCLUDE(CheckFunctionExists)
    CHECK_FUNCTION_EXISTS(sem_timedwait SEM_TIMEDWAIT)
    IF( SEM_TIMEDWAIT)
        ADD_DEFINITIONS (-DHAVE_SEM_TIMEDWAIT)
    ENDIF( SEM_TIMEDWAIT)
else( ENABLE_LINK )
    ADD_DEFINITIONS (-DNO_LINK)
endif( ENABLE_LINK )

# The debugger is enabled by default
if( NOT ENABLE_DEBUGGER )
    ADD_DEFINITIONS (-DNO_DEBUGGER)
else( NOT ENABLE_DEBUGGER )
    ADD_DEFINITIONS (-DBKPT_SUPPORT)
endif( NOT ENABLE_DEBUGGER )

# The ASM core is disabled by default because we don't know on which platform we are
IF( NOT ENABLE_ASM_CORE )
    ADD_DEFINITIONS (-DC_CORE)
ENDIF( NOT ENABLE_ASM_CORE )

# Enable internationalization
if( ENABLE_NLS )
    SET( LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale )
    ADD_DEFINITIONS ( -DENABLE_NLS )
    ADD_DEFINITIONS ( -DLOCALEDIR=\\\"${LOCALEDIR}\\\" )
    # for now, only GBALink.cpp uses gettext() directly
    IF(ENABLE_LINK)
        FIND_PATH(LIBINTL_INC libintl.h )
        FIND_LIBRARY(LIBINTL_LIB intl )
	IF(LIBINTL_LIB)
            SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${LIBINTL_LIB})
            SET(VBAMCORE_LIBS ${VBAMCORE_LIBS} ${LIBINTL_LIB})
	ENDIF(LIBINTL_LIB)
        INCLUDE(CheckFunctionExists)
        CHECK_FUNCTION_EXISTS(gettext GETTEXT_FN)
        IF(NOT LIBINTL_INC OR NOT GETTEXT_FN)
            message( SEND_ERROR "NLS requires libintl" )
        ENDIF(NOT LIBINTL_INC OR NOT GETTEXT_FN)
        INCLUDE_DIRECTORIES(${LIBINTL_INC})
    ENDIF(ENABLE_LINK)
endif( ENABLE_NLS )

# Compiler flags
IF ( WIN32 )
    SET( CMAKE_ASM_NASM_FLAGS "-I$(CMAKE_SOURCE_DIR)/vbam/filters/hq/asm/ -O1 -w-orphan-labels")
ELSE ( WIN32 )
    SET( CMAKE_ASM_NASM_FLAGS "-I$(CMAKE_SOURCE_DIR)/vbam/filters/hq/asm/ -O1 -DELF -w-orphan-labels")
ENDIF ( WIN32 )

SET( CMAKE_C_FLAGS_RELEASE "-O3")
SET( CMAKE_CXX_FLAGS_RELEASE "-O3")
SET( CMAKE_C_FLAGS_DEBUG "-g -Wall")
SET( CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
SET( CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

# Source files definition
SET(SRC_SDL
    vbam/sdl/debugger.cpp
    vbam/sdl/SDL.cpp
    vbam/sdl/filters.cpp
    vbam/sdl/text.cpp
    vbam/sdl/inputSDL.cpp
    vbam/sdl/expr.cpp
    vbam/sdl/exprNode.cpp
    vbam/sdl/expr-lex.cpp
)

SET(SRC_MAIN
    src/main.cpp

    src/Data/Data.cpp
    src/Data/World.cpp
    src/Data/Player.cpp
    src/Data/PokemonData.cpp
    src/Data/BattleData.cpp

    src/Bot/Bot.cpp
    src/Bot/PathFinder.cpp

    src/Action/AAction.cpp
    src/Action/MoveTo.cpp
    src/Action/TalkTo.cpp
    src/Action/UseWarp.cpp
    src/Action/Battle.cpp
    src/Action/MoveCursor.cpp
    src/Action/PressButton.cpp
    src/Action/Wait.cpp

    src/PokeScript/Script.cpp
    src/PokeScript/Readers.cpp
    src/PokeScript/Commands.cpp
    src/PokeScript/VM.cpp
    src/PokeScript/Executers.cpp
    src/PokeScript/Context.cpp

    src/Utils/ThumbDisas.cpp
    src/Utils/PokemonUtils.cpp
    src/Utils/PrintUtils.cpp

    src/lua/Lua.cpp
)

INCLUDE_DIRECTORIES(
    include
    LuaBridge
    ${ZLIB_INCLUDE_DIR}
    fex
    ${PNG_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    /usr/include/lua5.1
)

IF( ENABLE_GTK )
    INCLUDE_DIRECTORIES(
        ${GLIBMM_INCLUDE_DIRS}
        ${GIOMM_INCLUDE_DIRS}
        ${GTKMM_INCLUDE_DIRS}
        ${GDKMM_INCLUDE_DIRS}
        ${GTKGLMM_INCLUDE_DIRS}
    )

    LINK_DIRECTORIES(
        ${GLIBMM_LIBRARY_DIRS}
        ${GIOMM_LIBRARY_DIRS}
        ${GTKMM_LIBRARY_DIRS}
        ${GTKGLMM_LIBRARY_DIRS}
    )
ENDIF( ENABLE_GTK )

IF( ENABLE_FFMPEG )
    INCLUDE_DIRECTORIES(
        ${FFMPEG_INCLUDE_DIRS}
    )
ENDIF( ENABLE_FFMPEG )

IF( ENABLE_SDL )
    ADD_EXECUTABLE (
        pokebot
        WIN32
        ${SRC_SDL}
        ${SRC_MAIN}
    )

    IF( WIN32 )
        SET( WIN32_LIBRARIES wsock32 )
    ENDIF( WIN32 )

    TARGET_LINK_LIBRARIES (
        pokebot
        ${VBAMCORE_LIBS}
        ${WIN32_LIBRARIES}
        ${LIRC_CLIENT_LIBRARY}
    )

    INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pokebot DESTINATION bin)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/vbam/sdl/vbam.cfg-example
            DESTINATION ${SYSCONFDIR}
            RENAME vbam.cfg)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/vbam/vba-over.ini DESTINATION ${DATA_INSTALL_DIR})
ENDIF( ENABLE_SDL )

IF( ENABLE_WX )
    # since this has generated source files, it's easier to just
    # make from the subdir
    # otherwise out-of-tree builds have trouble

    add_subdirectory(vbam/wx)

ENDIF( ENABLE_WX )
