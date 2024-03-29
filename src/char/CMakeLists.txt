
#
# setup
#
set( SQL_CHAR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}  CACHE INTERNAL "" )


#
# char sql
#
if( BUILD_SQL_SERVERS )
message( STATUS "Creating target char-server" )
set( SQL_CHAR_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/char.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_auction.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_elemental.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_guild.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_homun.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_mail.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_mercenary.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_party.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_pet.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_quest.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_storage.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/inter.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/pincode.h"
	)
set( SQL_CHAR_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/char.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_auction.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_elemental.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_guild.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_homun.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_mail.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_mercenary.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_party.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_pet.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_quest.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/int_storage.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/inter.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pincode.c"
	)
set( DEPENDENCIES common_sql )
set( LIBRARIES ${GLOBAL_LIBRARIES} )
set( INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${COMMON_BASE_INCLUDE_DIRS} )
set( DEFINITIONS "${GLOBAL_DEFINITIONS} ${COMMON_BASE_DEFINITIONS}" )
set( SOURCE_FILES ${COMMON_BASE_HEADERS} ${COMMON_SQL_HEADERS} ${SQL_CHAR_HEADERS} ${SQL_CHAR_SOURCES} )
source_group( common FILES ${COMMON_BASE_HEADERS} ${COMMON_SQL_HEADERS} )
source_group( char FILES ${SQL_CHAR_HEADERS} ${SQL_CHAR_SOURCES} )
include_directories( ${INCLUDE_DIRS} )
add_executable( char-server ${SOURCE_FILES} )
add_dependencies( char-server ${DEPENDENCIES} )
target_link_libraries( char-server ${LIBRARIES} ${DEPENDENCIES} )
set_target_properties( char-server PROPERTIES COMPILE_FLAGS "${DEFINITIONS}" )
if( INSTALL_COMPONENT_RUNTIME )
	cpack_add_component( Runtime_charserver_sql DESCRIPTION "char-server (sql version)" DISPLAY_NAME "char-server" GROUP Runtime )
	install( TARGETS char-server
		DESTINATION "."
		COMPONENT Runtime_charserver_sql )
endif( INSTALL_COMPONENT_RUNTIME )
message( STATUS "Creating target char-server - done" )
endif( BUILD_SQL_SERVERS )
