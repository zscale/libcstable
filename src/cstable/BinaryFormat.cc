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
#include <stx/util/binarymessagewriter.h>
#include <stx/SHA1.h>

namespace stx {
namespace cstable {

namespace v0_2_0 {

void writeMetaBlock(const MetaBlock& mb, OutputStream* os) {
  util::BinaryMessageWriter buf;
  buf.appendUInt64(mb.transaction_id); // transaction id
  buf.appendUInt64(mb.num_rows); // number of rows
  buf.appendUInt64(mb.head_index_page); // head index page offset
  buf.appendUInt64(mb.file_size); // file size in bytes
  os->write((char*) buf.data(), buf.size());

  auto hash = SHA1::compute(buf.data(), buf.size());
  os->write((char*) hash.data(), hash.size()); // sha1 hash
}

void readMetaBlock(const MetaBlock& mb, InputStream* is);

}

}
}
