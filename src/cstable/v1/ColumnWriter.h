/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _FNORD_CSTABLE_COLUMNWRITER_H
#define _FNORD_CSTABLE_COLUMNWRITER_H
#include <stx/stdtypes.h>
#include <stx/exception.h>
#include <stx/autoref.h>
#include <stx/util/binarymessagewriter.h>
#include <stx/util/BitPackEncoder.h>
#include <stx/protobuf/MessageObject.h>
#include <cstable/BinaryFormat.h>
#include <cstable/ColumnWriter.h>

namespace stx {
namespace cstable {
namespace v1 {

class ColumnWriter : public cstable::ColumnWriter {
public:

  ColumnWriter(size_t r_max, size_t d_max);

  void addNull(uint64_t rep_level, uint64_t def_level) override;

  void write(void* buf, size_t buf_len);
  virtual void commit();

  virtual size_t bodySize() const;

protected:
  virtual size_t size() const = 0;
  virtual void write(util::BinaryMessageWriter* writer) = 0;

  size_t r_max_;
  size_t d_max_;
  util::BitPackEncoder rlvl_writer_;
  util::BitPackEncoder dlvl_writer_;
  size_t num_vals_;
};

} // namespace v1
} // namespace cstable
} // namespace stx

#endif
