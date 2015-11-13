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
#include <cstable/BitPackedIntColumnWriter.h>
#include <stx/SHA1.h>

namespace stx {
namespace cstable {

RefPtr<CSTableWriter> CSTableWriter::createFile(
    const String& filename,
    const Vector<ColumnConfig>& columns,
    Option<RefPtr<LockRef>> lockref /* = None<RefPtr<LockRef>>() */) {
  return new CSTableWriter(
      File::openFile(filename, File::O_WRITE | File::O_CREATE),
      columns);
}

RefPtr<CSTableWriter> CSTableWriter::reopenFile(
    const String& filename,
    Option<RefPtr<LockRef>> lockref /* = None<RefPtr<LockRef>>() */) {
  RAISE(kNotYetImplementedError);
}

CSTableWriter::CSTableWriter(
    File&& file,
    const Vector<ColumnConfig>& columns) :
    file_(std::move(file)),
    columns_(columns),
    column_metadata_(columns.size()),
    column_rlevel_metadata_(columns.size()),
    column_dlevel_metadata_(columns.size()),
    meta_block_offset_(14),
    meta_block_size_(52),
    current_txid_(0),
    num_rows_(0) {
  // create columns
  for (size_t i = 0; i < columns_.size(); ++i) {
    RefPtr<ColumnWriter> writer;

    switch (columns_[i].storage_type) {

      case ColumnType::UINT32_BITPACKED:
        writer = new BitPackedIntColumnWriter(
            page_mgr_,
            column_metadata_[i],
            column_rlevel_metadata_[i],
            column_dlevel_metadata_[i],
            columns_[i].rlevel_max,
            columns_[i].dlevel_max);
        break;

    }

    column_writers_by_id_.emplace(columns_[i].column_id, writer);
    column_writers_by_name_.emplace(columns_[i].column_name, writer);
  }

  // build header
  Buffer hdr;
  hdr.reserve(8192);

  auto os = BufferOutputStream::fromBuffer(&hdr);
  os->write(kMagicBytes, sizeof(kMagicBytes));
  os->appendUInt16(2); // version
  os->appendUInt64(0); // flags
  RCHECK(hdr.size() == meta_block_offset_, "invalid meta block offset");
  os->appendString(String(meta_block_size_ * 2, '\0')); // empty meta blocks
  os->appendString(String(128, '\0')); // 128 bytes reserved
  os->appendUInt32(columns_.size());
  for (const auto& col : columns_) {
    col.encode(os.get());
  }

  // pad header to next 512 byte boundary
  auto header_padding = padToNextSector(hdr.size()) - hdr.size();
  os->appendString(String(header_padding, '\0'));

  // flush header to disk & init pagemanager
  file_.pwrite(0, hdr.data(), hdr.size());
  page_mgr_ = mkRef(new PageManager(hdr.size()));
}

void CSTableWriter::commit() {
  auto txid = current_txid_ + 1;

  // write new index and fsync all changes
  file_.fsync();

  // build new meta block
  Buffer buf;
  buf.reserve(meta_block_size_);

  auto os = BufferOutputStream::fromBuffer(&buf);
  os->appendUInt64(txid); // transaction id
  os->appendUInt64(num_rows_); // number of rows
  os->appendUInt64(0); // head index page offset as multiple of 512 bytes
  os->appendUInt64(page_mgr_->getOffset()); // file size in bytes

  auto hash = SHA1::compute(buf.data(), buf.size());
  os->write((char*) hash.data(), hash.size()); // sha1 hash

  RCHECK(buf.size() == meta_block_size_, "invalid meta block size");

  // write to metablock slot
  auto mb_index = txid % 2;
  auto mb_offset = meta_block_offset_ + meta_block_size_ * mb_index;
  file_.pwrite(mb_offset, buf.data(), buf.size());

  // fsync one last time
  file_.fsync();
}

} // namespace cstable
} // namespace stx

