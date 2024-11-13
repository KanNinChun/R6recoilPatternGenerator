#include <iostream>
#include <vector>
#include <fstream>
#include <Windows.h>

struct Position {
    int x;
    int y;
};

void SetConsoleProperties() {
    HWND console = GetConsoleWindow();
    RECT rect;

    // Set console to topmost
    SetWindowPos(console, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // Move console to top-right corner of the screen
    GetWindowRect(console, &rect);
    int width = 400;  // Width of the console window
    int height = 300; // Height of the console window
    int xPos = GetSystemMetrics(SM_CXSCREEN) - width; // Top-right corner
    int yPos = 0;
    SetWindowPos(console, HWND_TOPMOST, xPos, yPos, width, height, SWP_SHOWWINDOW);
}

int main() {
    SetConsoleProperties();

    std::vector<Position> positions;
    bool recording = false;

    std::cout << "Press Numpad 2 to start recording positions:" << std::endl;

    while (!recording) {
        if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) { // Numpad key 2 to start
            recording = true;
            std::cout << "Recording started. Click on the picture positions to record (Press Numpad 1 to stop):" << std::endl;
            Sleep(500); // To avoid multiple triggers
        }
    }

    while (recording) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { // Left mouse button click
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            ScreenToClient(GetForegroundWindow(), &cursorPos);
            positions.push_back({ cursorPos.x, cursorPos.y });
            std::cout << "Recorded position: (" << cursorPos.x << ", " << cursorPos.y << ")" << std::endl;
            while (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                // Wait until the left mouse button is released
                Sleep(10);
            }
            Sleep(200); // Additional delay to avoid immediate subsequent clicks
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) { // Numpad key 1
            recording = false;
            std::cout << "Recording stopped." << std::endl;
        }
    }

    std::ofstream outFile("R6pattern.txt");

    if (outFile.is_open()) {
        for (size_t i = 0; i < positions.size(); ++i) {
            outFile << "[Bullet " << (i + 1) << "] -> " << positions[i].x << " , " << positions[i].y << std::endl;
        }
        outFile.close();
        std::cout << "Recoil pattern positions saved to R6pattern.txt" << std::endl;
    }
    else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }

    std::cout << "Press Enter to exit the program." << std::endl;
    std::cin.get(); // Wait for user input before closing

    return 0;
}
