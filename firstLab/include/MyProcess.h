//
// Created by meteo on 07.12.2023.
//
#pragma once

#include <windows.h>
#include <ostream>

class MyProcess {
private:
    int m_id;
    int m_processId;
    bool m_isAlive;

    DWORD CreateNewProcessAndReturnPID() {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcess(NULL,   // No module name (use command line)
                           "child_exe.exe",       // Command line
                           NULL,                   // MyProcess handle not inheritable
                           NULL,                   // Thread handle not inheritable
                           FALSE,                  // Set handle inheritance to FALSE
                           0,                      // No creation flags
                           NULL,                   // Use parent's environment block
                           NULL,                   // Use parent's starting directory
                           &si,                    // Pointer to STARTUPINFO structure
                           &pi)                    // Pointer to PROCESS_INFORMATION structure
                ) {
            std::cerr << "CreateProcess failed" << std::endl;
            return 0;
        }

//        CloseHandle(pi.hProcess);
//        CloseHandle(pi.hThread);

        return pi.dwProcessId;
    }

public:
    MyProcess(int id) : m_processId(CreateNewProcessAndReturnPID()), m_id(id), m_isAlive(true) {}

    [[nodiscard]] int getId() const {
        return m_id;
    }

    void setId(int mId) {
        m_id = mId;
    }

    [[nodiscard]] int getProcessId() const {
        return m_processId;
    }

    [[nodiscard]] bool isAlive() const {
        return m_isAlive;
    }

    void setIsAlive(bool mIsAlive) {
        m_isAlive = mIsAlive;
    }

    void setProcessId(int mProcessId) {
        m_processId = mProcessId;
    }

    bool operator==(const MyProcess &rhs) const {
        return m_id == rhs.m_id;
    }

    bool operator==(int id) const{
        return id == m_id;
    }

    bool operator!=(const MyProcess &rhs) const {
        return !(rhs == *this);
    }

    friend std::ostream &operator<<(std::ostream &os, const MyProcess &process) {
        os << std::boolalpha <<
           "id: " << process.m_id <<
           " processId: " << process.m_processId <<
           " m_isAlive: " << process.m_isAlive << std::endl;
        return os;
    }

    bool operator<(const MyProcess &rhs) const {
        if (m_id < rhs.m_id)
            return true;
        if (rhs.m_id < m_id)
            return false;
        if (m_processId < rhs.m_processId)
            return true;
        if (rhs.m_processId < m_processId)
            return false;
        return m_isAlive < rhs.m_isAlive;
    }

    bool operator>(const MyProcess &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const MyProcess &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const MyProcess &rhs) const {
        return !(*this < rhs);
    }

    void kill() {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, m_processId);
        if (hProcess == NULL) {
            std::cerr << "OpenProcess failed" << std::endl;
        }

        if (!TerminateProcess(hProcess, 0)) {
            std::cerr << "TerminateProcess failed" << std::endl;
            CloseHandle(hProcess);
        }

        CloseHandle(hProcess);
        std::cout << "Process with id: " << m_id << "  was killed successfully" << std::endl;
        m_isAlive = false;
    }
};



