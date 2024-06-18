get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

project(${CURRENT_DIR_NAME})

file(GLOB_RECURSE src src/*.h src/*.cpp src/*.hpp src/*.c)

include_directories(
	"src" 
	"src/AbstractCoreExtension" 
	"src/AbstractExtension" 
	"src/AbstractLogic"
	"src/LibraryCoreFactory"
	"src/LibraryFactory"
	"src/LibraryFactoryManager"
	"src/ExtensionManager"
	"src/RTTR")

add_executable(${PROJECT_NAME} ${src})
