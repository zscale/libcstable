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
    RefPtr<Buffer> meta_buf,
    RefPtr<Buffer> rlevel_meta_buf,
    RefPtr<Buffer> dlevel_meta_buf) :
    DefaultColumnWriter(
        config,
        page_mgr,
        meta_buf,
        rlevel_meta_buf,
        dlevel_meta_buf) {
  switch (config_.storage_type) {

    case ColumnType::UINT64_PLAIN:
      data_writer_ = mkScoped(new UInt64PageWriter(page_mgr));
      break;

    default:
      RAISEF(
          kIllegalArgumentError,
          "invalid storage type for unsigned integer column $0",
          config_.column_name);

  }
}

} // namespace cstable
} // namespace stx

