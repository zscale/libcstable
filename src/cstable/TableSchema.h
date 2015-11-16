/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stx/stdtypes.h>
#include <stx/exception.h>
#include <stx/autoref.h>
#include <stx/json/json.h>
#include <stx/protobuf/MessageSchema.h>

namespace stx {
namespace cstable {

enum class ColumnType : uint8_t {
  SUBRECORD = 0,
  BOOLEAN = 1,
  UNSIGNED_INT = 2,
  SIGNED_INT = 3,
  STRING = 4,
  FLOAT = 5,
  DATETIME = 6
};

enum class ColumnEncoding : uint8_t {
  BOOLEAN_BITPACKED = 1,
  UINT32_BITPACKED = 10,
  UINT32_PLAIN = 11,
  UINT64_PLAIN = 12,
  UINT64_LEB128 = 13,
  FLOAT_IEEE754 = 14,
  STRING_PLAIN = 100
};

class TableSchema : public RefCounted {
public:

  struct Column {
    String name;
    ColumnType type;
    ColumnEncoding encoding;
    uint64_t type_size;
    bool repeated;
    bool optional;
    ScopedPtr<TableSchema> subschema;
  };

  TableSchema();
  TableSchema(const TableSchema& other);

  void addBool(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::BOOLEAN_BITPACKED);

  void addBoolArray(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::BOOLEAN_BITPACKED);

  void addUnsignedInteger(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::UINT64_LEB128,
      uint64_t max_value = 0);

  void addUnsignedIntegerArray(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::UINT64_LEB128,
      uint64_t max_value = 0);

  void addSignedInteger(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::UINT64_LEB128,
      uint64_t max_value = 0);

  void addSignedIntegerArray(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::UINT64_LEB128,
      uint64_t max_value = 0);

  void addFloat(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::FLOAT_IEEE754);

  void addFloatArray(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::FLOAT_IEEE754);

  void addString(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::STRING_PLAIN,
      uint64_t max_length = 0);

  void addStringArray(
      const String& name,
      bool optional = true,
      ColumnEncoding encoding = ColumnEncoding::STRING_PLAIN,
      uint64_t max_length = 0);

  void addSubrecord(
      const String& name,
      TableSchema subschema,
      bool optional = true);

  void addSubrecordArray(
      const String& name,
      TableSchema subschema,
      bool optional = true);

  void addColumn(
      const String& name,
      ColumnType type,
      ColumnEncoding encoding,
      bool repeated,
      bool optional,
      uint64_t type_size = 0);

  const Vector<Column*>& columns() const;

  //void toJSON(json::JSONOutputStream* json) const;
  //void fromJSON(
  //    json::JSONObject::const_iterator begin,
  //    json::JSONObject::const_iterator end);

  static TableSchema fromProtobuf(const msg::MessageSchema& schema);

protected:
  HashMap<String, ScopedPtr<Column>> columns_by_name_;
  Vector<Column*> columns_;
};


} // namespace msg
} // namespace stx

