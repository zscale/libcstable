/**
 * This file is part of the "libstx" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _STX_MSG_MESSAGESCHEMA_H
#define _STX_MSG_MESSAGESCHEMA_H
#include <stx/stdtypes.h>
#include <stx/exception.h>
#include <stx/autoref.h>
#include <stx/util/binarymessagereader.h>
#include <stx/util/binarymessagewriter.h>
#include <stx/protobuf/MessageObject.h>
#include <stx/json/json.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

namespace stx {
namespace cstable {

class RecordSchema;

struct RecordSchemaField {

  static RecordSchemaField mkObjectField(
      String name,
      bool repeated,
      bool optional,
      RefPtr<msg::RecordSchema> schema);

  RecordSchemaField(
    String _name,
    FieldType _type,
    uint64_t _type_size,
    bool _repeated,
    bool _optional,
    EncodingHint _encoding = EncodingHint::NONE) :
    id(_id),
    name(_name),
    type(_type),
    type_size(_type_size),
    repeated(_repeated),
    optional(_optional),
    encoding(_encoding),
    schema(nullptr) {}

  String typeName() const;
  size_t typeSize() const;

  String name;
  FieldType type;
  uint64_t type_size;
  bool repeated;
  bool optional;
  EncodingHint encoding;
  RefPtr<RecordSchema> schema;
};

class RecordSchema : public RefCounted {
public:

  RecordSchema();
  RecordSchema(const RecordSchema& other);

  void addUnsignedIntegerField(
      const String& name);

  const Vector<RecordSchemaField>& fields() const;

  //String toString() const;
  //Buffer encode() const;
  //void encode(util::BinaryMessageWriter* buf) const;
  //void decode(util::BinaryMessageReader* buf);
  //static RefPtr<msg::RecordSchema> decode(const String& buf);

  //void toJSON(json::JSONOutputStream* json) const;
  //void fromJSON(
  //    json::JSONObject::const_iterator begin,
  //    json::JSONObject::const_iterator end);

protected:
  HashMap<String, RecordSchemaField> fields_;
};

} // namespace msg
} // namespace stx

#endif
