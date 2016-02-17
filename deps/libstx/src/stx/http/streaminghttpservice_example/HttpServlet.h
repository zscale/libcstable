/**
 * This file is part of the "libstx" project
 *   Copyright (c) 2015 Paul Asmuth
 *   Copyright (c) 2015 Laura Schlimmer
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _CM_HTTPSERVLET_H
#define _CM_HTTPSERVLET_H
#include "stx/http/httpservice.h"

namespace stx {
namespace http {

class HTTPServlet : public http::StreamingHTTPService {
public:

  void handleHTTPRequest(
      RefPtr<http::HTTPRequestStream> req,
      RefPtr<http::HTTPResponseStream> res);

};

}
}
#endif
