/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/UInt64PageWriter.h>
#include <stx/inspect.h>

namespace stx {
namespace cstable {

UInt64PageWriter::UInt64PageWriter(
    PageIndexKey key,
    RefPtr<PageManager> page_mgr,
    RefPtr<PageIndex> page_idx) :
    key_(key),
    page_mgr_(page_mgr),
    has_page_(false),
    page_os_(&page_buf_),
    meta_os_(&meta_buf_) {
  page_idx->addPageWriter(key_, this);
}

void UInt64PageWriter::writeValue(uint64_t value) {
  if (!has_page_ || page_buf_.size() + sizeof(uint64_t) > page_.size) {
    allocPage();
  }

  page_os_.appendUInt64(value);
}

void UInt64PageWriter::allocPage() {
  if (has_page_) {
    flushPage();
  }

  page_ = page_mgr_->allocPage(kPageSize);
  if (page_buf_.size() < page_.size) {
    page_buf_.reserve(page_.size - page_buf_.size());
  }

  has_page_ = true;
}

void UInt64PageWriter::flushPage() {
  meta_os_.appendVarUInt(page_.offset);
  meta_os_.appendVarUInt(page_.size);
  meta_os_.appendVarUInt(page_buf_.size());
  page_mgr_->writePage(page_, page_buf_);
}


} // namespace cstable
} // namespace stx

