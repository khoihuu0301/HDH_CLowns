// tuananh read partition boot sector( ntfs)
#include <cstdio>
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include<string>
#include<sstream>

using namespace std;
vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream. 
    string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}
char hexaDecNum[50];
string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}
int HexToDec(char num[]) {
    string hex = convertToString(num, strlen(num));
    unsigned long result = 0;
    for (int i = 0; i < hex.length(); i++) {
        if (hex[i] >= 48 && hex[i] <= 57)
        {
            result += (hex[i] - 48) * pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 65 && hex[i] <= 70) {
            result += (hex[i] - 55) * pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 97 && hex[i] <= 102) {
            result += (hex[i] - 87) * pow(16, hex.length() - i - 1);
        }
    }
    return result;
}

//char* StringToChar(string a) {
//    char* c = const_cast<char*>(a.c_str());
//    return c;
//}
//string DecimalToASCII(string dec) {
//    string ascii = "";
//    int decLen = dec.length();
//
//    for (int i = 0; i < decLen; i += 3)
//    {
//        ascii += (char)std::stoi(dec.substr(i, 3));
//    }
//
//    return ascii;
//}
void readNTFS(vector<string> spl) {

    //Byte per Sector: 2 bytes at offset B
    string BytePerSec;
    cout << "Byte per sector: ";
    for (int i = 12; i >= 11; i--) {
        string a = spl.at(i);
        BytePerSec += a;
    }
    char* temp2 = const_cast<char*>(BytePerSec.c_str());
    cout << HexToDec(temp2) << " bytes\n";

    //Sectors Per Cluster: 1 bytes at offset 0D
    cout << "Sector per cluster: ";
    string sector0D = spl.at(13);
    char* temp3 = const_cast<char*>(sector0D.c_str());
    int Sc = HexToDec(temp3);
    cout << Sc << endl;

    //Media descriptor: 1 bytes at 15
    cout << "Media descriptor: ";
    string mediaDes = spl.at(21);
    char* temp4 = const_cast<char*>(mediaDes.c_str());
    cout << HexToDec(temp4) << endl;

    //Sector per track 2 bytes at 18
    cout << "Sector per track: ";
    string SecPerTrack;
    for (int i = 25; i >= 24; i--) {
        string a = spl.at(i);
        SecPerTrack += a;
    }
    char* temp5 = const_cast<char*>(SecPerTrack.c_str());
    cout << HexToDec(temp5) << endl;

    //Total Sectors: 8 bytes at offset 28
    cout << "Total Sectors: ";
    string SecPerDisk;
    for (int i = 47; i >= 40; i--) {
        string a = spl.at(i);
        SecPerDisk += a;
    }
    char* temp6 = const_cast<char*>(SecPerDisk.c_str());
    cout << HexToDec(temp6) << " -> " << HexToDec(temp6) * 0.5 / 1024 / 1024 << " Gb\n";

    //Starting cluster address of MFT: 8 bytes at offset 30
    cout << "Starting cluster address of MFT:";
    string MFTClusStart;
    for (int i = 55; i >= 48; i--) {
        string a = spl.at(i);
        MFTClusStart += a;
    }
    char* temp7 = const_cast<char*>(MFTClusStart.c_str());
    cout << HexToDec(temp7) << endl;

    //Starting cluster address of MFT mirror: 8 bytes at offset 38
    cout << "Starting cluster address of MFT mirror:";
    string MFTClusStartcoppy;
    for (int i = 63; i >= 56; i--) {
        string a = spl.at(i);
        MFTClusStartcoppy += a;
    }
    char* temp8 = const_cast<char*>(MFTClusStartcoppy.c_str());
    cout << HexToDec(temp8) << endl;


    //Size of MFT entry: 1 bytes at offset 40
    cout << "Size of MFT entry: ";
    string sizeMTF = spl.at(64);
    char* temp9 = const_cast<char*>(sizeMTF.c_str());
    cout << HexToDec(temp9) << " bytes\n";
}
void readFAT32(vector<string> spl) {
    //type of FAT: read 8 byte at offset 52
    cout << "Type of FAT: ";
    for (int i = 82; i <= 88; i++) {
        string a = spl.at(i);
        char* temp1 = const_cast<char*>(a.c_str());
        cout << (char)HexToDec(temp1);
    }
    cout << endl;


    //Byte of 1 Sector: 2 byte at offset B
    string BytePerSec;
    cout << "Byte of sector: ";
    for (int i = 12; i >= 11; i--) {
        string a = spl.at(i);
        BytePerSec += a;
    }
    char* temp2 = const_cast<char*>(BytePerSec.c_str());
    cout << HexToDec(temp2) << endl;

    //Byte of 1 cluster: 1 byte at offset 0D
    cout << "Byte of 1 cluster: ";
    string sector0D = spl.at(13);
    char* temp3 = const_cast<char*>(sector0D.c_str());
    int Sc = HexToDec(temp3);
    cout << Sc << endl;

    //Sector of BootSector: 2 byte at offset 0E
    cout << "Sector of BootSector: ";
    string SecOfBoot;
    for (int i = 15; i >= 14; i--) {
        string a = spl.at(i);
        SecOfBoot += a;
    }
    char* temp4 = const_cast<char*>(SecOfBoot.c_str());
    int Sb = HexToDec(temp4);
    cout << Sb << endl;

    //Number of FAT table: 1 byte ? offset 10
    cout << "Number of FAT table: ";
    string sector10 = spl.at(16);
    char* temp5 = const_cast<char*>(sector10.c_str());
    int Nf = HexToDec(temp5);
    cout << Nf << endl;

    //Sector for RDET table: 2 byte at 11
    cout << "Sector for RDET table: ";
    string SecOfRDET;
    for (int i = 18; i >= 17; i--) {
        string a = spl.at(i);
        SecOfRDET += a;
    }
    char* temp6 = const_cast<char*>(SecOfRDET.c_str());
    cout << HexToDec(temp6) << endl;
    //Numb of Sector in Disk: 4 byte at offset 20
    cout << "Number of Sectors in Disk: ";
    string SumSec;
    for (int i = 35; i >= 32; i--) {
        string a = spl.at(i);
        SumSec += a;
    }
    char* temp7 = const_cast<char*>(SumSec.c_str());
    int Sv = HexToDec(temp7);
    cout << Sv << endl;
    //Size of Disk

    //Sector per FAT table: 2 byte at offset 16
    cout << "Sector for 1 FAT table: ";
    string secForFAT;
    for (int i = 23; i >= 22; i--) {
        string a = spl.at(i);
        secForFAT += a;
    }
    char* temp8 = const_cast<char*>(secForFAT.c_str());
    int Sf = HexToDec(temp8);
    cout << Sf << endl;

    //First sector of Fat1:
    cout << "Address of the first sector of FAT1 table: " << Sb << endl;

    //First sector of RDET:
    cout << "Address of the first sector of RDET table: " << Sb + Nf * Sf << endl;

}
int main()
{
    ifstream infile;
    infile.open("hex.txt");

    string data;
    getline(infile, data);
    vector<string> spl = split(data, ' ');
    stringstream builder;
    for (int i = 3; i <= 6; i++) {
        string a = spl.at(i);
        char* temp = const_cast<char*>(a.c_str());
        builder << (char)HexToDec(temp);
    }
    string id = builder.str();
    if (id == "NTFS") {
        cout << "===========================================READ NTFS FILE===========================================\n";
        readNTFS(spl);
        cout << "==========================================END===========================================\n";
    }
    else
    {
        cout << "===========================================READ FAT32===========================================\n";
        readFAT32(spl);
        cout << "===========================================END============================================\n";
    }
    system("pause");
    return 0;
}