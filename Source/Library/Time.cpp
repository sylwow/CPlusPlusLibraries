#include "Time.hpp"
#include "DetectOs.hpp"
#include <chrono>
#include <cmath>
#include <tuple>

#ifdef WINDOWS
#include <format>

namespace sd
{

    namespace ch
    {

        using namespace std::chrono;
        template <class Rep, class Period> using dur = duration<Rep, Period>;

        using HHMMSS = hh_mm_ss<milliseconds>;
    } // namespace ch

    namespace
    {
        template <class D> D roundToZero(ch::microseconds dur) { return dur < 0 ? ceil<D>(dur) : floor<D>(dur); }

        template <class D> auto castToMicroseconds(D dur) { return duration_cast<ch::microseconds>(dur); }

        template <class D> Time from(int duration) { return Time{castToMicroseconds(D{duration})}; }
        template <class D> Time from(double duration) { return Time{castToMicroseconds(D{1} * duration)}; }
        template <class D> auto to(Time time) { return time.toDuration<ch::dur<double, typename D::period>>().count(); }

        auto decomposeDays(ch::microseconds duration)
        {
            auto days = roundToZero<ch::days>(duration);
            auto rest = duration - days;
            return std::make_pair(days, rest);
        }

        auto decomposeHHMMSS(ch::microseconds duration)
        {
            auto [days, rest] = decomposeDays(duration);
            ch::milliseconds restInMili = roundToZero<ch::milliseconds>(rest);
            ch::microseconds restMicroseconds = rest - restInMili;
            ch::HHMMSS hh{restInMili};
            return std::make_pair(hh, restMicroseconds);
        }

#define DECOMPOSE(what)                                                                                                \
    auto hh = decomposeHHMMSS(duration).first;                                                                         \
    return hh.what().count() * (hh.is_negative() ? -1 : 1);

        auto decomposeHours(ch::microseconds duration) { DECOMPOSE(hours); }
        auto decomposeMinutes(ch::microseconds duration) { DECOMPOSE(minutes); }
        auto decomposeSeconds(ch::microseconds duration) { DECOMPOSE(seconds); }
        auto decomposeMiliseconds(ch::microseconds duration) { DECOMPOSE(subseconds); }
        auto decomposeMicroseconds(ch::microseconds duration) { return decomposeHHMMSS(duration).second.count(); };

    } // namespace

    const Time Time::max = Time{ch::microseconds::max()};
    const Time Time::min = Time{ch::microseconds::min()};
    const Time Time::zero = Time{ch::microseconds::zero()};

    Time Time::parse(const std::string &source, const std::string &format)
    {
        ch::microseconds duration;
        std::stringstream ss{source};
        from_stream(ss, format.c_str(), duration);
        return Time{duration};
    }

    Time Time::fromDays(int days) { return from<ch::days>(days); }
    Time Time::fromDays(double days) { return from<ch::days>(days); }
    Time Time::fromHours(int hours) { return from<ch::hours>(hours); }
    Time Time::fromHours(double hours) { return from<ch::hours>(hours); }
    Time Time::fromMinutes(int minutes) { return from<ch::minutes>(minutes); }
    Time Time::fromMinutes(double minutes) { return from<ch::minutes>(minutes); }
    Time Time::fromSeconds(int seconds) { return from<ch::seconds>(seconds); }
    Time Time::fromSeconds(double seconds) { return from<ch::seconds>(seconds); }
    Time Time::fromMiliseconds(int miliseconds) { return from<ch::milliseconds>(miliseconds); }
    Time Time::fromMiliseconds(double miliseconds) { return from<ch::milliseconds>(miliseconds); }
    Time Time::fromMicroseconds(long long microseconds) { return Time{microseconds}; }

    Time::Time(long long microseconds) : _time{ch::microseconds{microseconds}} {}
    Time::Time(int hour, int minute, int second) : _time{ch::hours{hour} + ch::minutes{minute} + ch::seconds{second}} {}
    Time::Time(int days, int hour, int minute, int second, int miliseconds, int microseconds)
        : _time{ch::days{days} + ch::hours{hour} + ch::minutes{minute} + ch::seconds{second} +
                ch::milliseconds{miliseconds} + ch::microseconds{microseconds}}
    {
    }

    int Time::days() const { return decomposeDays(raw()).first.count(); }
    int Time::hours() const { return decomposeHours(raw()); }
    int Time::minutes() const { return decomposeMinutes(raw()); }
    int Time::seconds() const { return decomposeSeconds(raw()); }
    int Time::miliseconds() const { return decomposeMiliseconds(raw()); }
    int Time::microseconds() const { return decomposeMicroseconds(raw()); }

    double Time::totalDays() const { return to<ch::days>(raw()); }
    double Time::totalHours() const { return to<ch::hours>(raw()); }
    double Time::totalMinutes() const { return to<ch::minutes>(raw()); }
    double Time::totalSeconds() const { return to<ch::seconds>(raw()); }
    double Time::totalMiliseconds() const { return to<ch::milliseconds>(raw()); }
    long long Time::totalMicroseconds() const { return raw().count(); }
    ch::microseconds Time::raw() const { return _time; }

    std::string Time::toString(const std::string &format) const { return std::format(format, raw()); }

#define TIME_OPERATION(operator, operand)                                                                              \
    _time operator operand;                                                                                            \
    return *this;

    Time &Time::add(const Time &other) { TIME_OPERATION(+=, other.raw()); }
    Time &Time::substract(const Time &other) { TIME_OPERATION(-=, other.raw()); }
    Time &Time::multiply(double factor) { TIME_OPERATION(*=, factor); }
    Time &Time::divide(double factor) { TIME_OPERATION(/=, factor); }
    Time &Time::negate() { TIME_OPERATION(=, -_time); }

    Time &Time::operator+=(const Time &other) { return add(other); }
    Time &Time::operator-=(const Time &other) { return substract(other); }
    Time &Time::operator*=(double factor) { return multiply(factor); }
    Time &Time::operator/=(double factor) { return divide(factor); }

    Time Time::operator+() const { return Time{*this}; }
    Time Time::operator-() const { return Time{*this}.negate(); }

    // Time::operator bool() const { return raw() != 0; }

    Time operator+(const Time &lhs, const Time &rhs) { return Time{lhs}.add(rhs); }
    Time operator-(const Time &lhs, const Time &rhs) { return Time{lhs}.substract(rhs); }
    Time operator*(const Time &time, double factor) { return Time{time}.multiply(factor); }
    Time operator/(const Time &time, double factor) { return Time{time}.divide(factor); }

    bool operator==(const Time &lhs, const Time &rhs) { return lhs.raw() == rhs.raw(); }
    bool operator!=(const Time &lhs, const Time &rhs) { return lhs.raw() != rhs.raw(); }
    bool operator<(const Time &lhs, const Time &rhs) { return lhs.raw() < rhs.raw(); }
    bool operator<=(const Time &lhs, const Time &rhs) { return lhs.raw() <= rhs.raw(); }
    bool operator>(const Time &lhs, const Time &rhs) { return lhs.raw() > rhs.raw(); }
    bool operator>=(const Time &lhs, const Time &rhs) { return lhs.raw() >= rhs.raw(); }

    Time operator""_d(long double days) { return Time::fromDays(static_cast<double>(days)); }
    Time operator""_d(unsigned long long days) { return Time::fromDays(static_cast<int>(days)); }

    Time operator""_h(long double hours) { return Time::fromHours(static_cast<double>(hours)); }
    Time operator""_h(unsigned long long hours) { return Time::fromHours(static_cast<int>(hours)); }

    Time operator""_min(long double minutes) { return Time::fromMinutes(static_cast<double>(minutes)); }
    Time operator""_min(unsigned long long minutes) { return Time::fromMinutes(static_cast<int>(minutes)); }

    Time operator""_s(long double seconds) { return Time::fromSeconds(static_cast<double>(seconds)); }
    Time operator""_s(unsigned long long seconds) { return Time::fromSeconds(static_cast<int>(seconds)); }

    Time operator""_ms(long double miliseconds) { return Time::fromMiliseconds(static_cast<double>(miliseconds)); }
    Time operator""_ms(unsigned long long miliseconds) { return Time::fromMiliseconds(static_cast<int>(miliseconds)); }

    Time operator""_us(unsigned long long microseconds)
    {
        return Time::fromMicroseconds(static_cast<long long>(microseconds));
    }
} // namespace sd
#endif
