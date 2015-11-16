/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stx/stdtypes.h>
#include <stx/util/binarymessagewriter.h>
#include <stx/util/BitPackEncoder.h>
#include <cstable/BinaryFormat.h>
#include <cstable/columns/v1/ColumnWriter.h>

namespace stx {
namespace cstable {
namespace v1 {

class DoubleColumnWriter : public ColumnWriter {
public:

  DoubleColumnWriter(
      uint64_t r_max,
      uint64_t d_max);

  void addDatum(
      uint64_t rep_level,
      uint64_t def_level,
      const void* data,
      size_t size) override;

  void addDatum(uint64_t rep_level, uint64_t def_level, double value);

  ColumnEncoding type() const override {
    return ColumnEncoding::FLOAT_IEEE754;
  }

  msg::FieldType fieldType() const override {
    return msg::FieldType::DOUBLE;
  }

protected:
  size_t size() const override;
  void write(util::BinaryMessageWriter* writer) override;

  util::BinaryMessageWriter data_writer_;
};

} // namespace v1
} // namespace cstable
} // namespace stx
