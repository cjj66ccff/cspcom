#���ҵ�ǰ·���µ����д����ļ�,���ݹ����
FILE(GLOB SRC_FILES "*.cpp" "*.c" "*.h")
#��ȡ����ǰ�ļ��е�����
STRING( REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} )
#message("${SRC_FILES}")

#message("noUi=${noUi}")
#���Ŀ¼�´��ڴ����ļ�
if(SRC_FILES)	
#���������ļ�
	include("subDirConfig.cmake")	
#�������ɵ���Ŀ����Ϊ��ǰĿ¼������
	set(targetName ${CURRENT_FOLDER})
#���Ӱ���Ŀ¼
	include_directories(${station_dir})
	include_directories(${qzcv_dir})
	include_directories(${station_include})
#�������ӿ�·������station3���ɵ�lib��·�������·����station3�µ�CMakeLists.txt�ж���
	link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
#��������ı���
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP")
#���Ӻ궨�嵽Դ�ļ��ı���֮�У��൱����Դ���У�����һ��#define PRJ_NAME targetName
	add_compile_definitions(PRJ_NAME="${targetName}")
	
#�ж��Ƿ����˲�����ui,������ܻ���subDirConfig.cmake��
	if (NOT noUi)
#���ñ���
		set(TS_FILES  "${targetName}_en.ts"  "${targetName}_zh.ts")
		set(qrcFile "${CMAKE_CURRENT_SOURCE_DIR}/${targetName}.qrc")
#���Ŀ¼��û��qrc�ļ����ʹ�customĿ¼��,���Ǹ�subDir.qrc.in�㵽��ǰ·����
#�������ֻ�����ļ�������������@targetName@�滻�ɵ�ǰ��targetName�����һ����������@copyonly
		if(NOT EXISTS ${qrcFile})
			configure_file("${subDirQrcPattern}" "${qrcFile}" @ONLY)
		endif()
#������ɵ���station3ģ��
		if(targetType STREQUAL  "mod")
#���ˣ�����д�ˣ����Ķ���
			set(infoFile "${CMAKE_CURRENT_SOURCE_DIR}/${targetName}.ini")
			if(NOT EXISTS ${infoFile})
				configure_file("${subDirInfoPattern}" "${infoFile}" @ONLY)
			endif()
		endif()
#�����ϣ����������䣬Ӧ��ҲҪ�ŵ�������Ǹ�if��������µ�,ver�汾�ļ�Ӧ���Ǹ�station3ģ�������ļ�һ����
		set(versionFile "${CMAKE_CURRENT_SOURCE_DIR}/${targetName}.ver")
		if(NOT EXISTS ${versionFile})
			configure_file("${subDirVersionPattern}" "${versionFile}" @ONLY)
		endif()
#���ú�������qt��ui_*.h,moc_*.cpp�ļ�
		qtGenerate(Generated_Files)
#����TS_FILES�ı���
		set(TS_FILES  "${targetName}_en.ts"  "${targetName}_zh.ts")
		set(trTargetName ${targetName}_tsfile)
#����һ���Զ����������Ŀ��ר���������������Ŀ�ĵ�ts�ļ���qm�ļ�
		add_custom_target(${trTargetName} COMMAND ${Qt5_LUPDATE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR} -ts 	${TS_FILES} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${Qt5_LRELEASE_EXECUTABLE} ${TS_FILES} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
#���ɷ����ļ�����Ŀ��ͳһ���ൽcustom/translateProject
		set_target_properties(${trTargetName} PROPERTIES FOLDER "custom/${parentDirName}/translate")
#�������ж���û��ts�ļ���qm�ļ�����Ϊʹ�����Զ�����Ŀ�����ɷ����ļ�����cmake��ʱ�򲻻��Զ����ã�������Ŀ�л���Ϊû��ts��qm�ļ�������
#������Ҫ��û�е�ʱ�������ɳ��ʼ��
		if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${targetName}_en.ts OR NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${targetName}_zh.ts)
			message(${targetName} " has not create translate files")
			execute_process(COMMAND ${qt_bin_path}/lupdate ${CMAKE_CURRENT_SOURCE_DIR} -ts ${TS_FILES} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
		endif()
		if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${targetName}_en.qm OR NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${targetName}_zh.qm)
			execute_process(COMMAND ${qt_bin_path}/lrelease ${TS_FILES} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
		endif()
		if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${targetName}_en.qm AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${targetName}_zh.qm)
			set(QM_FILES ${targetName}_en.qm ${targetName}_zh.qm)
		endif()
	endif()
	
#������ɵ���exe�ļ�
	if(targetType STREQUAL  "app")
#���ɵ�·����BinĿ¼��
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin")
		message(STATUS "Create Custom application: ${targetName}")
#����һ������exe����Ŀ������һ�������������Ŀ��Ҫ���ļ�
		add_executable(${targetName} ${SRC_FILES} ${Generated_Files} ${QM_FILES} ${TS_FILES}  ${versionFile}       "subDirConfig.cmake")
#����vs�ĵ��԰���·�����൱����vs����ҳ->����->������PATH=xxx
		set_target_properties(${targetName} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${OpenCV_bin_path};${qt_bin_path};$(PATH)")
		set_target_properties(${targetName} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "-develop")
#������ɵ��Ƕ�̬��
	elseif(targetType STREQUAL  "lib")
#���ɵ�·����BinĿ¼��
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin")
		message(STATUS "Create Custom dynamic libary: ${targetName}")
#����һ�����ɶ�̬�����Ŀ������һ�������������Ŀ��Ҫ���ļ�
		if (NOT noUi)
			add_library(${targetName} SHARED ${SRC_FILES} ${Generated_Files} ${QM_FILES}  ${TS_FILES} ${versionFile}   "subDirConfig.cmake")
		else()
			add_library(${targetName} SHARED ${SRC_FILES} "subDirConfig.cmake")
		endif()
	elseif(targetType STREQUAL "nocreate")
		return()
	else()
#�������ɵ�·����custom modules
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/custom modules")
		message(STATUS "Create Custom plugin module: ${targetName}")
#����һ�����ɶ�̬�����Ŀ������һ�������������Ŀ��Ҫ���ļ�
		add_library(${targetName} SHARED ${SRC_FILES} ${Generated_Files} ${QM_FILES} ${TS_FILES}  ${infoFile} ${versionFile}    "subDirConfig.cmake")
	endif()
#�������Ŀ���ൽvs��custom·����
	set_target_properties(${targetName} PROPERTIES FOLDER "custom/${parentDirName}")
	if (NOT noUi)
#���������ui�ļ����Ͱ��ļ����й���
		source_group("Generated Files" FILES ${Generated_Files})
		source_group("Form QRC Translate" FILES ${TS_FILES} ${QM_FILES}  ${infoFile} ${versionFile} )
	endif()
#�������ӿ�station��qzcv
	if(NOT noQZCV)
		target_link_libraries(${targetName} qzcv)
	endif()
	if(NOT noStation)
		target_link_libraries(${targetName} station)
	endif()
else()
#�����·����û��Դ�����ļ����Ͳ�����Ŀ¼,�ݹ����ͬ���Ĳ���
	message(STATUS "${CURRENT_FOLDER} is a parent dir without source files")
	if(parentDirName)
		set(parentDirName "${parentDirName}/${CURRENT_FOLDER}")
	else()
		set(parentDirName ${CURRENT_FOLDER})
	endif()
	SUBDIRLIST(children ${CMAKE_CURRENT_SOURCE_DIR})
	foreach(child ${children})
		if(${child} STREQUAL   ".git")
			continue()
		endif()
		configure_file("${subDirCmakeFilePath}"  "${CMAKE_CURRENT_SOURCE_DIR}/${child}/CMakeLists.txt" COPYONLY)
		set(subConfigFile "${CMAKE_CURRENT_SOURCE_DIR}/${child}/subDirConfig.cmake")
		if(NOT EXISTS ${subConfigFile})
			configure_file("${subDirConfigPattern}" "${subConfigFile}" COPYONLY)
		endif()
		add_subdirectory(${child})
	endforeach()
endif()


