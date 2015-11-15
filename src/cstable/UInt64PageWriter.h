/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stx/stdtypes.h>
#include <stx/util/binarymessagewriter.h>
#include <stx/util/BitPackEncoder.h>
#include <stx/io/outputstream.h>
#include <cstable/BinaryFormat.h>
#include <cstable/PageManager.h>
#include <cstable/PageWriter.h>

namespace stx {
namespace cstable {

class UInt64PageWriter : public UnsignedIntPageWriter {
public:
  static const uint64_t kPageSize = 512 * 2;

  UInt64PageWriter(
      RefPtr<PageManager> page_mgr);

  void writeValue(uint64_t value) override;

protected:

  void allocPage();
  void flushPage();

  RefPtr<PageManager> page_mgr_;
  bool has_page_;
  cstable::PageRef page_;
  Buffer page_buf_;
  Buffer meta_buf_;
  BufferOutputStream page_os_;
  BufferOutputStream meta_os_;
};

} // namespace cstable
} // namespace stx

