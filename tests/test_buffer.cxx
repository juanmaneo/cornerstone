#include "../cornerstone.hxx"
#include <cassert>

using namespace cornerstone;

static void do_test(buffer* buf);

void test_buffer() {
    
    buffer* buf = buffer::alloc(1024);
    assert(buf->size() == 1024);
    do_test(buf);
    buffer::release(buf);

    buf = buffer::alloc(0x10000);
    assert(buf->size() == 0x10000);
    do_test(buf);
    buffer::release(buf);
}

static void do_test(buffer* buf) {
    uint seed = (uint)std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);
    std::uniform_int_distribution<int32> distribution(1, 10000);
    auto rnd = std::bind(distribution, engine);

    // store int32 values into buffer
    std::vector<int32> vals;
    for (int i = 0; i < 100; ++i) {
        int32 val = rnd();
        vals.push_back(val);
        buf->put(val);
    }

    assert(buf->pos() == 100 * sz_int);

    ulong long_val = std::numeric_limits<uint>::max();
    long_val += rnd();
    buf->put(long_val);

    byte b = (byte)rnd();
    buf->put(b);
    buf->put("a string");
    byte b1 = (byte)rnd();
    buf->put(b1);
    buffer::safe_buffer buf1(std::move(buffer::safe_alloc(100)));
    buf1->put("another string");
    buf1->pos(0);
    buffer::safe_buffer buf2(buffer::make_safe(buffer::copy(*buf1)));
    buf->put(*buf1);
    buf->pos(0);
    for (int i = 0; i < 100; ++i) {
        int32 val = buf->get_int();
        assert(val == vals[i]);
    }

    assert(long_val == buf->get_ulong());
    assert(b == buf->get_byte());
    assert(strcmp("a string", buf->get_str()) == 0);
    assert(b1 == buf->get_byte());
    assert(strcmp("another string", buf->get_str()) == 0);
    assert(strcmp("another string", buf2->get_str()) == 0);
    assert(buf->pos() == (100 * sz_int + 2 * sz_byte + sz_ulong + strlen("a string") + 1 + strlen("another string") + 1));
}