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
#include <cstable/BinaryFormat.h>
#include <cstable/PageManager.h>
#include <cstable/PageIndex.h>
#include <cstable/PageReader.h>

namespace stx {
namespace cstable {

class UInt64PageReader : public UnsignedIntPageReader {
public:
  static const uint64_t kPageSize = 512 * 2;

  UInt64PageReader(
      PageIndexKey key,
      RefPtr<PageManager> page_mgr,
      PageIndexReader* page_idx);

  void readIndex(InputStream* os) const override;

protected:
  RefPtr<PageManager> page_mgr_;
  Vector<Pair<cstable::PageRef, uint64_t>> pages_;
};

} // namespace cstable
} // namespace stx

