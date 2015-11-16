/**
 * This file is part of the "libstx" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _STX_NET_TCPCONNECTION_H
#define _STX_NET_TCPCONNECTION_H
#include <stdlib.h>
#include "stx/net/inetaddr.h"
#include "stx/thread/taskscheduler.h"

namespace stx {
namespace net {

class TCPConnection {
public:

  /**
   * Open a new tcp (client) connection and block until the connection is
   * established or raise an exception if the connction fails
   */
  static std::unique_ptr<TCPConnection> connect(const InetAddr& addr);

  /**
   * Open a new tcp (client) connection and return immediately. IMPORTANT
   * the caller must call connection->checkErrors(); after the onReady callback
   * fires!
   */
  static void connectAsync(
      const InetAddr& addr,
      TaskScheduler* scheduler,
      std::function<void(std::unique_ptr<TCPConnection> conn)> on_ready);

  TCPConnection(int fd);
  ~TCPConnection();
  int fd() const;

  size_t read(void* dst, size_t size);
  size_t write(const void* data, size_t size);
  void close();
  void setNonblocking(bool nonblocking = true);

  /**
   * This will raise an exception if there are any pending errors on the
   * connection
   */
  void checkErrors() const;

protected:
  void connectImpl(const InetAddr& addr);

  bool closed_;
  int fd_;
};

}
}
#endif
