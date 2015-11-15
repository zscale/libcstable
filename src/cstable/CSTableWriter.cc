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
  auto file_os = FileOutputStream::fromFileDescriptor(file.fd());

  // build header
  FileHeader header;
  header.columns = columns;

  // write header
  size_t header_size = 0;
  switch (version) {
    case BinaryFormatVersion::v0_1_0:
      RAISE(kIllegalArgumentError, "unsupported version: v0.1.0");
    case BinaryFormatVersion::v0_2_0:
      header_size = cstable::v0_2_0::writeHeader(header, file_os.get());
      break;
  }

  // open cstable writer
  auto page_mgr = new PageManager(version, std::move(file), header_size);

  return new CSTableWriter(
      version,
      page_mgr,
      new PageIndex(version, page_mgr),
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
  // write new index
  auto idx_head = page_idx_->write(free_idx_ptr_);

  // build new meta block
  MetaBlock mb;
  mb.transaction_id = current_txid_ + 1;
  mb.num_rows = num_rows_;
  mb.head_index_page_offset = idx_head.offset;
  mb.head_index_page_size = idx_head.size;
  mb.file_size = page_mgr_->getOffset();

  // commit tx to disk
  page_mgr_->writeTransaction(mb);
  free_idx_ptr_ = cur_idx_ptr_;
  cur_idx_ptr_ = Some(idx_head);
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

