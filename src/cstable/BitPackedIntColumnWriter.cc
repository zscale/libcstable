/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/BitPackedIntColumnWriter.h>

namespace stx {
namespace cstable {

BitPackedIntColumnWriter::BitPackedIntColumnWriter(
    RefPtr<PageManager> page_mgr,
    RefPtr<Buffer> meta_buf,
    RefPtr<Buffer> rlevel_meta_buf,
    RefPtr<Buffer> dlevel_meta_buf,
    uint64_t r_max,
    uint64_t d_max,
    uint32_t max_value /* = 0xffffffff */) :
    ColumnWriter(r_max, d_max),
    data_writer_(page_mgr, max_value),
    rlevel_writer_(page_mgr, r_max),
    dlevel_writer_(page_mgr, d_max) {}

void BitPackedIntColumnWriter::addNull(uint64_t rep_level, uint64_t def_level) {
  rlevel_writer_.addDatum(rep_level);
  dlevel_writer_.addDatum(def_level);
}

void BitPackedIntColumnWriter::addDatum(
    uint64_t rep_level,
    uint64_t def_level,
    const void* data,
    size_t size) {
  RDCHECK(size == sizeof(uint32_t), "invalid value size (must be uint32)");
  addDatum(rep_level, def_level, *((uint32_t*) data));
}

void BitPackedIntColumnWriter::addDatum(
    uint64_t rep_level,
    uint64_t def_level,
    uint32_t value) {
  rlevel_writer_.addDatum(rep_level);
  dlevel_writer_.addDatum(def_level);
  data_writer_.addDatum(value);
}

//void commit();

} // namespace cstable
} // namespace stx

