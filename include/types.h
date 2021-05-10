#pragma once

#include "breep/network/tcp.hpp"
#include "breep/util/serialization.hpp"

enum class Request : unsigned char {
    blockchain
};

BREEP_DECLARE_TYPE(Request);