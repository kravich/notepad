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
    main.cpp
    mainwindow.cpp
    topeditorcontainer.cpp
    editortabwidget.cpp
    docengine.cpp
    frmabout.cpp
    notepadng.cpp
    frmpreferences.cpp
    iconprovider.cpp
    EditorNS/editor.cpp
    EditorNS/bannerfilechanged.cpp
    EditorNS/bannerbasicmessage.cpp
    EditorNS/bannerfileremoved.cpp
    EditorNS/customscintilla.cpp
    EditorNS/languageservice.cpp
    EditorNS/nulllexer.cpp
    EditorNS/lexerfactory.cpp
    EditorNS/scintillatheme.cpp
    clickablelabel.cpp
    frmencodingchooser.cpp
    frmindentationmode.cpp
    singleapplication.cpp
    localcommunication.cpp
    Search/frmsearchreplace.cpp
    Search/searchstring.cpp
    Search/advancedsearchdock.cpp
    frmlinenumberchooser.cpp
    keygrabber.cpp
    Sessions/sessions.cpp
    Sessions/persistentcache.cpp
    nngsettings.cpp
    nngrun.cpp
    Search/filesearcher.cpp
    Search/filereplacer.cpp
    Search/searchobjects.cpp
    Search/searchinstance.cpp
    stats.cpp
    Sessions/backupservice.cpp
    svgiconengine.cpp
)

set(notepadng_HEADERS
    include/mainwindow.h
    include/topeditorcontainer.h
    include/editortabwidget.h
    include/docengine.h
    include/frmabout.h
    include/notepadng.h
    include/frmpreferences.h
    include/iconprovider.h
    include/EditorNS/editor.h
    include/EditorNS/bannerfilechanged.h
    include/EditorNS/bannerbasicmessage.h
    include/EditorNS/bannerfileremoved.h
    include/EditorNS/customscintilla.h
    include/EditorNS/nulllexer.h
    include/EditorNS/lexerfactory.h
    include/EditorNS/scintillatheme.h
    include/clickablelabel.h
    include/frmencodingchooser.h
    include/EditorNS/languageservice.h
    include/frmindentationmode.h
    include/singleapplication.h
    include/localcommunication.h
    include/Search/frmsearchreplace.h
    include/Search/advancedsearchdock.h
    include/Search/searchhelpers.h
    include/Search/searchstring.h
    include/frmlinenumberchooser.h
    include/keygrabber.h
    include/Sessions/sessions.h
    include/Sessions/persistentcache.h
    include/nngsettings.h
    include/nngrun.h
    include/Search/filesearcher.h
    include/Search/searchobjects.h
    include/Search/filereplacer.h
    include/Search/searchinstance.h
    include/stats.h
    include/Sessions/backupservice.h
    include/svgiconengine.h
)

set(notepadng_FORMS
    mainwindow.ui
    frmabout.ui
    frmpreferences.ui
    frmencodingchooser.ui
    frmindentationmode.ui
    Search/dlgsearching.ui
    Search/frmsearchreplace.ui
    frmlinenumberchooser.ui
)

set(notepadng_RESOURCES
    resources.qrc
)

set(notepadng_ICON
    ../images/notepadng.icns
)

set(notepadng_TRANSLATIONS
    ../translations/notepadng_de.ts
    ../translations/notepadng_es.ts
    ../translations/notepadng_fr.ts
    ../translations/notepadng_hu.ts
    ../translations/notepadng_it.ts
    ../translations/notepadng_ja.ts
    ../translations/notepadng_pl.ts
    ../translations/notepadng_pt.ts
    ../translations/notepadng_ru.ts
    ../translations/notepadng_sl.ts
    ../translations/notepadng_sv.ts
    ../translations/notepadng_uk.ts
    ../translations/notepadng_zh.ts
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
        ${CMAKE_CURRENT_SOURCE_DIR}
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

set(notepadng_SUPPORT_FILES_PATH ../support_files)

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
