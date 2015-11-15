/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/BinaryFormat.h>
#include <cstable/ColumnConfig.h>
#include <stx/util/binarymessagewriter.h>
#include <stx/SHA1.h>

namespace stx {
namespace cstable {

namespace v0_2_0 {

size_t writeMetaBlock(const MetaBlock& mb, OutputStream* os) {
  util::BinaryMessageWriter buf;
  buf.appendUInt64(mb.transaction_id); // transaction id
  buf.appendUInt64(mb.num_rows); // number of rows
  buf.appendUInt64(mb.head_index_page); // head index page offset
  buf.appendUInt64(mb.file_size); // file size in bytes
  os->write((char*) buf.data(), buf.size());

  auto hash = SHA1::compute(buf.data(), buf.size());
  os->write((char*) hash.data(), hash.size()); // sha1 hash

  return buf.size() + hash.size();
}

size_t readMetaBlock(const MetaBlock& mb, InputStream* is) {
  return 0;
}

size_t writeHeader(const FileHeader& hdr, OutputStream* os) {
  util::BinaryMessageWriter buf;
  buf.append(kMagicBytes, sizeof(kMagicBytes));
  buf.appendUInt16(2); // version
  buf.appendUInt64(0); // flags
  RCHECK(buf.size() == kMetaBlockPosition, "invalid meta block offset");
  buf.appendString(String(kMetaBlockSize * 2, '\0')); // empty meta blocks
  buf.appendString(String(128, '\0')); // 128 bytes reserved
  buf.appendUInt32(hdr.columns.size());

  for (const auto& col : hdr.columns) {
    buf.appendVarUInt((uint8_t) col.logical_type);
    buf.appendVarUInt((uint8_t) col.storage_type);
    buf.appendVarUInt(col.column_id);
    buf.appendLenencString(col.column_name);
    buf.appendVarUInt(col.rlevel_max);
    buf.appendVarUInt(col.dlevel_max);
  }

  // pad header to next 512 byte boundary
  auto header_padding = padToNextSector(buf.size()) - buf.size();
  buf.appendString(String(header_padding, '\0'));

  os->write((const char*) buf.data(), buf.size());
  return buf.size();
}

size_t readHeader(FileHeader* hdr, InputStream* os) {
  // logical_type = (msg::FieldType) is->readVarUInt();
  // storage_type = (cstable::ColumnType) is->readVarUInt();
  // column_id = is->readVarUInt();
  // column_name = is->readLenencString();
  // rlevel_max = is->readVarUInt();
  // dlevel_max = is->readVarUInt();
  return 0;
}

}

}
}
