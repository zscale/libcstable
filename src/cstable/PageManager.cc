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
#include <stx/exception.h>
#include <stx/freeondestroy.h>

namespace stx {
namespace cstable {

PageManager::PageManager(
    File&& file,
    uint64_t offset) :
    file_(std::move(file)),
    offset_(offset) {}

PageRef PageManager::allocPage(uint64_t size) {
  PageRef page;
  page.offset = offset_;
  page.size = padToNextSector(size);
  offset_ += page.size;
  return page;
}

void PageManager::writePage(const PageRef& page, const Buffer& buffer) {
  writePage(page.offset, page.size, buffer.data(), buffer.size());
}

void PageManager::writePage(
    uint64_t page_offset,
    uint64_t page_size,
    const void* data,
    size_t data_size) {
  auto page_data = data;

  util::FreeOnDestroy tmp;
  if (data_size < page_size) {
    page_data = malloc(page_size);
    if (!page_data) {
      RAISE(kMallocError, "malloc() failed");
    }

    tmp.store((void*) page_data);
    memcpy(tmp.get(), data, data_size);
    memset((char*) tmp.get() + data_size, 0, page_size - data_size);
  }

  file_.pwrite(page_offset, page_data, page_size);
}

uint64_t PageManager::getOffset() const {
  return offset_;
}

File* PageManager::file() {
  return &file_;
}

} // namespace cstable
} // namespace stx

