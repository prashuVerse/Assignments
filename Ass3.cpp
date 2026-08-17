#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class IPSubnetCalculator {
private:
    string ip;
    vector<int> addr;
    char ipClass;
    int defaultBits;

    vector<int> parseIP(const string& ipAddress) {
        vector<int> parts;
        string temp;
        stringstream ss(ipAddress);

        while (getline(ss, temp, '.')) {
            if (temp.empty()) {
                return {};
            }
            parts.push_back(stoi(temp));
        }

        return parts;
    }

    void printIP(unsigned int value) {
        cout << ((value >> 24) & 255) << "."
             << ((value >> 16) & 255) << "."
             << ((value >> 8) & 255) << "."
             << (value & 255);
    }

    void displayClassDetails() {
        cout << "\nFirst Octet : " << addr[0];

        if (ipClass == 'A') {
            cout << "\n\n===== CLASS A =====";
            cout << "\nFirst Octet Range : 0 - 127";
            cout << "\nBinary Pattern    : 0XXXXXXX";
            cout << "\nIP Address Range  : 0.0.0.0 - 127.255.255.255";
            cout << "\nDefault Subnet Mask: 255.0.0.0";
            cout << "\nCIDR              : /8";
            cout << "\nNetwork Bits      : 8";
            cout << "\nHost Bits          : 24";
            cout << "\nHosts per Network  : 2^24 - 2";
            cout << "\nNumber of Networks : 2^7";

            if (addr[0] == 127) {
                cout << "\n\nNote: 127.x.x.x is reserved for Loopback.";
            }
        }
        else if (ipClass == 'B') {
            cout << "\n\n===== CLASS B =====";
            cout << "\nFirst Octet Range : 128 - 191";
            cout << "\nBinary Pattern    : 10XXXXXX";
            cout << "\nIP Address Range  : 128.0.0.0 - 191.255.255.255";
            cout << "\nDefault Subnet Mask: 255.255.0.0";
            cout << "\nCIDR              : /16";
            cout << "\nNetwork Bits      : 16";
            cout << "\nHost Bits          : 16";
            cout << "\nHosts per Network  : 2^16 - 2";
            cout << "\nNumber of Networks : 2^14";
        }
        else if (ipClass == 'C') {
            cout << "\n\n===== CLASS C =====";
            cout << "\nFirst Octet Range : 192 - 223";
            cout << "\nBinary Pattern    : 110XXXXX";
            cout << "\nIP Address Range  : 192.0.0.0 - 223.255.255.255";
            cout << "\nDefault Subnet Mask: 255.255.255.0";
            cout << "\nCIDR              : /24";
            cout << "\nNetwork Bits      : 24";
            cout << "\nHost Bits          : 8";
            cout << "\nHosts per Network  : 2^8 - 2";
            cout << "\nNumber of Networks : 2^21";
        }
        else if (ipClass == 'D') {
            cout << "\n\n===== CLASS D =====";
            cout << "\nFirst Octet Range : 224 - 239";
            cout << "\nBinary Pattern    : 1110XXXX";
            cout << "\nIP Address Range  : 224.0.0.0 - 239.255.255.255";
            cout << "\nDefault Subnet Mask: Not Applicable";
            cout << "\nCIDR              : Not Applicable";
            cout << "\nPurpose            : Multicast";
        }
        else {
            cout << "\n\n===== CLASS E =====";
            cout << "\nFirst Octet Range : 240 - 255";
            cout << "\nBinary Pattern    : 1111XXXX";
            cout << "\nIP Address Range  : 240.0.0.0 - 255.255.255.255";
            cout << "\nDefault Subnet Mask: Not Applicable";
            cout << "\nCIDR              : Not Applicable";
            cout << "\nPurpose            : Experimental / Reserved";
        }

        cout << endl;
    }

public:
    bool setIP(const string& inputIP) {
        try {
            addr = parseIP(inputIP);
        }
        catch (...) {
            return false;
        }

        if (addr.size() != 4) {
            return false;
        }

        for (int octet : addr) {
            if (octet < 0 || octet > 255) {
                return false;
            }
        }

        ip = inputIP;

        if (addr[0] <= 127) {
            ipClass = 'A';
            defaultBits = 8;
        }
        else if (addr[0] <= 191) {
            ipClass = 'B';
            defaultBits = 16;
        }
        else if (addr[0] <= 223) {
            ipClass = 'C';
            defaultBits = 24;
        }
        else if (addr[0] <= 239) {
            ipClass = 'D';
            defaultBits = 0;
        }
        else {
            ipClass = 'E';
            defaultBits = 0;
        }

        return true;
    }

    void showClassInformation() {
        displayClassDetails();
    }

    bool supportsSubnetting() {
        return ipClass == 'A' || ipClass == 'B' || ipClass == 'C';
    }

    void calculateSubnets(int requiredSubnets) {
        if (requiredSubnets <= 0) {
            cout << "Number of subnets must be greater than 0.\n";
            return;
        }

        int borrowedBits = 0;
        int possibleSubnets = 1;

        while (possibleSubnets < requiredSubnets) {
            borrowedBits++;
            possibleSubnets *= 2;
        }

        int totalNetworkBits = defaultBits + borrowedBits;
        int hostBits = 32 - totalNetworkBits;
        unsigned int addressesPerSubnet = 1u << hostBits;

        unsigned int mask = (totalNetworkBits == 32)
            ? 0xFFFFFFFFu
            : 0xFFFFFFFFu << (32 - totalNetworkBits);

        unsigned int ipValue =
            ((unsigned int)addr[0] << 24) |
            ((unsigned int)addr[1] << 16) |
            ((unsigned int)addr[2] << 8) |
            (unsigned int)addr[3];

        unsigned int network = ipValue & mask;

        cout << "\nIP Class: " << ipClass << endl;
        cout << "Borrowed bits: " << borrowedBits << endl;
        cout << "Actual number of subnets: " << possibleSubnets << endl;
        cout << "Subnet prefix: /" << totalNetworkBits << endl;
        cout << "Addresses per subnet: " << addressesPerSubnet << endl;
        cout << "Usable IP addresses per subnet: "
             << addressesPerSubnet - 2 << endl;

        cout << "Subnet Mask: ";
        printIP(mask);
        cout << "\n\n-----------------------------------------------\n";

        for (int i = 0; i < possibleSubnets; i++) {
            unsigned int subnetNetwork =
                network + (i * addressesPerSubnet);

            unsigned int broadcast =
                subnetNetwork + addressesPerSubnet - 1;

            cout << "\nSubnet " << i + 1 << endl;

            cout << "Network Address : ";
            printIP(subnetNetwork);

            cout << "\nFirst IP        : ";
            printIP(subnetNetwork + 1);

            cout << "\nLast IP         : ";
            printIP(broadcast - 1);

            cout << "\nBroadcast       : ";
            printIP(broadcast);

            cout << "\n-----------------------------------------------\n";
        }
    }
};

int main() {
    string ip;
    int requiredSubnets;

    IPSubnetCalculator calculator;

    cout << "Enter IPv4 address: ";
    cin >> ip;

    if (!calculator.setIP(ip)) {
        cout << "Invalid IP address!\n";
        return 0;
    }

    calculator.showClassInformation();

    if (!calculator.supportsSubnetting()) {
        cout << "\nSubnet calculation is not supported for Class D/E.\n";
        return 0;
    }

    cout << "\nEnter number of required subnets: ";
    cin >> requiredSubnets;

    calculator.calculateSubnets(requiredSubnets);

    return 0;
}