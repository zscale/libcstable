/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/v2/CSTableWriter.h>

namespace stx {
namespace cstable {
namespace v2 {

RefPtr<CSTableWriter> CSTableWriter::createFile(
    const String& filename,
    const Vector<ColumnConfig>& columns) {
  return new CSTableWriter(
      File::openFile(filename, File::O_WRITE | File::O_CREATE),
      columns);
}

RefPtr<CSTableWriter> CSTableWriter::reopenFile(const String& filename) {
  RAISE(kNotYetImplementedError);
}

CSTableWriter::CSTableWriter(
    File&& file,
    const Vector<ColumnConfig>& columns) :
    file_(std::move(file)),
    columns_(columns) {}

void CSTableWriter::commit() {

}

} // namespace v2
} // namespace cstable
} // namespace stx

