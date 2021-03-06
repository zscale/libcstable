/**
 * This file is part of the "libstx" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _libstx_HTTPHANDLER_H
#define _libstx_HTTPHANDLER_H
#include <memory>

namespace stx {
namespace http {
class HTTPServerConnection;
class HTTPRequest;

class HTTPHandler {
public:
  virtual ~HTTPHandler() {}
  virtual void handleHTTPRequest() = 0;
};

class HTTPHandlerFactory {
public:
  virtual ~HTTPHandlerFactory() {}
  virtual std::unique_ptr<HTTPHandler> getHandler(
      HTTPServerConnection* conn,
      HTTPRequest* req) = 0;
};

}
}
#endif
