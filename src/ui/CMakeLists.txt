find_package(Qt6
    COMPONENTS
        Core
        Gui
        Svg
        Widgets
        PrintSupport
        Network
        WebEngineWidgets
        WebChannel
        WebSockets
        DBus
        Core5Compat
        LinguistTools
    REQUIRED
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(uchardet REQUIRED IMPORTED_TARGET uchardet)

set(notepad_SRCS
    main.cpp
    mainwindow.cpp
    topeditorcontainer.cpp
    editortabwidget.cpp
    docengine.cpp
    frmabout.cpp
    notepad.cpp
    frmpreferences.cpp
    iconprovider.cpp
    EditorNS/editor.cpp
    EditorNS/bannerfilechanged.cpp
    EditorNS/bannerbasicmessage.cpp
    EditorNS/bannerfileremoved.cpp
    EditorNS/customqwebview.cpp
    EditorNS/languageservice.cpp
    clickablelabel.cpp
    frmencodingchooser.cpp
    EditorNS/bannerindentationdetected.cpp
    frmindentationmode.cpp
    singleapplication.cpp
    localcommunication.cpp
    Search/frmsearchreplace.cpp
    Search/searchstring.cpp
    Search/advancedsearchdock.cpp
    Extensions/extension.cpp
    frmlinenumberchooser.cpp
    Extensions/extensionsserver.cpp
    Extensions/Stubs/stub.cpp
    Extensions/runtimesupport.cpp
    Extensions/Stubs/windowstub.cpp
    Extensions/Stubs/notepadstub.cpp
    Extensions/Stubs/editorstub.cpp
    Extensions/extensionsloader.cpp
    Extensions/Stubs/menuitemstub.cpp
    Extensions/installextension.cpp
    keygrabber.cpp
    Sessions/sessions.cpp
    Sessions/persistentcache.cpp
    npsettings.cpp
    nprun.cpp
    Search/filesearcher.cpp
    Search/filereplacer.cpp
    Search/searchobjects.cpp
    Search/searchinstance.cpp
    stats.cpp
    Sessions/backupservice.cpp
    svgiconengine.cpp
)

set(notepad_HEADERS
    include/mainwindow.h
    include/topeditorcontainer.h
    include/editortabwidget.h
    include/docengine.h
    include/frmabout.h
    include/notepad.h
    include/frmpreferences.h
    include/iconprovider.h
    include/EditorNS/editor.h
    include/EditorNS/bannerfilechanged.h
    include/EditorNS/bannerbasicmessage.h
    include/EditorNS/bannerfileremoved.h
    include/EditorNS/customqwebview.h
    include/clickablelabel.h
    include/frmencodingchooser.h
    include/EditorNS/bannerindentationdetected.h
    include/EditorNS/languageservice.h
    include/frmindentationmode.h
    include/singleapplication.h
    include/localcommunication.h
    include/Search/frmsearchreplace.h
    include/Search/advancedsearchdock.h
    include/Search/searchhelpers.h
    include/Search/searchstring.h
    include/Extensions/extension.h
    include/frmlinenumberchooser.h
    include/Extensions/extensionsserver.h
    include/Extensions/Stubs/stub.h
    include/Extensions/runtimesupport.h
    include/Extensions/Stubs/windowstub.h
    include/Extensions/Stubs/notepadstub.h
    include/Extensions/Stubs/editorstub.h
    include/Extensions/extensionsloader.h
    include/Extensions/Stubs/menuitemstub.h
    include/Extensions/installextension.h
    include/keygrabber.h
    include/Sessions/sessions.h
    include/Sessions/persistentcache.h
    include/npsettings.h
    include/nprun.h
    include/Search/filesearcher.h
    include/Search/searchobjects.h
    include/Search/filereplacer.h
    include/Search/searchinstance.h
    include/stats.h
    include/Sessions/backupservice.h
    include/svgiconengine.h
)

set(notepad_FORMS
    mainwindow.ui
    frmabout.ui
    frmpreferences.ui
    frmencodingchooser.ui
    frmindentationmode.ui
    Search/dlgsearching.ui
    Search/frmsearchreplace.ui
    frmlinenumberchooser.ui
    Extensions/installextension.ui
)

set(notepad_RESOURCES
    resources.qrc
)

set(notepad_ICON
    ../../images/notepad.icns
)

set(notepad_TRANSLATIONS
    ../translations/notepad_de.ts
    ../translations/notepad_es.ts
    ../translations/notepad_fr.ts
    ../translations/notepad_hu.ts
    ../translations/notepad_it.ts
    ../translations/notepad_ja.ts
    ../translations/notepad_pl.ts
    ../translations/notepad_pt.ts
    ../translations/notepad_ru.ts
    ../translations/notepad_sl.ts
    ../translations/notepad_sv.ts
    ../translations/notepad_uk.ts
    ../translations/notepad_zh.ts
)

qt_add_translation(notepad_QMS ${notepad_TRANSLATIONS})

add_executable(notepad
    ${notepad_SRCS}
    ${notepad_HEADERS}
    ${notepad_FORMS}
    ${notepad_RESOURCES}
    ${notepad_ICON}
    ${notepad_QMS}
)

set_target_properties(notepad
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)

target_include_directories(notepad
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(notepad
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Svg
        Qt6::Widgets
        Qt6::PrintSupport
        Qt6::Network
        Qt6::WebEngineWidgets
        Qt6::WebChannel
        Qt6::WebSockets
        Qt6::DBus
        Qt6::Core5Compat
        PkgConfig::uchardet
)

set(GNU_COMPILE_OPTIONS
    -Werror=suggest-override
)

set(CLANG_COMPILE_OPTIONS
    ${GNU_COMPILE_OPTIONS}
    -Werror=inconsistent-missing-override
)

target_compile_options(notepad
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:${GNU_COMPILE_OPTIONS}>
        $<$<CXX_COMPILER_ID:Clang>:${CLANG_COMPILE_OPTIONS}>
)

set(notepad_SUPPORT_FILES ../../support_files)

install(TARGETS notepad)

install(CODE
    "execute_process(COMMAND rm -rf ${CMAKE_INSTALL_PREFIX}/share/notepad/editor)"
)

install(CODE
    "execute_process(COMMAND make DESTDIR=${CMAKE_INSTALL_PREFIX}/share/notepad/editor
                     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/editor)"
)

install(FILES ${notepad_SUPPORT_FILES}/shortcuts/notepad.desktop
    DESTINATION share/applications
)

install(DIRECTORY ${notepad_SUPPORT_FILES}/icons
    DESTINATION share
)

install(FILES ${notepad_SUPPORT_FILES}/notepad.appdata.xml
    DESTINATION share/metainfo
)

install(CODE
    "execute_process(COMMAND rm -rf ${CMAKE_INSTALL_PREFIX}/share/notepad/extension_tools)"
)

install(CODE
    "execute_process(COMMAND make DESTDIR=${CMAKE_INSTALL_PREFIX}/share/notepad/extension_tools
                     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/extension_tools)"
)
