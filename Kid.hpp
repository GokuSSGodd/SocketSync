#pragma once
#include "tools.hpp"
#include "Enums.hpp"
#include "Job.hpp"
#include "JobTable.hpp"

/**
 * @class Kid<br>
 * Represents a child client in the client-server chore simulation.<br>
 * Handles connecting to Mom, receiving the job table, selecting jobs, and completing them.<br>
 */
class Kid {
private:
    short kidID;                          ///< Unique identifier for the kid<br>
    Mood mood{};                          ///< Mood affecting job selection behavior<br>
    vector<Job> finishedJobs;             ///< List of jobs completed by this kid<br>
    Job* inProgress;                      ///< Pointer to the current job in progress<br>
    JobTable table;                       ///< Local copy of the job table received from Mom<br>
    int clientSock;                       ///< Socket descriptor for communicating with Mom<br>
    short buf;                            ///< Buffer for reading incoming socket data<br>

    /**
     * Selects a job for non-cooperative kids based on mood conditions.<br>
     */
    void non_Coop_Task_Select();

    /**
     * Selects a job for cooperative kids by picking the last available job.<br>
     */
    void coop_Task_Select();

    /**
     * Checks if the job is acceptable based on the kid's mood.<br>
     * @param job Job object to evaluate<br>
     * @return true if the kid is willing to do the job<br>
     */
    bool moodChecker(const Job& job) const;

    /**
     * Reads a short value from the socket.<br>
     * @return The value read<br>
     * @throws int 0 if there’s a disconnection<br>
     */
    short readData();

    /**
     * Writes a short value to the socket.<br>
     * @param msg Message to be sent to Mom<br>
     */
    void writeData(const short& msg) const;

    /**
     * Parses the incoming job table sent from Mom and fills the local table.<br>
     */
    void parseJobTable();

    /**
     * Sends a WANT_JOB message and receives a response.<br>
     * If ACK is received, the job is assigned.<br>
     * If NACK or QUIT is received, handles accordingly.<br>
     * @param job Job object reference<br>
     * @param index Index of the job in the table<br>
     * @return true if job was accepted<br>
     * @throws int 0 if Mom sends QUIT<br>
     */
    bool wantJob(Job& job, short index);

public:
    /**
     * Constructor<br>
     * Establishes a connection to Mom's server at localhost on port 1099<br>
     */
    Kid();

    /**
     * Destructor (default)<br>
     */
    ~Kid() = default;

    /**
     * Randomly assigns a mood to the kid from the Mood enum.<br>
     */
    void selectMood();

    /**
     * Selects a job based on the kid’s mood and job availability.<br>
     */
    void selectJob();

    /**
     * Main function that governs the kid’s behavior:<br>
     * - Receives ID and mood<br>
     * - Requests job table<br>
     * - Selects and completes jobs<br>
     * - Exits on QUIT message<br>
     */
    void run();

    /**
     * Prints the kid’s ID.<br>
     */
    void print() const;

    /**
     * Prints all jobs completed by the kid.<br>
     */
    void printCompletedJob() const;
};

