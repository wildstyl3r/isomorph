find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(el_pintor el_pintor.cpp)


target_link_libraries(el_pintor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets QGrypho isomorph)
