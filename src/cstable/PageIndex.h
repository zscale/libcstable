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
#include <stx/buffer.h>
#include <stx/io/file.h>
#include <cstable/BinaryFormat.h>

namespace stx {
namespace cstable {

class PageIndex : public RefCounted {
public:

  PageIndex(BinaryFormatVersion version);

protected:
  BinaryFormatVersion version_;
};

} // namespace cstable
} // namespace stx

