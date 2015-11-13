/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/ColumnWriter.h>

namespace stx {
namespace cstable {

ColumnWriter::ColumnWriter(
    size_t r_max,
    size_t d_max) :
    r_max_(r_max),
    d_max_(d_max) {}

size_t ColumnWriter::maxRepetitionLevel() const {
  return r_max_;
}

size_t ColumnWriter::maxDefinitionLevel() const {
  return d_max_;
}

DefaultColumnWriter::DefaultColumnWriter(
    ColumnConfig config,
    RefPtr<PageManager> page_mgr,
    RefPtr<Buffer> meta_buf,
    RefPtr<Buffer> rlevel_meta_buf,
    RefPtr<Buffer> dlevel_meta_buf) :
    ColumnWriter(config.rlevel_max, config.dlevel_max),
    config_(config),
    rlevel_writer_(new UInt64PageWriter(page_mgr, config_.rlevel_max)),
    dlevel_writer_(new UInt64PageWriter(page_mgr, config_.dlevel_max)) {}

void DefaultColumnWriter::addNull(uint64_t rep_level, uint64_t def_level) {
  rlevel_writer_->writeValue(rep_level);
  dlevel_writer_->writeValue(def_level);
}

void DefaultColumnWriter::addDatum(
    uint64_t rep_level,
    uint64_t def_level,
    const void* data,
    size_t size) {
  RAISE(kNotYetImplementedError);
}

} // namespace cstable
} // namespace stx

