/**
 * This file is part of the "libstx" project
 *   Copyright (c) 2011-2014 Paul Asmuth, Google Inc.
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _STX_HTTP_COOKIES_H
#define _STX_HTTP_COOKIES_H
#include <vector>
#include <string>
#include "stx/UnixTime.h"
#include <stx/uri.h>

namespace stx {
namespace http {

class Cookies {
public:
  typedef std::vector<std::pair<std::string, std::string>> CookieList;

  static bool getCookie(
      const CookieList& cookies,
      const std::string& key,
      std::string* dst);

  static CookieList parseCookieHeader(const std::string& header_str);

  static std::string mkCookie(
      const std::string& key,
      const std::string& value,
      const UnixTime& expire = UnixTime::epoch(),
      const std::string& path = "",
      const std::string& domain = "",
      bool secure = false,
      bool httponly = false);
};

}
}
#endif
