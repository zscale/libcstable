# This file is part of the "FnordMetric" project
#   Copyright (c) 2014 Paul Asmuth, Google Inc.
#
# FnordMetric is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License v3.0. You should have received a
# copy of the GNU General Public License along with this program. If not, see
# <http://www.gnu.org/licenses/>.

# cstable
add_library(cstable STATIC
    BinaryFormat.cc
    columns/BitPackedIntPageWriter.cc
    columns/UInt64PageWriter.cc
    columns/UnsignedIntColumnWriter.cc
    ColumnWriter.cc
    CSTableReader.cc
    CSTableWriter.cc
    RecordMaterializer.cc
    RecordShredder.cc
    PageManager.cc
    PageIndex.cc
    RecordSchema.cc
    v1/BitPackedIntColumnReader.cc
    v1/BitPackedIntColumnWriter.cc
    v1/BooleanColumnReader.cc
    v1/BooleanColumnWriter.cc
    v1/CSTableWriter.cc
    v1/ColumnWriter.cc
    v1/DoubleColumnReader.cc
    v1/DoubleColumnWriter.cc
    v1/LEB128ColumnReader.cc
    v1/LEB128ColumnWriter.cc
    v1/StringColumnReader.cc
    v1/StringColumnWriter.cc
    v1/UInt32ColumnReader.cc
    v1/UInt32ColumnWriter.cc
    v1/UInt64ColumnReader.cc
    v1/UInt64ColumnWriter.cc)

add_executable(test-cstable cstable_test.cc)
target_link_libraries(test-cstable cstable stx-protobuf stx-json stx-base)

add_executable(cstable-tool cstable.cc)
target_link_libraries(cstable-tool cstable stx-protobuf stx-json stx-base)
