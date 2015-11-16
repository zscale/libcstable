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

bool UnsignedIntColumnReader::readBoolean(
    uint64_t* rlvl,
    uint64_t* dlvl,
    bool* value) {
  uint64_t tmp;
  if (readUnsignedInt(rlvl, dlvl, &tmp)) {
    *value = tmp > 1;
    return true;
  } else {
    *value = false;
    return false;
  }
}

bool UnsignedIntColumnReader::readUnsignedInt(
    uint64_t* rlvl,
    uint64_t* dlvl,
    uint64_t* value) {
  *rlvl = rlevel_reader_->readUnsignedInt();
  *dlvl = dlevel_reader_->readUnsignedInt();

  if (*dlvl == config_.dlevel_max) {
    *value = data_reader_->readUnsignedInt();
    return true;
  } else {
    *value = 0;
    return false;
  }
}

bool UnsignedIntColumnReader::readSignedInt(
    uint64_t* rlvl,
    uint64_t* dlvl,
    int64_t* value) {
  uint64_t tmp;
  if (readUnsignedInt(rlvl, dlvl, &tmp)) {
    *value = tmp;
    return true;
  } else {
    *value = 0;
    return false;
  }
}

bool UnsignedIntColumnReader::readFloat(
    uint64_t* rlvl,
    uint64_t* dlvl,
    double* value) {
  uint64_t tmp;
  if (readUnsignedInt(rlvl, dlvl, &tmp)) {
    *value = tmp;
    return true;
  } else {
    *value = 0;
    return false;
  }
}

bool UnsignedIntColumnReader::readString(
    uint64_t* rlvl,
    uint64_t* dlvl,
    String* value) {
  uint64_t tmp;
  if (readUnsignedInt(rlvl, dlvl, &tmp)) {
    *value = StringUtil::toString(tmp);
    return true;
  } else {
    *value = "";
    return false;
  }
}

uint64_t UnsignedIntColumnReader::nextRepetitionLevel() {
  return 0;
}

bool UnsignedIntColumnReader::eofReached() const {
  return false;
}

ColumnType UnsignedIntColumnReader::type() const {
  return ColumnType::UNSIGNED_INT;
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

} // namespace cstable
} // namespace stx

