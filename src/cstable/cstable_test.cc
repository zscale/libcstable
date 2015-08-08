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
#include <cstable/CSTableWriter.h>
#include <cstable/CSTableReader.h>
#include <cstable/BitPackedIntColumnWriter.h>
#include <cstable/BooleanColumnWriter.h>
#include <cstable/DoubleColumnWriter.h>
#include <cstable/LEB128ColumnWriter.h>
#include <cstable/StringColumnWriter.h>
#include <cstable/UInt32ColumnWriter.h>
#include <cstable/UInt64ColumnWriter.h>


using namespace stx;

UNIT_TEST(CSTableTest);

TEST_CASE(CSTableTest, TestCSTableContainer, [] () {
  const String& filename = "/tmp/__fnord__cstabletest1.cstable";
  const uint64_t num_records = 10;

  FileUtil::rm(filename);

  RefPtr<cstable::BitPackedIntColumnWriter> column_writer = mkRef(
    new cstable::BitPackedIntColumnWriter(10, 10));
  auto tbl_writer = cstable::CSTableWriter(
    filename,
    num_records);

  tbl_writer.addColumn("key1", column_writer.get());
  tbl_writer.addColumn("key2", column_writer.get());
  tbl_writer.commit();

  cstable::CSTableReader tbl_reader(filename);
  EXPECT_EQ(tbl_reader.numRecords(), num_records);
  EXPECT_EQ(tbl_reader.hasColumn("key1"), true);
  EXPECT_EQ(tbl_reader.hasColumn("key2"), true);
});

TEST_CASE(CSTableTest, TestCSTableColumnWriterReader, [] () {
  const String& filename = "/tmp/__fnord__cstabletest2.cstable";
  const uint64_t num_records = 4000;
  uint64_t rep_max = 1;
  uint64_t def_max = 1;

  FileUtil::rm(filename);

  RefPtr<cstable::BitPackedIntColumnWriter> bitpacked_writer = mkRef(
    new cstable::BitPackedIntColumnWriter(rep_max, def_max));
  RefPtr<cstable::BooleanColumnWriter> boolean_writer = mkRef(
    new cstable::BooleanColumnWriter(rep_max, def_max));
  RefPtr<cstable::DoubleColumnWriter> double_writer = mkRef(
    new cstable::DoubleColumnWriter(rep_max, def_max));
  RefPtr<cstable::LEB128ColumnWriter> leb128_writer = mkRef(
    new cstable::LEB128ColumnWriter(rep_max, def_max));
  RefPtr<cstable::StringColumnWriter> string_writer = mkRef(
    new cstable::StringColumnWriter(rep_max, def_max));
  RefPtr<cstable::UInt32ColumnWriter> uint32_writer = mkRef(
    new cstable::UInt32ColumnWriter(rep_max, def_max));
  RefPtr<cstable::UInt64ColumnWriter> uint64_writer = mkRef(
    new cstable::UInt64ColumnWriter(rep_max, def_max));

  uint8_t boolean_v;
  double double_v;
  const String& string_v = "value";
  uint64_t uint64_v = 0;

  for (auto i = 0; i < num_records; i++) {
    boolean_v = i % 2;
    double_v = i * 1.1;
    uint64_v = static_cast<uint64_t>(i);
    bitpacked_writer->addDatum(rep_max, def_max, &i, sizeof(i));
    boolean_writer->addDatum(rep_max, def_max, &boolean_v, sizeof(boolean_v));
    double_writer->addDatum(rep_max, def_max, &double_v, sizeof(double_v));
    leb128_writer->addDatum(rep_max, def_max, &i, sizeof(i));
    string_writer->addDatum(rep_max, def_max, &string_v, sizeof(string_v));
    uint32_writer->addDatum(rep_max, def_max, &i, sizeof(i));
    uint64_writer->addDatum(rep_max, def_max, &uint64_v, sizeof(uint64_v));
  }

  auto tbl_writer = cstable::CSTableWriter(
    filename,
    num_records);
  tbl_writer.addColumn("bitpacked", bitpacked_writer.get());
  tbl_writer.addColumn("boolean", boolean_writer.get());
  tbl_writer.addColumn("double", double_writer.get());
  tbl_writer.addColumn("leb128", leb128_writer.get());
  tbl_writer.addColumn("string", string_writer.get());
  tbl_writer.addColumn("uint32", uint32_writer.get());
  tbl_writer.addColumn("uint64", uint64_writer.get());
  tbl_writer.commit();

  cstable::CSTableReader tbl_reader(filename);
  RefPtr<cstable::ColumnReader> bitpacked_reader = tbl_reader.getColumnReader(
    "bitpacked");
  RefPtr<cstable::ColumnReader> boolean_reader = tbl_reader.getColumnReader(
    "boolean");
  RefPtr<cstable::ColumnReader> double_reader = tbl_reader.getColumnReader(
    "double");
  RefPtr<cstable::ColumnReader> leb128_reader = tbl_reader.getColumnReader(
    "leb128");
  RefPtr<cstable::ColumnReader> string_reader = tbl_reader.getColumnReader(
    "string");
  RefPtr<cstable::ColumnReader> uint32_reader = tbl_reader.getColumnReader(
    "uint32");
  RefPtr<cstable::ColumnReader> uint64_reader = tbl_reader.getColumnReader(
    "uint64");


  EXPECT_EQ(bitpacked_reader->type() == msg::FieldType::UINT32, true);
  EXPECT_EQ(boolean_reader->type() == msg::FieldType::BOOLEAN, true);
  EXPECT_EQ(double_reader->type() == msg::FieldType::DOUBLE, true);
  EXPECT_EQ(leb128_reader->type() == msg::FieldType::UINT64, true);
  EXPECT_EQ(string_reader->type() == msg::FieldType::STRING, true);
  EXPECT_EQ(uint32_reader->type() == msg::FieldType::UINT32, true);
  EXPECT_EQ(uint64_reader->type() == msg::FieldType::UINT64, true);

  uint64_t rep_level;
  uint64_t def_level;
  size_t size;
  void* data;
  uint32_t* bitpacked_val;
  uint8_t* boolean_val;
  double* double_val;
  uint64_t* leb128_val;
  String* string_val;
  uint32_t* uint32_val;
  uint64_t* uint64_val;

  for (auto i = 0; i < num_records; i++) {
    EXPECT_EQ(
      bitpacked_reader->next(&rep_level, &def_level, &data, &size),
      true);
    bitpacked_val = static_cast<uint32_t*>(data);
    EXPECT_EQ(*bitpacked_val, i);

    EXPECT_EQ(
      boolean_reader->next(&rep_level, &def_level, &data, &size),
      true);
    boolean_val = static_cast<uint8_t*>(data);
    EXPECT_EQ(*boolean_val, i % 2);

    EXPECT_EQ(
      double_reader->next(&rep_level, &def_level, &data, &size),
      true);
    double_val = static_cast<double*>(data);
    EXPECT_EQ(*double_val, i * 1.1);

    EXPECT_EQ(
      leb128_reader->next(&rep_level, &def_level, &data, &size),
      true);
    leb128_val = static_cast<uint64_t*>(data);
    EXPECT_EQ(*leb128_val, i);

    EXPECT_EQ(
      string_reader->next(&rep_level, &def_level, &data, &size),
      true);
    string_val = static_cast<String*>(data);
    EXPECT_EQ(*string_val, string_v);

    EXPECT_EQ(
      uint32_reader->next(&rep_level, &def_level, &data, &size),
      true);
    uint32_val = static_cast<uint32_t*>(data);
    EXPECT_EQ(*uint32_val, i);

    EXPECT_EQ(
      uint64_reader->next(&rep_level, &def_level, &data, &size),
      true);
    uint64_val = static_cast<uint64_t*>(data);
    EXPECT_EQ(*uint64_val, static_cast<uint64_t>(i));
  }
});




TEST_CASE(CSTableTest, TestBooleanColumnWriterReader, [] () {
  const String& filename = "/tmp/__fnord__cstabletest3.cstable";
  const uint64_t num_records = 100;

  FileUtil::rm(filename);

  RefPtr<cstable::BooleanColumnWriter> column_writer = mkRef(
    new cstable::BooleanColumnWriter(1, 1));

  uint8_t v = 1;
  for (uint32_t i = 0; i < 10; i++) {
    column_writer->addDatum(1, 1, &v, sizeof(v));
  }


  auto tbl_writer = cstable::CSTableWriter(
    filename,
    num_records);
  tbl_writer.addColumn("key1", column_writer.get());
  tbl_writer.commit();


  cstable::CSTableReader tbl_reader(filename);
  RefPtr<cstable::ColumnReader> column_reader = tbl_reader.getColumnReader("key1");

  EXPECT_EQ(column_reader->type() == msg::FieldType::BOOLEAN, true);

uint64_t rep_level;
  uint64_t def_level;
  void* data;
  uint8_t* val;
  size_t size;

  for (uint32_t i = 0; i < 10; i++) {
    EXPECT_EQ(column_reader->next(&rep_level, &def_level, &data, &size), true);
    val = static_cast<uint8_t*>(data);
    EXPECT_EQ(size, sizeof(v));
    EXPECT_EQ(*val, v);
  }
});

TEST_CASE(CSTableTest, TestDoubleColumnWriterReader, [] () {
  const String& filename = "/tmp/__fnord__cstabletest4.cstable";
  const uint64_t num_records = 100;

  FileUtil::rm(filename);

  RefPtr<cstable::DoubleColumnWriter> column_writer = mkRef(
    new cstable::DoubleColumnWriter(1, 1));

  double v = 1.1;
  for (uint32_t i = 0; i < 10; i++) {
    column_writer->addDatum(1, 1, &v, sizeof(v));
  }


  auto tbl_writer = cstable::CSTableWriter(
    filename,
    num_records);
  tbl_writer.addColumn("key1", column_writer.get());
  tbl_writer.commit();


  cstable::CSTableReader tbl_reader(filename);
  RefPtr<cstable::ColumnReader> column_reader = tbl_reader.getColumnReader("key1");

  EXPECT_EQ(column_reader->type() == msg::FieldType::DOUBLE, true);

  uint64_t rep_level;
  uint64_t def_level;
  void* data;
  double* val;
  size_t size;

  for (uint32_t i = 0; i < 10; i++) {
    EXPECT_EQ(column_reader->next(&rep_level, &def_level, &data, &size), true);
    val = static_cast<double*>(data);
    EXPECT_EQ(size, sizeof(v));
    EXPECT_EQ(*val, v);
  }
});

TEST_CASE(CSTableTest, TestLEB128ColumnWriterReader, [] () {
  const String& filename = "/tmp/__fnord__cstabletest5.cstable";
  const uint64_t num_records = 100;

  FileUtil::rm(filename);

  RefPtr<cstable::LEB128ColumnWriter> column_writer = mkRef(
    new cstable::LEB128ColumnWriter(1, 1));

  uint64_t v = 10;
  for (uint32_t i = 0; i < 10; i++) {
    column_writer->addDatum(1, 1, &v, sizeof(v));
  }


  auto tbl_writer = cstable::CSTableWriter(
    filename,
    num_records);
  tbl_writer.addColumn("key1", column_writer.get());
  tbl_writer.commit();


  cstable::CSTableReader tbl_reader(filename);
  RefPtr<cstable::ColumnReader> column_reader = tbl_reader.getColumnReader("key1");

  EXPECT_EQ(column_reader->type() == msg::FieldType::UINT64, true);

  uint64_t rep_level;
  uint64_t def_level;
  void* data;
  uint64_t* val;
  size_t size;

  for (uint32_t i = 0; i < 10; i++) {
    EXPECT_EQ(column_reader->next(&rep_level, &def_level, &data, &size), true);
    val = static_cast<uint64_t*>(data);
    EXPECT_EQ(size, sizeof(v));
    EXPECT_EQ(*val, v);
  }
});


TEST_CASE(CSTableTest, TestStringColumnWriterReader, [] () {
  const String& filename = "/tmp/__fnord__cstabletest6.cstable";
  const uint64_t num_records = 100;

  FileUtil::rm(filename);

  RefPtr<cstable::StringColumnWriter> column_writer = mkRef(
    new cstable::StringColumnWriter(1, 1));

  const String& v = "value";
  for (uint32_t i = 0; i < 10; i++) {
    column_writer->addDatum(1, 1, &v, sizeof(v));
  }


  auto tbl_writer = cstable::CSTableWriter(
    filename,
    num_records);
  tbl_writer.addColumn("key1", column_writer.get());
  tbl_writer.commit();


  cstable::CSTableReader tbl_reader(filename);
  RefPtr<cstable::ColumnReader> column_reader = tbl_reader.getColumnReader("key1");

  EXPECT_EQ(column_reader->type() == msg::FieldType::STRING, true);

  uint64_t rep_level;
  uint64_t def_level;
  void* data;
  String* val;
  size_t size;

  for (uint32_t i = 0; i < 10; i++) {
    EXPECT_EQ(column_reader->next(&rep_level, &def_level, &data, &size), true);
    val = static_cast<String*>(data);
    EXPECT_EQ(size, sizeof(v));
    EXPECT_EQ(*val, v);
  }
});

TEST_CASE(CSTableTest, TestUInt32ColumnWriterReader, [] () {
  const String& filename = "/tmp/__fnord__cstabletest7.cstable";
  const uint64_t num_records = 100;

  FileUtil::rm(filename);

  RefPtr<cstable::UInt32ColumnWriter> column_writer = mkRef(
    new cstable::UInt32ColumnWriter(1, 1));

  uint32_t v = 1;
  for (uint32_t i = 0; i < 10; i++) {
    column_writer->addDatum(1, 1, &v, sizeof(v));
  }


  auto tbl_writer = cstable::CSTableWriter(
    filename,
    num_records);
  tbl_writer.addColumn("key1", column_writer.get());
  tbl_writer.commit();


  cstable::CSTableReader tbl_reader(filename);
  RefPtr<cstable::ColumnReader> column_reader = tbl_reader.getColumnReader("key1");

  EXPECT_EQ(column_reader->type() == msg::FieldType::UINT32, true);

  uint64_t rep_level;
  uint64_t def_level;
  void* data;
  uint32_t* val;
  size_t size;

  for (uint32_t i = 0; i < 10; i++) {
    EXPECT_EQ(column_reader->next(&rep_level, &def_level, &data, &size), true);
    val = static_cast<uint32_t*>(data);
    EXPECT_EQ(size, sizeof(v));
    EXPECT_EQ(*val, v);
  }
});

TEST_CASE(CSTableTest, TestUInt64ColumnWriterReader, [] () {
  const String& filename = "/tmp/__fnord__cstabletest8.cstable";
  const uint64_t num_records = 100;

  FileUtil::rm(filename);

  RefPtr<cstable::UInt64ColumnWriter> column_writer = mkRef(
    new cstable::UInt64ColumnWriter(1, 1));

  uint64_t v = 1;
  for (uint32_t i = 0; i < 10; i++) {
    column_writer->addDatum(1, 1, &v, sizeof(v));
  }


  auto tbl_writer = cstable::CSTableWriter(
    filename,
    num_records);
  tbl_writer.addColumn("key1", column_writer.get());
  tbl_writer.commit();


  cstable::CSTableReader tbl_reader(filename);
  RefPtr<cstable::ColumnReader> column_reader = tbl_reader.getColumnReader("key1");

  EXPECT_EQ(column_reader->type() == msg::FieldType::UINT64, true);

  uint64_t rep_level;
  uint64_t def_level;
  void* data;
  uint64_t* val;
  size_t size;

  for (uint32_t i = 0; i < 10; i++) {
    EXPECT_EQ(column_reader->next(&rep_level, &def_level, &data, &size), true);
    val = static_cast<uint64_t*>(data);
    EXPECT_EQ(size, sizeof(v));
    EXPECT_EQ(*val, v);
  }
});





