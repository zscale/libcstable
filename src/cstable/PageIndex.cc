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


} // namespace cstable
} // namespace stx

