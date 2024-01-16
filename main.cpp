#define NDEBUG
#include <fstream>
#include <cassert>
#include <tuple>
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

uint8_t hexSubStringToByte(const string& hexString, int start, int length) {
  auto subStr = hexString.substr(start, length);
  return strtol(subStr.c_str(), NULL, 16);
}

vector<uint8_t> GUIDStringToByteArray(const string& GUID) {
  vector<uint8_t> out(16);
  out[3]  = hexSubStringToByte(GUID, 1, 2);
  out[2]  = hexSubStringToByte(GUID, 3, 2);
  out[1]  = hexSubStringToByte(GUID, 5, 2);
  out[0]  = hexSubStringToByte(GUID, 7, 2);
  out[5]  = hexSubStringToByte(GUID, 10, 2);
  out[4]  = hexSubStringToByte(GUID, 12, 2);
  out[7]  = hexSubStringToByte(GUID, 15, 2);
  out[6]  = hexSubStringToByte(GUID, 17, 2);
  out[8]  = hexSubStringToByte(GUID, 20, 2);
  out[9]  = hexSubStringToByte(GUID, 22, 2);
  out[10] = hexSubStringToByte(GUID, 25, 2);
  out[11] = hexSubStringToByte(GUID, 27, 2);
  out[12] = hexSubStringToByte(GUID, 29, 2);
  out[13] = hexSubStringToByte(GUID, 31, 2);
  out[14] = hexSubStringToByte(GUID, 33, 2);
  out[15] = hexSubStringToByte(GUID, 35, 2);
  return out;
}

string toHexString(int val, int width) {
  string valueHexString;
  stringstream ss;
  ss << setfill('0') << setw(width) << hex << val;
  return ss.str();
}

// Input: { 0xFF, 0xAA, 0xCC }
// Ouput: FFAACC
string ByteArrayToString(vector<uint8_t> bytes) {
  stringstream ss;
  for(int i = 0; i < 16; i++) {
    ss << toHexString(bytes[i], 2);
  }
  return ss.str();
}

// Input is a GUID how it appears in binary format in the file,
// output is a human readable string how that GUID would appear in the GUIDs.txt
// string ByteArrayToGUIDString(char* start) {
string ByteArrayToGUIDString(vector<uint8_t> bytes) {
  stringstream ss;
  ss << "{";
  ss << toHexString(bytes[3] & 0xFF, 2);
  ss << toHexString(bytes[2] & 0xFF, 2);
  ss << toHexString(bytes[1] & 0xFF, 2);
  ss << toHexString(bytes[0] & 0xFF, 2);
  ss << '-';
  ss << toHexString(bytes[5] & 0xFF, 2);
  ss << toHexString(bytes[4] & 0xFF, 2);
  ss << '-';
  ss << toHexString(bytes[7] & 0xFF, 2);
  ss << toHexString(bytes[6] & 0xFF, 2);
  ss << '-';
  ss << toHexString(bytes[8] & 0xFF, 2);
  ss << toHexString(bytes[9] & 0xFF, 2);
  ss << '-';
  ss << toHexString(bytes[10] & 0xFF, 2);
  ss << toHexString(bytes[11] & 0xFF, 2);
  ss << toHexString(bytes[12] & 0xFF, 2);
  ss << toHexString(bytes[13] & 0xFF, 2);
  ss << toHexString(bytes[14] & 0xFF, 2);
  ss << toHexString(bytes[15] & 0xFF, 2);
  ss << '}';
  return ss.str();
}

bool isHexString(const string& s) {
  return s.find_first_not_of("0123456789abcdefABCDEF", 2) == string::npos;
}

bool isValidGUIDString(const string &s) {
  if(s[0] != '{' || s[37] != '}') {
    return false;
  } else if(!isHexString(s.substr(1, 8))) {
    return false;
  } else if(s[9] != '-') {
    return false;
  } else if(!isHexString(s.substr(9, 4))) {
    return false;
  } else if(s[14] != '-') {
    return false;
  } else if(!isHexString(s.substr(14, 4))) {
    return false;
  } else if(s[19] != '-') {
    return false;
  } else if(!isHexString(s.substr(19, 4))) {
    return false;
  } else if(s[24] != '-') {
    return false;
  } else if(!isHexString(s.substr(24, 12))) {
    return false;
  }
  return true;
}

void test_isValidGUIDString() {
  assert(isValidGUIDString("{c7d655d8-ec85-4d25-bc91-cf2cca64fbb7}") == true);
  assert(isValidGUIDString("{RRRRRRRR-ec85-4d25-bc91-cf2cca64fbb7}") == false);
  assert(isValidGUIDString("{c7d655d8-ec85-4d25-bc91-cf2cca64fbb7") == false);
  assert(isValidGUIDString("c7d655d8-ec85-4d25-bc91-cf2cca64fbb7") == false);
}

void test_toHextString() {
  assert(toHexString(0, 2).compare(0, 2, "00") == 0);
  assert(toHexString(0, 1).compare(0, 2, "0") == 0);
  assert(toHexString(15, 2).compare(0, 2, "0f") == 0);
  assert(toHexString(15, 1).compare(0, 2, "f") == 0);
}

void test_hexSubStringToByte() {
  assert(hexSubStringToByte("FFAA", 1, 1) == 15);
  assert(hexSubStringToByte("FFAA", 1, 2) == 250);
  assert(hexSubStringToByte("FFAA", 0, 1) == 15);
  assert(hexSubStringToByte("FFAA", 0, 2) == 255);
  assert(hexSubStringToByte("FFAA", 2, 2) == 170);
}

void test_GUIDStringToByteArray() {
const string testGUID = "{c7d655d8-ec85-4d25-bc91-cf2cca64fbb7}";
  const vector<uint8_t> correctOutput = { 216, 85, 214, 199, 133, 236, 37, 77, 188, 145, 207, 44, 202, 100, 251, 183 };
  auto bytes = GUIDStringToByteArray(testGUID);
  for(int i = 0; i < 16; i++) {
    assert(correctOutput[i] == bytes[i]);
  }
}

void test_ConversionBackAndForth() {
  const string testGUID = "{c7d655d8-ec85-4d25-bc91-cf2cca64fbb7}";
  assert(ByteArrayToGUIDString(GUIDStringToByteArray(testGUID)) == testGUID);
}

void run_tests() {
  test_toHextString();
  test_hexSubStringToByte();
  test_GUIDStringToByteArray();
  test_ConversionBackAndForth();
  test_isValidGUIDString();
}

int wrongUsage() {
  cerr << "Usage: guid_replacer.exe your_audio.bank [GUID to replace] [what to replace it with]" << endl;
  cerr << "Example: guid_replacer.exe your_audio.bank {bc5e80f9-a380-47c9-8799-3e02eea94feb} {aabbccdd-eeff-00ff-aabb-feffefbbaacc}" << endl;
  cerr << "Multiple replacements can be chained like this:" << endl;
  cerr << "guid_replacer.exe your_audio.bank {GUID_A1} {GUID_A2} {GUID_B1} {GUID_B2}" << endl;
  return -1;
}

int main(int argc, char** argv) {
  run_tests();
  vector<tuple<string, string>> guidReplacePairs;
  if(argc % 2 == 1) {
    return wrongUsage();
  }
  const string filename = argv[1];
  if(filename.find_first_of(".bank") == string::npos) {
    return wrongUsage();
  }
  for(int i = 2; i < argc; i += 2) {
    if(!isValidGUIDString(argv[i]) || !isValidGUIDString(argv[i+1])) {
      return wrongUsage();
    }
    guidReplacePairs.push_back(make_tuple(argv[i], argv[i+1]));
  }
  fstream new_file(filename, ios::in | ios:: out | ios::binary);
  const char* EVNTEVTB = "EVNTEVTB";
  if(!new_file)  {
    cout << "Could not open file '" << filename << "': " << strerror(errno) << endl;
  } else {
    // First find the offset where the string EVNTEVTB ends
    int currentMatchingPosition = 0;
    while(!new_file.eof()) {
      const size_t BUFFER_SIZE = 1024;
      string buffer(BUFFER_SIZE, '\0');
      int pos = new_file.tellg();
      new_file.read(&buffer[0], BUFFER_SIZE);
      for(int i = 0; i < BUFFER_SIZE; i++) {
        if(buffer[i] == EVNTEVTB[currentMatchingPosition]) {
          currentMatchingPosition++;
          if(currentMatchingPosition == 8) {
            currentMatchingPosition = 0;
            // End of EVNTEVTB string found at pos + i + 1, GUID starts 4 bytes later
            int GUIDstartPos = pos + i + 1 + 4;
            // Read out the GUID
            vector<uint8_t> GUID(17);
            GUID[16] = 0;
            buffer.copy((char*)&GUID[0], 16, i + 1 + 4);
            cout << "Found a GUID at offset " << GUIDstartPos << " - " << ByteArrayToGUIDString(GUID) << " - " << ByteArrayToString(GUID) << endl;
            for(auto it = guidReplacePairs.begin(); it != guidReplacePairs.end(); it++) {
              const string lookFor = get<0>(*it);
              const string replaceWith = get<1>(*it);
              if(lookFor.compare(ByteArrayToGUIDString(GUID)) == 0) {
                cout << "Replacing " << lookFor << " at offset " << GUIDstartPos << " with " << replaceWith << endl;
                new_file.seekp(GUIDstartPos);
                auto boop = GUIDStringToByteArray(replaceWith);
                new_file.write((char*)&boop[0], 16);
              }
            }
          }
        } else {
          currentMatchingPosition = 0;
        }
      }
    }
    new_file.close();
  }
  return 0;
}
