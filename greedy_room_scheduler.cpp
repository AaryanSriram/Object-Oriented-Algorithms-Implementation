#include <bits/stdc++.h>
using namespace std;

// -------------------- STRUCT --------------------
struct Meeting {
    int id;
    int start;
    int end;
    int room;
    int index;   // original input order (0-indexed)
};

// -------------------- FUNCTOR: SORT BY START TIME --------------------
struct CmpStart {
    bool operator()(const Meeting &a, const Meeting &b) const {
        if (a.start == b.start)
            return a.index < b.index;   // tie-break: preserve input order
        return a.start < b.start;
    }
};

// -------------------- FUNCTOR: SORT BY ROOM --------------------
struct CmpRoom {
    bool operator()(const Meeting &a, const Meeting &b) const {
        if (a.room == b.room)
            return a.start < b.start;   // secondary: start time within same room
        return a.room < b.room;         // primary: room number
    }
};

// -------------------- FUNCTION 1: ASSIGN ROOMS --------------------
int assignRooms(vector<Meeting> &meetings) {
    sort(meetings.begin(), meetings.end(), CmpStart());

    set<pair<int, int>> active;   // {endTime, roomNumber}
    int roomCount = 0;

    for (auto &m : meetings) {
        if (!active.empty() && active.begin()->first <= m.start) {
            auto it = active.begin();
            int roomNo = it->second;
            active.erase(it);

            m.room = roomNo;
            active.insert({m.end, roomNo});
        } else {
            roomCount++;
            m.room = roomCount;
            active.insert({m.end, roomCount});
        }
    }

    return roomCount;
}

// -------------------- FUNCTION 2: MOST USED ROOM --------------------
int findMostUsedRoom(const vector<Meeting> &meetings, int totalRooms) {
    vector<int> freq(totalRooms + 1, 0);

    for (const auto &m : meetings)
        freq[m.room]++;

    int bestRoom = 1;
    for (int i = 2; i <= totalRooms; i++) {
        if (freq[i] > freq[bestRoom])
            bestRoom = i;
    }

    return bestRoom;
}

// -------------------- FUNCTION 3: LONGEST MEETING --------------------
int findLongestMeeting(const vector<Meeting> &meetings) {
    int bestId = meetings[0].id;
    int bestDur = meetings[0].end - meetings[0].start;

    for (const auto &m : meetings) {
        int dur = m.end - m.start;
        if (dur > bestDur || (dur == bestDur && m.id < bestId)) {
            bestDur = dur;
            bestId = m.id;
        }
    }

    return bestId;
}

// -------------------- FUNCTION 4: FIND MEETINGS AT TIME --------------------
void findMeetingsAtTime(const vector<Meeting> &meetings, int time) {
    vector<Meeting> sorted_meetings = meetings;
    sort(sorted_meetings.begin(), sorted_meetings.end(), CmpStart());

    bool found = false;
    for (const auto &m : sorted_meetings) {
        if (m.start <= time && time < m.end) {
            if (found) cout << " ";
            cout << m.id;
            found = true;
        }
    }

    if (!found)
        cout << "None";

    cout << "\n";
}

// -------------------- FUNCTION 5: PRINT SORTED LIST --------------------
void printSortedList(vector<Meeting> meetings) {
    sort(meetings.begin(), meetings.end(), CmpStart());

    for (const auto &m : meetings) {
        cout << "Meeting " << m.id
             << " [" << m.start << "," << m.end << "]\n";
    }
}

// -------------------- FUNCTION 6: PRINT SCHEDULE --------------------
void printSchedule(vector<Meeting> meetings) {
    sort(meetings.begin(), meetings.end(), CmpRoom());

    for (const auto &m : meetings) {
        cout << "Meeting " << m.id
             << " -> Room " << m.room
             << " [" << m.start << "," << m.end << "]\n";
    }
}

// -------------------- MAIN --------------------
int main() {

    int n;
    cin >> n;

    vector<Meeting> meetings(n);

    for (int i = 0; i < n; i++) {
        cin >> meetings[i].id >> meetings[i].start >> meetings[i].end;
        meetings[i].room = -1;
        meetings[i].index = i;
    }

    // assign rooms
    int totalRooms = assignRooms(meetings);

    // precompute query results
    int mostUsedRoom = findMostUsedRoom(meetings, totalRooms);
    int longestMeetingId = findLongestMeeting(meetings);

    // process queries
    int q;
    cin >> q;

    while (q--) {
        string command;
        cin >> command;

        if (command == "sortedlist") {
            printSortedList(meetings);
        }
        else if (command == "schedule") {
            printSchedule(meetings);
        }
        else if (command == "rooms") {
            cout << totalRooms << "\n";
        }
        else if (command == "mostused") {
            cout << mostUsedRoom << "\n";
        }
        else if (command == "longest") {
            cout << longestMeetingId << "\n";
        }
        else if (command == "find") {
            int time;
            cin >> time;
            findMeetingsAtTime(meetings, time);
        }
    }

    return 0;
}