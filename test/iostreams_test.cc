#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zstd.hpp>
#include <boost/iostreams/filtering_stream.hpp>

int main()
{
  {
    std::string uncompressed ("hello");
    std::string compressed;
    std::string expected ("\x1f\x8b\x08\x00\x00\x00\x00\x00\x04\xff\xcb\x48\xcd\xc9\xc9\x07\x00\x86\xa6\x10\x36\x05\x00\x00\x00", 25);

    boost::iostreams::filtering_ostream out;
    out.push(
        boost::iostreams::gzip_compressor(boost::iostreams::zlib::best_speed));
    out.push(boost::iostreams::back_inserter(compressed));
    boost::iostreams::write(out,
                            reinterpret_cast<const char*>(uncompressed.data()),
                            uncompressed.size());
    boost::iostreams::close(out);

    if (compressed != expected) {
      return 1;
    }
  }
  {
    std::string original ("hello");
    std::string compressed;
    std::string uncompressed;

    boost::iostreams::filtering_ostream compressed_out;
    compressed_out.push(
        boost::iostreams::zstd_compressor(boost::iostreams::zstd::best_speed));
    compressed_out.push(boost::iostreams::back_inserter(compressed));
    boost::iostreams::write(compressed_out,
                            reinterpret_cast<const char*>(original.data()),
                            original.size());
    boost::iostreams::close(compressed_out);

    boost::iostreams::filtering_ostream uncompressed_out;
    uncompressed_out.push(
        boost::iostreams::zstd_decompressor());
    uncompressed_out.push(boost::iostreams::back_inserter(uncompressed));
    boost::iostreams::write(uncompressed_out,
                            reinterpret_cast<const char*>(compressed.data()),
                            compressed.size());
    boost::iostreams::close(uncompressed_out);

    if (uncompressed != original) {
      return 1;
    }
  }

  return 0;
}
