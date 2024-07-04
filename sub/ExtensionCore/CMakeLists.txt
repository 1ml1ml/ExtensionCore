get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

project(${CURRENT_DIR_NAME})

file(GLOB_RECURSE src src/*.h src/*.cpp src/*.hpp src/*.c)

include_directories(
	"src" 
	"src/Global"
	"src/AbstractCoreExtension" 
	"src/AbstractExtension" 
	"src/AbstractCoreLogic"
	"src/AbstractLogic"
	"src/LibraryCoreFactory"
	"src/LibraryFactory"
	"src/LibraryFactoryManager"
	"src/ExtensionManager")

add_compile_definitions(${PROJECT_NAME}_PROJECT)

add_library(${PROJECT_NAME} SHARED ${src})

# 指定库的输出名称
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# 安装目标和头文件
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY src/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

