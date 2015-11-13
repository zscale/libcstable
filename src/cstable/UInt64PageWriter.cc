/**
 * This file is part of the "libcstable" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libcstable is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <cstable/UInt64PageWriter.h>

namespace stx {
namespace cstable {

UInt64PageWriter::UInt64PageWriter(
    RefPtr<PageManager> page_mgr) {}

void UInt64PageWriter::writeValue(uint64_t value) {

}

} // namespace cstable
} // namespace stx

