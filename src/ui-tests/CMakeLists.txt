find_package(Qt6
    COMPONENTS
        Test
    REQUIRED
)

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
        Qt6::Test
)

target_link_libraries(ui-tests
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
)

set_target_properties(ui-tests
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)
