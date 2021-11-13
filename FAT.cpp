#pragma warning(disable:4996)
#include <cstdio>
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;
int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        printf("Success!\n");
    }
}

string hexStr(BYTE* data, int len)
{
    stringstream ss;
    ss << hex;

    for (int i(0); i < len; ++i)
    {
        ss << setw(2) << setfill('0') << (int)data[i];
        ss << " ";
    }
    return ss.str();
}

vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream. 
    string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}
void main()
{
    ofstream asc;
    asc.open("asc.txt");
    HANDLE device = NULL;
    VOLUME_DISK_EXTENTS vde = { 0 };
    DWORD dw;
    DeviceIoControl(
        device,
        IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
        NULL,
        0,
        (LPVOID)&vde,
        (DWORD)sizeof(VOLUME_DISK_EXTENTS),
        &dw, NULL
    );
    BYTE sector[512];
    printf("Drive letter of USB?: ");
    char path[] = "\\\\.\\?:";

    scanf_s("%c", path + 4, 1);
    char diskPath[20] = { 0 };
    sprintf_s(diskPath,
        sizeof(diskPath),
        "\\\\.\\PhysicalDrive%d",
        vde.Extents[0].DiskNumber
    );
    ReadSector(L"\\\\.\\P:", 0, sector);
    for (int i = 0; i < 512; i++)
    {
        if (0 == i % 16)
        {

            asc << "\n";

        }

        BYTE b = sector[i];
        if (isascii(b))
        {
            asc << b;
        }
        else asc << ".";
    }
    ofstream he;
    he.open("hex.txt");
    string a = hexStr(sector, 512);
    he << a;

    vector<string> spl = split(a, ' ');
        

    system("pause");


}