set(EXEC_SRC 
  decode_without_daq.cpp
  EventDict.cpp
  Event.cpp
  DataDef.cpp
)

add_executable(wns_decode ${EXEC_SRC})

add_custom_command(OUTPUT EventDict.cpp EventDict.h
        COMMAND rootcint -f EventDict.cpp -c ${CMAKE_CURRENT_SOURCE_DIR}/Event.h ${CMAKE_CURRENT_SOURCE_DIR}/EventLinkDef.h
        COMMENT "rootcint .."
        )

target_link_libraries(wns_decode ${ROOT_LIBS})

install(TARGETS wns_decode
        DESTINATION ${PROJECT_SOURCE_DIR}/bin )
