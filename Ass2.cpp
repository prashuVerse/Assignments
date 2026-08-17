#include <iostream>
#include <string>
using namespace std;

class HammingCode
{
private:
    char code[8];
    char received[8];

public:

    void generateCode()
    {
        string data;

        cout << "Enter 4-bit Data: ";
        cin >> data;

    

        code[3] = data[0];
        code[5] = data[1];
        code[6] = data[2];
        code[7] = data[3];

        // Calculate parity bits

        int p1 = (code[3]-'0') ^
                 (code[5]-'0') ^
                 (code[7]-'0');

        int p2 = (code[3]-'0') ^
                 (code[6]-'0') ^
                 (code[7]-'0');

        int p4 = (code[5]-'0') ^
                 (code[6]-'0') ^
                 (code[7]-'0');

        code[1] = p1 + '0';
        code[2] = p2 + '0';
        code[4] = p4 + '0';

        cout << "\nGenerated Hamming Code : ";

        for(int i=1;i<=7;i++)
            cout << code[i];

        cout << endl;
    }

    void receiveCode()
    {
        string temp;

        cout << "\nEnter Received 7-bit Code : ";
        cin >> temp;

        for(int i=1;i<=7;i++)
            received[i] = temp[i-1];
    }

    void detectError()
    {
       

        int receivedP1 = received[1]-'0';
        int receivedP2 = received[2]-'0';
        int receivedP4 = received[4]-'0';

       

        int calcP1 = (received[3]-'0') ^
                     (received[5]-'0') ^
                     (received[7]-'0');

        int calcP2 = (received[3]-'0') ^
                     (received[6]-'0') ^
                     (received[7]-'0');

        int calcP4 = (received[5]-'0') ^
                     (received[6]-'0') ^
                     (received[7]-'0');

        

        int s1 = (receivedP1 != calcP1);
        int s2 = (receivedP2 != calcP2);
        int s4 = (receivedP4 != calcP4);

        cout << "\nReceived Parity Bits\n";
        cout << "P1 = " << receivedP1 << endl;
        cout << "P2 = " << receivedP2 << endl;
        cout << "P4 = " << receivedP4 << endl;

        cout << "\nCalculated Parity Bits\n";
        cout << "P1 = " << calcP1 << endl;
        cout << "P2 = " << calcP2 << endl;
        cout << "P4 = " << calcP4 << endl;

        cout << "\nSyndrome Bits\n";
        cout << "S1 = " << s1 << endl;
        cout << "S2 = " << s2 << endl;
        cout << "S4 = " << s4 << endl;

        int errorPosition = s4 * 4 + s2 * 2 + s1;

        cout << "\nSyndrome (S4S2S1) = "
             << s4 << s2 << s1 << endl;

        if(errorPosition == 0)
        {
            cout << "\nNo Error Detected." << endl;
        }
        else
        {
            cout << "\nError Detected at Position "
                 << errorPosition << endl;

            // Correct the error

            if(received[errorPosition] == '0')
                received[errorPosition] = '1';
            else
                received[errorPosition] = '0';

            cout << "\nCorrected Hamming Code : ";

            for(int i=1;i<=7;i++)
                cout << received[i];

            cout << endl;
        }
    }
};

int main()
{
    HammingCode h;

    h.generateCode();

    h.receiveCode();

    h.detectError();

    return 0;
}