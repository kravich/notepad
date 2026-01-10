find_package(Qt5
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
    globals.cpp
    Extensions/Stubs/menuitemstub.cpp
    Extensions/installextension.cpp
    keygrabber.cpp
    Sessions/sessions.cpp
    Sessions/persistentcache.cpp
    nqqsettings.cpp
    nqqrun.cpp
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
    include/globals.h
    include/Extensions/Stubs/menuitemstub.h
    include/Extensions/installextension.h
    include/keygrabber.h
    include/Sessions/sessions.h
    include/Sessions/persistentcache.h
    include/nqqsettings.h
    include/nqqrun.h
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
        libs/qtpromise/include
)

target_link_libraries(notepad
    PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::PrintSupport
        Qt5::Network
        Qt5::WebEngineWidgets
        Qt5::WebChannel
        Qt5::WebSockets
        Qt5::DBus
        PkgConfig::uchardet
)
