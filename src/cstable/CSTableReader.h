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

namespace stx {
namespace cstable {

class CSTableReader : public RefCounted {
public:

  static RefPtr<CSTableReader> openFile(const String& filename);

  virtual RefPtr<ColumnReader> getColumnReader(const String& column_name) = 0;

  virtual ColumnType getColumnType(const String& column_name) = 0;

  virtual Set<String> columns() const = 0;

  virtual bool hasColumn(const String& column_name) const = 0;

  virtual size_t numRecords() const = 0;

};

} // namespace cstable
} // namespace stx

