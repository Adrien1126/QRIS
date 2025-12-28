#include "time/date.hpp"
#include <sstream>
#include <iomanip>
#include "date.hpp"

namespace qris::time {

int Date::dateToSerialNumber() const
{
    int refYear = 1900; 
    int serialDate = 0; 
    // Ajout des années et conversion en nombre de jours
    for (int i = refYear; i < year_; i++) {
        if (isLeapYear(i)) {
            serialDate += 366; 
        }
        else {
            serialDate += 365; 
        }
    }

    for (int j = 1; j < month_; j++) {
        serialDate += daysInMonth(j, year_); 
    }

    serialDate += day_; 
    return serialDate; 
}

std::string Date::toString() const
{
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << year_ << "-"
        << std::setw(2) << std::setfill('0') << month_ << "-"
        << std::setw(2) << std::setfill('0') << day_;
    return oss.str();
}


Date::Date(int serialNumber)
{
    if (serialNumber < 1) {
        throw std::invalid_argument("Serial number must be >= 1");
    }

    int remainingDays = serialNumber - 1; // 01/01/1900 correspond à 0 jour écoulé
    int y = 1900;

    // Consommer les années complètes
    while (true) {
        int daysInYear = isLeapYear(y) ? 366 : 365;
        if (remainingDays >= daysInYear) {
            remainingDays -= daysInYear;
            ++y;
        } else {
            break;
        }
    }

    int m = 1;

    // Consommer les mois complets de l'année courante
    while (true) {
        int dim = daysInMonth(m, y);
        if (remainingDays >= dim) {
            remainingDays -= dim;
            ++m;
        } else {
            break;
        }
    }

    int d = remainingDays + 1;

    // Affectation finale (validation par invariants déjà garantis)
    year_  = y;
    month_ = m;
    day_   = d;

    validate(day_, month_, year_);
}

bool Date::operator==(const Date &other) const
{
    return this->dateToSerialNumber() == other.dateToSerialNumber();
}

bool Date::operator!=(const Date &other) const
{
    return this->dateToSerialNumber() != other.dateToSerialNumber();
}

bool Date::operator<(const Date &other) const
{
    return this->dateToSerialNumber() < other.dateToSerialNumber();
}

bool Date::operator<=(const Date &other) const
{
    return this->dateToSerialNumber() <= other.dateToSerialNumber();
}

bool Date::operator>(const Date &other) const
{
    return this->dateToSerialNumber() > other.dateToSerialNumber();
}

bool Date::operator>=(const Date &other) const
{
    return this->dateToSerialNumber() >= other.dateToSerialNumber();
}

Date Date::addDays(int days) const
{
    return Date(this->dateToSerialNumber() + days); 
}

int Date::diff(const Date &other) const
{
    return this->dateToSerialNumber() - other.dateToSerialNumber();
}

std::ostream& operator<<(std::ostream& os, const Date& d)
{
    os << d.toString();
    return os;
}

} //namespace qris::time


