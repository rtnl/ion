#pragma once

#include "./_prelude.hpp"
#include "./Object.hpp"

#define ION_STREAM_IMPL_STREAM_CHECK() \
    if (_stream == nullptr) { return {ErrStreamNull}; }

#define ION_STREAM_IMPL_W_OBJECT_KIND(OBJECT_KIND) \
    if (_flag_array_w) {                           \
      if (_kind_array_w != OBJECT_KIND) {          \
        return {ErrArrayKind};                     \
      }                                            \
    } else {                                       \
      const auto r = write_kind(OBJECT_KIND);      \
      if (r.is_err()) {                            \
        return r;                                  \
      }                                            \
    }

#define ION_STREAM_IMPL_W_OBJECT_LINEAR(TYPE, OBJECT_KIND)       \
  ResultUnit KINETIC_CONCAT(write_, TYPE) (const TYPE value) {   \
    ION_STREAM_IMPL_STREAM_CHECK()                               \
    ION_STREAM_IMPL_W_OBJECT_KIND(OBJECT_KIND);                  \
    const auto r = _stream->KINETIC_CONCAT(write_, TYPE)(value); \
    if (r.is_err()) {                                            \
      return r;                                                  \
    }                                                            \
    return {{}};                                                 \
  }

#define ION_STREAM_IMPL_R_OBJECT_KIND(OBJECT_KIND) \
    if (_flag_array_r) {                           \
      if (_kind_array_r != OBJECT_KIND) {          \
        return {ErrArrayKind};                     \
      }                                            \
    } else {                                       \
      const auto kind_r = read_kind();             \
      if (kind_r.is_err()) {                       \
        return {kind_r.get_error()};               \
      }                                            \
      if (kind_r.get() != OBJECT_KIND) {           \
        return {ErrObjectKind};                    \
      }                                            \
    }

#define ION_STREAM_IMPL_R_OBJECT_LINEAR(TYPE, OBJECT_KIND)         \
  KINETIC_NAMESPACE::Result<TYPE> KINETIC_CONCAT(read_, TYPE) () { \
    ION_STREAM_IMPL_STREAM_CHECK()                                 \
    ION_STREAM_IMPL_R_OBJECT_KIND(OBJECT_KIND)                     \
    const auto value_r = _stream->KINETIC_CONCAT(read_, TYPE)();   \
    if (value_r.is_err()) {                                        \
      return {value_r.get_error()};                                \
    }                                                              \
    return {value_r.get()};                                        \
  }

namespace ion {

const KINETIC_NAMESPACE::Error ErrStreamNull{
  KINETIC_NAMESPACE::ErrorKind::ValueNone,
  "stream is null"};

const KINETIC_NAMESPACE::Error ErrArrayKind{
  KINETIC_NAMESPACE::ErrorKind::ValueIncorrect,
  "value kind does not match array kind"};

const KINETIC_NAMESPACE::Error ErrObjectKind{
  KINETIC_NAMESPACE::ErrorKind::ValueIncorrect,
  "object kind does not match"};

const KINETIC_NAMESPACE::Error ErrNestingUnimplemented{
  KINETIC_NAMESPACE::ErrorKind::SourceTodo,
  "nested array not implemented"};

const KINETIC_NAMESPACE::Error ErrArrayNotOpen{
  KINETIC_NAMESPACE::ErrorKind::ValueIncorrect,
  "array is not open"};

class Stream {
  using Unit       = KINETIC_NAMESPACE::Unit;
  using Error      = KINETIC_NAMESPACE::Error;
  using ErrorKind  = KINETIC_NAMESPACE::ErrorKind;
  using ResultUnit = KINETIC_NAMESPACE::Result<Unit>;
  using ResultSize = KINETIC_NAMESPACE::Result<usize>;

private:
  using ByteOrder = KINETIC_NAMESPACE::ByteOrder;
  using StreamT   = KINETIC_NAMESPACE::BinaryStream<ByteOrder::BigEndian>;

  std::shared_ptr<StreamT> _stream;

  bool       _flag_array_w;
  ObjectKind _kind_array_w;
  bool       _flag_array_r;
  ObjectKind _kind_array_r;

public:
  Stream(const std::shared_ptr<StreamT> & stream)
    : _stream(stream)
    , _flag_array_w(false)
    , _kind_array_w(ObjectKind::None)
    , _flag_array_r(false)
    , _kind_array_r(ObjectKind::None)
  {}

  ~Stream() = default;

private:
  ResultUnit write_kind(const ObjectKind kind) {
    ION_STREAM_IMPL_STREAM_CHECK();
    return _stream->write_u8(ObjectKind_index(kind));
  }

  KINETIC_NAMESPACE::Result<ObjectKind> read_kind() {
    ION_STREAM_IMPL_STREAM_CHECK();
    const auto r = _stream->read_u8();
    if (r.is_err()) {
      return {r.get_error()};
    }
    return from(r.get());
  }

public:
  ResultUnit write_none() {
    ION_STREAM_IMPL_W_OBJECT_KIND(ObjectKind::None);
    return {{}};
  }

  ION_STREAM_IMPL_W_OBJECT_LINEAR(u8,   ObjectKind::U8);
  ION_STREAM_IMPL_W_OBJECT_LINEAR(u16,  ObjectKind::U16);
  ION_STREAM_IMPL_W_OBJECT_LINEAR(u32,  ObjectKind::U32);
  ION_STREAM_IMPL_W_OBJECT_LINEAR(u64,  ObjectKind::U64);
  ION_STREAM_IMPL_W_OBJECT_LINEAR(i8,   ObjectKind::I8);
  ION_STREAM_IMPL_W_OBJECT_LINEAR(i16,  ObjectKind::I16);
  ION_STREAM_IMPL_W_OBJECT_LINEAR(i32,  ObjectKind::I32);
  ION_STREAM_IMPL_W_OBJECT_LINEAR(i64,  ObjectKind::I64);
  ION_STREAM_IMPL_W_OBJECT_LINEAR(rune, ObjectKind::Rune);

  ResultUnit write_array_open(const ObjectKind kind, const u32 length) {
    ION_STREAM_IMPL_STREAM_CHECK();

    if (_flag_array_w) {
      return {ErrNestingUnimplemented};
    }

    {
      const auto r = write_kind(ObjectKind::Array);
      if (r.is_err()) {
        return r;
      }
    }

    {
      const auto r = write_kind(kind);
      if (r.is_err()) {
        return r;
      }
    }

    {
      const auto r = _stream->write_u32(length);
      if (r.is_err()) {
        return r;
      }
    }

    _flag_array_w = true;
    _kind_array_w = kind;

    return {{}};
  }

  ResultUnit write_array_close() {
    ION_STREAM_IMPL_STREAM_CHECK();

    if (!_flag_array_w) {
      return {ErrArrayNotOpen};
    }

    _flag_array_w = false;
    _kind_array_w = ObjectKind::None;

    return {{}};
  }

  ResultUnit read_none() {
    ION_STREAM_IMPL_STREAM_CHECK();
    ION_STREAM_IMPL_R_OBJECT_KIND(ObjectKind::None);
    return {{}};
  }

  ION_STREAM_IMPL_R_OBJECT_LINEAR(u8, ObjectKind::U8);
  ION_STREAM_IMPL_R_OBJECT_LINEAR(u16, ObjectKind::U16);
  ION_STREAM_IMPL_R_OBJECT_LINEAR(u32, ObjectKind::U32);
  ION_STREAM_IMPL_R_OBJECT_LINEAR(u64, ObjectKind::U64);
  ION_STREAM_IMPL_R_OBJECT_LINEAR(i8, ObjectKind::I8);
  ION_STREAM_IMPL_R_OBJECT_LINEAR(i16, ObjectKind::I16);
  ION_STREAM_IMPL_R_OBJECT_LINEAR(i32, ObjectKind::I32);
  ION_STREAM_IMPL_R_OBJECT_LINEAR(i64, ObjectKind::I64);
  ION_STREAM_IMPL_R_OBJECT_LINEAR(rune, ObjectKind::Rune);

  ResultSize read_array_open(const ObjectKind kind) {
    ION_STREAM_IMPL_STREAM_CHECK();

    if (_flag_array_r) {
      return {ErrNestingUnimplemented};
    }

    {
      const auto kind_r = read_kind();
      if (kind_r.is_err()) {
        return {kind_r.get_error()};
      }

      if (kind_r.get() != ObjectKind::Array) {
        return {ErrObjectKind};
      }
    }

    {
      const auto kind_r = read_kind();
      if (kind_r.is_err()) {
        return {kind_r.get_error()};
      }

      if (kind_r.get() != kind) {
        return {ErrArrayKind};
      }
    }

    const auto length_r = _stream->read_u32();
    if (length_r.is_err()) {
      return {length_r.get_error()};
    }

    _flag_array_r = true;
    _kind_array_r = kind;

    return {length_r.get()};
  }

  ResultUnit read_array_close() {
    if (!_flag_array_r) {
      return {ErrArrayNotOpen};
    }

    _flag_array_r = false;
    _kind_array_r = ObjectKind::None;

    return {{}};
  }
};

}
