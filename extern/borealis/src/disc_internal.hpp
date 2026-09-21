#pragma once

#include "borealis/disc.hpp"

#include <SDL3/SDL_iostream.h>
#include <nod.h>

#include <cstddef>
#include <cstdint>

namespace borealis::disc::detail {

struct NodApi {
    NodResult (*openStream)(const NodDiscStream*, const NodDiscOptions*, NodHandle**);
    void (*freeHandle)(NodHandle*);
    std::int64_t (*read)(NodHandle*, std::uint8_t*, std::size_t);
    NodResult (*header)(const NodHandle*, NodDiscHeader*);
    std::uint64_t (*discSize)(const NodHandle*);
    const char* (*errorMessage)();
};

const NodApi& default_nod_api() noexcept;

Status status_from_nod_result(NodResult result) noexcept;
Result inspect_stream(SDL_IOStream* stream, Catalog catalog, const NodApi& api);
Result verify_stream(SDL_IOStream* stream, Catalog catalog, Progress* progress, const NodApi& api);

}  // namespace borealis::disc::detail
