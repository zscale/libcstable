/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/columns/v1/UInt64ColumnWriter.h>


namespace cstable {
namespace v1 {

UInt64ColumnWriter::UInt64ColumnWriter(
    uint64_t r_max,
    uint64_t d_max,
    ColumnType type) :
    ColumnWriter(r_max, d_max),
    type_(type) {}

void UInt64ColumnWriter::addDatum(
    uint64_t rep_level,
    uint64_t def_level,
    const void* data,
    size_t size) {
  if (size != sizeof(uint64_t)) {
    RAISE(kIllegalArgumentError, "size != sizeof(uint64_t)");
  }

  addDatum(rep_level, def_level, *((const uint64_t*) data));
}

void UInt64ColumnWriter::addDatum(
    uint64_t rep_level,
    uint64_t def_level,
    uint64_t value) {
  rlvl_writer_.encode(rep_level);
  dlvl_writer_.encode(def_level);
  data_writer_.appendUInt64(value);
  ++num_vals_;
}

void UInt64ColumnWriter::write(stx::util::BinaryMessageWriter* writer) {
  writer->append(data_writer_.data(), data_writer_.size());
}

size_t UInt64ColumnWriter::size() const {
  return sizeof(uint64_t) + data_writer_.size();
}

} // namespace v1
} // namespace cstable

