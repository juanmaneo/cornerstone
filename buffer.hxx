#ifndef _BUFFER_HXX_
#define _BUFFER_HXX_

namespace cornerstone {
    class buffer {
        __nocopy__(buffer)
    public:
        typedef std::shared_ptr<buffer> safe_buffer;
        static void release(buffer* buff);
        static buffer* alloc(const size_t size);
        static buffer* copy(const buffer& buf);
        static safe_buffer safe_alloc(const size_t size) {
            return std::move(safe_buffer(alloc(size), &buffer::release));
        }

        static safe_buffer make_safe(buffer* buff) {
            return std::move(safe_buffer(buff, &buffer::release));
        }

        size_t size() const;
        size_t pos() const;
        void pos(size_t p);

        int32 get_int();
        ulong get_ulong();
        byte get_byte();
        const char* get_str();
        byte* data() const;

        void put(byte b);
        void put(int32 val);
        void put(ulong val);
        void put(const std::string& str);
        void put(const buffer& buf);
    };
}
#endif //_BUFFER_HXX_