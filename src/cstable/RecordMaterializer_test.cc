/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stx/test/unittest.h"
#include "stx/protobuf/MessageEncoder.h"
#include "stx/protobuf/MessagePrinter.h"
#include "cstable/CSTableReader.h"
#include "cstable/RecordMaterializer.h"
#include "cstable/v1/StringColumnReader.h"
#include "cstable/RecordShredder.h"
#include "cstable/RecordSchema.h"

using namespace stx;
using namespace stx::cstable;
using namespace stx::msg;

UNIT_TEST(RecordMaterializerTest);

