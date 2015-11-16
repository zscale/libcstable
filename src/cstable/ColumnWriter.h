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
#include <cstable/PageManager.h>
#include <cstable/PageIndex.h>
#include <cstable/columns/UInt64PageWriter.h>

namespace stx {
namespace cstable {

class ColumnWriter : public RefCounted {
public:

  ColumnWriter(size_t r_max, size_t d_max);

  virtual void writeNull(uint64_t rlvl, uint64_t dlvl) = 0;

  virtual void writeUnsignedInt(
      uint64_t rlvl,
      uint64_t dlvl,
      uint64_t value) = 0;

  virtual void writeSignedInt(
      uint64_t rlvl,
      uint64_t dlvl,
      int64_t value) = 0;

  virtual void writeDouble(
      uint64_t rlvl,
      uint64_t dlvl,
      double value) = 0;

  void writeString(
      uint64_t rlvl,
      uint64_t dlvl,
      const String& value);

  virtual void writeString(
      uint64_t rlvl,
      uint64_t dlvl,
      const char* data,
      size_t size) = 0;

  /**
   * Deprecated, alias for writeNull
   */
  virtual void addNull(uint64_t rep_level, uint64_t def_level) = 0;

  /**
   * Deprecated, do not use
   */
  virtual void addDatum(
      uint64_t rep_level,
      uint64_t def_level,
      const void* data,
      size_t size) = 0;

  virtual ColumnEncoding type() const = 0;
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
      RefPtr<PageIndex> page_idx);

  void writeNull(uint64_t rlvl, uint64_t dlvl) override;

  /**
   * Deprecated, alias for writeNull
   */
  void addNull(uint64_t rep_level, uint64_t def_level) override;

  /**
   * Deprecated, do not use
   */
  void addDatum(
      uint64_t rep_level,
      uint64_t def_level,
      const void* data,
      size_t size) override;

  ColumnEncoding type() const override {
    return ColumnEncoding::UINT32_BITPACKED;
  }

  msg::FieldType fieldType() const override {
    return msg::FieldType::UINT32;
  }

protected:
  ColumnConfig config_;
  ScopedPtr<UnsignedIntPageWriter> rlevel_writer_;
  ScopedPtr<UnsignedIntPageWriter> dlevel_writer_;
};
} // namespace cstable
} // namespace stx

