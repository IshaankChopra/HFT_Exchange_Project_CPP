# High-Frequency Trading Simulator: Market Data & Strategy Engine

## 🚀 Project Overview

This project implements a simulated low-latency trading environment using modern C++ (C++17/20) and low-level networking techniques (sockets). The primary goal is to demonstrate proficiency in handling market data distribution via Multicast UDP and order routing via TCP, adhering to strict Linux-based development requirements.

The system is composed of three primary executables designed to run simultaneously as separate processes:

1.  **Injector:** Reads historical market data logs (`SCH.log`, `SCS.log`) and broadcasts updates over Multicast UDP.
2.  **Strategy:** Subscribes to the Multicast data feed, maintains an order book, generates order requests, and sends them to the Exchange via TCP.
3.  **Exchange (Mock):** Receives orders via TCP and immediately sends back synthetic execution reports.

---

## 💻 Technical Requirements

* **Operating System:** Linux (Ubuntu 22.04+ or compatible).
* **Compiler:** GCC or Clang (modern C++17/20 support required).
* **Build System:** CMake (v3.10 or higher).
* **Networking:** POSIX Sockets (Multicast UDP and TCP).

> **Note for macOS Users (Apple Silicon):** This project requires a Linux environment for submission compatibility. Development is done using **UTM** for near-native virtualization of **Ubuntu ARM64**.

---

## 🛠️ Setup and Build Instructions

### 1. Project Files

Ensure your project directory contains the following structure and data files:

```text
HFT_Project/
├── build/                 # (Created during the build process)
├── data/                  # <-- PLACE YOUR SCH.log and SCS.log HERE
├── src/
│   ├── common/            # Protocol definitions
│   ├── network/           # Socket wrappers (Multicast, TCP)
│   ├── injector/
│   ├── strategy/
│   └── exchange/
└── CMakeLists.txt
