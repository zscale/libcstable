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
#include <stx/exception.h>
#include <stx/autoref.h>
#include <stx/protobuf/MessageObject.h>
#include <cstable/BinaryFormat.h>
#include <cstable/ColumnConfig.h>
#include <cstable/PageManager.h>
#include <cstable/BitPackedIntPageWriter.h>

namespace stx {
namespace cstable {

class ColumnWriter : public RefCounted {
public:
  ColumnWriter(size_t r_max, size_t d_max);

  virtual void addNull(uint64_t rep_level, uint64_t def_level) = 0;
  virtual void addDatum(
      uint64_t rep_level,
      uint64_t def_level,
      const void* data,
      size_t size) = 0;

  virtual ColumnType type() const = 0;
  virtual msg::FieldType fieldType() const = 0;

  size_t maxRepetitionLevel() const;
  size_t maxDefinitionLevel() const;

protected:
  size_t r_max_;
  size_t d_max_;
};

class DefaultColumnWriter : public ColumnWriter {
public:

  DefaultColumnWriter(
      ColumnConfig config,
      RefPtr<PageManager> page_mgr,
      RefPtr<Buffer> meta_buf,
      RefPtr<Buffer> rlevel_meta_buf,
      RefPtr<Buffer> dlevel_meta_buf);

  void addNull(uint64_t rep_level, uint64_t def_level) override;

  void addDatum(
      uint64_t rep_level,
      uint64_t def_level,
      const void* data,
      size_t size) override;

  ColumnType type() const override {
    return ColumnType::UINT32_BITPACKED;
  }

  msg::FieldType fieldType() const override {
    return msg::FieldType::UINT32;
  }

protected:
  ColumnConfig config_;
  BitPackedIntPageWriter rlevel_writer_;
  BitPackedIntPageWriter dlevel_writer_;
};
} // namespace cstable
} // namespace stx

