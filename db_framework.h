#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows

#include <atomic>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <ctime>
#include <stdio.h>
#include <functional>
#include <condition_variable>
#include <queue>
#include <vector>
#include <numeric>
#include <future>

#include <pqxx/pqxx>

#include <boost/predef.h> // Tools to identify the OS.
// We need this to enable cancelling of I/O operations on
// Windows XP, Windows Server 2003 and earlier.
// Refer to "http://www.boost.org/doc/libs/1_58_0/
// doc/html/boost_asio/reference/basic_stream_socket/
// cancel/overload1.html" for details.
#ifdef BOOST_OS_WINDOWS
#define _WIN32_WINNT 0x0501

#if _WIN32_WINNT <= 0x0502 // Windows Server 2003 or earlier.
#define BOOST_ASIO_DISABLE_IOCP
#define BOOST_ASIO_ENABLE_CANCELIO
#endif
#endif
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "design_pattern.h"
#include "schema.h"
#include "tcp.h"

using namespace boost;

using PostgreSQL = pqxx::connection;
using TPostgres = std::shared_ptr<PostgreSQL>;

using TComponent = std::shared_ptr<DP::Component>;
using TComposite = std::shared_ptr<DP::Composite<TComponent>>;