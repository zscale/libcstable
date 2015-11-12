/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/PageManager.h>
#include <cstable/BinaryFormat.h>

namespace stx {
namespace cstable {

PageManager::PageManager(uint64_t offset) : offset_(offset) {}

PageRef PageManager::allocPage(uint64_t size) {
  PageRef page;
  page.offset = offset_;
  page.size = padToNextSector(size);
  offset_ += page.size;
  return page;
}

uint64_t PageManager::getOffset() const {
  return offset_;
}

} // namespace cstable
} // namespace stx

