/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _FNORD_CSTABLE_BINARYFORMAT_H
#define _FNORD_CSTABLE_BINARYFORMAT_H
#include <stdlib.h>
#include <stdint.h>

namespace stx {
namespace cstable {

/**
 * // http://tools.ietf.org/html/rfc5234
 *
 * Assumptions:
 *   - aligned 512 write bytes to disk are atomic
 *
 * v1:
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
 * v2:
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
 *       <uint64_t>              // head index page offset as multiple of 512 bytes
 *       <uint64_t>              // file size in bytes
 *       <20 bytes>              // sha1 checksum
 *
 *   <column_info> :=
 *       <lenenc_int>            // column logical type
 *       <lenenc_int>            // column storage type
 *       <lenenc_int>            // column id
 *       <lenenc_int>            // length of the column name
 *       <char>*                 // column name
 *       <lenenc_int>            // max repetition level
 *       <lenenc_int>            // max definition level
 *
 *   <index_page> :=
 *       <uint32_t>              // page data size as multiple of 512 bytes, inluding 8 bytes metadata
 *       <uint32_t>              // next index page file offset as multiple of 512 bytes
 *       <char*>                 // index page data (index_entries back2back)
 *
 *   <index_entry> :=
 *      <uint8_t>                // entry type (0x1=data, 0x2=repetition level, 0x3=definition level)
 *      <lenenc_int>             // field id
 *      <lenenc_int>             // number of reference pages
 *      <index_page_ref>*        // page references
 *
 *   <index_page_ref> :=
 *      <lenenc_int>             // data page offset as multiple of 512
 *      <lenenc_int>             // data page size as multiple of 512
 *      [ <char>* ]              // optional metadata (depending on column storage type)
 *
 *  <data_page>       :=
 *      <char>*                  // page data
 *
 */
class BinaryFormat {
public:
  static const uint16_t kVersion = 1;
  static const uint32_t kMagicBytes = 0x17231723;
};

static const char kMagicBytes[4] = {0x23, 0x17, 0x23, 0x17};

enum class ColumnType : uint8_t {
  BOOLEAN = 1,
  UINT32_BITPACKED = 10,
  UINT32_PLAIN = 11,
  UINT64_PLAIN = 12,
  UINT64_LEB128 = 13,
  DOUBLE = 14,
  STRING_PLAIN = 100,
};

}
}

#endif
