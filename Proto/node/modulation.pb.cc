// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: node/modulation.proto

#include "node/modulation.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace zb {
namespace dcts {
namespace node {
namespace modulation {
constexpr RecognizeResult::RecognizeResult(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : signal_type_(0)

  , mod_type_(0)

  , center_freq_(0)
  , band_width_(0){}
struct RecognizeResultDefaultTypeInternal {
  constexpr RecognizeResultDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~RecognizeResultDefaultTypeInternal() {}
  union {
    RecognizeResult _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT RecognizeResultDefaultTypeInternal _RecognizeResult_default_instance_;
}  // namespace modulation
}  // namespace node
}  // namespace dcts
}  // namespace zb
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_node_2fmodulation_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_node_2fmodulation_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_node_2fmodulation_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_node_2fmodulation_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::zb::dcts::node::modulation::RecognizeResult, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::zb::dcts::node::modulation::RecognizeResult, signal_type_),
  PROTOBUF_FIELD_OFFSET(::zb::dcts::node::modulation::RecognizeResult, mod_type_),
  PROTOBUF_FIELD_OFFSET(::zb::dcts::node::modulation::RecognizeResult, center_freq_),
  PROTOBUF_FIELD_OFFSET(::zb::dcts::node::modulation::RecognizeResult, band_width_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::zb::dcts::node::modulation::RecognizeResult)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::zb::dcts::node::modulation::_RecognizeResult_default_instance_),
};

const char descriptor_table_protodef_node_2fmodulation_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\025node/modulation.proto\022\027zb.dcts.node.mo"
  "dulation\"\245\001\n\017RecognizeResult\0225\n\013signal_t"
  "ype\030\001 \001(\0162 .zb.dcts.node.modulation.SigT"
  "ype\0222\n\010mod_type\030\002 \001(\0162 .zb.dcts.node.mod"
  "ulation.ModType\022\023\n\013center_freq\030\003 \001(\001\022\022\n\n"
  "band_width\030\004 \001(\001*\227\003\n\007ModType\022\013\n\007UNKNOWM\020"
  "\000\022\006\n\002AM\020\001\022\010\n\004AMSC\020\002\022\010\n\004AMTC\020\003\022\007\n\003SSB\020\004\022\007"
  "\n\003DSB\020\005\022\007\n\003VSB\020\006\022\007\n\003LSB\020\007\022\007\n\003USB\020\010\022\006\n\002FM"
  "\020\013\022\007\n\003ASK\020\025\022\010\n\004ASK2\020\026\022\010\n\004ASK4\020\027\022\010\n\004ASK8\020"
  "\030\022\t\n\005ASK16\020\031\022\007\n\003FSK\020\037\022\010\n\004FSK2\020 \022\010\n\004FSK4\020"
  "!\022\010\n\004FSK8\020\"\022\t\n\005FSK16\020#\022\007\n\003MSK\020$\022\007\n\003PSK\020)"
  "\022\010\n\004BPSK\020*\022\t\n\005OQPSK\020+\022\010\n\004QPSK\020,\022\013\n\007Pi4QP"
  "SK\020-\022\010\n\004PSK8\020.\022\t\n\005PSK16\020/\022\t\n\005D8PSK\0200\022\007\n\003"
  "QAM\0203\022\t\n\005QAM16\0204\022\t\n\005QAM32\0205\022\t\n\005QAM64\0206\022\n"
  "\n\006QAM128\0207\022\n\n\006QAM256\0208\022\n\n\006QAM512\0209\022\013\n\007QA"
  "M1024\020:\022\006\n\002CW\020F\022\t\n\005Noise\020P*C\n\007SigType\022\r\n"
  "\tUN_DETECT\020\000\022\017\n\013NARROW_BAND\020\001\022\r\n\tWIDE_BA"
  "ND\020\002\022\t\n\005PULSE\020\003b\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_node_2fmodulation_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_node_2fmodulation_2eproto = {
  false, false, 703, descriptor_table_protodef_node_2fmodulation_2eproto, "node/modulation.proto", 
  &descriptor_table_node_2fmodulation_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_node_2fmodulation_2eproto::offsets,
  file_level_metadata_node_2fmodulation_2eproto, file_level_enum_descriptors_node_2fmodulation_2eproto, file_level_service_descriptors_node_2fmodulation_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK ::PROTOBUF_NAMESPACE_ID::Metadata
descriptor_table_node_2fmodulation_2eproto_metadata_getter(int index) {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_node_2fmodulation_2eproto);
  return descriptor_table_node_2fmodulation_2eproto.file_level_metadata[index];
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_node_2fmodulation_2eproto(&descriptor_table_node_2fmodulation_2eproto);
namespace zb {
namespace dcts {
namespace node {
namespace modulation {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ModType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_node_2fmodulation_2eproto);
  return file_level_enum_descriptors_node_2fmodulation_2eproto[0];
}
bool ModType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 70:
    case 80:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* SigType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_node_2fmodulation_2eproto);
  return file_level_enum_descriptors_node_2fmodulation_2eproto[1];
}
bool SigType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}


// ===================================================================

class RecognizeResult::_Internal {
 public:
};

RecognizeResult::RecognizeResult(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:zb.dcts.node.modulation.RecognizeResult)
}
RecognizeResult::RecognizeResult(const RecognizeResult& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&signal_type_, &from.signal_type_,
    static_cast<size_t>(reinterpret_cast<char*>(&band_width_) -
    reinterpret_cast<char*>(&signal_type_)) + sizeof(band_width_));
  // @@protoc_insertion_point(copy_constructor:zb.dcts.node.modulation.RecognizeResult)
}

void RecognizeResult::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&signal_type_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&band_width_) -
    reinterpret_cast<char*>(&signal_type_)) + sizeof(band_width_));
}

RecognizeResult::~RecognizeResult() {
  // @@protoc_insertion_point(destructor:zb.dcts.node.modulation.RecognizeResult)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void RecognizeResult::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void RecognizeResult::ArenaDtor(void* object) {
  RecognizeResult* _this = reinterpret_cast< RecognizeResult* >(object);
  (void)_this;
}
void RecognizeResult::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void RecognizeResult::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void RecognizeResult::Clear() {
// @@protoc_insertion_point(message_clear_start:zb.dcts.node.modulation.RecognizeResult)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&signal_type_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&band_width_) -
      reinterpret_cast<char*>(&signal_type_)) + sizeof(band_width_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* RecognizeResult::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // .zb.dcts.node.modulation.SigType signal_type = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_signal_type(static_cast<::zb::dcts::node::modulation::SigType>(val));
        } else goto handle_unusual;
        continue;
      // .zb.dcts.node.modulation.ModType mod_type = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_mod_type(static_cast<::zb::dcts::node::modulation::ModType>(val));
        } else goto handle_unusual;
        continue;
      // double center_freq = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 25)) {
          center_freq_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // double band_width = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 33)) {
          band_width_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* RecognizeResult::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:zb.dcts.node.modulation.RecognizeResult)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .zb.dcts.node.modulation.SigType signal_type = 1;
  if (this->signal_type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->_internal_signal_type(), target);
  }

  // .zb.dcts.node.modulation.ModType mod_type = 2;
  if (this->mod_type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      2, this->_internal_mod_type(), target);
  }

  // double center_freq = 3;
  if (!(this->center_freq() <= 0 && this->center_freq() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(3, this->_internal_center_freq(), target);
  }

  // double band_width = 4;
  if (!(this->band_width() <= 0 && this->band_width() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(4, this->_internal_band_width(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:zb.dcts.node.modulation.RecognizeResult)
  return target;
}

size_t RecognizeResult::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:zb.dcts.node.modulation.RecognizeResult)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .zb.dcts.node.modulation.SigType signal_type = 1;
  if (this->signal_type() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_signal_type());
  }

  // .zb.dcts.node.modulation.ModType mod_type = 2;
  if (this->mod_type() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_mod_type());
  }

  // double center_freq = 3;
  if (!(this->center_freq() <= 0 && this->center_freq() >= 0)) {
    total_size += 1 + 8;
  }

  // double band_width = 4;
  if (!(this->band_width() <= 0 && this->band_width() >= 0)) {
    total_size += 1 + 8;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void RecognizeResult::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:zb.dcts.node.modulation.RecognizeResult)
  GOOGLE_DCHECK_NE(&from, this);
  const RecognizeResult* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<RecognizeResult>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:zb.dcts.node.modulation.RecognizeResult)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:zb.dcts.node.modulation.RecognizeResult)
    MergeFrom(*source);
  }
}

void RecognizeResult::MergeFrom(const RecognizeResult& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:zb.dcts.node.modulation.RecognizeResult)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.signal_type() != 0) {
    _internal_set_signal_type(from._internal_signal_type());
  }
  if (from.mod_type() != 0) {
    _internal_set_mod_type(from._internal_mod_type());
  }
  if (!(from.center_freq() <= 0 && from.center_freq() >= 0)) {
    _internal_set_center_freq(from._internal_center_freq());
  }
  if (!(from.band_width() <= 0 && from.band_width() >= 0)) {
    _internal_set_band_width(from._internal_band_width());
  }
}

void RecognizeResult::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:zb.dcts.node.modulation.RecognizeResult)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void RecognizeResult::CopyFrom(const RecognizeResult& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:zb.dcts.node.modulation.RecognizeResult)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RecognizeResult::IsInitialized() const {
  return true;
}

void RecognizeResult::InternalSwap(RecognizeResult* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(RecognizeResult, band_width_)
      + sizeof(RecognizeResult::band_width_)
      - PROTOBUF_FIELD_OFFSET(RecognizeResult, signal_type_)>(
          reinterpret_cast<char*>(&signal_type_),
          reinterpret_cast<char*>(&other->signal_type_));
}

::PROTOBUF_NAMESPACE_ID::Metadata RecognizeResult::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace modulation
}  // namespace node
}  // namespace dcts
}  // namespace zb
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::zb::dcts::node::modulation::RecognizeResult* Arena::CreateMaybeMessage< ::zb::dcts::node::modulation::RecognizeResult >(Arena* arena) {
  return Arena::CreateMessageInternal< ::zb::dcts::node::modulation::RecognizeResult >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>