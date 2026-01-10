find_package(Qt5
    COMPONENTS
        Test
    REQUIRED
)

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

set(ui-tests_SRCS
    tst_notepadtest.cpp
)

add_executable(ui-tests
    ${ui-tests_SRCS}
)

target_include_directories(ui-tests
    PRIVATE
        ../ui
        ../ui/libs/qtpromise/include
)

target_link_libraries(ui-tests
    PRIVATE
        Qt5::Test
)

target_link_libraries(ui-tests
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
)

set_target_properties(ui-tests
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)
