/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/CSTableWriter.h>
#include <cstable/UnsignedIntColumnWriter.h>
#include <stx/SHA1.h>

namespace stx {
namespace cstable {

RefPtr<CSTableWriter> CSTableWriter::createFile(
    const String& filename,
    const Vector<ColumnConfig>& columns,
    Option<RefPtr<LockRef>> lockref /* = None<RefPtr<LockRef>>() */) {
  return createFile(
      filename,
      BinaryFormatVersion::v0_2_0,
      columns,
      lockref);
}

RefPtr<CSTableWriter> CSTableWriter::createFile(
    const String& filename,
    BinaryFormatVersion version,
    const Vector<ColumnConfig>& columns,
    Option<RefPtr<LockRef>> lockref /* = None<RefPtr<LockRef>>() */) {
  auto file = File::openFile(filename, File::O_WRITE | File::O_CREATE);

  // build header
  Buffer hdr;
  hdr.reserve(8192);

  auto os = BufferOutputStream::fromBuffer(&hdr);
  os->write(kMagicBytes, sizeof(kMagicBytes));
  os->appendUInt16(2); // version
  os->appendUInt64(0); // flags
  RCHECK(hdr.size() == cstable::v0_2_0::kMetaBlockPosition, "invalid meta block offset");
  os->appendString(String(cstable::v0_2_0::kMetaBlockSize * 2, '\0')); // empty meta blocks
  os->appendString(String(128, '\0')); // 128 bytes reserved
  os->appendUInt32(columns.size());
  for (const auto& col : columns) {
    col.encode(os.get());
  }

  // pad header to next 512 byte boundary
  auto header_padding = padToNextSector(hdr.size()) - hdr.size();
  os->appendString(String(header_padding, '\0'));

  // flush header to disk & init pagemanager
  file.pwrite(0, hdr.data(), hdr.size());

  auto page_mgr = mkRef(
      new PageManager(version, std::move(file), hdr.size()));

  auto page_idx = mkRef(new PageIndex(version));

  return new CSTableWriter(
      version,
      page_mgr,
      page_idx,
      columns);
}

RefPtr<CSTableWriter> CSTableWriter::reopenFile(
    const String& filename,
    Option<RefPtr<LockRef>> lockref /* = None<RefPtr<LockRef>>() */) {
  RAISE(kNotYetImplementedError);
}

CSTableWriter::CSTableWriter(
    BinaryFormatVersion version,
    RefPtr<PageManager> page_mgr,
    RefPtr<PageIndex> page_idx,
    const Vector<ColumnConfig>& columns) :
    version_(version),
    page_mgr_(page_mgr),
    page_idx_(page_idx),
    columns_(columns),
    current_txid_(0),
    num_rows_(0) {
  // create columns
  for (size_t i = 0; i < columns_.size(); ++i) {
    RefPtr<DefaultColumnWriter> writer;

    switch (columns_[i].logical_type) {
      case msg::FieldType::UINT64:
      case msg::FieldType::UINT32:
        writer = new UnsignedIntColumnWriter(columns_[i], page_mgr_, page_idx_);
        break;
    }

    column_writers_by_id_.emplace(columns_[i].column_id, writer);
    column_writers_by_name_.emplace(columns_[i].column_name, writer);
  }
}

void CSTableWriter::commit() {
  // build new meta block
  MetaBlock mb;
  mb.transaction_id = current_txid_ + 1;
  mb.num_rows = num_rows_;
  mb.head_index_page = 0;
  mb.file_size = page_mgr_->getOffset();

  // commit tx to disk
  page_mgr_->writeTransaction(mb);
}

RefPtr<DefaultColumnWriter> CSTableWriter::getColumnByName(
    const String& column_name) const {
  auto col = column_writers_by_name_.find(column_name);
  if (col == column_writers_by_name_.end()) {
    RAISEF(kNotFoundError, "column not found: $0", column_name);
  }

  return col->second;
}

RefPtr<DefaultColumnWriter> CSTableWriter::getColumnById(
    uint32_t column_id) const {
  auto col = column_writers_by_id_.find(column_id);
  if (col == column_writers_by_id_.end()) {
    RAISEF(kNotFoundError, "column not found: $0", column_id);
  }

  return col->second;
}

} // namespace cstable
} // namespace stx

