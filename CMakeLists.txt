cmake_minimum_required(VERSION 3.10)

project(notepadng)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")

find_package(Qt6
    COMPONENTS
        Core
        Gui
        Svg
        Widgets
        PrintSupport
        Network
        DBus
        Xml
        Core5Compat
        LinguistTools
    REQUIRED
)

find_package(QScintilla REQUIRED)

find_package(PkgConfig REQUIRED)

pkg_check_modules(uchardet REQUIRED IMPORTED_TARGET uchardet)

set(notepadng_SRCS
    src/main.cpp
    src/mainwindow.cpp
    src/topeditorcontainer.cpp
    src/editortabwidget.cpp
    src/docengine.cpp
    src/frmabout.cpp
    src/notepadng.cpp
    src/frmpreferences.cpp
    src/iconprovider.cpp
    src/EditorNS/editor.cpp
    src/EditorNS/bannerfilechanged.cpp
    src/EditorNS/bannerbasicmessage.cpp
    src/EditorNS/bannerfileremoved.cpp
    src/EditorNS/customscintilla.cpp
    src/EditorNS/languageservice.cpp
    src/EditorNS/nulllexer.cpp
    src/EditorNS/lexerfactory.cpp
    src/EditorNS/scintillatheme.cpp
    src/clickablelabel.cpp
    src/frmencodingchooser.cpp
    src/frmindentationmode.cpp
    src/singleapplication.cpp
    src/localcommunication.cpp
    src/Search/frmsearchreplace.cpp
    src/Search/searchstring.cpp
    src/Search/advancedsearchdock.cpp
    src/frmlinenumberchooser.cpp
    src/keygrabber.cpp
    src/Sessions/sessions.cpp
    src/Sessions/persistentcache.cpp
    src/nngsettings.cpp
    src/nngrun.cpp
    src/Search/filesearcher.cpp
    src/Search/filereplacer.cpp
    src/Search/searchobjects.cpp
    src/Search/searchinstance.cpp
    src/stats.cpp
    src/Sessions/backupservice.cpp
    src/svgiconengine.cpp
)

set(notepadng_HEADERS
    src/include/mainwindow.h
    src/include/topeditorcontainer.h
    src/include/editortabwidget.h
    src/include/docengine.h
    src/include/frmabout.h
    src/include/notepadng.h
    src/include/frmpreferences.h
    src/include/iconprovider.h
    src/include/EditorNS/editor.h
    src/include/EditorNS/bannerfilechanged.h
    src/include/EditorNS/bannerbasicmessage.h
    src/include/EditorNS/bannerfileremoved.h
    src/include/EditorNS/customscintilla.h
    src/include/EditorNS/nulllexer.h
    src/include/EditorNS/lexerfactory.h
    src/include/EditorNS/scintillatheme.h
    src/include/clickablelabel.h
    src/include/frmencodingchooser.h
    src/include/EditorNS/languageservice.h
    src/include/frmindentationmode.h
    src/include/singleapplication.h
    src/include/localcommunication.h
    src/include/Search/frmsearchreplace.h
    src/include/Search/advancedsearchdock.h
    src/include/Search/searchhelpers.h
    src/include/Search/searchstring.h
    src/include/frmlinenumberchooser.h
    src/include/keygrabber.h
    src/include/Sessions/sessions.h
    src/include/Sessions/persistentcache.h
    src/include/nngsettings.h
    src/include/nngrun.h
    src/include/Search/filesearcher.h
    src/include/Search/searchobjects.h
    src/include/Search/filereplacer.h
    src/include/Search/searchinstance.h
    src/include/stats.h
    src/include/Sessions/backupservice.h
    src/include/svgiconengine.h
)

set(notepadng_FORMS
    src/mainwindow.ui
    src/frmabout.ui
    src/frmpreferences.ui
    src/frmencodingchooser.ui
    src/frmindentationmode.ui
    src/Search/dlgsearching.ui
    src/Search/frmsearchreplace.ui
    src/frmlinenumberchooser.ui
)

set(notepadng_RESOURCES
    src/resources.qrc
)

set(notepadng_ICON
    images/notepadng.icns
)

set(notepadng_TRANSLATIONS
    translations/notepadng_de.ts
    translations/notepadng_es.ts
    translations/notepadng_fr.ts
    translations/notepadng_hu.ts
    translations/notepadng_it.ts
    translations/notepadng_ja.ts
    translations/notepadng_pl.ts
    translations/notepadng_pt.ts
    translations/notepadng_ru.ts
    translations/notepadng_sl.ts
    translations/notepadng_sv.ts
    translations/notepadng_uk.ts
    translations/notepadng_zh.ts
)

qt_add_translation(notepadng_QMS ${notepadng_TRANSLATIONS})

add_executable(notepadng
    ${notepadng_SRCS}
    ${notepadng_HEADERS}
    ${notepadng_FORMS}
    ${notepadng_RESOURCES}
    ${notepadng_ICON}
    ${notepadng_QMS}
)

set_target_properties(notepadng
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)

target_include_directories(notepadng
    PRIVATE
        src
)

target_link_libraries(notepadng
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Svg
        Qt6::Widgets
        Qt6::PrintSupport
        Qt6::Network
        Qt6::DBus
        Qt6::Xml
        Qt6::Core5Compat
        QScintilla::QScintilla
        PkgConfig::uchardet
)

set(GNU_COMPILE_OPTIONS
    -Werror=suggest-override
)

set(CLANG_COMPILE_OPTIONS
    ${GNU_COMPILE_OPTIONS}
    -Werror=inconsistent-missing-override
)

target_compile_options(notepadng
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:${GNU_COMPILE_OPTIONS}>
        $<$<CXX_COMPILER_ID:Clang>:${CLANG_COMPILE_OPTIONS}>
)

set(notepadng_SUPPORT_FILES_PATH support_files)

file(GLOB_RECURSE notepadng_SUPPORT_FILES ${notepadng_SUPPORT_FILES_PATH}/*)
add_custom_target(support_files SOURCES ${notepadng_SUPPORT_FILES})

install(TARGETS notepadng)

install(FILES ${notepadng_SUPPORT_FILES_PATH}/Languages.json
    DESTINATION share/notepadng
)

install(DIRECTORY ${notepadng_SUPPORT_FILES_PATH}/themes
    DESTINATION share/notepadng
)

install(FILES ${notepadng_SUPPORT_FILES_PATH}/shortcuts/notepadng.desktop
    DESTINATION share/applications
)

install(DIRECTORY ${notepadng_SUPPORT_FILES_PATH}/icons
    DESTINATION share
)

install(FILES ${notepadng_SUPPORT_FILES_PATH}/notepadng.appdata.xml
    DESTINATION share/metainfo
)

include(CTest)

if(BUILD_TESTING)
    # FIXME: Implement tests
    #add_subdirectory(tests)
endif()
