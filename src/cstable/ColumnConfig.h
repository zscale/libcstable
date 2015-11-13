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
#include <stx/io/file.h>
#include <stx/protobuf/MessageObject.h>
#include <cstable/BinaryFormat.h>

namespace stx {
namespace cstable {

struct ColumnConfig {
  uint32_t column_id;
  String column_name;
  cstable::ColumnType storage_type;
  msg::FieldType logical_type;
  size_t rlevel_max;
  size_t dlevel_max;
};

} // namespace cstable
} // namespace stx

