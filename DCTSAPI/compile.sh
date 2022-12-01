#!/bin/bash 

CPP_GENERATED=../Proto
COMMON_PROTO_FILES=dcts.proto
NODE_PROTO_FILES=node/*.proto
MODULE_PROTO_FILES=node/module/*.proto
ZCZH_PROTO_FILES=node/zczh/*.proto
GPRC_CPP_PLUGIN=/usr/local/bin/grpc_cpp_plugin

protoc -I . --cpp_out=$CPP_GENERATED --grpc_out=$CPP_GENERATED --plugin=protoc-gen-grpc=$GPRC_CPP_PLUGIN \
$COMMON_PROTO_FILES \
$NODE_PROTO_FILES \
$MODULE_PROTO_FILES \
$ZCZH_PROTO_FILES
