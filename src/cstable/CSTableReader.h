/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stx/stdtypes.h>
#include <stx/io/file.h>
#include <cstable/ColumnReader.h>
#include <cstable/BinaryFormat.h>
#include <cstable/PageManager.h>

namespace stx {
namespace cstable {

class CSTableReader : public RefCounted {
public:

  static RefPtr<CSTableReader> openFile(const String& filename);

  RefPtr<ColumnReader> getColumnByName(const String& column_name);

  RefPtr<ColumnReader> getColumnReader(const String& column_name) {
    return getColumnByName(column_name);
  }

  ColumnType getColumnType(const String& column_name);
  ColumnEncoding getColumnEncoding(const String& column_name);

  Set<String> columns() const;

  bool hasColumn(const String& column_name) const ;

  size_t numRecords() const;

protected:

  CSTableReader(
      BinaryFormatVersion version,
      Vector<ColumnConfig> columns,
      Vector<RefPtr<ColumnReader>> column_readers,
      uint64_t num_rows);

  BinaryFormatVersion version_;
  Vector<ColumnConfig> columns_;
  HashMap<uint32_t, RefPtr<ColumnReader>> column_readers_by_id_;
  HashMap<String, RefPtr<ColumnReader>> column_readers_by_name_;
  uint64_t num_rows_;
};

} // namespace cstable
} // namespace stx

