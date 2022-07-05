#子项目的配置信息文件,会被子项目的CMakeLists.txt所包含
#set target type
#app: application exe; lib: dynamci libary; mod: plugin module; 
set(targetType mod)

#set whether ui is needed
set(noUi OFF)

#add definition of Pre-compiled
#Example:  add_definitions(-Dmy_DEFINE) 
#means add definition my_DEFINE

#add_definitions(-Dmy_DEFINE) 

#add include directory 
#Example: include_directories("C:/my/include")
# means add include "C:/my/include"
include_directories("G:/TwinCAT/AdsApi/TcAdsDll/Include")
#include_directories("C:/my/include")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../demoMod")
#include_directories("${CMAKE_SOURCE_DIR}/common/match")

#add directory link libraries
#Example: link_directories("C:/my/lib")
#means add libary directory:  "C:/my/lib"

#link_directories("C:/my/lib")



#add link librarys
#Example: link_libraries("mylib")
#means add libary "mylib.lib"
link_libraries("G:/TwinCAT/AdsApi/TcAdsDll/x64/lib/TcAdsDll.lib")

#link_libraries("mylib")
