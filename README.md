libcstable v0.2.0
=================

libcstable allows you to write and read cstables (columnar storage tables).

A cstable is a disk-based data structure that stores a list of records.
Each record must adhere to a strict schema. Nesting and repetition within
records is fully supported. This means you can store, for example, arbitrarily
JSON objects or protobufs message as records.

The layout of the data on disk is "column oriented", i.e. rather than storing
a list of records, one record after another, we store a list of "columns", one
column after another. A column contains the values of a particular field from
_all records_.

This concept is best illustrated by example

The key features of libcstable:

  - Large list of tightly packed column storage formats (see below)
  - Record shredding and re-materialization to/from Protobuf and JSON
  - Streaming writes -- write huge tables with very little memory
  - Snapshot consistency when appending to tables that are being read concurrently
  - Well-documented, modern C++11 code
  - Battle tested in production


Column Types
-----------

### Logical Types

  - BooleanColumn
    - UINT32_BITPACKED (8 bools per byte)
    - LEB128_RLE

  - UnsignedIntegerColumn
    - UINT32_PLAIN
    - UINT64_PLAIN
    - UINT32_BITPACKED
    - LEB128
    - LEB128_RLE
    - LEB128_DELTA

  - SignedIntegerColumn

  - DoubleColumn
    - IEE754_PLAIN

  - StringColumn
    - STRING_PLAIN
    - STRING_CATALOG

  - DateTimeColumn
    - UINT64_PLAIN
    - LEB128
    - LEB128_RLE
    - LEB128_DELTA

### Storage Types

  - UINT32_PLAIN
  - UINT32_BITPACKED
  - UINT64_PLAIN
  - LEB128
  - LEB128_RLE
  - LEB128_DELTA
  - IEE754_PLAIN
  - STRING_PLAIN
  - STRING_CATALOG


Shredding & Materializing Records
---------------------------------
