#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <string>
#include <windows.h>

class ClipboardManager {
public:
    ClipboardManager() = default;
    virtual ~ClipboardManager() = default;

    // Disallow copy and assignment to ensure unique access to the clipboard
    ClipboardManager(const ClipboardManager&) = delete;
    ClipboardManager& operator=(const ClipboardManager&) = delete;

    // Sets text to the system clipboard
    virtual void SetText(const std::string& text);

    // Additional functionality like getting text from the clipboard can be added here
    // virtual std::string GetText();
};

#endif // CLIPBOARDMANAGER_H
