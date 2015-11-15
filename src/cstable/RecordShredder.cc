/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "stx/io/fileutil.h"
#include "stx/ieee754.h"
#include "stx/human.h"
#include <cstable/RecordShredder.h>

namespace stx {
namespace cstable {

RecordShredder::RecordShredder(
    const RecordSchema* schema,
    CSTableWriter* writer) :
    record_schema_(schema),
    writer_(writer) {}

void writeProtoNull(
    uint32_t r,
    uint32_t d,
    const String& column,
    RecordSchema::Column* field,
    CSTableWriter* writer) {
  switch (field->type) {

    case ColumnType::SUBRECORD:
      for (const auto& f : field->subschema->columns()) {
        writeProtoNull(r, d, column + "." + f->name, f, writer);
      }

      break;

    default:
      auto col = writer->getColumnByName(column);
      col->addNull(r, d);
      break;

  }
}

void writeProtoField(
    uint32_t r,
    uint32_t d,
    const msg::MessageObject& msg,
    const String& column,
    RecordSchema::Column* field,
    CSTableWriter* writer) {
  auto col = writer->getColumnByName(column);

  switch (field->type) {

    case ColumnType::STRING: {
      auto& str = msg.asString();
      col->addDatum(r, d, str.data(), str.size());
      break;
    }

    case ColumnType::UNSIGNED_INT: {
      uint64_t val = msg.asUInt64();
      col->addDatum(r, d, &val, sizeof(val));
      break;
    }

    case ColumnType::SIGNED_INT: {
      uint64_t val = msg.asUInt64();
      col->addDatum(r, d, &val, sizeof(val));
      break;
    }

    case ColumnType::DATETIME: {
      uint64_t val = msg.asUInt64();
      col->addDatum(r, d, &val, sizeof(val));
      break;
    }

    case ColumnType::FLOAT: {
      uint64_t val = IEEE754::toBytes(msg.asDouble());
      col->addDatum(r, d, &val, sizeof(val));
      break;
    }

    case ColumnType::BOOLEAN: {
      uint8_t val = msg.asBool() ? 1 : 0;
      col->addDatum(r, d, &val, sizeof(val));
      break;
    }

    case ColumnType::SUBRECORD:
      RAISE(kIllegalStateError);

  }
}

static void addProtoRecordField(
    uint32_t r,
    uint32_t rmax,
    uint32_t d,
    const msg::MessageObject& msg,
    RefPtr<msg::MessageSchema> msg_schema,
    const String& column,
    RecordSchema::Column* field,
    CSTableWriter* writer) {
  auto next_r = r;
  auto next_d = d;

  if (field->repeated) {
    ++rmax;
  }

  if (field->optional || field->repeated) {
    ++next_d;
  }

  size_t n = 0;
  auto field_id = msg_schema->fieldId(field->name);
  for (const auto& o : msg.asObject()) {
    if (o.id != field_id) { // FIXME
      continue;
    }

    ++n;

    switch (field->type) {
      case ColumnType::SUBRECORD: {
        auto o_schema = msg_schema->fieldSchema(field_id);
        for (const auto& f : field->subschema->columns()) {
          addProtoRecordField(
              next_r,
              rmax,
              next_d,
              o,
              o_schema,
              column + field->name + ".",
              f,
              writer);
        }
        break;
      }

      default:
        writeProtoField(next_r, next_d, o, column + field->name, field, writer);
        break;
    }

    next_r = rmax;
  }

  if (n == 0) {
    if (!(field->optional || field->repeated)) {
      RAISEF(kIllegalArgumentError, "missing field: $0", column + field->name);
    }

    writeProtoNull(r, d, column + field->name, field, writer);
    return;
  }
}

void RecordShredder::addRecord(const msg::DynamicMessage& msg) {
  for (const auto& f : record_schema_->columns()) {
    addProtoRecordField(0, 0, 0, msg.data(), msg.schema(), "", f, writer_);
  }

  writer_->addRow();
}

////void RecordShredder::addRecordsFromCSV(CSVInputStream* csv) {
////  Vector<String> columns;
////  csv->readNextRow(&columns);
////
////  Set<String> missing_columns;
////  for (const auto& col : columns_) {
////    missing_columns.emplace(col.first);
////  }
////
////  Vector<RefPtr<ColumnWriter>> column_writers;
////  Vector<msg::FieldType> field_types;
////  for (const auto& col : columns) {
////    if (columns_.count(col) == 0) {
////      RAISEF(kRuntimeError, "column '$0' not found in schema", col);
////    }
////
////    missing_columns.erase(col);
////    column_writers.emplace_back(columns_[col]);
////    field_types.emplace_back(schema_->fieldType(schema_->fieldId(col)));
////  }
////
////  Vector<RefPtr<ColumnWriter>> missing_column_writers;
////  for (const auto& col : missing_columns) {
////    auto writer = columns_[col];
////    if (writer->maxDefinitionLevel() == 0) {
////      RAISEF(kRuntimeError, "missing required column: $0", col);
////    }
////
////    missing_column_writers.emplace_back(writer);
////  }
////
////  Vector<String> row;
////  while (csv->readNextRow(&row)) {
////    for (size_t i = 0; i < row.size() && i < columns.size(); ++i) {
////      const auto& col = column_writers[i];
////      const auto& val = row[i];
////
////      if (Human::isNullOrEmpty(val)) {
////        if (col->maxDefinitionLevel() == 0) {
////          RAISEF(
////              kRuntimeError,
////              "missing value for required column: $0",
////              columns[i]);
////        }
////
////        col->addNull(0, 0);
////        continue;
////      }
////
////      switch (field_types[i]) {
////
////        case msg::FieldType::STRING: {
////          col->addDatum(0, col->maxDefinitionLevel(), val.data(), val.size());
////          break;
////        }
////
////        case msg::FieldType::UINT32: {
////          uint32_t v;
////          try {
////            v = std::stoull(val);
////          } catch (const StandardException& e) {
////            RAISEF(kTypeError, "can't convert '$0' to UINT32", val);
////          }
////
////          col->addDatum(0, col->maxDefinitionLevel(), &v, sizeof(v));
////          break;
////        }
////
////        case msg::FieldType::DATETIME: {
////          auto t = Human::parseTime(val);
////          if (t.isEmpty()) {
////            RAISEF(kTypeError, "can't convert '$0' to DATETIME", val);
////          }
////
////          uint64_t v = t.get().unixMicros();
////          col->addDatum(0, col->maxDefinitionLevel(), &v, sizeof(v));
////          break;
////        }
////
////        case msg::FieldType::UINT64: {
////          uint64_t v;
////          try {
////            v = std::stoull(val);
////          } catch (const StandardException& e) {
////            RAISEF(kTypeError, "can't convert '$0' to UINT64", val);
////          }
////
////          col->addDatum(0, col->maxDefinitionLevel(), &v, sizeof(v));
////          break;
////        }
////
////        case msg::FieldType::DOUBLE: {
////          double v;
////          try {
////            v = std::stod(val);
////          } catch (const StandardException& e) {
////            RAISEF(kTypeError, "can't convert '$0' to DOUBLE", val);
////          }
////
////          col->addDatum(0, col->maxDefinitionLevel(), &v, sizeof(v));
////          break;
////        }
////
////        case msg::FieldType::BOOLEAN: {
////          auto b = Human::parseBoolean(val);
////          uint8_t v = !b.isEmpty() && b.get() ? 1 : 0;
////          col->addDatum(0, col->maxDefinitionLevel(), &v, sizeof(v));
////          break;
////        }
////
////        case msg::FieldType::OBJECT:
////          RAISE(kIllegalStateError, "can't read OBJECTs from CSV");
////
////      }
////    }
////
////    for (auto& col : missing_column_writers) {
////      col->addNull(0, 0);
////    }
////
////    writer_->addRow();
////  }
////}
//
} // namespace cstable
} // namespace stx
