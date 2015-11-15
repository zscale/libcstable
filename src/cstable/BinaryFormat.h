/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stx/io/inputstream.h>
#include <stx/io/outputstream.h>
#include <stx/protobuf/MessageObject.h>

namespace stx {
namespace cstable {

/**
 * // http://tools.ietf.org/html/rfc5234
 *
 * Assumptions:
 *   - aligned 512 write bytes to disk are atomic
 *
 * v0.1.x:
 *
 *   <cstable> :=
 *       <header>
 *       <body>
 *
 *   <header> :=
 *       %x23 %x17 %x23 %x17     // magic bytes
 *       %x01 %x00               // cstable file format version
 *       <uint64_t>              // flags
 *       <uint64_t>              // number of rows in the table
 *       <uint32_t>              // number of columns
 *       <column_header>*        // column header for each column
 *
 *   <column_header> :=
 *       <uint32_t>              // column type
 *       <uint32_t>              // length of the column name
 *       <char>*                 // column name
 *       <uint32_t>              // max repetition level
 *       <uint32_t>              // max definition level
 *       <uint64_t>              // column data start offset
 *       <uint64_t>              // column data size
 *
 * v0.2.0:
 *
 *   <cstable> :=
 *       <header>
 *       ( <index_page> / data_page )*
 *
 *   <header> :=
 *       %x23 %x17 %x23 %x17     // magic bytes
 *       %x02 %x00               // cstable file format version
 *       <uint64_t>              // flags
 *       <metablock>             // metablock a
 *       <metablock>             // metablock b
 *       <128 bytes>             // reserved
 *       <uint32_t>              // number of columns
 *       <column_info>*          // column info for each column
 *       %x00*                   // padding to next 512 byte boundary
 *
 *   <metablock> :=
 *       <uint64_t>              // transaction id
 *       <uint64_t>              // number of rows
 *       <uint64_t>              // head index page offset
 *       <uint32_t>              // head index page size
 *       <uint64_t>              // file size in bytes
 *       <20 bytes>              // sha1 checksum
 *
 *   <column_info> :=
 *       <lenenc_int>            // column logical type
 *       <lenenc_int>            // column storage type
 *       <lenenc_int>            // column id
 *       <lenenc_string>         // column name
 *       <lenenc_int>            // max repetition level
 *       <lenenc_int>            // max definition level
 *
 *   <index_page> :=
 *       <uint64_t>              // next index page file offset
 *       <uint32_t>              // next index page size
 *       <uint32_t>              // bytes used in this page
 *       <char*>                 // index page data (index_entries back2back)
 *
 *   <index_entry> :=
 *      <uint8_t>                // entry type (0x1=data, 0x2=repetition level, 0x3=definition level)
 *      <lenenc_int>             // field id
 *      <lenenc_int>             // column index data size
 *      <char>*                  // column index data
 *
 *  <data_page>       :=
 *      <char>*                  // page data
 *
 */
static const char kMagicBytes[4] = {0x23, 0x17, 0x23, 0x17};

static const size_t kSectorSize = 512;

enum class ColumnType : uint8_t {
  BOOLEAN = 1,
  UINT32_BITPACKED = 10,
  UINT32_PLAIN = 11,
  UINT64_PLAIN = 12,
  UINT64_LEB128 = 13,
  DOUBLE = 14,
  STRING_PLAIN = 100,
};

inline uint64_t padToNextSector(uint64_t val) {
  return (((val) + (kSectorSize - 1)) / kSectorSize) * kSectorSize;
}

struct ColumnConfig {
  uint32_t column_id;
  String column_name;
  cstable::ColumnType storage_type;
  msg::FieldType logical_type;
  size_t rlevel_max;
  size_t dlevel_max;
};

struct MetaBlock {
  uint64_t transaction_id;
  uint64_t num_rows;
  uint64_t head_index_page_offset;
  uint64_t head_index_page_size;
  uint64_t file_size;
};

struct FileHeader {
  Vector<ColumnConfig> columns;
};

enum class PageIndexEntryType {
  DATA = 1,
  RLEVEL = 2,
  DLEVEL = 3
};

enum class BinaryFormatVersion {
  v0_1_0,
  v0_2_0
};

/* v0.1.0 */
namespace v0_1_0 {
const uint16_t kVersion = 1;
const uint32_t kMagicBytesUInt32 = 0x17231723;
}

/* v0.2.0 */
namespace v0_2_0 {
static const uint16_t kVersion = 2;
const size_t kMetaBlockPosition = 14;
const size_t kMetaBlockSize = 56;
size_t writeMetaBlock(const MetaBlock& mb, OutputStream* os);
size_t readMetaBlock(MetaBlock* mb,InputStream* is);
size_t writeHeader(const FileHeader& hdr, OutputStream* os);
size_t readHeader(FileHeader* mb, OutputStream* os);
}

}
}

