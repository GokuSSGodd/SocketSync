#pragma once
#include "tools.hpp"
#include "Enums.hpp"

/**
 * @class Job<br>
 * Represents a chore/task assigned by Mom to the Kids.<br>
 * -------------------------------------------------------<br>
 * Each Job has attributes such as:<br>
 * - `slow`: Time required to complete the task (1–5)<br>
 * - `dirty`: Dirtiness level of the task (1–5)<br>
 * - `heavy`: Weight or difficulty of the task (1–5)<br>
 * - `value`: Calculated as slow × (dirty + heavy)<br>
 * - `status`: Current status of the job (NOT_STARTED, WORKING, COMPLETE)<br>
 * - `kidID`: ID of the kid currently working on or who completed the job<br>
 * -------------------------------------------------------<br>
 */
class Job {
private:
    short jobNumber;  ///< Job identifier number<br>
    short slow;       ///< Time to complete the job<br>
    short dirty;      ///< Dirtiness level of the job<br>
    short heavy;      ///< Weight/difficulty level of the job<br>
    short value;      ///< Total value (score) of the job<br>
    short kidID;      ///< ID of the kid assigned to the job<br>

public:
    JobStatus status; ///< Current status of the job<br>

    /**
     * Default constructor<br>
     * Randomizes slow, dirty, heavy values between 1–5<br>
     * Sets value and initializes status to NOT_STARTED<br>
     */
    Job();

    /**
     * Constructor with Job Number<br>
     * @param index Job number to assign<br>
     * Also randomizes other attributes as above<br>
     */
    Job(short index);

    ~Job() = default;

    /**
     * Assigns the job to a kid<br>
     * @param kidID Kid's ID<br>
     * @param jobNumber Job number<br>
     * Sets job status to WORKING<br>
     */
    void chooseJob(const short& kidID, short jobNumber);

    /**
     * Marks job as completed and logs a message<br>
     */
    void announceDone();

    /**
     * Prints job details (slow, dirty, heavy, value)<br>
     * @param os Output stream<br>
     * @return ostream& Reference for chaining<br>
     */
    ostream& print(ostream& os) const;

    friend class Kid;
    friend class Mom;
};

/**
 * Overloaded output stream operator<br>
 * @param out Output stream<br>
 * @param job Job object<br>
 * @return ostream& for chaining<br>
 */
inline ostream& operator << (ostream& out, Job& job){
    return job.print(out);
}
