/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/ColumnConfig.h>

namespace stx {
namespace cstable {

void ColumnConfig::encode(OutputStream* os) const {
  os->appendVarUInt((uint8_t) logical_type);
  os->appendVarUInt((uint8_t) storage_type);
  os->appendVarUInt(column_id);
  os->appendLenencString(column_name);
  os->appendVarUInt(rlevel_max);
  os->appendVarUInt(dlevel_max);
}

void ColumnConfig::decode(InputStream* is) {
  logical_type = (msg::FieldType) is->readVarUInt();
  storage_type = (cstable::ColumnType) is->readVarUInt();
  column_id = is->readVarUInt();
  column_name = is->readLenencString();
  rlevel_max = is->readVarUInt();
  dlevel_max = is->readVarUInt();
}


} // namespace cstable
} // namespace stx

