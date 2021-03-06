# Compile flags as defined in global CMakeLists
set(CMAKE_CXX_FLAGS "${COLOBOT_CXX_FLAGS} ${MXE_CFLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE ${COLOBOT_CXX_FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_DEBUG ${COLOBOT_CXX_FLAGS_DEBUG})


# Subdirectories

add_subdirectory(CBot)

if(TOOLS)
    add_subdirectory(tools)
endif()


# Optional libraries
set(OPTIONAL_LIBS "")
set(OPTIONAL_INCLUDES "")

if(OPENAL_SOUND)
    set(OPTIONAL_LIBS ${OPENAL_LIBRARY})
    set(OPTIONAL_INCLUDES ${OPENAL_INCLUDE_DIR})
endif()

if(WINGETOPT)
    set(OPTIONAL_LIBS ${OPTIONAL_LIBS} wingetopt)
    set(OPTIONAL_INCLUDES ${OPTIONAL_INCLUDES} ${WINGETOPT_INCLUDE_DIR})
endif()

# Additional libraries per platform
if(MXE) # MXE requires special treatment
    set(PLATFORM_LIBS ${MXE_LIBS})
elseif(PLATFORM_WINDOWS)
    # because it isn't included in standard linking libraries
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        find_library(LIBINTL_LIBRARY NAMES intl.lib)
        set(PLATFORM_LIBS ${LIBINTL_LIBRARY})
    else()
        set(PLATFORM_LIBS "-lintl")
    endif()
elseif(PLATFORM_GNU)
    set(PLATFORM_LIBS "-lX11")
elseif(PLATFORM_LINUX)
    # for clock_gettime
    set(PLATFORM_LIBS "-lrt -lX11")
elseif(PLATFORM_MACOSX)
    find_library(LIBINTL_LIBRARY NAMES intl libintl)
    find_path(LIBINTL_INCLUDE_PATH NAMES libintl.h)
    set(PLATFORM_LIBS ${LIBINTL_LIBRARY})
endif()


# Configure file
configure_file(common/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/common/config.h)

set(OPENAL_SRC "")

if(OPENAL_SOUND)
    set(OPENAL_SRC
        sound/oalsound/alsound.cpp
        sound/oalsound/buffer.cpp
        sound/oalsound/channel.cpp
    )
endif()

set(RES_FILES "")

if(PLATFORM_WINDOWS)
    set(RES_FILES "../desktop/colobot.rc")
endif()

# Source files
set(BASE_SOURCES
    app/app.cpp
    app/controller.cpp
    app/input.cpp
    app/pausemanager.cpp
    app/system.cpp
    app/${SYSTEM_CPP_MODULE}
    app/system_other.cpp
    common/event.cpp
    common/image.cpp
    common/iman.cpp
    common/key.cpp
    common/logger.cpp
    common/misc.cpp
    common/pathman.cpp
    common/profile.cpp
    common/restext.cpp
    common/stringutils.cpp
    common/resources/resourcemanager.cpp
    common/resources/inputstreambuffer.cpp
    common/resources/outputstreambuffer.cpp
    common/resources/inputstream.cpp
    common/resources/outputstream.cpp
    common/resources/sndfile.cpp
    graphics/core/color.cpp
    graphics/core/nulldevice.cpp
    graphics/engine/camera.cpp
    graphics/engine/cloud.cpp
    graphics/engine/engine.cpp
    graphics/engine/lightman.cpp
    graphics/engine/lightning.cpp
    graphics/engine/modelfile.cpp
    graphics/engine/modelmanager.cpp
    graphics/engine/particle.cpp
    graphics/engine/planet.cpp
    graphics/engine/pyro.cpp
    graphics/engine/terrain.cpp
    graphics/engine/text.cpp
    graphics/engine/water.cpp
    graphics/opengl/gldevice.cpp
    object/auto/auto.cpp
    object/auto/autobase.cpp
    object/auto/autoconvert.cpp
    object/auto/autoderrick.cpp
    object/auto/autodestroyer.cpp
    object/auto/autoegg.cpp
    object/auto/autoenergy.cpp
    object/auto/autofactory.cpp
    object/auto/autoflag.cpp
    object/auto/autohuston.cpp
    object/auto/autoinfo.cpp
    object/auto/autojostle.cpp
    object/auto/autokid.cpp
    object/auto/autolabo.cpp
    object/auto/automush.cpp
    object/auto/autonest.cpp
    object/auto/autonuclear.cpp
    object/auto/autopara.cpp
    object/auto/autoportico.cpp
    object/auto/autoradar.cpp
    object/auto/autorepair.cpp
    object/auto/autoresearch.cpp
    object/auto/autoroot.cpp
    object/auto/autosafe.cpp
    object/auto/autostation.cpp
    object/auto/autotower.cpp
    object/brain.cpp
    object/level/parser.cpp
    object/level/parserline.cpp
    object/level/parserparam.cpp
    object/level/parserexceptions.cpp
    object/mainmovie.cpp
    object/motion/motion.cpp
    object/motion/motionant.cpp
    object/motion/motionbee.cpp
    object/motion/motiondummy.cpp
    object/motion/motionhuman.cpp
    object/motion/motionmother.cpp
    object/motion/motionspider.cpp
    object/motion/motiontoto.cpp
    object/motion/motionvehicle.cpp
    object/motion/motionworm.cpp
    object/object.cpp
    object/robotmain.cpp
    object/objman.cpp
    object/task/task.cpp
    object/task/taskadvance.cpp
    object/task/taskbuild.cpp
    object/task/taskdeletemark.cpp
    object/task/taskfire.cpp
    object/task/taskfireant.cpp
    object/task/taskflag.cpp
    object/task/taskgoto.cpp
    object/task/taskgungoal.cpp
    object/task/taskinfo.cpp
    object/task/taskmanager.cpp
    object/task/taskmanip.cpp
    object/task/taskpen.cpp
    object/task/taskrecover.cpp
    object/task/taskreset.cpp
    object/task/tasksearch.cpp
    object/task/taskshield.cpp
    object/task/taskspiderexplo.cpp
    object/task/tasktake.cpp
    object/task/taskterraform.cpp
    object/task/taskturn.cpp
    object/task/taskwait.cpp
    physics/physics.cpp
    script/cbottoken.cpp
    script/cmdtoken.cpp
    script/script.cpp
    script/scriptfunc.cpp
    sound/sound.cpp
    ui/button.cpp
    ui/check.cpp
    ui/color.cpp
    ui/compass.cpp
    ui/control.cpp
    ui/displayinfo.cpp
    ui/displaytext.cpp
    ui/edit.cpp
    ui/editvalue.cpp
    ui/gauge.cpp
    ui/group.cpp
    ui/image.cpp
    ui/interface.cpp
    ui/key.cpp
    ui/label.cpp
    ui/list.cpp
    ui/maindialog.cpp
    ui/mainmap.cpp
    ui/mainshort.cpp
    ui/map.cpp
    ui/scroll.cpp
    ui/shortcut.cpp
    ui/slider.cpp
    ui/studio.cpp
    ui/target.cpp
    ui/window.cpp
    ${OPENAL_SRC}
)

set(MAIN_SOURCES
    app/main.cpp
    ${RES_FILES}
)


# Libraries
set(LIBS
    CBot
    clipboard
    localename
    ${SDL_LIBRARY}
    ${SDLIMAGE_LIBRARY}
    ${SDLTTF_LIBRARY}
    ${OPENGL_LIBRARY}
    ${PNG_LIBRARIES}
    ${GLEW_LIBRARY}
    ${Boost_LIBRARIES}
    ${LIBSNDFILE_LIBRARY}
    ${OPTIONAL_LIBS}
    ${PLATFORM_LIBS}
    ${PHYSFS_LIBRARY}
)

set(COLOBOT_LIBS ${LIBS} PARENT_SCOPE)


# Includes
set(LOCAL_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SYSTEM_INCLUDES
    ${SDL_INCLUDE_DIR}
    ${SDLIMAGE_INCLUDE_DIR}
    ${SDLTTF_INCLUDE_DIR}
    ${PNG_INCLUDE_DIRS}
    ${GLEW_INCLUDE_PATH}
    ${Boost_INCLUDE_DIRS}
    ${LIBSNDFILE_INCLUDE_DIR}
    ${LOCALENAME_INCLUDE_DIR}
    ${CLIPBOARD_INCLUDE_DIR}
    ${PHYSFS_INCLUDE_PATH}
    ${LIBINTL_INCLUDE_PATH}
    ${OPTIONAL_INCLUDES}
)

set(COLOBOT_LOCAL_INCLUDES ${LOCAL_INCLUDES} PARENT_SCOPE)
set(COLOBOT_SYSTEM_INCLUDES ${SYSTEM_INCLUDES} PARENT_SCOPE)

include_directories(${LOCAL_INCLUDES})
include_directories(SYSTEM ${SYSTEM_INCLUDES})


# Link directories
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/CBot
)


# Targets

add_library(colobotbase STATIC ${BASE_SOURCES})

add_executable(colobot ${MAIN_SOURCES})
target_link_libraries(colobot colobotbase ${LIBS})

install(TARGETS colobot RUNTIME DESTINATION ${COLOBOT_INSTALL_BIN_DIR})
if(NOT CBOT_STATIC)
    set_target_properties(colobot PROPERTIES INSTALL_RPATH ${COLOBOT_INSTALL_LIB_DIR})
endif()
