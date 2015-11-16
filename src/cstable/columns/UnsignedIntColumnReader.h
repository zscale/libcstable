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
#include <cstable/ColumnReader.h>

namespace stx {
namespace cstable {

class UnsignedIntColumnReader : public ColumnReader {
public:

  UnsignedIntColumnReader(
      ColumnConfig config,
      RefPtr<PageManager> page_mgr,
      PageIndexReader* page_idx);

  msg::FieldType type() const override;
  ColumnEncoding storageType() const override;

  uint64_t maxRepetitionLevel() const override;
  uint64_t maxDefinitionLevel() const override;

  bool next(
      uint64_t* rep_level,
      uint64_t* def_level,
      void** data,
      size_t* data_len) override;

  uint64_t nextRepetitionLevel() override;

  bool eofReached() const override;

};

} // namespace cstable
} // namespace stx

