/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stx/stdtypes.h>
#include <stx/io/file.h>
#include <stx/util/binarymessagewriter.h>
#include <stx/autoref.h>
#include <stx/csv/CSVInputStream.h>
#include <cstable/ColumnWriter.h>
#include <cstable/CSTableWriter.h>
#include <stx/protobuf/MessageSchema.h>
#include <stx/protobuf/MessageObject.h>

namespace stx {
namespace cstable {

class RecordShredder {
public:

  static Vector<ColumnConfig> columnsFromSchema(
      const msg::MessageSchema* schema);

  RecordShredder(
      const msg::MessageSchema* schema,
      CSTableWriter* writer);

  void addRecord(const msg::MessageObject& msg);
  void addRecordsFromCSV(CSVInputStream* csv);

protected:

  void addRecordField(
      uint32_t r,
      uint32_t rmax,
      uint32_t d,
      const msg::MessageObject& msg,
      const String& column,
      const msg::MessageSchemaField& field);

  void writeNull(
      uint32_t r,
      uint32_t d,
      const String& column,
      const msg::MessageSchemaField& field);

  void writeField(
      uint32_t r,
      uint32_t d,
      const msg::MessageObject& msg,
      const String& column,
      const msg::MessageSchemaField& field);

  const msg::MessageSchema* schema_;
  CSTableWriter* writer_;
  HashMap<String, RefPtr<ColumnWriter>> columns_;
};

} // namespace cstable
} // namespace stx

