#
# Nicholas Yue nicholas_yue@users.sourceforge.net
#
# Note:
# (1) A dummy file include/config.h is required (remance from configure)
# (2) Default installation directory is /usr/local, override with -DCMAKE_INSTALL_PREFIX="" during cmake
#     invocation
# (3) Do the usual "make clean all" to build the library
# (4) To install either "make install" or "make install DESTDIR=<your directory>"
# (5) Need to include changes in include/log4cpp/Portability.hh for OSX to build

PROJECT ( LOG4CPP )

INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/include )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/. )

IF (WIN32)
  ADD_DEFINITIONS ( -D_CRT_SECURE_NO_WARNINGS )
ELSE (WIN32)
  IF (APPLE)
    ADD_DEFINITIONS ( -DNDEBUG -DLOG4CPP_HAVE_SSTREAM )
  ELSE (APPLE)
    ADD_DEFINITIONS ( -pthread -DNDEBUG -DLOG4CPP_HAVE_SSTREAM )
  ENDIF (APPLE)
ENDIF (WIN32)

IF ( CMAKE_BUILD_TYPE MATCHES "Debug" )
  SET ( LOG4CPP_LIBRARY_NAME "log4cppD" )
ELSE ( CMAKE_BUILD_TYPE MATCHES "Debug" )
  SET ( LOG4CPP_LIBRARY_NAME "log4cpp" )
ENDIF ( CMAKE_BUILD_TYPE MATCHES "Debug" )

ADD_LIBRARY ( ${LOG4CPP_LIBRARY_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/Appender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/AppenderSkeleton.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/AppendersFactory.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/BufferingAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/FactoryParams.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/LayoutsFactory.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/LevelEvaluator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Localtime.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/PassThroughLayout.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/TriggeringEventEvaluatorFactory.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/LayoutAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/FileAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/DailyRollingFileAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/RollingFileAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/FixedContextCategory.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/IdsaAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/OstreamAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/StringQueueAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/SyslogAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/RemoteSyslogAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/SimpleLayout.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/BasicLayout.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/PatternLayout.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Category.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/CategoryStream.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/HierarchyMaintainer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Configurator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/BasicConfigurator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/SimpleConfigurator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/PropertyConfigurator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/PropertyConfiguratorImpl.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/LoggingEvent.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Priority.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/NDC.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Filter.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/TimeStamp.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/StringUtil.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Properties.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Win32DebugAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/NTEventLogAppender.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/DllMain.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/DummyThreads.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/MSThreads.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/OmniThreads.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/PThreads.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/PortabilityImpl.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/AbortAppender.cpp
)

IF (WIN32)
  TARGET_LINK_LIBRARIES (${LOG4CPP_LIBRARY_NAME} kernel32 user32 ws2_32 advapi32 )
  SET_TARGET_PROPERTIES(${LOG4CPP_LIBRARY_NAME} PROPERTIES LINK_FLAGS /NODEFAULTLIB:msvcrt )
ENDIF (WIN32)

INSTALL (
  DIRECTORY include/log4cpp
  DESTINATION include
  PATTERN "config.h" EXCLUDE
  PATTERN ".svn" EXCLUDE
  PATTERN "*.am" EXCLUDE
  PATTERN "*.in" EXCLUDE
  )

INSTALL (
  TARGETS ${LOG4CPP_LIBRARY_NAME}
  ARCHIVE DESTINATION lib
  )
