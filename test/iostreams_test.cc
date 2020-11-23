#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/read.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/lzma.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/zstd.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include <string>
#include <sstream>
#include <utility>

template <typename Compressor, typename Decompressor,
          typename... CompressorArgs>
bool test_iostream_roundtrip(CompressorArgs &&...args)
{
std::string original = "hello, world! this test verifies that iostreams can "
                       "compress and decompress a string";
  std::stringstream compressed;
  std::stringstream uncompressed;

  {
    boost::iostreams::filtering_ostream compressed_out;
    compressed_out.push(Compressor(std::forward<CompressorArgs>(args)...));
    compressed_out.push(compressed);
    boost::iostreams::write(compressed_out, original.data(), original.size());
  }

  boost::iostreams::filtering_ostream uncompressed_out;
  uncompressed_out.push(Decompressor());
  uncompressed_out.push(uncompressed);
  boost::iostreams::copy(compressed, uncompressed_out);

  return uncompressed.str() == original;
}

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

  using namespace boost::iostreams;
  if (!test_iostream_roundtrip<gzip_compressor, gzip_decompressor>(zlib::best_speed))
    return 1;
  if (!test_iostream_roundtrip<zlib_compressor, zlib_decompressor>(zlib::best_speed))
    return 1;
  if (!test_iostream_roundtrip<zstd_compressor, zstd_decompressor>(zstd::best_speed))
    return 1;
  if (!test_iostream_roundtrip<lzma_compressor, lzma_decompressor>(lzma::best_speed))
    return 1;
  if (!test_iostream_roundtrip<bzip2_compressor, bzip2_decompressor>())
    return 1;

  return 0;
}
