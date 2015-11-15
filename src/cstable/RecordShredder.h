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
#include <cstable/RecordSchema.h>
#include <stx/protobuf/MessageSchema.h>
#include <stx/protobuf/MessageObject.h>

namespace stx {
namespace cstable {

class RecordShredder {
public:

  static Vector<ColumnConfig> columnsFromSchema(
      const msg::MessageSchema* schema);

  RecordShredder(
      const RecordSchema* schema,
      CSTableWriter* writer);

  void addRecordFromJSON(const String& json);
  void addRecordFromJSON(
      const json::JSONObject::const_iterator& begin,
      const json::JSONObject::const_iterator& end);

  //void addRecord(const msg::MessageObject& msg);
  void addRecordsFromCSV(CSVInputStream* csv);

protected:
  const RecordSchema* schema_;
  CSTableWriter* writer_;
};

} // namespace cstable
} // namespace stx

