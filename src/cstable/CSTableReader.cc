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
#include <cstable/v1/BooleanColumnReader.h>
#include <cstable/v1/BitPackedIntColumnReader.h>
#include <cstable/v1/UInt32ColumnReader.h>
#include <cstable/v1/UInt64ColumnReader.h>
#include <cstable/v1/LEB128ColumnReader.h>
#include <cstable/v1/DoubleColumnReader.h>
#include <cstable/v1/StringColumnReader.h>
#include <stx/io/file.h>
#include <stx/io/mmappedfile.h>

namespace stx {
namespace cstable {

static RefPtr<v1::ColumnReader> openColumnV1(
    const ColumnConfig& c,
    RefPtr<VFSFile> mmap) {
  auto csize = c.body_size;
  auto cdata = mmap->structAt<void>(c.body_offset);
  auto rmax = c.rlevel_max;
  auto dmax = c.dlevel_max;

  switch (c.storage_type) {
    case ColumnType::BOOLEAN:
      return new v1::BooleanColumnReader(rmax, dmax, cdata, csize);
    case ColumnType::UINT32_BITPACKED:
      return new v1::BitPackedIntColumnReader(rmax, dmax, cdata, csize);
    case ColumnType::UINT32_PLAIN:
      return new v1::UInt32ColumnReader(rmax, dmax, cdata, csize);
    case ColumnType::UINT64_PLAIN:
      return new v1::UInt64ColumnReader(rmax, dmax, cdata, csize);
    case ColumnType::UINT64_LEB128:
      return new v1::LEB128ColumnReader(rmax, dmax, cdata, csize);
    case ColumnType::DOUBLE:
      return new v1::DoubleColumnReader(rmax, dmax, cdata, csize);
    case ColumnType::STRING_PLAIN:
      return new v1::StringColumnReader(rmax, dmax, cdata, csize);
    default:
      RAISEF(
          kRuntimeError,
          "unsupported column type: $0",
          (uint32_t) c.storage_type);
  }
}

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

  Vector<RefPtr<ColumnReader>> column_readers;

  switch (version) {
    case BinaryFormatVersion::v0_1_0: {
      auto mmap = mkRef(new io::MmappedFile(std::move(file)));
      for (const auto& col : header.columns) {
        auto reader = openColumnV1(col, mmap.get());
        reader->storeMmap(mmap.get());
        column_readers.emplace_back(reader.get());
      }

      return new DefaultCSTableReader(
          version,
          header.columns,
          column_readers,
          header.num_rows);
    }

    case BinaryFormatVersion::v0_2_0: {
      auto page_mgr = new PageManager(
          version,
          std::move(file),
          metablock.file_size);

      break;
    }
  }
}

DefaultCSTableReader::DefaultCSTableReader(
    BinaryFormatVersion version,
    Vector<ColumnConfig> columns,
    Vector<RefPtr<ColumnReader>> column_readers,
    uint64_t num_rows) :
    version_(version),
    columns_(columns),
    num_rows_(num_rows) {
  RCHECK(column_readers.size() == columns.size(), "illegal column list");

  for (size_t i = 0; i < columns.size(); ++i) {
    if (columns_[i].column_id > 0) {
      column_readers_by_id_.emplace(columns_[i].column_id, column_readers[i]);
    }

    column_readers_by_name_.emplace(columns_[i].column_name, column_readers[i]);
  }
}

RefPtr<ColumnReader> DefaultCSTableReader::getColumnByName(
    const String& column_name) {
  auto col = column_readers_by_name_.find(column_name);
  if (col == column_readers_by_name_.end()) {
    RAISEF(kNotFoundError, "column not found: $0", column_name);
  }

  return col->second;
}

//RefPtr<ColumnReader> DefaultCSTableReader::getColumnById(
//    uint32_t column_id) const {
//  auto col = column_readers_by_id_.find(column_id);
//  if (col == column_readers_by_id_.end()) {
//    RAISEF(kNotFoundError, "column not found: $0", column_id);
//  }
//
//  return col->second;
//}

ColumnType DefaultCSTableReader::getColumnType(const String& column_name) {
  auto col = getColumnByName(column_name);
  return col->storageType();
}

Set<String> DefaultCSTableReader::columns() const {
  Set<String> cols;

  for (const auto& c : columns_) {
    cols.emplace(c.column_name);
  }

  return cols;
}

bool DefaultCSTableReader::hasColumn(const String& column_name) const {
  auto col = column_readers_by_name_.find(column_name);
  return col != column_readers_by_name_.end();
}

size_t DefaultCSTableReader::numRecords() const {
  return num_rows_;
}


} // namespace cstable
} // namespace stx

