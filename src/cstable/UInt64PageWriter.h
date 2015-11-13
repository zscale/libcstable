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
#include <cstable/PageWriter.h>

namespace stx {
namespace cstable {

class UInt64PageWriter : public UnsignedIntPageWriter {
public:

  UInt64PageWriter(
      RefPtr<PageManager> page_mgr);

  void writeValue(uint64_t value) override;

};

} // namespace cstable
} // namespace stx

