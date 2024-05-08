#include "ClipboardManager.h"

void ClipboardManager::SetText(const std::string& text) {
    const size_t len = (text.length() + 1) * sizeof(wchar_t); // Adjust for Unicode
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    if (!hMem) {
        // Handle memory allocation error
        return;
    }
    // Use wchar_t for Unicode text
    memcpy(GlobalLock(hMem), text.c_str(), len);
    GlobalUnlock(hMem);

    if (OpenClipboard(nullptr)) { // Use nullptr instead of 0 for clarity
        EmptyClipboard();
        SetClipboardData(CF_UNICODETEXT, hMem);
        CloseClipboard();
    } else {
        // Handle failure to open clipboard
        GlobalFree(hMem);
    }
}
