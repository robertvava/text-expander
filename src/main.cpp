#include <windows.h>
#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <cctype> 
#include <iterator>


bool listeningForShortcut = false;
std::string currentTyped; 

std::map<std::string, std::string> shortcuts; // Stores keys and their expansions

void SetClipboardText(const std::string& text) {
    const size_t len = text.length() + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), text.c_str(), len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

void SimulatePaste() {
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('V', 0, 0, 0);
    keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {

        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vkCode = pKeyboard->vkCode;

        BYTE keyboardState[256];
        WCHAR charBuffer[2]; 
        char typedChar = 0;

        GetKeyboardState(keyboardState);

        // Translate the key press to a character, considering the keyboard state
        int charsTranslated = ToUnicode(
            pKeyboard->vkCode,
            pKeyboard->scanCode,
            keyboardState,
            charBuffer,
            sizeof(charBuffer) / sizeof(charBuffer[0]),
            0);

        if (charsTranslated == 1) {
            typedChar = char(charBuffer[0]); // Convert to ASCII char

        }
        
        if (!listeningForShortcut && typedChar == '/') { // Triggers listening to keyboard events
            currentTyped.clear();                        // Delete cache
            listeningForShortcut = true;

        } else if (listeningForShortcut) {

            if (vkCode == VK_BACK && !currentTyped.empty() && currentTyped.length() != 1) { // Handle backspace and avoid getting an error when length < 0
                
                currentTyped.pop_back();
                
            } else {
                currentTyped += typedChar; // Accumulate characters
            }

            // std::cout << "Current Typed: " << currentTyped << std::endl; // for debugging
    
            // Check if the accumulated characters match any key (after removing the leading "/")
            if (shortcuts.find(currentTyped.substr(1)) != shortcuts.end()) {
                
                std::string expansionText = shortcuts[currentTyped.substr(1)];
                
                for (size_t i = 0; i < currentTyped.length()+1; ++i) {
                    keybd_event(VK_BACK, 0, 0, 0); 
                    keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
                    // Sleep(5); // Small delay, not actually needed
                }

                
                SetClipboardText(expansionText);
                SimulatePaste();
               

                listeningForShortcut = false; 
                currentTyped.clear(); 
                return 1; // Block further processing to prevent the "/" from appearing
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {

    // Test functionality. TODO: Add database. 
    shortcuts["env"] = "Lorem Ipsum et dolores.";

    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (keyboardHook == NULL) {
        std::cout << "Failed to install keyboard hook!" << std::endl;
    } else {
        std::cout << "Keyboard hook installed successfully." << std::endl;

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Unhook before exiting
        UnhookWindowsHookEx(keyboardHook);
    }
    return 0;
}