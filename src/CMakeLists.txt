cmake_minimum_required(VERSION 3.5)
project(LineacPie LANGUAGES C)
find_package(gd)
add_executable(pie main.c myLibXML.c myLibPie.c main.c myLibXML.h myLibPie.h myLibPie3D.c myLibPie3D.h )
target_link_libraries (pie m gd)
install(TARGETS pie
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
