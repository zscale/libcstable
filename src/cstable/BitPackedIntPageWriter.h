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
#include <cstable/BinaryFormat.h>
#include <cstable/PageManager.h>

namespace stx {
namespace cstable {

class BitPackedIntPageWriter {
public:

  BitPackedIntPageWriter(
      RefPtr<Buffer> metadata_buf,
      RefPtr<PageManager> page_mgr,
      uint32_t max_value = 0xffffffff);

  void addDatum(uint32_t value);

protected:
  uint32_t max_value_;
};

} // namespace cstable
} // namespace stx

