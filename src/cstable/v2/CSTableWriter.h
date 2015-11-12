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
#include <stx/exception.h>
#include <stx/io/file.h>
#include <cstable/ColumnWriter.h>

namespace stx {
namespace cstable {
namespace v2 {

struct ColumnConfig {
};

class CSTableWriter : public RefCounted {
public:

  /**
   * Create a new cstable. This method implicitly requires a write lock on the
   * new table and starts a transaction
   */
  static RefPtr<CSTableWriter> createFile(
      const String& filename,
      const Vector<ColumnConfig>& columns);

  static RefPtr<CSTableWriter> reopenFile(const String& filename);

  /**
   * Commit the current implicit transaction. Note that after commiting you
   * can't start a new transaction on the same writer. Create a new writer to
   * execute successive transactions
   */
  void commit();

  /**
   * Retrieve the writer for a specific column
   */
  RefPtr<ColumnWriter> getColumn(const String& column_name) const;
  RefPtr<ColumnWriter> getColumn(uint32_t column_id) const;

protected:

  CSTableWriter(
      File&& file,
      const Vector<ColumnConfig>& columns);

  File file_;
  Vector<ColumnConfig> columns_;
};

} // namespace v2
} // namespace cstable
} // namespace stx

