#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacit)
    : capacity(capacit), taken(0), total_written(0), total_read(0), inpu_ended(false), buf(), eror(false) {}

/*
Function to write data to the byte stream.
I check the number of available bytes, and try to fit in what I can.
If the entire data can be fitted, that happens.
*/
size_t ByteStream::write(const string &data) {
    const size_t available = capacity - taken;
    if (available == 0)
        return 0;
    const size_t limit = (data.length() <= available) ? data.length() : available;

    for (size_t i = 0; i < limit; i++) {
        buf.push_back(data[i]);
    }
    taken = buf.size();
    total_written += limit;
    return limit;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string output = "";

    const size_t limit = (len <= taken) ? len : taken;

    for (size_t i = 0; i < limit; i++)
        output += buf.at(i);

    return output;
}

//! \param[in] len bytes will be removed from the output side of the buffer
// I erase the len number of bytes from the vector if it has that many number of bytes,
// otherwise, I just erase the entire vector.
void ByteStream::pop_output(const size_t len) {
    auto i = buf.begin();
    if (len <= taken) {
        buf.erase(i, i + len);
        taken -= len;
        total_read += len;
    } else {
        buf.erase(i, i + taken);
        taken = 0;
        total_read += taken;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    const string result = peek_output(len);
    pop_output(len);
    return result;
}

void ByteStream::set_error() { eror = true; }

bool ByteStream::error() const { return eror; }

void ByteStream::end_input() { inpu_ended = true; }

bool ByteStream::input_ended() const { return inpu_ended; }

size_t ByteStream::buffer_size() const { return taken; }

bool ByteStream::buffer_empty() const { return buf.empty(); }

bool ByteStream::eof() const { return inpu_ended && buf.empty(); }

size_t ByteStream::bytes_written() const { return total_written; }

size_t ByteStream::bytes_read() const { return total_read; }

size_t ByteStream::remaining_capacity() const { return capacity - taken; }
