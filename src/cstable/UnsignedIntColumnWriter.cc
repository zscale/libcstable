/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/UnsignedIntColumnWriter.h>

namespace stx {
namespace cstable {

UnsignedIntColumnWriter::UnsignedIntColumnWriter(
    ColumnConfig config,
    RefPtr<PageManager> page_mgr,
    RefPtr<PageIndex> page_idx) :
    DefaultColumnWriter(config, page_mgr, page_idx) {
  PageIndexKey key = {
    .column_id = config.column_id,
    .entry_type = PageIndexEntryType::DATA
  };

  switch (config_.storage_type) {

    case ColumnType::UINT64_PLAIN:
      data_writer_ = mkScoped(new UInt64PageWriter(key, page_mgr, page_idx));
      break;

    default:
      RAISEF(
          kIllegalArgumentError,
          "invalid storage type for unsigned integer column '$0'",
          config_.column_name);

  }
}

void UnsignedIntColumnWriter::writeUnsignedInt(
    uint64_t rlvl,
    uint64_t dlvl,
    uint64_t value) {
  if (rlevel_writer_.get()) {
    rlevel_writer_->writeValue(rlvl);
  }

  if (dlevel_writer_.get()) {
    dlevel_writer_->writeValue(dlvl);
  }

  data_writer_->writeValue(value);
}

void UnsignedIntColumnWriter::writeSignedInt(
    uint64_t rlvl,
    uint64_t dlvl,
    int64_t value) {
  if (value < 0) {
    value = 0;
  }

  writeUnsignedInt(rlvl, dlvl, (uint64_t) value);
}


void UnsignedIntColumnWriter::writeDouble(
    uint64_t rlvl,
    uint64_t dlvl,
    double value) {
  if (value < 0) {
    value = 0;
  }

  writeUnsignedInt(rlvl, dlvl, value);
}

void UnsignedIntColumnWriter::writeString(
    uint64_t rlvl,
    uint64_t dlvl,
    const char* data,
    size_t size) {
  uint64_t value = std::stoull(String(data, size));
  writeUnsignedInt(rlvl, dlvl, value);
}

} // namespace cstable
} // namespace stx

