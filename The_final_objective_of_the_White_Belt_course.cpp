#include <iostream>
#include <string>
#include <map>
#include <set>
#include <exception>
#include <sstream>
#include <iomanip>
using namespace std;


class Date {
public:
  Date() {
    year = 0;
    month = 0;
    day = 0;
  }

  Date(const int& new_year,
       const int& new_month,
       const int& new_day) {
    stringstream ss;
    year = new_year;
    if(new_month > 12 || new_month < 1) {
      ss << "Month value is invalid: " << new_month;
      throw invalid_argument(ss.str());
    }
    month = new_month;
    if(new_day > 31 || new_day < 1) {
      ss << "Day value is invalid: " << new_day;
      throw invalid_argument(ss.str());
    }
    day = new_day;
  }

  int GetYear() const {
    return year;
  }

  int GetMonth() const {
    return month;
  }

  int GetDay() const {
    return day;
  }

private:
  int year;
  int month;
  int day;
};

ostream& operator << (ostream& stream, const Date& date) {
  stream << setw(4) << setfill('0') << date.GetYear() << "-"
         << setw(2) << setfill('0') << date.GetMonth() << "-"
         << setw(2) << setfill('0') << date.GetDay();
  return stream;
}

istream& operator >> (istream& stream, Date& date) {
  string data;
  stream >> data;
  stringstream ss(data);

  int year = 0;
  int month = 0;
  int day = 0;

  bool ok = true;
  ok = ok && (ss >> year);
  ok = ok && (ss.peek() == '-');
  ok = ok && (ss.ignore(1));

  ok = ok && (ss >> month);
  ok = ok && (ss.peek() == '-');
  ok = ok && (ss.ignore(1));

  ok = ok && (ss >> day);
  ok = ok && (ss.eof());

  if(!ok) {
    throw logic_error("Wrong date format: " + data);
  } else {
    date = {year, month, day};
  }
  return stream;
}

bool operator < (const Date& lhs, const Date& rhs) {
  if(lhs.GetYear() == rhs.GetYear()) {
    if(lhs.GetMonth() == rhs.GetMonth()) {
      return (lhs.GetDay() < rhs.GetDay());
    } else {
      return (lhs.GetMonth() < rhs.GetMonth());
    }
  } else {
    return (lhs.GetYear() < rhs.GetYear());
  }
}

bool operator > (const Date& lhs, const Date& rhs) {
  if(lhs.GetYear() == rhs.GetYear()) {
    if(lhs.GetMonth() == rhs.GetMonth()) {
      return (lhs.GetDay() > rhs.GetDay());
    } else {
      return (lhs.GetMonth() > rhs.GetMonth());
    }
  } else {
    return (lhs.GetYear() > rhs.GetYear());
  }
}

bool operator == (const Date& lhs, const Date& rhs) {
  return (lhs.GetYear() == rhs.GetYear() &&
          lhs.GetMonth() == rhs.GetMonth() &&
          lhs.GetDay() == rhs.GetDay());
}

class Database {
public:
  void AddEvent(const Date& date, const string& event) {
    if(event.size()) {
      database[date].insert(event);
    }
  }

  bool DeleteEvent(const Date& date, const string& event) {
    if(database.count(date)) {
      if(database.at(date).count(event)) {
        database.at(date).erase(event);
        return true;
      }
    }
    return false;
  }

  int DeleteDate(const Date& date) {
    int n = 0;
    if(database.count(date)) {
      n = database.at(date).size();
      database.erase(date);
    }
    return n;
  }

  set<string> Find(const Date& date) const {
    if(database.count(date)) {
      return database.at(date);
    } else {
      throw exception();
    }
  }

  void Print() const {
    for(const auto& db : database) {
      for(const auto& ev : db.second) {
        cout << db.first << " " << ev << endl;
      }
    }
  }

private:
  map<Date, set<string>> database;
};

int main() {
  Database db;

  string command;
  while (getline(cin, command)) {
    stringstream stream(command);
    string cmnd;
    stream >> cmnd;
    Date date;
    string event;
    try {
      if(cmnd.size()) {
        if("Add" == cmnd) {
          stream >> date;
          stream >> event;
          db.AddEvent(date, event);
        } else if("Del" == cmnd) {
          stream >> date;
          if(stream.peek() < 0) {
            cout << "Deleted " << db.DeleteDate(date)
                 << " events" << endl;
          } else {
            stream >> event;
            if(db.DeleteEvent(date, event)) {
              cout << "Deleted successfully" << endl;
            } else {
              cout << "Event not found" << endl;
            }
          }
        } else if("Find" == cmnd) {
          stream >> date;
          set<string> setEvents = db.Find(date);
          for(const auto se : setEvents) {
            cout << se << endl;
          }
        } else if("Print" == cmnd) {
          db.Print();
        } else {
          cout << "Unknown command: " << cmnd << endl;
        }
      }
    } catch (invalid_argument& ex) {
      cout << ex.what() << endl;
    } catch (logic_error& ex) {
      cout << ex.what() << endl;
    } catch (exception&) {

    }
  }

  return 0;
}

