#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <iomanip>
#include <limits>
using namespace std;

struct Vehicle {
    string owner;
    string number;
    int slot;
    string type;
    string inTime;
    string outTime;
};

class ParkingLot {
    vector<Vehicle> vehicles;
    stack<Vehicle> undoStack;
    int totalSlots;
public:
    ParkingLot(int n) {
        totalSlots = n;
    }

    string currentTime() {
        time_t now = time(0);
        char buf[80];
        strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", localtime(&now));
        return string(buf);
    }

    void parkVehicle(string owner, string number, string type) {
        for (auto &v : vehicles) {
            if (v.number == number && v.outTime == "-") {
                cout << "❌ Vehicle already parked in slot " << v.slot << "!\n";
                return;
            }
        }
        int slot = -1;
        for (int i = 1; i <= totalSlots; i++) {
            bool occupied = false;
            for (auto &v : vehicles) {
                if (v.slot == i && v.outTime == "-") {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                slot = i;
                break;
            }
        }
        if (slot == -1) {
            cout << "❌ Parking Full! No slots available.\n";
            return;
        }

        Vehicle v{owner, number, slot, type, currentTime(), "-"};
        vehicles.push_back(v);
        undoStack.push(v);
        cout << "✅ Vehicle parked successfully at Slot " << slot << ".\n";
    }

    void unparkVehicle(string number) {
        for (auto &v : vehicles) {
            if (v.number == number && v.outTime == "-") {
                v.outTime = currentTime();
                cout << "✅ Vehicle " << number << " unparked at " << v.outTime << endl;
                return;
            }
        }
        cout << "❌ Vehicle not found!\n";
    }

    void searchVehicle(string number) {
        for (auto &v : vehicles) {
            if (v.number == number) {
                cout << "🔍 Found: " << v.owner << " | " << v.number
                     << " | Slot " << v.slot << " | " << v.type
                     << " | In: " << v.inTime << " | Out: " << v.outTime << endl;
                return;
            }
        }
        cout << "❌ Vehicle not found!\n";
    }

    void status() {
        if (vehicles.empty()) {
            cout << "\n--- Parking Records ---\n";
            cout << "No records yet.\n";
            return;
        }

        int count = 0;
        cout << "\n--- Parking Records ---\n";
        cout << left << setw(15) << "Owner"
             << setw(15) << "Number"
             << setw(6)  << "Slot"
             << setw(10) << "Type"
             << setw(20) << "In-Time"
             << setw(20) << "Out-Time" << endl;

        for (auto &v : vehicles) {
            cout << left << setw(15) << v.owner
                 << setw(15) << v.number
                 << setw(6)  << v.slot
                 << setw(10) << v.type
                 << setw(20) << v.inTime
                 << setw(20) << v.outTime << endl;
            if (v.outTime == "-") count++;
        }
        cout << "Currently Parked Vehicles: " << count << endl;
    }

    void undoLast() {
        if (undoStack.empty()) {
            cout << "❌ Nothing to undo.\n";
            return;
        }
        Vehicle last = undoStack.top();
        undoStack.pop();

        for (auto it = vehicles.begin(); it != vehicles.end(); ++it) {
            if (it->number == last.number && it->outTime == "-") {
                vehicles.erase(it);
                cout << "↩️ Undo successful, removed vehicle " << last.number << endl;
                return;
            }
        }
        cout << "❌ Last operation already undone.\n";
    }
};

int main() {
    ParkingLot lot(10);
    int choice;
    do {
        cout << "\n===== Smart Parking Menu =====\n";
        cout << "1. Park Vehicle\n2. Unpark Vehicle\n3. Search Vehicle\n4. Status Check\n5. Undo Last Park\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        // fix: clear buffer only ONCE after reading numeric choice
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            string owner, number, type;
            cout << "Enter Owner Name: "; getline(cin, owner);
            cout << "Enter Vehicle Number: "; getline(cin, number);
            cout << "Enter Type (Car/Bike): "; getline(cin, type);
            lot.parkVehicle(owner, number, type);

        } else if (choice == 2) {
            string number;
            cout << "Enter Vehicle Number to unpark: "; getline(cin, number);
            lot.unparkVehicle(number);

        } else if (choice == 3) {
            string number;
            cout << "Enter Vehicle Number to search: "; getline(cin, number);
            lot.searchVehicle(number);

        } else if (choice == 4) {
            lot.status();

        } else if (choice == 5) {
            lot.undoLast();
        }
    } while (choice != 6);

    cout << "👋 Exiting Smart Parking System.\n";
    return 0;
}
