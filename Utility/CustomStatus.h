/**
 * @file CustomStatus.h
 * @brief 自定义grpc status
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2020-09-08
 * 
 * @copyright Copyright (c) 2020  中国电子科技集团公司第四十一研究所
 * 
 */

#pragma once
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/security/server_credentials.h>

const grpc::Status NODE_NOT_FOUND(grpc::NOT_FOUND, "node not found");
const grpc::Status TASK_NOT_FOUND(grpc::NOT_FOUND, "task not found");
const grpc::Status API_NOT_IMPLEMENTED(grpc::UNIMPLEMENTED, "this API is unimplemented");
const grpc::Status NO_AVAILABLE_DEVICE(grpc::RESOURCE_EXHAUSTED, "no available device");
const grpc::Status INVALID_PARAM(grpc::INVALID_ARGUMENT, "invalid argument");
const grpc::Status USER_ABORT(grpc::ABORTED, "user aborted");
const grpc::Status REQUEST_CANCEL(grpc::INTERNAL, "task can not send request");
const grpc::Status NO_RESOURCE(grpc::NOT_FOUND, "resource not found");
const grpc::Status UNAVAILABLE(grpc::UNAVAILABLE, "unavailable");