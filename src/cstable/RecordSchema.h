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
#include <cstable/BinaryFormat.h>

namespace stx {
namespace cstable {

class RecordSchema {
public:

  struct Column {
    String name;
    ColumnType type;
    ColumnEncoding encoding;
    uint64_t type_size;
    bool repeated;
    bool optional;
    ScopedPtr<RecordSchema> subschema;
  };

  RecordSchema();
  RecordSchema(const RecordSchema& other);

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
      RecordSchema subschema,
      bool optional = true);

  void addSubrecordArray(
      const String& name,
      RecordSchema subschema,
      bool optional = true);

  const Vector<Column*>& columns() const;

  //String toString() const;
  //Buffer encode() const;
  //void encode(util::BinaryMessageWriter* buf) const;
  //void decode(util::BinaryMessageReader* buf);
  //static RefPtr<msg::RecordSchema> decode(const String& buf);

  //void toJSON(json::JSONOutputStream* json) const;
  //void fromJSON(
  //    json::JSONObject::const_iterator begin,
  //    json::JSONObject::const_iterator end);

protected:
  HashMap<String, ScopedPtr<Column>> columns_by_name_;
  Vector<Column*> columns_;
};


} // namespace msg
} // namespace stx

