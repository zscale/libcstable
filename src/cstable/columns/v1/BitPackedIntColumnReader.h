/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _FNORD_CSTABLE_UINT16COLUMNREADER_H
#define _FNORD_CSTABLE_UINT16COLUMNREADER_H
#include <stx/stdtypes.h>
#include <stx/util/binarymessagereader.h>
#include <stx/util/BitPackDecoder.h>
#include <cstable/columns/v1/ColumnReader.h>

namespace stx {
namespace cstable {
namespace v1 {

class BitPackedIntColumnReader : public ColumnReader {
public:

  BitPackedIntColumnReader(
      uint64_t r_max,
      uint64_t d_max,
      void* data,
      size_t size);

  bool next(uint64_t* rep_level, uint64_t* def_level, uint32_t* data);

  bool next(
      uint64_t* rep_level,
      uint64_t* def_level,
      void** data,
      size_t* data_len) override;

  msg::FieldType type() const override {
    return msg::FieldType::UINT32;
  }

  ColumnEncoding storageType() const override {
    return ColumnEncoding::UINT32_BITPACKED;
  }

protected:
  uint32_t max_value_;
  util::BitPackDecoder data_reader_;
  uint32_t cur_val_;
};

} // namespace v1
} // namespace cstable
} // namespace stx

#endif
