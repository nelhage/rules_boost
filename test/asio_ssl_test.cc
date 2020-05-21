// Based off of https://github.com/boostorg/asio/blob/develop/test/ssl/stream.cpp
//
// This test eahcks that asio SSL builds and links properly, but does
// not meaningfully exercise its functionality.
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace {
    bool verify_callback(bool, boost::asio::ssl::verify_context&)
    {
        return false;
    }

    void handshake_handler(const boost::system::error_code&)
    {
    }

    void buffered_handshake_handler(const boost::system::error_code&, std::size_t)
    {
    }

    void shutdown_handler(const boost::system::error_code&)
    {
    }

    void write_some_handler(const boost::system::error_code&, std::size_t)
    {
    }

    void read_some_handler(const boost::system::error_code&, std::size_t)
    {
    }
};

int main()
{
    using namespace boost::asio;
    namespace ip = boost::asio::ip;

    try
    {
        io_context ioc;
        char mutable_char_buffer[128] = "";
        const char const_char_buffer[128] = "";
        boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
        boost::system::error_code ec;

        // ssl::stream constructors.

        ssl::stream<ip::tcp::socket> stream1(ioc, context);
        ip::tcp::socket socket1(ioc, ip::tcp::v4());
        ssl::stream<ip::tcp::socket&> stream2(socket1, context);

        // basic_io_object functions.

        ssl::stream<ip::tcp::socket>::executor_type ex = stream1.get_executor();
        (void)ex;

        // ssl::stream functions.

        SSL* ssl1 = stream1.native_handle();
        (void)ssl1;

        ssl::stream<ip::tcp::socket>::lowest_layer_type& lowest_layer
            = stream1.lowest_layer();
        (void)lowest_layer;

        const ssl::stream<ip::tcp::socket>& stream3 = stream1;
        const ssl::stream<ip::tcp::socket>::lowest_layer_type& lowest_layer2
            = stream3.lowest_layer();
        (void)lowest_layer2;

        stream1.set_verify_mode(ssl::verify_none);
        stream1.set_verify_mode(ssl::verify_none, ec);

        stream1.set_verify_depth(1);
        stream1.set_verify_depth(1, ec);

        stream1.set_verify_callback(verify_callback);
        stream1.set_verify_callback(verify_callback, ec);

        stream1.handshake(ssl::stream_base::client);
        stream1.handshake(ssl::stream_base::server);
        stream1.handshake(ssl::stream_base::client, ec);
        stream1.handshake(ssl::stream_base::server, ec);

        stream1.handshake(ssl::stream_base::client, buffer(mutable_char_buffer));
        stream1.handshake(ssl::stream_base::server, buffer(mutable_char_buffer));
        stream1.handshake(ssl::stream_base::client, buffer(const_char_buffer));
        stream1.handshake(ssl::stream_base::server, buffer(const_char_buffer));
        stream1.handshake(ssl::stream_base::client,
                          buffer(mutable_char_buffer), ec);
        stream1.handshake(ssl::stream_base::server,
                          buffer(mutable_char_buffer), ec);
        stream1.handshake(ssl::stream_base::client,
                          buffer(const_char_buffer), ec);
        stream1.handshake(ssl::stream_base::server,
                          buffer(const_char_buffer), ec);

        stream1.async_handshake(ssl::stream_base::client, handshake_handler);
        stream1.async_handshake(ssl::stream_base::server, handshake_handler);


        stream1.async_handshake(ssl::stream_base::client,
                                buffer(mutable_char_buffer), buffered_handshake_handler);
        stream1.async_handshake(ssl::stream_base::server,
                                buffer(mutable_char_buffer), buffered_handshake_handler);
        stream1.async_handshake(ssl::stream_base::client,
                                buffer(const_char_buffer), buffered_handshake_handler);
        stream1.async_handshake(ssl::stream_base::server,
                                buffer(const_char_buffer), buffered_handshake_handler);


        stream1.shutdown();
        stream1.shutdown(ec);

        stream1.async_shutdown(shutdown_handler);

        stream1.write_some(buffer(mutable_char_buffer));
        stream1.write_some(buffer(const_char_buffer));
        stream1.write_some(buffer(mutable_char_buffer), ec);
        stream1.write_some(buffer(const_char_buffer), ec);

        stream1.async_write_some(buffer(mutable_char_buffer), write_some_handler);
        stream1.async_write_some(buffer(const_char_buffer), write_some_handler);

        stream1.read_some(buffer(mutable_char_buffer));
        stream1.read_some(buffer(mutable_char_buffer), ec);

        stream1.async_read_some(buffer(mutable_char_buffer), read_some_handler);
    }
    catch (std::exception&)
    {
    }

    return 0;
}
