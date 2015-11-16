/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/columns/UnsignedIntColumnReader.h>
#include <cstable/columns/UInt64PageReader.h>

namespace stx {
namespace cstable {

UnsignedIntColumnReader::UnsignedIntColumnReader(
    ColumnConfig config,
    ScopedPtr<UnsignedIntPageReader> rlevel_reader,
    ScopedPtr<UnsignedIntPageReader> dlevel_reader,
    RefPtr<PageManager> page_mgr,
    PageIndexReader* page_idx) :
    config_(config),
    rlevel_reader_(std::move(rlevel_reader)),
    dlevel_reader_(std::move(dlevel_reader)) {
  PageIndexKey key = {
    .column_id = config.column_id,
    .entry_type = PageIndexEntryType::DATA
  };

  switch (config_.storage_type) {

    case ColumnEncoding::UINT64_PLAIN:
      data_reader_ = mkScoped(new UInt64PageReader(page_mgr));
      break;

    default:
      RAISEF(
          kIllegalArgumentError,
          "invalid storage type for unsigned integer column '$0'",
          config_.column_name);

  }

  page_idx->addPageReader(key, data_reader_.get());
}

msg::FieldType UnsignedIntColumnReader::type() const {
  return msg::FieldType::UINT64;
}

ColumnEncoding UnsignedIntColumnReader::storageType() const {
  return config_.storage_type;
}

uint64_t UnsignedIntColumnReader::maxRepetitionLevel() const {
  return config_.rlevel_max;
}

uint64_t UnsignedIntColumnReader::maxDefinitionLevel() const {
  return config_.dlevel_max;
}

bool UnsignedIntColumnReader::next(
    uint64_t* rep_level,
    uint64_t* def_level,
    void** data,
    size_t* data_len) {
  return false;
}

uint64_t UnsignedIntColumnReader::nextRepetitionLevel() {
  return 0;
}

bool UnsignedIntColumnReader::eofReached() const {
  return false;
}

} // namespace cstable
} // namespace stx

