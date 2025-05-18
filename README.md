🔌 SocketSync

SocketSync is a network-based task orchestration system built in C++ that simulates real-time coordination between a central controller and multiple distributed worker clients using TCP sockets. Designed for concurrency and systems programming environments, SocketSync replaces shared memory with structured communication protocols, enabling synchronized task execution across fully independent processes.
📘 Introduction

SocketSync connects a central dispatcher (Mom) with four autonomous worker processes (Kids) over socket connections. Each Kid uses a behavioral profile (mood) to decide which job to take from the shared task list. Communication is handled through a custom protocol using enum-based message codes for reliability and structure.

This architecture mimics a distributed system where synchronized task execution is achieved without shared memory—only through carefully orchestrated socket messaging.
✨ Features

    🔌 TCP Socket-Based Communication
    All interactions between server and clients are handled through reliable socket connections.

    🧠 Mood-Driven Task Selection
    Each Kid process operates independently with a randomized behavioral logic: lazy, prissy, greedy, overtired, or cooperative.

    🗂️ Custom Protocol Messaging
    Uses enum message types (ACK, NACK, NEED_A_JOB, JOB_DONE, QUIT, etc.) for structured and deterministic communication.

    ⏱️ Time-Bound Execution
    The simulation runs for 21 time units (~3.5 simulated hours), after which all Kids receive a QUIT signal and terminate gracefully.

    🧾 Task Validation & Resolution
    The Mom process ensures that job assignments are confirmed or denied based on real-time availability.

    🏆 Scoring and Bonus Logic
    Each completed job earns points, and the top-performing Kid receives a performance bonus.

💻 Usage
🔧 Compilation

make

Compiles both the server (mom) and client (kid) binaries.
▶️ Running the Simulation

    Start the Dispatcher (Mom):

./mom

    In four separate terminals, start each Worker (Kid):

./kid

Each Kid will:

    Connect via socket

    Request and execute jobs

    Report completions

    Exit upon receiving the QUIT signal

🛠️ Project Structure

.
├── main.cpp             # Server (Mom) entry point
├── kidmain.cpp          # Client (Kid) entry point
├── Mom.[cpp|hpp]        # Task dispatcher and controller logic
├── Kid.[cpp|hpp]        # Worker logic and behavioral task selection
├── Job.[cpp|hpp]        # Shared job model
├── JobTable.hpp         # Task list and job metadata
├── Enums.hpp            # Protocol message types and mood enums
├── Printer.[cpp|hpp]    # Output utility
├── tools.[cpp|hpp]      # Utility functions
├── makefile             # Build configuration
└── output.txt           # Example simulation output

📄 Protocol Overview

Communication between Mom and the Kids is managed through enum-based message codes:

    NEED_A_JOB

    JOB_CHOICE

    ACK

    NACK

    JOB_DONE

    QUIT

Jobs are transmitted as blocks of integers (not strings), and responses are validated before execution proceeds.
📈 Sample Output

Refer to output.txt for a snapshot of a full run including:

    Worker mood assignments

    Job table exchanges

    Task completions

    Scoring summaries and the final winner

🤝 Contributing

Contributions to SocketSync are welcome! Whether you're improving performance, adding resilience features, or expanding the protocol, feel free to fork the repo and open a pull request. For major feature ideas, please open an issue to start a discussion.
📜 License

SocketSync is licensed under the MIT License. See the LICENSE file for full terms.
