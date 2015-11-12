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
#include <stx/SHA1.h>

namespace stx {
namespace cstable {
namespace v2 {

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
    meta_block_offset_(14),
    meta_block_size_(52),
    current_txid_(0) {}

void CSTableWriter::commit() {
  file_.fsync();

  writeMetaBlock(current_txid_++);

  // writeMetaBlock();
  file_.fsync();
}

void CSTableWriter::writeHeader() {
  Buffer buf;
  buf.reserve(8192);

  auto os = BufferOutputStream::fromBuffer(&buf);
  os->write(kMagicBytes, sizeof(kMagicBytes));
  os->appendUInt16(2); // version
  os->appendUInt64(0); // flags
  RCHECK(buf.size() == meta_block_offset_, "invalid meta block offset");
  os->appendString(String(meta_block_size_ * 2, '\0')); // empty meta blocks
  os->appendString(String(128, '\0')); // 128 bytes reserved
}

void CSTableWriter::writeMetaBlock(uint64_t transaction_id) {
  Buffer buf;
  buf.reserve(meta_block_size_);

  auto os = BufferOutputStream::fromBuffer(&buf);
  os->appendUInt64(transaction_id); // transaction id
  os->appendUInt64(0); // number of rows
  os->appendUInt64(0); // head index page offset as multiple of 512 bytes
  os->appendUInt64(0); // file size in bytes

  auto hash = SHA1::compute(buf.data(), buf.size());
  os->write((char*) hash.data(), hash.size()); // sha1 hash

  RCHECK(buf.size() == meta_block_size_, "invalid meta block size");

  auto mb_index = transaction_id % 2;
  auto mb_offset = meta_block_offset_ + meta_block_size_ * mb_index;
  file_.pwrite(mb_offset, buf.data(), buf.size());
}

} // namespace v2
} // namespace cstable
} // namespace stx

