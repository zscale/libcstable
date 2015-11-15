/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/PageIndex.h>
#include <cstable/PageWriter.h>

namespace stx {
namespace cstable {

PageIndex::PageIndex(
    BinaryFormatVersion version,
    RefPtr<PageManager> page_mgr) :
    version_(version),
    page_mgr_(page_mgr) {}

void PageIndex::addPageWriter(PageIndexKey key, PageWriter* page_writer) {
  page_writers_.emplace_back(key, page_writer);
}

PageRef PageIndex::write(Option<PageRef> head) {
  Buffer buf;
  auto os = BufferOutputStream::fromBuffer(&buf);

  for (auto& p : page_writers_) {
    const auto& key = p.first;
    auto& writer = p.second;

    Buffer col_buf;
    auto col_os = BufferOutputStream::fromBuffer(&col_buf);
    writer->writeIndex(col_os.get());

    os->appendVarUInt((uint8_t) key.entry_type);
    os->appendVarUInt(key.column_id);
    os->appendVarUInt(col_buf.size());
    os->write((char*) col_buf.data(), col_buf.size());
  }
}


} // namespace cstable
} // namespace stx

