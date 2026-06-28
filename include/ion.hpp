#pragma once

#include <kinetic>
#include <cassert>

#define ION_IONSTREAM_IMPL_DIR_W(_TY)                                     \
  kinetic::R_Size write_##_TY(const _TY v) {                              \
    using ResultT = kinetic::R_Size;                                      \
    const auto index_r = write_index(IonObjectKindTranslate<_TY>::Index); \
    if (index_r.is_err()) return ResultT::err(index_r.get_error());       \
    return _io->write_all((const u8 *)(&v), sizeof(_TY));                 \
  }

#define ION_IONSTREAM_IMPL_DIR_R(_TY)                               \
  kinetic::Result<_TY> read_##_TY() {                               \
    using ResultT = kinetic::Result<_TY>;                           \
    const auto index_r = read_index();                              \
    if (index_r.is_err()) return ResultT::err(index_r.get_error()); \
    const auto index = index_r.unwrap();                            \
    assert(index == IonObjectKindTranslate<_TY>::Index);            \
    _TY v;                                                          \
    const auto io_r = _io->read_exact((u8 *)(&v), sizeof(_TY));     \
    if (io_r.is_err()) {                                            \
      return ResultT::err(io_r.get_error());                        \
    }                                                               \
    return ResultT::ok(v);                                          \
  }

#define ION_IONSTREAM_IMPL_DIR_RW(_TY) \
  ION_IONSTREAM_IMPL_DIR_R(_TY)        \
  ION_IONSTREAM_IMPL_DIR_W(_TY)

#define ION_IONOBJECTKINDTRANSLATE_IMPL(_TY)                               \
  template <> struct IonObjectKindTranslate<_TY> {                         \
    static constexpr IonObjectKind      Kind  = IonObjectKind::_TY;        \
    static constexpr IonObjectKindIndex Index = IonObjectKind_index(Kind); \
  };

namespace ion {

enum class IonObjectKind {
  None,
  u8,
  u16,
  u32,
  u64,
  i8,
  i16,
  i32,
  i64,
  Array,
  List,
};

using IonObjectKindIndex = u8;

static constexpr IonObjectKindIndex IonObjectKind_index(const IonObjectKind kind) {
  switch (kind) {
    case IonObjectKind::None:  return 0x01;
    case IonObjectKind::u8:    return 0x02;
    case IonObjectKind::u16:   return 0x03;
    case IonObjectKind::u32:   return 0x04;
    case IonObjectKind::u64:   return 0x05;
    case IonObjectKind::i8:    return 0x06;
    case IonObjectKind::i16:   return 0x07;
    case IonObjectKind::i32:   return 0x08;
    case IonObjectKind::i64:   return 0x09;
    case IonObjectKind::Array: return 0x0A;
    case IonObjectKind::List:  return 0x0B;

    default:
      return 0;
  }
}

template<class T, class = void>
struct IonObjectKindTranslate {
  static constexpr IonObjectKind      Kind  = IonObjectKind::None;
  static constexpr IonObjectKindIndex Index = IonObjectKind_index(Kind);
};

ION_IONOBJECTKINDTRANSLATE_IMPL(u8);
ION_IONOBJECTKINDTRANSLATE_IMPL(u16);
ION_IONOBJECTKINDTRANSLATE_IMPL(u32);
ION_IONOBJECTKINDTRANSLATE_IMPL(u64);
ION_IONOBJECTKINDTRANSLATE_IMPL(i8);
ION_IONOBJECTKINDTRANSLATE_IMPL(i16);
ION_IONOBJECTKINDTRANSLATE_IMPL(i32);
ION_IONOBJECTKINDTRANSLATE_IMPL(i64);

template <typename IO>
class IonStream {
  static_assert(std::is_base_of<kinetic::Reader, IO>::value, "IO must implement Reader");
  static_assert(std::is_base_of<kinetic::Writer, IO>::value, "IO must implement Writer");

private:
  std::shared_ptr<IO> _io;

public:
  explicit IonStream(const std::shared_ptr<IO> io) noexcept
    : _io(io)
  {}

  kinetic::Result<IonObjectKindIndex> read_index() {
    using ResultT = kinetic::Result<IonObjectKindIndex>;

    IonObjectKindIndex index = 0;

    const auto io_r = _io->read_exact((u8 *)(&index), sizeof(IonObjectKindIndex));
    if (io_r.is_err()) {
      return ResultT::err(io_r.get_error());
    }

    return ResultT::ok(index);
  }

  kinetic::R_Size write_index(const IonObjectKindIndex index) {
    return _io->write_all((const u8 *)(&index), sizeof(IonObjectKindIndex));
  }

  ION_IONSTREAM_IMPL_DIR_RW(u8);
  ION_IONSTREAM_IMPL_DIR_RW(u16);
  ION_IONSTREAM_IMPL_DIR_RW(u32);
  ION_IONSTREAM_IMPL_DIR_RW(u64);
  ION_IONSTREAM_IMPL_DIR_RW(i8);
  ION_IONSTREAM_IMPL_DIR_RW(i16);
  ION_IONSTREAM_IMPL_DIR_RW(i32);
  ION_IONSTREAM_IMPL_DIR_RW(i64);
};

}
