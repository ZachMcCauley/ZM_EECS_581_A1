#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>

using namespace std;

int strToInt(string str) { //strToInt written by ChatGPT
    if (str.empty())
        throw invalid_argument("Empty string");

    // Reject leading zeros, e.g. "02"
    if (str.length() > 1 && str[0] == '0')
        throw invalid_argument("Leading zero");

    int value = 0;

    for (int i = 0; i < str.length(); i++)
    {
        // Make sure the character is a digit using ASCII values
        if (str[i] < '0' || str[i] > '9')
            throw invalid_argument("Non-digit character");

        // Convert ASCII digit to its numerical value
        int digit = str[i] - '0';

        // Build the number
        value = value * 10 + digit;

        // IPv4 octets cannot exceed 255
        if (value > 255)
            throw invalid_argument("Number greater than 255");
    }

    return value;
}

int portToInt(string str) { //portToInt written by hand based on ChatGPT written code
    //check for invalid cases
    if (str.empty()) throw invalid_argument("Empty string");
    if (str.length() > 1 && str[0] == '0') throw invalid_argument("Leading zero");

    int value = 0;
    for (int i = 0; i < str.length(); i++) {
        //check for non number characters
        if (str[i] < '0' || str[i] > '9') throw invalid_argument("Non-digit character");

        int digit = str[i] - '0'; //convert char to integer using ascii values (subtract 48 from ascii value and store)
        value = value * 10 + digit;

        if (value > 65535) throw invalid_argument("Number greater than 65536");
    }
    return value;
}

void tokenize(const string str, queue<string>& tokens) //tokenize written by ChatGPT
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] >= 48 && str[i] <= 57)
        {
            string number;

            // Collect all adjacent digits
            while (i < str.length() && str[i] >= 48 && str[i] <= 57)
            {
                number += str[i];
                i++;
            }
            tokens.push(number);

            // The for-loop will increment i again, so move back one
            i--;
        }
        else //if (str[i] == 46 || str[i] == 58)
        {
            // Every non-digit character gets its own token
            tokens.push(string(1, str[i]));
        }
    }
}

bool extractIPv4(const string& str, unsigned long& outAddress, int& outPort) { //extractIPv4 written by hand
    //parse the input into tokens
    queue<string> tokens;
    tokenize(str, tokens);

    //initialize default values
    outAddress = 0;
    outPort = -1;

    while (true) { //loop until queue is empty
        if (tokens.size() < 7) return false; //an address needs 7 or 9 tokens to be valid
        try {
            if (tokens.front() == "." || tokens.front() == ":") {
                while (tokens.front() == "." || tokens.front() == ":") tokens.pop(); //need extra pop(s) to make sure character is not adjacent to otherwise valid IP
                throw invalid_argument("IP format not matched");
            } //check for stray characters
            outAddress = outAddress + (strToInt(tokens.front()) << 24); //first octet
            tokens.pop();
            if (tokens.front() == ".") tokens.pop(); //check for dots inbetween octets
            else throw invalid_argument("IP format not matched");
            outAddress = outAddress + (strToInt(tokens.front()) << 16); //second octet
            tokens.pop();
            if (tokens.front() == ".") tokens.pop();
            else throw invalid_argument("IP format not matched");
            outAddress = outAddress + (strToInt(tokens.front()) << 8); //third octet
            tokens.pop();
            if (tokens.front() == ".") tokens.pop();
            else throw invalid_argument("IP format not matched");
            outAddress = outAddress + strToInt(tokens.front()); //fourth octet
            tokens.pop();
            if (tokens.front() == ".") throw invalid_argument("IP format not matched"); //check for stray dots
            if (tokens.front() == ":") { //check for port number
                tokens.pop();
                if (tokens.empty()) {
                    //colon present but no port number 
                    outAddress = 0;
                    return false;
                }
                try {
                    outPort = portToInt(tokens.front());
                } catch (...) {
                    outPort = -1;
                    outAddress = 0;
                    return false;
                }
                tokens.pop();
                if (tokens.front() == ":" || tokens.front() == ".") throw invalid_argument("IP format not matched"); //check for stray characters
            }
            break;
        }
        catch (...) {
            //IP format not fully filled, reset values
            outAddress = 0;
            tokens.pop();
        }
    }
    return true;
}

string decimalToIP(const unsigned long outAddress) //decimalToIP written by ChatGPT
{
    unsigned long a = (outAddress >> 24) & 255;
    unsigned long b = (outAddress >> 16) & 255;
    unsigned long c = (outAddress >> 8) & 255;
    unsigned long d = outAddress & 255;

    return to_string(a) + "." +
           to_string(b) + "." +
           to_string(c) + "." +
           to_string(d);
}

int main() { //main written by hand
    string noisy_text;
    while (true) {
        cout << "Enter a string (or 'END' to Quit): ";
        getline(cin, noisy_text);
        if (noisy_text == "END") break;
        unsigned long outAddress;
        int outPort;
        if (extractIPv4(noisy_text, outAddress, outPort)) {
            string ipAddress = decimalToIP(outAddress);
            cout << "Extracted IPv4 address: " << ipAddress << " (decimal value: " << outAddress << ", port: ";
            if (outPort == -1) cout << "none)\n";
            else cout << outPort << ")\n";
        } else {
            cout << "Invalid input: no valid IPv4 address found\n";
        }
    }
    cout << "Program terminated.\n";

    return 0;
}