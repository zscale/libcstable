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
#include <cstable/ColumnWriter.h>

namespace stx {
namespace cstable {

class UnsignedIntColumnWriter : public DefaultColumnWriter {
public:

  UnsignedIntColumnWriter(
      ColumnConfig config,
      RefPtr<PageManager> page_mgr,
      RefPtr<Buffer> meta_buf,
      RefPtr<Buffer> rlevel_meta_buf,
      RefPtr<Buffer> dlevel_meta_buf);

  void writeValue(uint64_t rep_level, uint64_t def_level, uint32_t value);

protected:
  ScopedPtr<UnsignedIntPageWriter> data_writer_;
};

} // namespace cstable
} // namespace stx

