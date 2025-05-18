#include "Mom.hpp"
#include "Printer.hpp"

/**
 * Updates the Mom server's socket address information (e.g., port number and IP). <br>
 * -------------------------------------------------------
 *  - Calls `getsockname` to retrieve the socket's local address structure after binding.
 *  - This information is stored in the `info` member variable for later use (e.g., printing).
 *  - If the call fails, it terminates the program using `fatal()`.
 * -------------------------------------------------------
 * @throws Terminates the program if `getsockname` fails.
 */
void Mom::refresh() {
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int status = getsockname( fd, (sockaddr*)&info, &addrlen);
    if ( status < 0 ) fatal("Socket: getsockname failed on socket %d." + fd);
    cout << *this;
}

/**
 * Creates, binds, and configures the server (Mom's) listening socket. <br>
 * -------------------------------------------------------
 * - Creates a TCP socket using `AF_INET` and `SOCK_STREAM`.
 * - Sets the socket's address family, port (converted to network byte order), and binds it to all available interfaces.
 * - Uses `bind()` to associate the socket with the specified port.
 * - Calls `refresh()` to retrieve and print the socket's local address.
 * - Calls `listen()` to mark the socket as a passive socket that will accept incoming client (kid) connections.
 * -------------------------------------------------------
 * @param port The port number on which the Mom server listens for client connections.
 * @throws Terminates the program if socket creation, binding, or listening fails.
 */
void Mom::listen(int port) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd<0) fatal("Socket: Can't create socket");

    info.sin_family = AF_INET;
    info.sin_port = htons(port);
    info.sin_addr.s_addr = INADDR_ANY;

    // sockaddr is used to set fields of kernel socket.
    int status = ::bind(fd, (sockaddr*)&info, sizeof(struct sockaddr_in));
    if (status < 0) fatal("Can't bind socket (%d)" + fd);
    refresh();
    cout << "Just bound socket " << fd << ": " << *this << endl;

    // Declare that this is the welcome socket and it listens for kid contacts.
    status = ::listen(fd, 4);
    if (status < 0) fatal("Socket: Unable to listen on socket %d." + fd);
    cout << "Just called listen(); now waiting for a client to show up\n";
}

/**
 * Accepts connections from kid clients and assigns each one a socket. <br>
 * -------------------------------------------------------
 * - Calls listen() to prepare the Mom server to accept connections.
 * - Sets up the welcome socket in the poll structure to monitor incoming connections.
 * - Repeats for MAXCLIENTS:
 *     - Accepts a connection from a kid using accept().
 *     - Initializes the corresponding pollfd entry in the worker array.
 *     - Sends an ACK message and the kid's assigned ID back to the client.
 *     - Logs the connection using Printer.
 * -------------------------------------------------------
 * @throws If accept() fails, it prints an error message but continues to attempt connections.
 */
void Mom::addClient() {
    listen(PORT);
    welcomeFd = fd;
    welcome -> fd = welcomeFd;
    welcome -> events = POLLIN;
    for (nCli = 0; nCli < MAXCLIENTS; nCli++) {
        sockaddr_in newCaller{};
        socklen_t sockLen = sizeof(newCaller);
        short newfd = accept(welcomeFd, (sockaddr*)&newCaller, &sockLen);
        if (newfd < 0){cerr << "No new client was added" << endl; continue;}
        worker[nCli].fd = newfd;
        worker[nCli].events = POLLIN;
        worker[nCli].revents = 0;
        message = static_cast<short>(messageCodes::ACK);
        write(newfd, &message, sizeof(short));
        write(newfd, &nCli,sizeof(short));
        ss << kidNames[nCli] << " has connected to Mom with ID: " << nCli << endl;
        Printer::write(ss, cout);
    }
}

/**
 * Sends the entire job table to a specific kid client over the given socket.
 * -------------------------------------------------------
 * - Iterates through all 10 jobs.
 * - Packs job attributes into a short array (entireJT) in a fixed order:
 *   jobNumber, slow, dirty, heavy, value, status.
 * - Sends the full array to the kid using write().
 * -------------------------------------------------------
 * @param fd The socket file descriptor of the kid client.
 */
void Mom::sendJobTable(int fd) {
    short index = 0;
    for (short i = 0; i < 10; i++) {
        Job job = table.jobs[i];
        entireJT[index++] = job.jobNumber;
        entireJT[index++] = job.slow;
        entireJT[index++] = job.dirty;
        entireJT[index++] = job.heavy;
        entireJT[index++] = job.value;
        entireJT[index++] = static_cast<short>(job.status);
    }
    write(fd, entireJT, sizeof(entireJT));
}

/**
 * Handles a job request from a kid and sends an appropriate response. <br>
 * -------------------------------------------------------
 * - If the requested job is NOT_STARTED:
 *     - Updates the job's status to WORKING and assigns it to the requesting kid.
 *     - Sends an ACK message to confirm assignment.
 * - If the job is already taken:
 *     - Sends a NACK message.
 *     - Resends the current state of the job table to help the kid choose again.
 * -------------------------------------------------------
 * @param kidIndex Index of the kid making the request.
 * @param jobChoiceIndex Index of the job the kid wants to perform.
 */
void Mom::jobRequest(short kidIndex, short jobChoiceIndex) {
   if (table.jobs[jobChoiceIndex].status == JobStatus::NOT_STARTED) {
       table.jobs[jobChoiceIndex].status = JobStatus::WORKING;
       table.jobs[jobChoiceIndex].kidID = kidIndex;
       message = static_cast<short>(messageCodes::ACK);
       write(worker[kidIndex].fd, &message, sizeof(short));
   }
   else {
        message = static_cast<short>(messageCodes::NACK);
        write(worker[kidIndex].fd, &message, sizeof(short));
        // write(worker[kidIndex].fd, entireJT, sizeof(entireJT));
    }
}

/**
 * Processes a message received from a kid client. <br>
 * -------------------------------------------------------
 * - Reads a message code from the kid’s socket.
 * - If the connection is closed or error occurs, closes the socket and removes the kid.
 * - If the message is:
 *   - NEED_JOB: Sends the full job table.
 *   - WANT_JOB: Reads the requested job index and processes the job request.
 *   - JOB_DONE: Reads the completed job index, updates status to COMPLETE, and logs it.
 * -------------------------------------------------------
 * @param kidIndex The index of the kid in the worker socket array.
 */
void Mom::processMessage(short kidIndex) {
    long nBytes = read(worker[kidIndex].fd, &message, sizeof(short));
    if (nBytes <= 0) {
        close(worker[kidIndex].fd);
        worker[kidIndex] = worker[--nCli];
    }
    if (message == static_cast<short>(messageCodes::NEED_JOB)) {sendJobTable(worker[kidIndex].fd);}
    if (message == static_cast<short>(messageCodes::WANT_JOB)) {
        short jobChoiceIndex;
        read(worker[kidIndex].fd, &jobChoiceIndex, sizeof(short));
        jobRequest(kidIndex,jobChoiceIndex);
    }
    if (message == static_cast<short>(messageCodes::JOB_DONE)) {
        short jobIndex;
        read(worker[kidIndex].fd, &jobIndex,sizeof(short));
        table.jobs[jobIndex].status = JobStatus::COMPLETE;
        table.jobs[jobIndex].kidID = kidIndex;
        scanJobTable();
    }
}

/**
 * Initializes the job table with 10 random jobs. <br>
 * -------------------------------------------------------
 * - Creates 10 Job objects using their index as jobNumber.
 * - Assigns each to the job table.
 * - Prints out job details to both the terminal and output file.
 */
void Mom::initializeJobTable() {
    for (short i = 0; i < 10; i++) {
        Job newJob(i);
        table.jobs[i] = newJob;
        ss << "Job" << i << endl;
        Printer::write(ss, cout);
        ss << newJob << endl;
        Printer::write(ss, cout);
    }
}

/**
 * Scans the job table for completed jobs and refreshes them. <br>
 * -------------------------------------------------------
 * - Iterates through all jobs in the job table.
 * - If a job has a status of COMPLETE:
 *     - Adds it to the `completedJobs` vector for end-of-session tracking.
 *     - Replaces the completed job with a new one at the same index.
 *     - Logs the replacement action using the Printer utility.
 * -------------------------------------------------------
 */
void Mom::scanJobTable() {
    for (short i = 0; i < 10; i++) {
        if (table.jobs[i].status == JobStatus::COMPLETE) {
            completedJobs.push_back(table.jobs[i]);
            table.jobs[i] = Job(i);
            ss<<"Adding new job at index: "<< i <<endl;
            Printer::write(ss, cout);
        }
    }
}


/**
 * Prints a basic message from Mom to standard output.<br>
 * -------------------------------------------------------
 * - Overloaded by operator<< to be usable with ostream objects.
 * -------------------------------------------------------
 * @param os Output stream to write to.
 * @return Reference to the output stream.
 */
ostream& Mom::print(ostream& os) const{
    os << "Hey kids, This is Mama" << endl;
    return os;
}

/**
 * Controls the main polling loop for Mom’s server process.
 * -------------------------------------------------------
 * - Displays a startup banner and initializes the job table.
 * - Accepts client (Kid) connections via sockets.
 * - Starts a timed loop (21 seconds) that:
 *     - Uses `poll` to monitor socket activity.
 *     - Scans the job table for completed tasks and refreshes it by replacing them with new jobs.
 *     - Processes incoming messages from kids.
 * - After the timer ends:
 *     - Sends the QUIT message to all connected kids.
 *     - Closes all sockets.
 *     - Performs a final scan of completed jobs.
 *     - Tallies total earnings for each kid.
 *     - Awards a bonus to the top earner.
 *     - Prints a summary report with total values and the winner.
 * -------------------------------------------------------
 */
void Mom::run() {
    banner();
    ss <<*this;
    Printer::write(ss,cout);
    initializeJobTable();
    ss << "Job Table Initialized" << endl;
    Printer::write(ss, cout);
    addClient();
    time(&startTime);
    while (difftime(time(&currentTime), startTime) < 21) {
        status = poll(ufds, 1 + nCli, 1000);
        scanJobTable();
        for (short i = 0; i < nCli; i++) {
            if (worker[i].revents & POLLIN) {
                processMessage(i);
            }
        }
    }

    for (short i = 0; i < nCli; i++) {
        message = static_cast<short>(messageCodes::QUIT);
        write(worker[i].fd, &message, sizeof(short));
    }
    close(welcomeFd);
    scanJobTable();
    unordered_map<string, short> totalEarnings;

    for (Job& job : completedJobs) {
        totalEarnings[kidNames[job.kidID]] += job.value;
    }

    string winner;
    short maxEarnings = -1;
    for (auto& [name, value] : totalEarnings) {
        if (value > maxEarnings) {
            maxEarnings = value;
            winner = name;
        }
    }

    totalEarnings[winner] += 5;
    ss << "--------------------Mama-----------------------------" << endl;
    Printer::write(ss, cout);

    for (Job& job : completedJobs) {
        ss << "Child " << kidNames[job.kidID] << " has earned a total value of " << job.value << " on this job " << job.jobNumber << endl;
        Printer::write(ss, cout);
    }

    ss << "The winner for today is " << winner << ", who had a total of " << totalEarnings[winner] << endl;
    Printer::write(ss, cout);
}