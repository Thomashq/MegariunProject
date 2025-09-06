cmake_minimum_required(VERSION 3.22)
project(MegariunProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Pastas ----
set(SRC_DIR    "${CMAKE_SOURCE_DIR}/src")
set(INC_DIR    "${CMAKE_SOURCE_DIR}/include")
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
set(TP_DIR     "${CMAKE_SOURCE_DIR}/third-party")

# GLFW (headers + lib prebuilt no Windows)
set(GLFW_INC_DIR  "${TP_DIR}/include")           # contém include/GLFW/...
set(GLFW_LIB_FILE "${TP_DIR}/libs/glfw3.lib")    # .lib estática do GLFW (Windows)

# GLAD
set(GLAD_INC_DIR  "${TP_DIR}/glad/include")
set(GLAD_SRC_FILE "${TP_DIR}/glad/src/glad.c")

# Dear ImGui (branch docking)
set(IMGUI_DIR "${TP_DIR}/imgui")
set(IMGUI_CORE_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
)
set(IMGUI_BACKENDS_SOURCES
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

# GLM (header-only via submodule)
set(GLM_INC_DIR "${TP_DIR}/glm")
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${GLM_INC_DIR}")

# ---- Fontes do app (varre recursivo) ----
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
  "${SRC_DIR}/*.c"  "${SRC_DIR}/*.cc"  "${SRC_DIR}/*.cpp"
)

# (Opcional) listar headers no Solution Explorer
file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS
  "${INC_DIR}/*.h"  "${INC_DIR}/*.hpp"
)

# Exclui o arquivo legado (mantém no repo mas não compila)
if (EXISTS "${SRC_DIR}/MegariunProject.cpp")
  list(FILTER APP_SOURCES EXCLUDE REGEX ".*/MegariunProject\\.cpp$")
endif()

# GLAD (anexa se existir)
if (EXISTS "${GLAD_SRC_FILE}")
  list(APPEND APP_SOURCES "${GLAD_SRC_FILE}")
endif()

# ---- Executável ----
add_executable(${PROJECT_NAME} ${APP_SOURCES} ${APP_HEADERS} "include/Canvas.h")

# ---- Biblioteca ImGui ----
add_library(imgui STATIC
  ${IMGUI_CORE_SOURCES}
  ${IMGUI_BACKENDS_SOURCES}
 "include/Canvas.h")
target_include_directories(imgui PUBLIC
  "${IMGUI_DIR}"
  "${IMGUI_DIR}/backends"
  "${GLAD_INC_DIR}"
  "${GLFW_INC_DIR}"
)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

option(IMGUI_WITH_DEMO "Build Dear ImGui demo window" OFF)
if (IMGUI_WITH_DEMO)
  target_sources(imgui PRIVATE "${IMGUI_DIR}/imgui_demo.cpp")
endif()

# ---- Includes do executável (AQUI resolve o 'cannot open Globals.h') ----
target_include_directories(${PROJECT_NAME} PRIVATE
  "${INC_DIR}"
  "${SRC_DIR}"          # se você tiver headers internos em src/
  "${GLFW_INC_DIR}"
  "${GLAD_INC_DIR}"
  "${IMGUI_DIR}"
  "${IMGUI_DIR}/backends"
)

# ---- Defines úteis ----
file(TO_CMAKE_PATH "${ASSETS_DIR}/shaders" SHADERS_DIR_NORM)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  GLFW_INCLUDE_NONE
  NOMINMAX
  SHADERS_DIR="${SHADERS_DIR_NORM}"
)

# ---- Linkagem ----
if (WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui
    glm
    "${GLFW_LIB_FILE}"
    opengl32 user32 gdi32 shell32
  )
else()
  find_package(OpenGL REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui
    glm
    "${TP_DIR}/libs/libglfw3.a"   # ajuste conforme seu ambiente
    OpenGL::GL
  )
endif()

# ---- Copiar shaders p/ pasta do executável (Debug/Release) ----
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${ASSETS_DIR}/shaders"
          "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/shaders"
  COMMENT "Copiando shaders para a pasta de saída..."
)

# ---- Qualidade de vida (VS) ----
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${APP_SOURCES} ${APP_HEADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
