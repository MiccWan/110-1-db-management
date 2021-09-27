#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <vector>
#include <iterator>
#include <unordered_map>

using namespace std;
using tp = chrono::system_clock::time_point;

void log_tp_date(tp tp)
{
    time_t tt = chrono::system_clock::to_time_t(tp);
    tm local_tm = *localtime(&tt);

    cout << local_tm.tm_year + 1900 << '-' << local_tm.tm_mon + 1 << '-' << local_tm.tm_mday << endl;
}

vector<string> split(string str, char delim)
{
    string tmp;
    vector<string> vec;
    stringstream ss(str);
    while (getline(ss, tmp, delim))
    {
        vec.push_back(tmp);
    }
    return vec;
}
chrono::system_clock::time_point strtotp(string str)
{
    tm tm = {};
    stringstream ss(str);
    ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
    auto tp = chrono::system_clock::from_time_t(mktime(&tm));
    return tp;
}

chrono::system_clock::time_point datetotp(string str)
{
    tm tm = {};
    stringstream ss(str);
    ss >> get_time(&tm, "%Y-%m-%d");
    auto tp = chrono::system_clock::from_time_t(mktime(&tm));
    return tp;
}

struct record
{
    string id, user_id, mission_id, status;
    int correct_count, answered_count, answer_duration, group;
    chrono::system_clock::time_point started_at, finished_at, created_at;
    record(string line_str)
    {
        vector<string> data = split(line_str, ',');
        id = data[0];
        user_id = data[1];
        mission_id = data[2];
        status = data[3];

        if (!(status == "Completed"))
        {
            return;
        }

        correct_count = stoi(data[4]);
        answered_count = stoi(data[5]);
        answer_duration = stoi(data[6]);

        started_at = strtotp(data[7]);
        finished_at = strtotp(data[8]);
        created_at = strtotp(data[9]);

        group = 0;
    }
};

struct player
{
    string id;
    int total[4] = {0};
    int correct[4] = {0};
    int rate[4] = {0};
    player(string user_id)
    {
        id = user_id;
    }
};

vector<record>
getValidRecords(string file_name, string given_date)
{
    ifstream stream(file_name);
    string line, tmp;
    vector<record> recs;
    tp given_time_tp = datetotp(given_date);
    int cnt = 0;

    // extract csv header
    getline(stream, line);

    // get, parse and filter records
    while (getline(stream, line))
    {
        ++cnt;
        record rec(line);
        if (rec.status == "Completed")
        {
            auto diff = given_time_tp - rec.started_at - chrono::seconds(1);
            if (diff >= chrono::hours(0 * 24) && diff < chrono::hours(28 * 24))
            {
                recs.push_back(rec);
                // cout << "a valid date:";
                // log_tp_date(rec.started_at);
            }
        }
    }

    cout << "valid/total rows:" << recs.size() << '/' << cnt << endl;

    return recs;
}

int main()
{
    // vector<record> recs = getValidRecords("../data/user_missions.csv", "2021-02-22");
    vector<record> recs = getValidRecords("../data/user_missions_another.csv", "2021-02-22");
    unordered_map<string, player> players;
    for (auto rec : recs)
    {
        cout << rec.answered_count << endl;
        return 0;
    }
}