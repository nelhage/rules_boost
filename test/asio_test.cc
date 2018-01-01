// from http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tuttimer1/src.html
//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main()
{
    boost::asio::io_context io;

    auto start = std::chrono::system_clock::now();

    boost::asio::deadline_timer t(io, boost::posix_time::seconds(3));
    t.wait();

    auto end = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > 2 ? 0 : -1;
}