/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/RecordSchema.h>

namespace stx {
namespace cstable {

RecordSchema::RecordSchema() {}

RecordSchema::RecordSchema(const RecordSchema& other) {
  for (const auto& c : other.columns_by_name_) {
    auto col_cpy = mkScoped(new Column());
    col_cpy->name = c.second->name;
    col_cpy->type = c.second->type;
    col_cpy->encoding = c.second->encoding;
    col_cpy->type_size = c.second->type_size;
    col_cpy->repeated = c.second->repeated;
    col_cpy->optional = c.second->optional;
    if (c.second->subschema.get()) {
      col_cpy->subschema = mkScoped(new RecordSchema(*c.second->subschema));
    }

    columns_.emplace_back(col_cpy.get());
    columns_by_name_.emplace(col_cpy->name, std::move(col_cpy));
  }
}

void RecordSchema::addUnsignedInteger(
    const String& name,
    bool optional /* = true */,
    ColumnEncoding encoding /* = ColumnEncoding::UINT64_LEB128 */,
    uint64_t max_value /* = 0 */) {
  auto col = mkScoped(new Column {
    .name = name,
    .type = ColumnType::UNSIGNED_INT,
    .encoding = encoding,
    .type_size = max_value,
    .repeated = false,
    .optional = optional
  });

  columns_.emplace_back(col.get());
  columns_by_name_.emplace(name, std::move(col));
}

void RecordSchema::addUnsignedIntegerArray(
    const String& name,
    bool optional /* = true */,
    ColumnEncoding encoding /* = ColumnEncoding::UINT64_LEB128 */,
    uint64_t max_value /* = 0 */) {
  auto col = mkScoped(new Column {
    .name = name,
    .type = ColumnType::UNSIGNED_INT,
    .encoding = encoding,
    .type_size = max_value,
    .repeated = true,
    .optional = optional
  });

  columns_.emplace_back(col.get());
  columns_by_name_.emplace(name, std::move(col));
}

void RecordSchema::addFloat(
    const String& name,
    bool optional /* = true */,
    ColumnEncoding encoding /* = ColumnEncoding::FLOAT_IEEE754 */) {
  auto col = mkScoped(new Column {
    .name = name,
    .type = ColumnType::FLOAT,
    .encoding = encoding,
    .type_size = 0,
    .repeated = false,
    .optional = optional
  });

  columns_.emplace_back(col.get());
  columns_by_name_.emplace(name, std::move(col));
}

void RecordSchema::addFloatArray(
    const String& name,
    bool optional /* = true */,
    ColumnEncoding encoding /* = ColumnEncoding::FLOAT_IEEE754 */) {
  auto col = mkScoped(new Column {
    .name = name,
    .type = ColumnType::FLOAT,
    .encoding = encoding,
    .type_size = 0,
    .repeated = true,
    .optional = optional
  });

  columns_.emplace_back(col.get());
  columns_by_name_.emplace(name, std::move(col));
}

void RecordSchema::addString(
    const String& name,
    bool optional /* = true */,
    ColumnEncoding encoding /* = ColumnEncoding::STRING_PLAIN */,
    uint64_t max_len /* = 0 */) {
  auto col = mkScoped(new Column {
    .name = name,
    .type = ColumnType::STRING,
    .encoding = encoding,
    .type_size = max_len,
    .repeated = false,
    .optional = optional
  });

  columns_.emplace_back(col.get());
  columns_by_name_.emplace(name, std::move(col));
}

void RecordSchema::addStringArray(
    const String& name,
    bool optional /* = true */,
    ColumnEncoding encoding /* = ColumnEncoding::STRING_PLAIN */,
    uint64_t max_len /* = 0 */) {
  auto col = mkScoped(new Column {
    .name = name,
    .type = ColumnType::STRING,
    .encoding = encoding,
    .type_size = max_len,
    .repeated = true,
    .optional = optional
  });

  columns_.emplace_back(col.get());
  columns_by_name_.emplace(name, std::move(col));
}

void RecordSchema::addSubrecord(
    const String& name,
    RecordSchema schema,
    bool optional /* = true */) {
  auto col = mkScoped(new Column {
    .name = name,
    .type = ColumnType::SUBRECORD,
    .repeated = false,
    .optional = optional,
    .subschema = mkScoped(new RecordSchema(schema))
  });

  columns_.emplace_back(col.get());
  columns_by_name_.emplace(name, std::move(col));
}

void RecordSchema::addSubrecordArray(
    const String& name,
    RecordSchema schema,
    bool optional /* = true */) {
  auto col = mkScoped(new Column {
    .name = name,
    .type = ColumnType::SUBRECORD,
    .repeated = true,
    .optional = optional,
    .subschema = mkScoped(new RecordSchema(schema))
  });

  columns_.emplace_back(col.get());
  columns_by_name_.emplace(name, std::move(col));
}

const Vector<RecordSchema::Column*>& RecordSchema::columns() const {
  return columns_;
}

} // namespace msg
} // namespace stx

