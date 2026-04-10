# CMakeList.txt : CMake project for SagePaint, include source and define
# project specific logic here.
#

# Add source to this project's executable.

#add this               WIN32     to avoid console window
add_executable (SagePaint "SagePaint.cpp" "SagePaint.h" "../include/debug.h" "app/MainApp.h" "app/MainApp.cpp"     "app/graphics/Common.h"     "app/graphics/GameObject.h" "app/graphics/drawables/Model.h" "app/graphics/drawables/Model.cpp" "app/graphics/GameObject.cpp" "app/graphics/Common.cpp" "app/graphics/drawables/CanvasModel.h" "app/graphics/drawables/CanvasModel.cpp" "app/graphics/CanvasObject.h" "app/graphics/CanvasObject.cpp"    "app/file/Image.cpp" "app/file/Image.h" "app/input/InputFunctions.h" "app/input/InputFunctions.cpp" "app/input/InputManager.h" "app/input/InputManager.cpp" "app/input/InputMap.cpp" "app/input/InputMap.h" "app/file/FileManager.h" "app/file/FileManager.cpp"  "app/CanvasManager.cpp" "app/CanvasManager.h" "app/tools/PencilTool.cpp" "app/tools/PencilTool.h"   "app/tools/ToolManager.cpp" "app/tools/ToolManager.h" "app/tools/LineTool.cpp" "app/tools/LineTool.h" "app/file/Layer.h" "app/file/Layer.cpp" "app/tools/FillTool.h" "app/tools/FillTool.cpp" "app/tools/ShapeTool.h" "app/tools/ShapeTool.cpp" "app/ProjectManager.cpp" "app/ProjectManager.h" "app/tools/SelectTool.h" "app/tools/SelectTool.cpp" "app/ui/UIManager.h" "app/ui/UIManager.cpp") #console
 

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
  


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        GLEW::GLEW
        glfw
        glm::glm
        imgui::imgui
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SagePaint PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(
    TARGET SagePaint
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/SagePaint/shaders
            $<TARGET_FILE_DIR:SagePaint>/shaders
)

add_custom_command(
    TARGET SagePaint
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/SagePaint/icons
            $<TARGET_FILE_DIR:SagePaint>/icons
)
