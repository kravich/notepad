find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)

find_path(QScintilla_INCLUDE_DIRS
    NAMES Qsci/qsciscintilla.h
    PATH_SUFFIXES qt6
)

find_library(QScintilla_LIBRARIES
    NAMES libqscintilla2_qt6.so.15
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QScintilla
    FOUND_VAR QScintilla_FOUND
    REQUIRED_VARS QScintilla_INCLUDE_DIRS QScintilla_LIBRARIES
)

if(QScintilla_FOUND)
    add_library(QScintilla::QScintilla SHARED IMPORTED)
    set_target_properties(QScintilla::QScintilla PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${QScintilla_INCLUDE_DIRS}
        INTERFACE_LINK_LIBRARIES ${QScintilla_LIBRARIES}
        IMPORTED_LOCATION ${QScintilla_LIBRARIES}
    )
    target_link_libraries(QScintilla::QScintilla INTERFACE Qt6::Widgets Qt6::PrintSupport)
endif()
