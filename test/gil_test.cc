#include <boost/gil.hpp>
using namespace boost::gil;

template <typename Out>
struct halfdiff_cast_channels {
    template <typename T> Out operator()(const T& in1, const T& in2) const {
        return Out((in1-in2)/2);
    }
};

template <typename SrcView, typename DstView>
void x_gradient(const SrcView& src, const DstView& dst) {
    typedef typename channel_type<DstView>::type dst_channel_t;

    for (int y=0; y<src.height(); ++y) {
        typename SrcView::x_iterator src_it = src.row_begin(y);
        typename DstView::x_iterator dst_it = dst.row_begin(y);

        for (int x=1; x<src.width()-1; ++x)
            static_transform(src_it[x-1], src_it[x+1], dst_it[x], 
                               halfdiff_cast_channels<dst_channel_t>());
    }
}

void ComputeXGradientGray8(const unsigned char* src_pixels, ptrdiff_t src_row_bytes, int w, int h,
                                   signed char* dst_pixels, ptrdiff_t dst_row_bytes) {
    gray8c_view_t src = interleaved_view(w, h, (const gray8_pixel_t*)src_pixels,src_row_bytes);
    gray8s_view_t dst = interleaved_view(w, h, (     gray8s_pixel_t*)dst_pixels,dst_row_bytes);
    x_gradient(src,dst);
}

int main() {
    return 0;
}
