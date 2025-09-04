cmake_minimum_required(VERSION 3.22)
project(MegariunProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Pastas do projeto ----
set(SRC_DIR         "${CMAKE_SOURCE_DIR}/src")
set(TP_DIR          "${CMAKE_SOURCE_DIR}/third-party")

# GLFW (headers e lib pré-compilada)
set(GLFW_INC_DIR    "${TP_DIR}/include")            # contém include/GLFW/...
set(GLFW_LIB_FILE   "${TP_DIR}/libs/glfw3.lib")

# GLAD (loader gerado)
set(GLAD_INC_DIR    "${TP_DIR}/glad/include")
set(GLAD_SRC_FILE   "${TP_DIR}/glad/src/glad.c")

# Dear ImGui (branch docking)
set(IMGUI_DIR       "${TP_DIR}/imgui")
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

# ---- Fontes do seu app ----
set(PROJ_SOURCES
  "${SRC_DIR}/MegariunProject.cpp"
)
if (EXISTS "${GLAD_SRC_FILE}")
  list(APPEND PROJ_SOURCES "${GLAD_SRC_FILE}")
endif()

# ---- Executável ----
add_executable(${PROJECT_NAME} ${PROJ_SOURCES} "include/Shader.h")

# ---- Biblioteca ImGui (core + backends) ----
add_library(imgui STATIC
    ${IMGUI_CORE_SOURCES}
    ${IMGUI_BACKENDS_SOURCES}
 "include/Shader.h")

# Includes para ImGui (PUBLIC para propagar ao executável)
target_include_directories(imgui PUBLIC
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
    "${GLAD_INC_DIR}"     # necessário pelo backend OpenGL3 do ImGui
    "${GLFW_INC_DIR}"
)

# Informe ao backend OpenGL do ImGui que você usa GLAD como loader
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

# ---- Includes do executável ----
target_include_directories(${PROJECT_NAME} PRIVATE
  "${SRC_DIR}"
  "${GLFW_INC_DIR}"
  "${GLAD_INC_DIR}"
  "${IMGUI_DIR}"
  "${IMGUI_DIR}/backends"
)

# ---- Defines úteis do executável ----
target_compile_definitions(${PROJECT_NAME} PRIVATE
  GLFW_INCLUDE_NONE     # vamos incluir GLAD antes de GLFW
  NOMINMAX              # evita conflito com macros do Windows
)

# ---- Linkagem por plataforma ----
if (WIN32)
  # Windows: link com GLFW prebuilt, OpenGL32 e libs do sistema
  target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui
    "${GLFW_LIB_FILE}"
    opengl32 user32 gdi32 shell32
  )
else()
  # Linux/macOS (ajuste os nomes/caminhos conforme seu ambiente)
  find_package(OpenGL REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui
    "${TP_DIR}/libs/libglfw3.a"  # ajuste se necessário
    OpenGL::GL
  )
endif()

# ---- (Opcional) Diretório de trabalho no Visual Studio ----
set_property(TARGET ${PROJECT_NAME} PROPERTY
  VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# ---- (Opcional) Mostrar src/ como árvore no Solution Explorer ----
# source_group(TREE "${SRC_DIR}" FILES ${PROJ_SOURCES})

# ---- (Opcional) Saída do exe em bin/<Config> ----
# set_target_properties(${PROJECT_NAME} PROPERTIES
#   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
# )
