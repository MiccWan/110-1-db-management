#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using tp = chrono::system_clock::time_point;

// ###########################################################################
//  Util
// ###########################################################################

// @see: https://stackoverflow.com/a/29447725/11309695
double to_prec(double v, int prec) {
  double p = pow(10.0f, (double)prec);
  return round(v * p) / p;
}

// @see: https://stackoverflow.com/a/18974171/11309695
double slope(const vector<int>& x, const vector<int>& y) {
  int n = x.size();

  double avgX = accumulate(x.begin(), x.end(), 0.0) / n;
  double avgY = accumulate(y.begin(), y.end(), 0.0) / n;

  double numerator = 0.0;
  double denominator = 0.0;

  for (int i = 0; i < n; ++i) {
    numerator += (x[i] - avgX) * (y[i] - avgY);
    denominator += (x[i] - avgX) * (x[i] - avgX);
  }

  return numerator / denominator;
}

// @see: https://stackoverflow.com/a/65075284/11309695
vector<string> split(string str, char delim) {
  string tmp;
  vector<string> vec;
  stringstream ss(str);
  while (getline(ss, tmp, delim)) {
    vec.push_back(tmp);
  }
  return vec;
}

// @see: https://stackoverflow.com/a/21021900/11309695
tp str_to_tp(string str) {
  tm tm = {};
  stringstream ss(str);
  ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
  auto tp = chrono::system_clock::from_time_t(mktime(&tm));
  return tp;
}

tp date_to_tp(string str) {
  tm tm = {};
  stringstream ss(str);
  ss >> get_time(&tm, "%Y-%m-%d");
  auto tp = chrono::system_clock::from_time_t(mktime(&tm));
  return tp;
}

// ###########################################################################
//  structs
// ###########################################################################

struct Record {
  string id, user_id, mission_id, status;
  int correct_count, answered_count, answer_duration;
  tp started_at, finished_at, created_at;
  Record(string line_str) {
    vector<string> data = split(line_str, ',');
    id = data[0];
    user_id = data[1];
    mission_id = data[2];
    status = data[3];
    if (status != "Completed") return;
    correct_count = stoi(data[4]);
    answered_count = stoi(data[5]);
    answer_duration = stoi(data[6]);
    started_at = str_to_tp(data[7]);
    finished_at = str_to_tp(data[8]);
    created_at = str_to_tp(data[9]);
  }
};

struct Player {
  string user_id;
  int answered[4] = {0};
  int correct[4] = {0};
  double correct_rate[4] = {0};
  double slopes[3] = {0};
  int x = 0;
  double s = 0;
  Player(string _user_id) { user_id = _user_id; }

  double mean_correct_rate() {
    double sum = 0;
    for (int i = 0; i < 4; ++i) sum += correct_rate[i];
    return sum / 4;
  }
};

struct PlayerCollection {
  int size;
  int mean_correct_rate;

  PlayerCollection(vector<Player> players) {
    size = players.size();
    if (size) {
      double sum = 0;
      for (auto player : players) sum += to_prec(player.mean_correct_rate(), 3);
      mean_correct_rate = floor((sum / size) * 100 + 1e-10);
    } else {
      mean_correct_rate = 0;
    }
  }
};

// ###########################################################################
//  wrapped control flow
// ###########################################################################

unordered_map<string, Player> all_players;

vector<Record> get_valid_records(string file_name, tp given_time_tp) {
  ifstream fs(file_name);
  string line, tmp;
  vector<Record> recs;

  // extract(omit) csv header
  getline(fs, line);

  // get, parse and filter records
  while (getline(fs, line)) {
    Record rec(line);
    if (rec.status == "Completed") {
      auto diff = given_time_tp - rec.started_at - chrono::seconds(1);
      if (diff >= chrono::hours(0 * 24) && diff < chrono::hours(28 * 24)) {
        recs.push_back(rec);
      }
    }
  }
  return recs;
}

Player& get_player(string user_id) {
  if (all_players.find(user_id) != all_players.end()) {
    return all_players.at(user_id);
  } else {
    all_players.insert(make_pair(user_id, Player(user_id)));
    return all_players.at(user_id);
  }
}

// ###########################################################################
//  main
// ###########################################################################

int main() {
  string file_name, given_time_str;
  int t;
  getline(cin, file_name);
  getline(cin, given_time_str);
  cin >> t;
  // file_name = "../data/user_missions.csv";
  // given_time_str = "2021-02-22";
  // t = 15;

  // ######################################
  //  pre-processing
  // ######################################

  tp given_time_tp = date_to_tp(given_time_str);

  // step 1, 2: filter by field "status" and "started_at"
  vector<Record> recs = get_valid_records(file_name, given_time_tp);

  // step 3-1: sum up "correct" and "answered" by "user_id"
  for (auto rec : recs) {
    auto diff = given_time_tp - rec.started_at - chrono::seconds(1);
    auto week = chrono::duration_cast<chrono::hours>(diff).count() / (24 * 7);
    Player& p = get_player(rec.user_id);
    p.correct[week] += rec.correct_count;
    p.answered[week] += rec.answered_count;
  }

  // step 3-2, 4: calculate "correct_rate"
  for (auto& _p : all_players) {
    Player& p = _p.second;

    // calculate non-empty weeks
    for (int i = 0; i < 4; ++i) {
      if (p.answered[i] != 0) {
        p.correct_rate[i] = to_prec((double)p.correct[i] / (double)p.answered[i], 3);
      }
    }

    // calculate mean
    int j = 0;
    double sum_rate = 0;
    for (int i = 0; i < 4; ++i) {
      if (p.answered[i] != 0) {
        ++j;
        sum_rate += p.correct_rate[i];
      }
    }

    // fill empty weeks with mean
    double mean_rate = to_prec(sum_rate / (double)j, 3);
    for (int i = 0; i < 4; ++i) {
      if (p.answered[i] == 0) {
        p.correct_rate[i] = mean_rate;
      }
    }
  }

  // step 5: calculate "the slopes"
  for (auto& _p : all_players) {
    Player& p = _p.second;
    p.slopes[0] = slope(vector<int>{-1, -2, -3, -4},
                        vector<int>{p.answered[0], p.answered[1], p.answered[2], p.answered[3]});
    p.slopes[1] =
        slope(vector<int>{-1, -2, -3}, vector<int>{p.answered[0], p.answered[1], p.answered[2]});
    p.slopes[2] = slope(vector<int>{-1, -2}, vector<int>{p.answered[0], p.answered[1]});
  }

  // ######################################
  //  classification
  // ######################################

  // calculate s, x
  for (auto& _p : all_players) {
    Player& p = _p.second;
    for (int i = 0; i < 4; ++i) {
      p.s += p.answered[i];
    }
    p.s /= 4;
    p.x = (int)(p.slopes[0] >= 0) + (int)(p.slopes[1] >= 0) + (int)(p.slopes[2] >= 0);
  }

  // classify
  vector<Player> p_vecs[4];
  for (auto& _p : all_players) {
    Player& p = _p.second;
    if (p.s >= t && p.x >= 2) {
      p_vecs[0].push_back(p);
    } else if (p.s >= t) {
      p_vecs[1].push_back(p);
    } else if (p.x >= 2) {
      p_vecs[2].push_back(p);
    } else {
      p_vecs[3].push_back(p);
    }
  }

  // calculate collection data and log
  for (int i = 0; i < 4; ++i) {
    PlayerCollection p_col(p_vecs[i]);
    cout << p_col.size << "," << p_col.mean_correct_rate << endl;
  }
}