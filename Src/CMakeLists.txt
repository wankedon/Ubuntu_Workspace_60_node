##############aux_source_directory(. SRC_LIST)

include_directories(./)
include_directories(${PROJECT_SOURCE_DIR}/Spectrum)
include_directories(${PROJECT_SOURCE_DIR}/SignalDetection)
include_directories(${PROJECT_SOURCE_DIR}/TaskManager)
include_directories(${PROJECT_SOURCE_DIR}/Utility)
include_directories(${PROJECT_SOURCE_DIR}/PchHeader)
include_directories(${PROJECT_SOURCE_DIR}/ConfigLoader)
include_directories(${PROJECT_SOURCE_DIR}/DeviceManager)
include_directories(${PROJECT_SOURCE_DIR}/Proto)
include_directories(${PROJECT_SOURCE_DIR}/DistComm)

file(GLOB_RECURSE SRC_LIST "*.c*")
file(GLOB_RECURSE DEVICE_LIST ${PROJECT_SOURCE_DIR}/DeviceManager/*.cpp)
file(GLOB_RECURSE CONFIG_LIST ${PROJECT_SOURCE_DIR}/ConfigLoader/*.cpp)
file(GLOB_RECURSE DISTCOMM_LIST ${PROJECT_SOURCE_DIR}/DistComm/*.cpp)
file(GLOB_RECURSE HEADER_LIST ${PROJECT_SOURCE_DIR}/PchHeader/*.cpp)
file(GLOB_RECURSE PROTO_LIST ${PROJECT_SOURCE_DIR}/Proto/*.cc)
file(GLOB_RECURSE SIGNAL_LIST ${PROJECT_SOURCE_DIR}/SignalDetection/*.cpp)
file(GLOB_RECURSE SPECTRUM_LIST ${PROJECT_SOURCE_DIR}/Spectrum/*.cpp)
file(GLOB_RECURSE TASK_LIST ${PROJECT_SOURCE_DIR}/TaskManager/*.cpp)
file(GLOB_RECURSE UTILITY_LIST ${PROJECT_SOURCE_DIR}/Utility/*.cpp)

file(GLOB LIB_CZMQ_LIST "/home/forlinx/Workspace/czmq/installed/lib/lib*.a*")#czmq
file(GLOB LIB_LIST "/home/forlinx/Workspace/vcpkg/installed/arm64-linux/lib/lib*.a*")#vcpkg

add_executable(DCTSNode ${SRC_LIST} ${PROTO_LIST} ${DISTCOMM_LIST} ${UTILITY_LIST} ${HEADER_LIST} ${DEVICE_LIST} ${CONFIG_LIST} ${SIGNAL_LIST}
${SPECTRUM_LIST} ${TASK_LIST})
target_link_libraries(DCTSNode detector ModulationRecognition TransmissionSystemRecognition AnalogDemod ${LIB_CZMQ_LIST} ${LIB_LIST} ${_REFLECTION}  ${_GRPC_GRPCPP}  ${_PROTOBUF_LIBPROTOBUF} dl) 
