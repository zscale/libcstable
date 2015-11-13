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
#include <cstable/BitPackedIntPageWriter.h>
#include <cstable/v1/ColumnWriter.h>

namespace stx {
namespace cstable {

class BitPackedIntColumnWriter : public ColumnWriter {
public:

  BitPackedIntColumnWriter(
      RefPtr<PageManager> page_mgr,
      RefPtr<Buffer> meta_buf,
      RefPtr<Buffer> rlevel_meta_buf,
      RefPtr<Buffer> dlevel_meta_buf,
      uint64_t r_max,
      uint64_t d_max,
      uint32_t max_value = 0xffffffff);

  void addNull(uint64_t rep_level, uint64_t def_level) override;

  void addDatum(
      uint64_t rep_level,
      uint64_t def_level,
      const void* data,
      size_t size) override;

  void addDatum(uint64_t rep_level, uint64_t def_level, uint32_t value);
  void commit();

  ColumnType type() const override {
    return ColumnType::UINT32_BITPACKED;
  }

  msg::FieldType fieldType() const override {
    return msg::FieldType::UINT32;
  }

protected:
  uint32_t max_value_;
  BitPackedIntPageWriter data_writer_;
  BitPackedIntPageWriter rlevel_writer_;
  BitPackedIntPageWriter dlevel_writer_;
};

} // namespace cstable
} // namespace stx

