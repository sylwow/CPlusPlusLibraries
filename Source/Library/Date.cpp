#include "Date.hpp"

namespace sd
{
    using Year = std::chrono::year;
    using Month = std::chrono::month;
    using Day = std::chrono::day;
    using CYears = std::chrono::years;
    using CMonths = std::chrono::months;
    using CDays = std::chrono::days;
    using Hours = std::chrono::hours;
    using Minutes = std::chrono::minutes;
    using Seconds = std::chrono::seconds;
    using Milliseconds = std::chrono::milliseconds;
    using Microseconds = std::chrono::microseconds;
    using YearMontDay = std::chrono::year_month_day;

    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    using TimePointDays = std::chrono::time_point<std::chrono::system_clock, std::chrono::days>;

    using namespace std::chrono;
    namespace
    {
        template <class D> auto castToMicroseconds(D dur) { return duration_cast<Microseconds>(dur); }

        TimePointDays toTotalDays(TimePoint timePoint) { return floor<CDays>(timePoint); }

        TimePoint createTimePoint(YearMontDay ymd, Time dayTime)
        {
            if (!ymd.ok())
                ymd = ymd.year() / ymd.month() / last;
            return sys_days{ymd} + dayTime.raw();
        }

        auto decomposeTimePoint(TimePoint timePoint)
        {
            auto days = toTotalDays(timePoint);
            Microseconds rest = castToMicroseconds(timePoint - days);
            return std::make_pair(YearMontDay{days}, rest);
        }

    } // namespace

    Date Date::parse(const std::string &source, const std::string &format)
    {
        TimePoint timePoint;
        std::stringstream ss{source};
        from_stream(ss, format.c_str(), timePoint);
        return Date{timePoint};
    }

    Date Date::now() { return Date{system_clock::now()}; }
    Date Date::max() { return Date{system_clock::now().max()}; }
    Date Date::min() { return Date{system_clock::now().min()}; }
    Date Date::today() { return Date{Date::now().yearMonthDay(), 0}; }

    Date::Date(long long microseconds) { add(Microseconds{microseconds}); }
    Date::Date(int year, unsigned month, unsigned day, int hour, int minute, int second, int milisecond)
        : Date{{Year{year}, Month{month}, Day{day}},
               Hours{hour} + Minutes{minute} + Seconds{second} + Milliseconds{milisecond}}
    {
    }
    Date::Date(std::chrono::year_month_day date, Time timeOfDay) : Date{createTimePoint(date, timeOfDay)} {}
    Date::Date(TimePoint timePoint) { _timePoint = timePoint; }

    int Date::year() const { return int{yearMonthDay().year()}; }
    int Date::day() const { return unsigned{yearMonthDay().day()}; }
    int Date::month() const { return unsigned{yearMonthDay().month()}; }
    int Date::hour() const { return timeOfDay().hours(); }
    int Date::minute() const { return timeOfDay().minutes(); }
    int Date::second() const { return timeOfDay().seconds(); }
    int Date::milisecond() const { return timeOfDay().miliseconds(); }
    long long Date::microsecond() const { return timeOfDay().microseconds(); }

    TimePoint Date::raw() const { return _timePoint; }

    YearMontDay Date::yearMonthDay() const { return decomposeTimePoint(raw()).first; }

    Time Date::timeOfDay() const { return Time{decomposeTimePoint(raw()).second}; }

    std::string Date::toString(const std::string &format) const { return std::format(format, raw()); }

    Date &Date::add(const Time &time)
    {
        _timePoint += time.raw();
        return *this;
    }
    Date &Date::addYears(int years)
    {
        auto [ymd, timeOfDay] = decomposeTimePoint(raw());
        ymd += CYears{years};
        _timePoint = createTimePoint(ymd, castToMicroseconds(timeOfDay));
        return *this;
    }
    Date &Date::addMonths(int months)
    {
        auto [ymd, timeOfDay] = decomposeTimePoint(raw());
        ymd += CMonths{months};
        _timePoint = createTimePoint(ymd, castToMicroseconds(timeOfDay));
        return *this;
    }
    Date &Date::addDays(int days) { return add(Time::fromDays(days)); }
    Date &Date::addHours(int hours) { return add(Time::fromHours(hours)); }
    Date &Date::addMinutes(int minutes) { return add(Time::fromMinutes(minutes)); }
    Date &Date::addSeconds(int seconds) { return add(Time::fromSeconds(seconds)); }
    Date &Date::addMiliseconds(int miliseconds) { return add(Time::fromMiliseconds(miliseconds)); }
    Date &Date::addMicroseconds(long long microseconds) { return add(Time::fromMicroseconds(microseconds)); }

    Time Date::substract(const Date &date) const { return Time{castToMicroseconds(raw() - date.raw())}; }
    Date &Date::substract(const Time &time) { return add(-time); }
    Date &Date::substractYears(int years) { return addYears(-years); }
    Date &Date::substractMonths(int months) { return addMonths(-months); }
    Date &Date::substractDays(int days) { return addDays(-days); }
    Date &Date::substractHours(int hours) { return addHours(-hours); }
    Date &Date::substractMinutes(int minutes) { return addMinutes(-minutes); }
    Date &Date::substractSeconds(int seconds) { return addSeconds(-seconds); }
    Date &Date::substractMiliseconds(int miliseconds) { return addMiliseconds(-miliseconds); }
    Date &Date::substractMicroseconds(long long microseconds) { return addMicroseconds(-microseconds); }

    Date &Date::operator+=(const Time &time) { return add(time); }
    Date &Date::operator+=(const Years &years) { return addYears(years.value); }
    Date &Date::operator+=(const Months &months) { return addMonths(months.value); }

    Date &Date::operator-=(const Time &time) { return substract(time); }
    Date &Date::operator-=(const Years &years) { return substractYears(years.value); }
    Date &Date::operator-=(const Months &months) { return substractMonths(months.value); }

    Date operator+(const Date &date, const Time &time) { return Date{date}.add(time); }
    Date operator+(const Date &date, const Years &years) { return Date{date}.addYears(years.value); }
    Date operator+(const Date &date, const Months &months) { return Date{date}.addMonths(months.value); }

    Date operator-(const Date &date, const Time &time) { return Date{date}.substract(time); }
    Date operator-(const Date &date, const Years &years) { return Date{date}.substractYears(years.value); }
    Date operator-(const Date &date, const Months &months) { return Date{date}.substractMonths(months.value); }

    Time operator-(const Date &lhs, const Date &rhs) { return Date{lhs}.substract(rhs); }

    bool operator==(const Date &lhs, const Date &rhs) { return lhs.raw() == rhs.raw(); }
    bool operator!=(const Date &lhs, const Date &rhs) { return lhs.raw() != rhs.raw(); }
    bool operator<(const Date &lhs, const Date &rhs) { return lhs.raw() < rhs.raw(); }
    bool operator<=(const Date &lhs, const Date &rhs) { return lhs.raw() <= rhs.raw(); }
    bool operator>(const Date &lhs, const Date &rhs) { return lhs.raw() > rhs.raw(); }
    bool operator>=(const Date &lhs, const Date &rhs) { return lhs.raw() >= rhs.raw(); }

    Years operator"" _y(unsigned long long years) { return Years(static_cast<int>(years)); }
    Months operator"" _mo(unsigned long long months) { return Months(static_cast<int>(months)); }

} // namespace sd