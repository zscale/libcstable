/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2015 Paul Asmuth
 *   Copyright (c) 2015 Laura Schlimmer, FnordCorp B.V.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <stx/stdtypes.h>
#include <stx/io/file.h>
#include <stx/test/unittest.h>
#include <cstable/v1/CSTableWriter.h>
#include <cstable/v1/BitPackedIntColumnWriter.h>
#include <cstable/v1/BooleanColumnWriter.h>
#include <cstable/v1/DoubleColumnWriter.h>
#include <cstable/v1/LEB128ColumnWriter.h>
#include <cstable/v1/StringColumnWriter.h>
#include <cstable/v1/UInt32ColumnWriter.h>
#include <cstable/v1/UInt64ColumnWriter.h>
#include <cstable/CSTableWriter.h>
#include <cstable/CSTableReader.h>

using namespace stx;

UNIT_TEST(CSTableTest);

TEST_CASE(CSTableTest, TestV1CSTableContainer, [] () {
  String filename = "/tmp/__fnord__cstabletest1.cstable";
  auto num_records = 10;

  FileUtil::rm(filename);

  Vector<cstable::ColumnConfig> columns;
  columns.emplace_back(cstable::ColumnConfig {
    .column_name = "key1",
    .storage_type = cstable::ColumnType::UINT32_BITPACKED,
    .logical_type = msg::FieldType::UINT32,
    .rlevel_max = 10,
    .dlevel_max = 10
  });

  columns.emplace_back(cstable::ColumnConfig {
    .column_name = "key2",
    .storage_type = cstable::ColumnType::UINT32_BITPACKED,
    .logical_type = msg::FieldType::UINT32,
    .rlevel_max = 10,
    .dlevel_max = 10
  });

  auto tbl_writer = cstable::CSTableWriter::createFile(
      filename,
      cstable::BinaryFormatVersion::v0_1_0,
      columns);

  tbl_writer->commit();

  auto fhash = SHA1::compute(FileUtil::read(filename));
  //EXPECT_EQ(fhash.toString(), "4dabd2586ed0061d29304979fcc9ffae82112804");

  auto tbl_reader = cstable::CSTableReader::openFile(filename);
  EXPECT_EQ(tbl_reader->numRecords(), num_records);
  EXPECT_EQ(tbl_reader->hasColumn("key1"), true);
  EXPECT_EQ(tbl_reader->hasColumn("key2"), true);
});

TEST_CASE(CSTableTest, TestV1CSTableColumnWriterReader, [] () {
  String filename = "/tmp/__fnord__cstabletest2.cstable";
  auto num_records = 4000;
  auto rep_max = 1;
  auto def_max = 1;

  FileUtil::rm(filename);

  auto bitpacked_writer = mkRef(
    new cstable::v1::BitPackedIntColumnWriter(rep_max, def_max));
  auto boolean_writer = mkRef(
    new cstable::v1::BooleanColumnWriter(rep_max, def_max));
  auto double_writer = mkRef(
    new cstable::v1::DoubleColumnWriter(rep_max, def_max));
  auto leb128_writer = mkRef(
    new cstable::v1::LEB128ColumnWriter(rep_max, def_max));
  auto string_writer = mkRef(
    new cstable::v1::StringColumnWriter(rep_max, def_max));
  auto uint32_writer = mkRef(
    new cstable::v1::UInt32ColumnWriter(rep_max, def_max));
  auto uint64_writer = mkRef(
    new cstable::v1::UInt64ColumnWriter(rep_max, def_max));

  for (auto i = 0; i < num_records; i++) {
    uint8_t boolean_v = i % 2;
    double double_v = i * 1.1;
    uint64_t uint64_v = static_cast<uint64_t>(i);
    const String string_v = "value";

    bitpacked_writer->addDatum(rep_max, def_max, &i, sizeof(i));
    boolean_writer->addDatum(rep_max, def_max, &boolean_v, sizeof(boolean_v));
    double_writer->addDatum(rep_max, def_max, &double_v, sizeof(double_v));
    leb128_writer->addDatum(rep_max, def_max, &i, sizeof(i));
    string_writer->addDatum(rep_max, def_max, string_v.data(), string_v.size());
    uint32_writer->addDatum(rep_max, def_max, &i, sizeof(i));
    uint64_writer->addDatum(rep_max, def_max, &uint64_v, sizeof(uint64_v));
  }

  cstable::v1::CSTableWriter tbl_writer(filename, num_records);
  tbl_writer.addColumn("bitpacked", bitpacked_writer.get());
  tbl_writer.addColumn("boolean", boolean_writer.get());
  tbl_writer.addColumn("double", double_writer.get());
  tbl_writer.addColumn("leb128", leb128_writer.get());
  tbl_writer.addColumn("string", string_writer.get());
  tbl_writer.addColumn("uint32", uint32_writer.get());
  tbl_writer.addColumn("uint64", uint64_writer.get());
  tbl_writer.commit();

  auto fhash = SHA1::compute(FileUtil::read(filename));
  EXPECT_EQ(fhash.toString(), "f602dfda7577e72a3efcc3907adee4675ae1eab1");

  auto tbl_reader = cstable::CSTableReader::openFile(filename);
  auto bitpacked_reader = tbl_reader->getColumnReader("bitpacked");
  auto boolean_reader = tbl_reader->getColumnReader("boolean");
  auto double_reader = tbl_reader->getColumnReader("double");
  auto leb128_reader = tbl_reader->getColumnReader("leb128");
  auto string_reader = tbl_reader->getColumnReader("string");
  auto uint32_reader = tbl_reader->getColumnReader("uint32");
  auto uint64_reader = tbl_reader->getColumnReader("uint64");

  EXPECT_EQ(bitpacked_reader->type() == msg::FieldType::UINT32, true);
  EXPECT_EQ(boolean_reader->type() == msg::FieldType::BOOLEAN, true);
  EXPECT_EQ(double_reader->type() == msg::FieldType::DOUBLE, true);
  EXPECT_EQ(leb128_reader->type() == msg::FieldType::UINT64, true);
  EXPECT_EQ(string_reader->type() == msg::FieldType::STRING, true);
  EXPECT_EQ(uint32_reader->type() == msg::FieldType::UINT32, true);
  EXPECT_EQ(uint64_reader->type() == msg::FieldType::UINT64, true);

  for (auto i = 0; i < num_records; i++) {
    uint64_t rep_level;
    uint64_t def_level;
    size_t size;
    void* data;

    EXPECT_EQ(
      bitpacked_reader->next(&rep_level, &def_level, &data, &size),
      true);
    EXPECT_EQ(*static_cast<uint32_t*>(data), i);

    EXPECT_EQ(
      boolean_reader->next(&rep_level, &def_level, &data, &size),
      true);
    EXPECT_EQ(*static_cast<uint8_t*>(data), i % 2);

    EXPECT_EQ(
      double_reader->next(&rep_level, &def_level, &data, &size),
      true);
    EXPECT_EQ(*static_cast<double*>(data), i * 1.1);

    EXPECT_EQ(
      leb128_reader->next(&rep_level, &def_level, &data, &size),
      true);
    EXPECT_EQ(*static_cast<uint64_t*>(data), i);

    EXPECT_EQ(
      string_reader->next(&rep_level, &def_level, &data, &size),
      true);
    EXPECT_EQ(String((const char*) data, size), "value");

    EXPECT_EQ(
      uint32_reader->next(&rep_level, &def_level, &data, &size),
      true);
    EXPECT_EQ(*static_cast<uint32_t*>(data), i);

    EXPECT_EQ(
      uint64_reader->next(&rep_level, &def_level, &data, &size),
      true);
    EXPECT_EQ(*static_cast<uint64_t*>(data), static_cast<uint64_t>(i));
  }
});

//TEST_CASE(CSTableTest, TestV2CSTableContainer, [] () {
//  String filename = "/tmp/__fnord__cstabletest2.cstable";
//  FileUtil::rm(filename);
//
//  Vector<cstable::ColumnConfig> columns;
//  columns.emplace_back(cstable::ColumnConfig {
//    .column_id = 1,
//    .column_name = "mycol",
//    .storage_type = cstable::ColumnType::UINT32_BITPACKED,
//    .logical_type = msg::FieldType::UINT32,
//    .rlevel_max = 0,
//    .dlevel_max = 0
//  });
//
//  auto tbl_writer = cstable::CSTableWriter::createFile(filename, columns);
//  tbl_writer->commit();
//
//  EXPECT_EQ(FileUtil::size(filename), 512);
//});

TEST_CASE(CSTableTest, TestV2UInt64Plain, [] () {
  String filename = "/tmp/__fnord__cstabletest2.cstable";
  FileUtil::rm(filename);

  Vector<cstable::ColumnConfig> columns;
  columns.emplace_back(cstable::ColumnConfig {
    .column_id = 1,
    .column_name = "mycol",
    .storage_type = cstable::ColumnType::UINT64_PLAIN,
    .logical_type = msg::FieldType::UINT64,
    .rlevel_max = 0,
    .dlevel_max = 0
  });

  {
    auto tbl_writer = cstable::CSTableWriter::createFile(filename, columns);
    auto mycol = tbl_writer->getColumnByName("mycol");

    for (size_t i = 1; i < 10000; ++i) {
      mycol->writeUnsignedInt(0, 0, 23 * i);
      mycol->writeUnsignedInt(0, 0, 42 * i);
      mycol->writeUnsignedInt(0, 0, 17 * i);
    }

    tbl_writer->commit();
  }

  {
    auto tbl_reader = cstable::CSTableReader::openFile(filename);
    auto mycol = tbl_reader->getColumnByName("mycol");
  }
});

