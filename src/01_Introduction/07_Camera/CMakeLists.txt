#默认使用目录名作为项目名
GET_DIR_NAME(DIRNAME)
set(TARGET_NAME ${DIRNAME})
#多个源文件用 [空格] 分隔
#如：set(STR_TARGET_SOURCES "main.cpp src_2.cpp")
file(GLOB ALL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.vs"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.gs"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.fs"
)
set(STR_TARGET_SOURCES "")
foreach(SOURCE ${ALL_SOURCES})
	set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${SOURCE}")
endforeach(SOURCE ${ALL_SOURCES})
#set(STR_TARGET_SOURCES "${DIRNAME}.cpp ${DIRNAME}.fs ${DIRNAME}.vs")
#多个库文件用 [空格] 分隔，如果为空，就输入[一个空格]
#如：set(STR_TARGET_LIBS "lib1.lib lib2.lib")
set(STR_TARGET_LIBS "glfw3.lib glad Image Operation EventManager Glfw Shader Camera opengl32.lib")

SETUP_PROJECT(${MODE} ${TARGET_NAME} ${STR_TARGET_SOURCES} ${STR_TARGET_LIBS})

# copy shader files to build directory
file(GLOB SHADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.vs"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.fs"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.gs"
)

foreach(SHADER ${SHADERS})
	string(REGEX MATCH "([^/]*)$" TMP ${SHADER})
	add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} "${CMAKE_SOURCE_DIR}/data/shaders/${FOLDER_NAME}/${DIRNAME}/${TMP}")
endforeach(SHADER)