/**
 * This file is part of the "libstx" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "stx/http/statshttpservlet.h"
#include "stx/stats/statssink.h"

namespace stx {
namespace stats {

StatsHTTPServlet::StatsHTTPServlet() : stats_repo_(StatsRepository::get()) {}

StatsHTTPServlet::StatsHTTPServlet(StatsRepository* stats_repo) :
    stats_repo_(stats_repo) {}

void StatsHTTPServlet::handleHTTPRequest(
    stx::http::HTTPRequest* req,
    stx::http::HTTPResponse* res) {
  Buffer buf;

  TextStatsSink sink([&buf] (const String& line) {
    buf.append(line);
    buf.append("\n");
  });

  stats_repo_->forEachStat([&sink] (const ExportedStat& exp) {
    exp.stat->exportAll(exp.path, &sink);
  });

  res->setStatus(stx::http::kStatusOK);
  res->addBody(buf);
}

}
}
