#pragma once

template <typename Buffer>
concept BufferType = requires (Buffer buffer) { std::next(buffer.begin()); buffer.end(); };

template <typename Buffer>
concept MutableBufferType = BufferType<Buffer> && requires (Buffer buffer) { *buffer.begin() = {}; };