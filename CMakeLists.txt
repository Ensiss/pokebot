# The project's name is 'Pokebot' it uses C and C++ code
PROJECT(Pokebot C CXX)

cmake_minimum_required( VERSION 2.6.0 )
if( COMMAND cmake_policy )
    cmake_policy( SET CMP0003 NEW )
    cmake_policy( SET CMP0005 OLD )
endif( COMMAND cmake_policy )
SET( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeScripts )

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
    SET( DATA_INSTALL_DIR "share/vbam" )
ENDIF( NOT DATA_INSTALL_DIR )

SET( PKGDATADIR ${CMAKE_INSTALL_PREFIX}/${DATA_INSTALL_DIR} )

# Set the configuration file location
IF( NOT SYSCONFDIR )
    SET( SYSCONFDIR "/etc" )
ENDIF( NOT SYSCONFDIR )

# C defines
ADD_DEFINITIONS (-DHAVE_NETINET_IN_H -DHAVE_ARPA_INET_H -DHAVE_ZLIB_H -DFINAL_VERSION -DSDL -DUSE_OPENGL -DSYSCONFDIR='"${SYSCONFDIR}"' -DWITH_LIRC='${WITHLIRC}')
ADD_DEFINITIONS (-DVERSION='"${VERSION}"' -DPKGDATADIR='"${PKGDATADIR}"' -DPACKAGE='')

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

    src/Action/MoveTo.cpp
    src/Action/Battle.cpp

    src/PokeScript/Disass.cpp
    src/PokeScript/Args.cpp
    src/PokeScript/Print.cpp

    src/Utils/ThumbDisas.cpp
    src/Utils/PokemonUtils.cpp
    src/Utils/PrintUtils.cpp
)

INCLUDE_DIRECTORIES(
    include
    ${ZLIB_INCLUDE_DIR}
    fex
    ${PNG_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
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
