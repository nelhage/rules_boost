#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <sstream>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;

int main(int, char*[])
{
    // Construct the sink
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    auto log_stream = boost::make_shared< std::ostringstream >();

    // Add a stream to write log to
    sink->locked_backend()->add_stream(log_stream);

    // Register the sink in the logging core
    logging::core::get()->add_sink(sink);

    // Register a console log with a format, which
    // tests our usage of log_setup library.
    // Tests resolution of https://github.com/nelhage/rules_boost/issues/151.
    logging::add_console_log(std::cout, boost::log::keywords::format = "*%Severity%* %Message%");

    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

    if(log_stream->str().size() != 162)
	return 1;

    return 0;
}

