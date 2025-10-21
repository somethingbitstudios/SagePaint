# CMakeList.txt : CMake project for SagePaint, include source and define
# project specific logic here.
#

# Add source to this project's executable.


#add_executable (SagePaint WIN32 "SagePaint.cpp" "SagePaint.h") #windows no console option
add_executable (SagePaint "SagePaint.cpp" "SagePaint.h" "../include/Debug.h" "app/MainApp.h" "app/MainApp.cpp") #console


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        GLEW::GLEW
        glfw
        glm::glm
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SagePaint PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
