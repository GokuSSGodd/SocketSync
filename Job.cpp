#include "Job.hpp"
#include "Printer.hpp"

/**
 * Default Constructor<br>
 * -------------------------------------------------------<br>
 * - Randomly generates values between 1–5 for `slow`, `dirty`, and `heavy`. <br>
 * - Calculates the job value as: <br>
 *     value = slow × (dirty + heavy) <br>
 * - Sets the initial job status to `NOT_STARTED`. <br>
 * -------------------------------------------------------<br>
 */
Job::Job() {
    slow = rand() % 5 + 1;
    dirty = rand() % 5 + 1;
    heavy = rand() % 5 + 1;
    value = slow * (dirty + heavy);
    status = JobStatus::NOT_STARTED;
};

/**
 * Parameterized Constructor<br>
 * -------------------------------------------------------<br>
 * @param index Index to assign as job number.<br>
 * - Initializes the job using the same logic as the default constructor.<br>
 * - Assigns `jobNumber` to the given index.<br>
 * -------------------------------------------------------<br>
 */
Job::Job(short index) : jobNumber(index) {
    slow = rand() % 5 + 1;
    dirty = rand() % 5 + 1;
    heavy = rand() % 5 + 1;
    value = slow * (dirty + heavy);
    status = JobStatus::NOT_STARTED;
};

/**
 * Assigns a job to a kid<br>
 * -------------------------------------------------------<br>
 * @param kidID The ID of the kid taking the job.<br>
 * @param jobNumber The job number to assign.<br>
 * - Sets job status to `WORKING`.<br>
 * -------------------------------------------------------<br>
 */
void Job::chooseJob(const short& kidID, const short jobNumber) {
    this->jobNumber = jobNumber;
    this->kidID = kidID;
    status = JobStatus::WORKING;
};

/**
 * Marks job as completed<br>
 * -------------------------------------------------------<br>
 * - Sets job status to `COMPLETE`.<br>
 * - Logs the completion message to both console and file.<br>
 * -------------------------------------------------------<br>
 */
void Job::announceDone() {
    status = JobStatus::COMPLETE;
    ss << "Job ID:" << jobNumber << " is completed" << endl;
    Printer::write(ss, cout);
};

/**
 * Print function for the Job object<br>
 * -------------------------------------------------------<br>
 * @param os Output stream to write to (e.g., std::cout)<br>
 * @return Reference to the stringstream with job details<br>
 * -------------------------------------------------------<br>
 */
ostream& Job::print(ostream& os) const {
    ss << "The job value is : " << value
        << " it has " << slow << " slow"
       << " it has " << dirty << " dirty"
       << " it has " << heavy << " heavy" << endl;
    return ss;
}