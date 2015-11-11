/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2014 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stx/stdtypes.h>
#include <stx/autoref.h>
#include <stx/protobuf/MessageObject.h>

namespace stx {
namespace cstable {

class ColumnReader : public RefCounted {
public:

  virtual msg::FieldType type() const = 0;
  virtual uint64_t maxRepetitionLevel() const = 0;
  virtual uint64_t maxDefinitionLevel() const = 0;

  virtual bool next(
      uint64_t* rep_level,
      uint64_t* def_level,
      void** data,
      size_t* data_len) = 0;

  virtual uint64_t nextRepetitionLevel() = 0;

  virtual bool eofReached() const = 0;

};

} // namespace cstable
} // namespace stx

