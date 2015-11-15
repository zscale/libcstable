/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/CSTableReader.h>
#include <cstable/v1/CSTableReader.h>

namespace stx {
namespace cstable {

RefPtr<CSTableReader> CSTableReader::openFile(const String& filename) {
  auto file = File::openFile(filename, File::O_READ);
  auto file_is = FileInputStream::fromFileDescriptor(file.fd());

  BinaryFormatVersion version;
  FileHeader header;
  MetaBlock metablock;
  Option<PageRef> free_index;
  readHeader(
      &version,
      &header,
      &metablock,
      &free_index,
      file_is.get());

  if (version == BinaryFormatVersion::v0_1_0) {
    return new v1::CSTableReader(std::move(file));
  }

  auto page_mgr = new PageManager(
      version,
      std::move(file),
      metablock.file_size);

  return new DefaultCSTableReader(page_mgr);
}

DefaultCSTableReader::DefaultCSTableReader(
    RefPtr<PageManager> page_mgr) :
    page_mgr_(page_mgr) {}

RefPtr<ColumnReader> DefaultCSTableReader::getColumnByName(
    const String& column_name) {
}

ColumnType DefaultCSTableReader::getColumnType(const String& column_name) {
}

Set<String> DefaultCSTableReader::columns() const {
}

bool DefaultCSTableReader::hasColumn(const String& column_name) const {
}

size_t DefaultCSTableReader::numRecords() const {
}


} // namespace cstable
} // namespace stx

