#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <limits>
using namespace std;

class Car {
public:
    string carNo;
    string ownerName;
    string phoneNo;
};

class Park {
public:
    int slot;
    bool isAvailable;
    Park()
    {
        slot=0;
        isAvailable=true;
    }
    Park(int s) {
        isAvailable=true;
        slot = s;
    
    }

    bool validslot(int n)
    {
        if(slot>0 && slot<=n)
        {
           return true;
        }
        else
        {
            throw runtime_error("Slot " + to_string(slot) + " not in parking. Plaese enter valid slot.");
           
        }
        return false;
    
    }
};

void showMenu() {
    cout << "----- Car Parking Management System -----" << endl;
    cout << "1. Park car" << endl;
    cout << "2. Exit car" << endl;
    cout << "3. View parking slots" << endl;
    cout << "4. View parked cars" << endl;
    cout << "5. Exit program" << endl;
    cout << "Enter your choice (1-5): ";
}

void saveCarToDisk(Car car, int slot) {
    ofstream outfile("cars.txt", ios::app);
    if (!outfile) {
        cerr << "Error: Could not open file." << endl;
        return;
    }
    outfile << slot << "," << car.carNo << "," << car.ownerName << "," << car.phoneNo << endl;
    outfile.close();
}

void removeCarFromDisk(int slot) {
    ofstream outfile("temp.txt");
    ifstream infile("cars.txt");
    if (!infile || !outfile) {
        cerr << "Error: Could not open file." << endl;
        return;
    }
    string line;
    while (getline(infile, line)) {
        if (line.substr(0, line.find(",")) == to_string(slot)) {
            continue;
        }
        outfile << line << endl;
    }
    infile.close();
    outfile.close();
    remove("cars.txt");
    rename("temp.txt", "cars.txt");
}

void showParkedCars() {
    ifstream infile("cars.txt");
    if (!infile) {
        cerr << "Error: Could not open file." << endl;
        return;
    }
    cout << "----- Parked Cars -----" << endl;
    string line;
    while (getline(infile, line)) {
        int pos = line.find(",");
        cout << "Slot " << line.substr(0, pos) << ":" << endl;
        line = line.substr(pos + 1);
        pos = line.find(",");
        cout << "  Car Number: " << line.substr(0, pos) << endl;
        line = line.substr(pos + 1);
        pos = line.find(",");
        cout << "  Owner Name: " << line.substr(0, pos) << endl;
        line = line.substr(pos + 1);
        cout << "  Phone Number: " << line << endl;
    }
    infile.close();
}

int main() {
    int n;
    cout<<"Enter number of available parking slots"<<endl;
    cin>>n;
    Park parkingSlots[n];
    // Initialize 5 parking slots
    for(int i=0;i<n;i++)
    {
        
        parkingSlots[i]=Park(i+1);
        
    }


    int choice;
    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: // Park car
            {
                // Get car details from user
                Car car;
            cout << "Enter car number: ";
            cin.ignore();
            getline(cin, car.carNo);
             cout << "Enter owner name: ";
            getline(cin, car.ownerName);
            cout << "Enter phone number: ";
            getline(cin, car.phoneNo);

            // Find an available parking slot
           
            int s;
            cout<<"Enter slot"<<endl;
            cin>>s;
            s=s-1;
            while(!parkingSlots[s].isAvailable || s<0 || s>n)
            {
                cout<<"Please enter another slot."<<endl;
                cin>>s;
                s=s-1;
            }
            // Park the car and mark the slot as occupied
            parkingSlots[s].isAvailable = false;
            cout << "Car parked at slot " << parkingSlots[s].slot << "." << endl;

            // Save the car to disk
            saveCarToDisk(car, parkingSlots[s].slot);
           
            break;
        }

        case 2: // Exit car
        {
            // Get parking slot number from user
            int slotNo;
            cout << "Enter parking slot number: ";
            cin >> slotNo;

            try
            {
                parkingSlots[slotNo].validslot(n);
            }
            catch(exception &e)
            {
                cerr<<"Error: "<<e.what()<<endl;
            }
            

            // Mark the parking slot as available
            parkingSlots[parkingSlots->slot-1].isAvailable = true;
            cout << "Car exited from slot " << slotNo << "." << endl;

            // Remove the car from disk
            removeCarFromDisk(slotNo);
            break;
        }

        case 3: // View parking slots
        {
            cout << "----- Parking Slots -----" << endl;
            for (int i = 0; i < n; i++) {
                cout << "Slot " << parkingSlots[i].slot;
                if (parkingSlots[i].isAvailable) {
                    cout << " (available)";
                } else {
                    cout << " (occupied)";
                }
                cout << endl;
            }
            break;
        }

        case 4: // View parked cars
        {
            showParkedCars();
            break;
        }

        case 5: // Exit program
        {
            cout << "Exiting program..." << endl;
            break;
        }

        default:
            cerr << "Error: Invalid choice." << endl;
    }

    // Ignore any remaining characters in input stream
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
} while (choice != 5);

return 0;
}

