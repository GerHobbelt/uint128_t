#include "uint128_t.build"

#include <algorithm>
#include <cctype>
#include <sstream>

const uint128_t uint128_0(0);
const uint128_t uint128_1(1);

uint128_t::uint128_t(const std::string & s, uint8_t base) {
    init(s.c_str(), s.size(), base);
}

uint128_t::uint128_t(const char *s, const std::size_t len, uint8_t base) {
    init(s, len, base);
}

uint128_t::uint128_t(const bool & b)
    : uint128_t((uint8_t) b)
{}

void uint128_t::init(const char *s, std::size_t len, uint8_t base) {
    if ((s == NULL) || !len || (s[0] == '\x00')){
        LOWER = UPPER = 0;
        return;
    }

    while (*s && len && std::isspace(*s)) {
        ++s;
        len--;
    }

    // no prefixes
    switch (base) {
        case 16:
            _init_hex(s, len);
            break;
        case 10:
            _init_dec(s, len);
            break;
        case 8:
            _init_oct(s, len);
            break;
        case 2:
            _init_bin(s, len);
            break;
        default:
            // should probably throw error here
            break;
    }
}

void uint128_t::_init_hex(const char *s, std::size_t len) {
    // 2**128 = 0x100000000000000000000000000000000.
    static const std::size_t MAX_LEN = 32;

    LOWER = UPPER = 0;
    if (!s || !len) {
        return;
    }

    const std::size_t max_len = std::min(len, MAX_LEN);
    const std::size_t starting_index = (MAX_LEN < len)?(len - MAX_LEN):0;
    const std::size_t double_lower = sizeof(LOWER) * 2;
    const std::size_t lower_len = (max_len >= double_lower)?double_lower:max_len;
    const std::size_t upper_len = (max_len >= double_lower)?(max_len - double_lower):0;

    std::stringstream lower_s, upper_s;
    upper_s << std::hex << std::string(s + starting_index, upper_len);
    lower_s << std::hex << std::string(s + starting_index + upper_len, lower_len);

    // should check for errors
    upper_s >> UPPER;
    lower_s >> LOWER;
}

void uint128_t::_init_dec(const char *s, std::size_t len){
    // 2**128 = 340282366920938463463374607431768211456.
    static const std::size_t MAX_LEN = 39;

    LOWER = UPPER = 0;
    if (!s || !len) {
        return;
    }

    const std::size_t max_len = std::min(len, MAX_LEN);
    const std::size_t starting_index = (MAX_LEN < len)?(len - MAX_LEN):0;
    s += starting_index;

    for (std::size_t i = 0; *s && ('0' <= *s) && (*s <= '9') && (i < max_len); ++s, ++i){
        *this *= 10;
        *this += *s - '0';
    }
}

void uint128_t::_init_oct(const char *s, std::size_t len){
    // 2**128 = 0o4000000000000000000000000000000000000000000.
    static const std::size_t MAX_LEN = 43;

    LOWER = UPPER = 0;
    if (!s || !len) {
        return;
    }

    const std::size_t max_len = std::min(len, MAX_LEN);
    const std::size_t starting_index = (MAX_LEN < len)?(len - MAX_LEN):0;
    s += starting_index;

    for (std::size_t i = 0; *s && ('0' <= *s) && (*s <= '7') && (i < max_len); ++s, ++i){
        *this *= 8;
        *this += *s - '0';
    }
}

void uint128_t::_init_bin(const char *s, std::size_t len){
    // 2**128 = 0x100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.
    static const std::size_t MAX_LEN = 128;

    LOWER = UPPER = 0;
    if (!s || !len) {
        return;
    }

    const std::size_t max_len = std::min(len, MAX_LEN);
    const std::size_t starting_index = (MAX_LEN < len)?(len - MAX_LEN):0;
    const std::size_t eight_lower = sizeof(LOWER) * 8;
    const std::size_t lower_len = (max_len >= eight_lower)?eight_lower:max_len;
    const std::size_t upper_len = (max_len >= eight_lower)?(max_len - eight_lower):0;
    s += starting_index;

    for (std::size_t i = 0; *s && ('0' <= *s) && (*s <= '1') && (i < upper_len); ++s, ++i){
        UPPER <<= 1;
        UPPER |= *s - '0';
    }

    for (std::size_t i = 0; *s && ('0' <= *s) && (*s <= '1') && (i < lower_len); ++s, ++i){
        LOWER <<= 1;
        LOWER |= *s - '0';
    }
}

uint128_t & uint128_t::operator=(const bool & rhs) {
    UPPER = 0;
    LOWER = rhs;
    return *this;
}

uint128_t::operator bool() const{
    return (bool) (UPPER | LOWER);
}

uint128_t::operator uint8_t() const{
    return (uint8_t) LOWER;
}

uint128_t::operator uint16_t() const{
    return (uint16_t) LOWER;
}

uint128_t::operator uint32_t() const{
    return (uint32_t) LOWER;
}

uint128_t::operator uint64_t() const{
    return (uint64_t) LOWER;
}

uint128_t uint128_t::operator&(const uint128_t & rhs) const{
    return uint128_t(UPPER & rhs.UPPER, LOWER & rhs.LOWER);
}

uint128_t & uint128_t::operator&=(const uint128_t & rhs){
    UPPER &= rhs.UPPER;
    LOWER &= rhs.LOWER;
    return *this;
}

uint128_t uint128_t::operator|(const uint128_t & rhs) const{
    return uint128_t(UPPER | rhs.UPPER, LOWER | rhs.LOWER);
}

uint128_t & uint128_t::operator|=(const uint128_t & rhs){
    UPPER |= rhs.UPPER;
    LOWER |= rhs.LOWER;
    return *this;
}

uint128_t uint128_t::operator^(const uint128_t & rhs) const{
    return uint128_t(UPPER ^ rhs.UPPER, LOWER ^ rhs.LOWER);
}

uint128_t & uint128_t::operator^=(const uint128_t & rhs){
    UPPER ^= rhs.UPPER;
    LOWER ^= rhs.LOWER;
    return *this;
}

uint128_t uint128_t::operator~() const{
    return uint128_t(~UPPER, ~LOWER);
}

uint128_t uint128_t::operator<<(const uint128_t & rhs) const{
    const uint64_t shift = rhs.LOWER;
    if (((bool) rhs.UPPER) || (shift >= 128)){
        return uint128_0;
    }
    else if (shift == 64){
        return uint128_t(LOWER, 0);
    }
    else if (shift == 0){
        return *this;
    }
    else if (shift < 64){
        return uint128_t((UPPER << shift) + (LOWER >> (64 - shift)), LOWER << shift);
    }
    else if ((128 > shift) && (shift > 64)){
        return uint128_t(LOWER << (shift - 64), 0);
    }
    else{
        return uint128_0;
    }
}

uint128_t & uint128_t::operator<<=(const uint128_t & rhs){
    *this = *this << rhs;
    return *this;
}

uint128_t uint128_t::operator>>(const uint128_t & rhs) const{
    const uint64_t shift = rhs.LOWER;
    if (((bool) rhs.UPPER) || (shift >= 128)){
        return uint128_0;
    }
    else if (shift == 64){
        return uint128_t(0, UPPER);
    }
    else if (shift == 0){
        return *this;
    }
    else if (shift < 64){
        return uint128_t(UPPER >> shift, (UPPER << (64 - shift)) + (LOWER >> shift));
    }
    else if ((128 > shift) && (shift > 64)){
        return uint128_t(0, (UPPER >> (shift - 64)));
    }
    else{
        return uint128_0;
    }
}

uint128_t & uint128_t::operator>>=(const uint128_t & rhs){
    *this = *this >> rhs;
    return *this;
}

bool uint128_t::operator!() const{
    return !(bool) (UPPER | LOWER);
}

bool uint128_t::operator&&(const uint128_t & rhs) const{
    return ((bool) *this && rhs);
}

bool uint128_t::operator||(const uint128_t & rhs) const{
     return ((bool) *this || rhs);
}

bool uint128_t::operator==(const uint128_t & rhs) const{
    return ((UPPER == rhs.UPPER) && (LOWER == rhs.LOWER));
}

bool uint128_t::operator!=(const uint128_t & rhs) const{
    return ((UPPER != rhs.UPPER) | (LOWER != rhs.LOWER));
}

bool uint128_t::operator>(const uint128_t & rhs) const{
    if (UPPER == rhs.UPPER){
        return (LOWER > rhs.LOWER);
    }
    return (UPPER > rhs.UPPER);
}

bool uint128_t::operator<(const uint128_t & rhs) const{
    if (UPPER == rhs.UPPER){
        return (LOWER < rhs.LOWER);
    }
    return (UPPER < rhs.UPPER);
}

bool uint128_t::operator>=(const uint128_t & rhs) const{
    return ((*this > rhs) | (*this == rhs));
}

bool uint128_t::operator<=(const uint128_t & rhs) const{
    return ((*this < rhs) | (*this == rhs));
}

uint128_t uint128_t::operator+(const uint128_t & rhs) const{
    return uint128_t(UPPER + rhs.UPPER + ((LOWER + rhs.LOWER) < LOWER), LOWER + rhs.LOWER);
}

uint128_t & uint128_t::operator+=(const uint128_t & rhs){
    UPPER += rhs.UPPER + ((LOWER + rhs.LOWER) < LOWER);
    LOWER += rhs.LOWER;
    return *this;
}

uint128_t uint128_t::operator-(const uint128_t & rhs) const{
    return uint128_t(UPPER - rhs.UPPER - ((LOWER - rhs.LOWER) > LOWER), LOWER - rhs.LOWER);
}

uint128_t & uint128_t::operator-=(const uint128_t & rhs){
    *this = *this - rhs;
    return *this;
}

// Algorithm summary:
//
// First we do a 64-bit to 128-bit long multiply for the low bits, then we use a normal 64-bit
// multiply on the high bits.
// This allows us to take advantage of not only compiler intrinsics but native 64-bit arithmetic.

// First we define the generic multlong64 methods. These will all do basically what _umul128 does.

// MSVC _umul128
#if _UINT128_T_MULT_TYPE == _UINT128_T_MULT_MSVC
#include <intrin.h>
_UINT128_T_MULT_TARGET uint64_t uint128_t::multlong64(uint64_t lhs, uint64_t rhs, uint64_t *high){
    return _umul128(lhs, rhs, high);
}

// GCC __uint128_t
#elif _UINT128_T_MULT_TYPE == _UINT128_T_MULT_GCC
_UINT128_T_MULT_TARGET uint64_t uint128_t::multlong64(uint64_t lhs, uint64_t rhs, uint64_t *high){
    __uint128_t product = static_cast<__uint128_t>(lhs) * static_cast<__uint128_t>(rhs);
    *high = static_cast<uint64_t>(product >> 64);
    return static_cast<uint64_t>(product & 0xFFFFFFFFFFFFFFFF);
}

// Portable version
#else
// The double cast helps MSVC
_UINT128_T_MULT_TARGET static inline uint64_t lower32(uint64_t val){
    return static_cast<uint64_t>(static_cast<uint32_t>(val));
}
_UINT128_T_MULT_TARGET static inline uint64_t upper32(uint64_t val){
    return static_cast<uint64_t>(static_cast<uint32_t>(val >> 32));
}

_UINT128_T_MULT_TARGET uint64_t uint128_t::multlong64(uint64_t lhs, uint64_t rhs, uint64_t *high){
    // This is a fast yet simple grade school 2x2 long multiply.
    // The way we add the cross products avoids the need to track 64-bit carries due to the properties
    // of multiplying by 11 (technically 0x100000001) capping the sums at 0xFFFFFFFFFFFFFFFF, and it
    // tries to match the powerful ARMv6's UMAAL function which was explicitly designed for
    // multiprecision multiplication:
    //
    //    void umaal(uint32_t &RdLo, uint32_t &RdHi, const uint32_t Rn, const uint32_t Rm){
    //        uint64_t product = static_cast<uint64_t>(Rn) * static_cast<uint64_t>(Rm);
    //        product += RdLo;
    //        product += RdHi;
    //        RdLo = static_cast<uint32_t>(product & 0xFFFFFFFF);
    //        RdHi = static_cast<uint32_t>(product >> 32);
    //    }
    //
    // This allows a 64-bit to 128-bit multiply to be calculated in 4 instructions, ~3 cycles each.
    //
    // It is still fast for other platforms, though.
    //
    // TODO: Use better variable names

    // Calculate the cross products...
    uint64_t lo_lo = lower32(lhs) * lower32(rhs);
    uint64_t hi_lo = upper32(lhs) * lower32(rhs);
    uint64_t lo_hi = lower32(lhs) * upper32(rhs);
    uint64_t hi_hi = upper32(lhs) * upper32(rhs);

    // then add them together.
    uint64_t cross = upper32(lo_lo) + lower32(hi_lo) + lo_hi;
    uint64_t top = upper32(hi_lo) + upper32(cross) + hi_hi;

    // Done
    *high = top;
    return (cross << 32) | (lo_lo & 0xFFFFFFFF);
}
#endif

// Now we do the full 128-bit multiply.
//
// This is based on the 64-bit multiply idiom on ARM, only for 128-bit integers instead of 64-bit.
//
//     @ {r0, r1} * {r2, r3} (little endian)
//     umull   r12, lr, r2, r0    @ {r12, lr} = static_cast<uint64_t>(r2) * static_cast<uint64_t>(r0)
//     mla     r4,  r2, r1, lr    @ r4 = r2 * r1 + lr;
//     mla     r1,  r3, r0, r4    @ r1 = r3 * r0 + r4
//     @ result is in {r12, r1}

_UINT128_T_MULT_TARGET uint128_t uint128_t::operator*(const uint128_t & rhs) const{
    uint64_t high;
    uint64_t low = multlong64(LOWER, rhs.LOWER, &high);
    uint128_t acc(high, low);
    acc.UPPER += LOWER * rhs.UPPER;
    acc.UPPER += UPPER * rhs.LOWER;
    return acc;
}

_UINT128_T_MULT_TARGET uint128_t & uint128_t::operator*=(const uint128_t & rhs){
    *this = *this * rhs;
    return *this;
}

void uint128_t::ConvertToVector(std::vector<uint8_t> & ret, const uint64_t & val) const {
    ret.push_back(static_cast<uint8_t>(val >> 56));
    ret.push_back(static_cast<uint8_t>(val >> 48));
    ret.push_back(static_cast<uint8_t>(val >> 40));
    ret.push_back(static_cast<uint8_t>(val >> 32));
    ret.push_back(static_cast<uint8_t>(val >> 24));
    ret.push_back(static_cast<uint8_t>(val >> 16));
    ret.push_back(static_cast<uint8_t>(val >> 8));
    ret.push_back(static_cast<uint8_t>(val));
}

void uint128_t::export_bits(std::vector<uint8_t> &ret) const {
    ConvertToVector(ret, const_cast<const uint64_t&>(UPPER));
    ConvertToVector(ret, const_cast<const uint64_t&>(LOWER));
}

std::pair <uint128_t, uint128_t> uint128_t::divmod(const uint128_t & lhs, const uint128_t & rhs) const{
    // Save some calculations /////////////////////
    if (rhs == uint128_0){
        throw std::domain_error("Error: division or modulus by 0");
    }
    else if (rhs == uint128_1){
        return std::pair <uint128_t, uint128_t> (lhs, uint128_0);
    }
    else if (lhs == rhs){
        return std::pair <uint128_t, uint128_t> (uint128_1, uint128_0);
    }
    else if ((lhs == uint128_0) || (lhs < rhs)){
        return std::pair <uint128_t, uint128_t> (uint128_0, lhs);
    }

    std::pair <uint128_t, uint128_t> qr (uint128_0, uint128_0);
    for(uint8_t x = lhs.bits(); x > 0; x--){
        qr.first  <<= uint128_1;
        qr.second <<= uint128_1;

        if ((lhs >> (x - 1U)) & 1){
            ++qr.second;
        }

        if (qr.second >= rhs){
            qr.second -= rhs;
            ++qr.first;
        }
    }
    return qr;
}

uint128_t uint128_t::operator/(const uint128_t & rhs) const{
    return divmod(*this, rhs).first;
}

uint128_t & uint128_t::operator/=(const uint128_t & rhs){
    *this = *this / rhs;
    return *this;
}

uint128_t uint128_t::operator%(const uint128_t & rhs) const{
    return divmod(*this, rhs).second;
}

uint128_t & uint128_t::operator%=(const uint128_t & rhs){
    *this = *this % rhs;
    return *this;
}

uint128_t & uint128_t::operator++(){
    return *this += uint128_1;
}

uint128_t uint128_t::operator++(int){
    uint128_t temp(*this);
    ++*this;
    return temp;
}

uint128_t & uint128_t::operator--(){
    return *this -= uint128_1;
}

uint128_t uint128_t::operator--(int){
    uint128_t temp(*this);
    --*this;
    return temp;
}

uint128_t uint128_t::operator+() const{
    return *this;
}

uint128_t uint128_t::operator-() const{
    return ~*this + uint128_1;
}

const uint64_t & uint128_t::upper() const{
    return UPPER;
}

const uint64_t & uint128_t::lower() const{
    return LOWER;
}

uint8_t uint128_t::bits() const{
    uint8_t out = 0;
    if (UPPER){
        out = 64;
        uint64_t up = UPPER;
        while (up){
            up >>= 1;
            out++;
        }
    }
    else{
        uint64_t low = LOWER;
        while (low){
            low >>= 1;
            out++;
        }
    }
    return out;
}

std::string uint128_t::str(uint8_t base, const unsigned int & len) const{
    if ((base < 2) || (base > 16)){
        throw std::invalid_argument("Base must be in the range [2, 16]");
    }
    std::string out = "";
    if (!(*this)){
        out = "0";
    }
    else{
        std::pair <uint128_t, uint128_t> qr(*this, uint128_0);
        do{
            qr = divmod(qr.first, base);
            out = "0123456789abcdef"[(uint8_t) qr.second] + out;
        } while (qr.first);
    }
    if (out.size() < len){
        out = std::string(len - out.size(), '0') + out;
    }
    return out;
}

uint128_t operator<<(const bool & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

uint128_t operator<<(const uint8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

uint128_t operator<<(const uint16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

uint128_t operator<<(const uint32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

uint128_t operator<<(const uint64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

uint128_t operator<<(const int8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

uint128_t operator<<(const int16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

uint128_t operator<<(const int32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

uint128_t operator<<(const int64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

uint128_t operator>>(const bool & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

uint128_t operator>>(const uint8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

uint128_t operator>>(const uint16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

uint128_t operator>>(const uint32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

uint128_t operator>>(const uint64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

uint128_t operator>>(const int8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

uint128_t operator>>(const int16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

uint128_t operator>>(const int32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

uint128_t operator>>(const int64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

std::ostream & operator<<(std::ostream & stream, const uint128_t & rhs){
    if (stream.flags() & stream.oct){
        stream << rhs.str(8);
    }
    else if (stream.flags() & stream.dec){
        stream << rhs.str(10);
    }
    else if (stream.flags() & stream.hex){
        stream << rhs.str(16);
    }
    return stream;
}
