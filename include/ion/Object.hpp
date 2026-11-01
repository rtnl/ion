#pragma once

#include "./_prelude.hpp"

namespace ion {

enum class ObjectKind {
  None,
  U8,
  U16,
  U32,
  U64,
  I8,
  I16,
  I32,
  I64,
  Rune,
  Array,
  List,
  Object,
};

using ObjectKindIndex = u8;

static u8 ObjectKind_index(const ObjectKind kind) {
  switch (kind) {
    case ObjectKind::None:   return 0x00;
    case ObjectKind::U8:     return 0x01;
    case ObjectKind::U16:    return 0x02;
    case ObjectKind::U32:    return 0x03;
    case ObjectKind::U64:    return 0x04;
    case ObjectKind::I8:     return 0x05;
    case ObjectKind::I16:    return 0x06;
    case ObjectKind::I32:    return 0x07;
    case ObjectKind::I64:    return 0x08;
    case ObjectKind::Rune:   return 0x09;
    case ObjectKind::Array:  return 0x0A;
    case ObjectKind::List:   return 0x0B;
    case ObjectKind::Object: return 0x0C;
  }
}

static KINETIC_NAMESPACE::Result<ObjectKind> from(const u8 index) {
  switch (index) {
    case 0x00: return {ObjectKind::None};
    case 0x01: return {ObjectKind::U8};
    case 0x02: return {ObjectKind::U16};
    case 0x03: return {ObjectKind::U32};
    case 0x04: return {ObjectKind::U64};
    case 0x05: return {ObjectKind::I8};
    case 0x06: return {ObjectKind::I16};
    case 0x07: return {ObjectKind::I32};
    case 0x08: return {ObjectKind::I64};
    case 0x09: return {ObjectKind::Rune};
    case 0x0A: return {ObjectKind::Array};
    case 0x0B: return {ObjectKind::List};
    case 0x0C: return {ObjectKind::Object};

    default:
      return {KINETIC_NAMESPACE::ErrorKind::ValueIncorrect, ""};
  }
}

}
