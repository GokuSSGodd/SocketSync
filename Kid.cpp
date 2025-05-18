#include "Kid.hpp"
#include "Printer.hpp"

/**
 * Constructor for the Kid class. <br>
 * -------------------------------------------------------
 * - Initializes a TCP socket for the kid.
 * - Resolves localhost and configures socket address.
 * - Connects to the Mom (server) on the predefined port.
 * - Prints client socket information upon successful connection. <br>
 * -------------------------------------------------------
 */
Kid::Kid():inProgress(nullptr){

    // ================================================================
    // Install a socket in the client's file table.
    // Make an internet-transmitted, file-i/o-style, protocol-whatever plug
    clientSock = socket( AF_INET, SOCK_STREAM, 0 );
    if ( clientSock<0 ) fatal( "Can't assign fd for client socket");

    // ================================================================
    // Use a domain name server to get the IP address associated with the host.
    // The name server returns a list of IP addresses.  We want the first one.
    hostInfo* remoteHost = gethostbyname(LOCALHOST);
    if(remoteHost == nullptr) fatal("unknown host: "+string(LOCALHOST)+"\n");

    // ================================================================
    // This series of system calls sets up the client's socket structure.
    sockInfo clientInfo;
    clientInfo.sin_family = AF_INET;	// Address family internet

    // Copy info about the server into the client's socket structure.
    memmove( &clientInfo.sin_addr, remoteHost->h_addr_list[0], remoteHost->h_length);

    // Copy the agreed-upon port number into the client's socket structure.
    // Convert it first from the local representation to the internet standard.
    clientInfo.sin_port = htons( PORT );
    printSockInfo( "client", clientInfo );

    // ================================================================
    // Info in the client struct is now complete and ready to connect to server.
    int status = connect( clientSock, (sockUnion*) &clientInfo, sizeof clientInfo );
    if ( status < 0 ) fatal("Connection to "+string(LOCALHOST)+" refused.");
    cout <<"connection established to " <<LOCALHOST<<".\n";
}

/**
 * Reads a short message from the server.
 * -------------------------------------------------------
 * - Receives a message from the Mom.
 * - If connection is lost, throws an int to exit job loop.
 * -------------------------------------------------------
 * @return short The message code received from Mom.
 * @throws int 0 if the socket is closed.
 */
short Kid::readData() {
    // wait for server to acknowledge the connection.
    long nBytes = read( clientSock, &buf, sizeof(short));
    if(!nBytes) throw 0;
    return buf;
}

/**
 * Sends a short message to the server.
 * -------------------------------------------------------
 * @param msg The short integer message to write to the socket.
 */
void Kid::writeData(const short& msg) const {
    long nBytes = write( clientSock, &msg, sizeof(short));
    if( nBytes < 0 ) fatal("%s: Error while writing to socket.");
}

/**
 * Randomly assigns one of the five moods to the kid.
 * ------------------------------------------------------- <br>
 * Mood types: LAZY, PRISSY, OVERTIRED, GREEDY, COOPERATIVE.
 */
void Kid::selectMood() {
    mood = static_cast<Mood>(rand()%5);
}

/**
 * Checks if the job aligns with the Kid's mood preferences.
 * -------------------------------------------------------
 * LAZY      -> avoids heavy jobs. <br>
 * PRISSY    -> avoids dirty jobs. <br>
 * OVERTIRED -> avoids slow jobs. <br>
 * GREEDY    -> selects jobs with value > 40. <br>
 * COOPERATIVE -> always accepts.  <br>
 * -------------------------------------------------------
 * @param job The job to evaluate.
 * @return true if the job is suitable based on mood; false otherwise.
 */
bool Kid::moodChecker(const Job& job) const {
    return (job.heavy < 3 && mood == Mood::LAZY) ||
           (job.dirty < 3 && mood == Mood::PRISSY) ||
           (job.slow < 3 && mood == Mood::OVERTIRED) ||
           (job.value > 40 && mood == Mood::GREEDY);
}

/**
 * Sends a job request to Mom and processes the response.
 * -------------------------------------------------------
 * - Sends WANT_JOB with job index.
 * - Receives ACK or NACK or QUIT.
 * - On ACK, assigns job to Kid.
 * - On NACK, returns false to keep searching.
 * - On QUIT, throws int to exit.
 * -------------------------------------------------------
 * @param job Job to be attempted.
 * @param index Index of the job in the table.
 * @return true if job was accepted; false if rejected.
 * @throws int 0 if Mom sends QUIT.
 */
bool Kid::wantJob(Job& job, const short index) {
    writeData(static_cast<short>(messageCodes::WANT_JOB));
    writeData(index);
    readData();
    ss<<messageCodes[buf]<<endl;
    Printer::write(ss,cout);
    if (buf == static_cast<short>(messageCodes::NACK)) return false;
    if (buf == static_cast<short>(messageCodes::QUIT)) throw 0;
    job.chooseJob(kidID,index);
    inProgress = &job;
    return true;
}

/**
 * Selects a job for non-cooperative moods.
 * -------------------------------------------------------
 * - Loops from beginning of job table.
 * - Tries to find the first job matching moodChecker.
 * - Sends WANT_JOB request for match.
 */
void Kid::non_Coop_Task_Select() {
    for (short j=0; j<10;j++) {
        if (table.jobs[j].status != JobStatus::NOT_STARTED ) continue;
        if (moodChecker(table.jobs[j]) && wantJob(table.jobs[j], j)) break;
    }
}

/**
 * Selects a job for cooperative kids.
 * -------------------------------------------------------
 * - Loops from end of job table backwards.
 * - Chooses last available NOT_STARTED job.
 */
void Kid::coop_Task_Select() {

    for ( short j = 9 ;j >= 0; j--) {
        if (table.jobs[j].status != JobStatus::NOT_STARTED ) continue;
        if (wantJob(table.jobs[j], j)) break;
    }
}

/**
 * Determines which job selection strategy to use.
 * -------------------------------------------------------
 * - If mood is COOPERATIVE: use coop_Task_Select.
 * - Otherwise: use non_Coop_Task_Select.
 */
void Kid::selectJob() {
    if (mood != Mood::COOPERATIVE) non_Coop_Task_Select();
    else coop_Task_Select();
}

/** Sending message to mom <br>
 * Name: NEED A JOB <br>
 * Receiving message from mom <br>
 *  Field      | Description         | Example          <br>
 *  -----------|---------------------|----------------  <br>
 *  jobNumber  | Job ID number       | 1                <br>
 *  slow       | Time to complete    | 5                <br>
 *  dirty      | Dirtiness level     | 2                <br>
 *  heavy      | Weight factor       | 3                <br>
 *  value      | Value of job        | 25               <br>
 *  status     | Job status enum     | 0 (NOT_STARTED)  <br>
 *
 * Job Number, Slow, Dirty, Heavy, Value, Status <br>
 * 1 5 2 3 25 0<br>
 */
void Kid::parseJobTable() {
    // Requesting for Job
    writeData(static_cast<short>(messageCodes::NEED_JOB));

    //If mom sends to QUIT signal, I will quit
    //If mom sends ACKNOWLEDGE signal, I get the job table
    if (readData() == static_cast<short>(messageCodes::QUIT)) throw 0;
    short jobDesc[60];
    long nBytes = read( clientSock, &jobDesc, sizeof(jobDesc));
    if(!nBytes) throw 0;
    ss<<"-----------------------------------------------------"<<endl;
    ss<<"Retrieving Job Table"<<endl;
    for (short j=0;j<60;j+=6) {
        Job job{};
        job.jobNumber = jobDesc[j];
        job.slow = jobDesc[j+1];
        job.dirty = jobDesc[j+2];
        job.heavy = jobDesc[j+3];
        job.value = jobDesc[j+4];
        job.status = static_cast<JobStatus>(jobDesc[j+5]);
        table.jobs[job.jobNumber] = job;
        ss<<"Job number: "<<job.jobNumber <<" has been added"<<endl;
    }
    ss<<"Retrieved Job Table"<<endl;
    ss<<"-----------------------------------------------------"<<endl;
    Printer::write(ss,cout);
}

/**
 * Main loop for Kid behavior.
 * -------------------------------------------------------
 * - Gets assigned Kid ID and sets mood.
 * - In loop:
 *     - Requests job table from Mom.
 *     - Selects job based on mood.
 *     - Sleeps for job's duration (simulating work).
 *     - Sends JOB_DONE to Mom when finished.
 * - Exits gracefully if Mom sends QUIT.
 */
void Kid::run() {
    //Gets the ID
    ss<<messageCodes[readData()]<<endl; //First Acknowledgement
    Printer::write(ss,cout);
    kidID = readData(); //KidID received
    ss<<"Kid ID: "<<kidID<<endl;
    Printer::write(ss,cout);
    //Selects the mood of the kid
    selectMood();
    ss<<kidID<<" mood is: "<<moodName[static_cast<short>(mood)] <<endl;
    Printer::write(ss, cout);

    try {
        while (table.quitFlag) {
            parseJobTable();
            selectJob();
            if (inProgress != nullptr && inProgress->status == JobStatus::WORKING) {
                sleep(inProgress->slow);
                inProgress->announceDone();
                finishedJobs.push_back(*inProgress);
                writeData(static_cast<short>(messageCodes::JOB_DONE));
                writeData(inProgress->jobNumber);
                ss<<"Job Completed status: "<< jobStatusName[static_cast<short>(inProgress->status)]<<endl;
                Printer::write(ss, cout);
                inProgress = nullptr;
            }
        }
    }catch (int _) {
        ss<<"Quitting the program"<<endl;
        Printer::write(ss,cout);
    }

}

/**
 * Prints the Kid's ID to the console.
 */
void Kid::print() const {
    Printer::writeln("This is Kid: "+kidID,cout);
}

/**
 * Displays all jobs completed by the Kid.
 */
void Kid::printCompletedJob() const{
    for (Job job: finishedJobs) {
        ss << job << " was completed by "<< job.kidID<<endl;
        Printer::write(ss, cout);
    }
}