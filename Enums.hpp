#pragma once

/**
 * @enum Mood<br>
 * Represents the different moods a Kid can have.<br>
 * Affects how Kids select jobs from the JobTable.<br>
 */
enum class Mood {
    LAZY,         ///< Avoids heavy jobs<br>
    PRISSY,       ///< Avoids dirty jobs<br>
    OVERTIRED,    ///< Avoids slow jobs<br>
    COOPERATIVE,  ///< Chooses the last available job<br>
    GREEDY        ///< Always goes for the highest-value job<br>
};

/**
 * Array mapping Mood enum to string names<br>
 */
const string moodName[] = {
    "LAZY",
    "PRISSY",
    "OVERTIRED",
    "COOPERATIVE",
    "GREEDY"
};

/**
 * @enum JobStatus<br>
 * Represents the current status of a job.<br>
 */
enum class JobStatus {
    NOT_STARTED, ///< Job has not been taken yet<br>
    WORKING,     ///< Job is currently being worked on<br>
    COMPLETE     ///< Job is finished<br>
};

/**
 * Array mapping JobStatus enum to string names<br>
 */
const string jobStatusName[] = {
    "NOT_STARTED",
    "WORKING",
    "COMPLETE"
};

/**
 * @enum messageCodes<br>
 * Enum used for message-based communication between Mom and Kids.<br>
 * Represents various protocol message types.<br>
 */
enum class messageCodes {
    ACK,        ///< Acknowledgement<br>
    NACK,       ///< Negative Acknowledgement (Job denied)<br>
    QUIT,       ///< Signals termination<br>
    WANT_JOB,   ///< Kid wants to request a specific job<br>
    NEED_JOB,   ///< Kid needs the full job table<br>
    JOB_DONE    ///< Kid finished a job and is reporting it<br>
};

/**
 * Array mapping messageCodes enum to string descriptions<br>
 */
const string messageCodes[] = {
    "ACKNOWLEDGE",
    "NON ACKNOWLEDGE",
    "TIME TO QUIT",
    "WANT JOB",
    "NEED A JOB",
    "JOB DONE"
};
