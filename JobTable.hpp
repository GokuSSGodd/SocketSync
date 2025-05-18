#pragma once
#include "tools.hpp"
#include "Job.hpp"

/**
 * @class JobTable<br>
 * Stores and manages a fixed-size list of Job objects.<br>
 * -------------------------------------------------------<br>
 * - Contains 10 Job slots used by the Mom and accessed by the Kids.<br>
 * - Includes a `quitFlag` to indicate when to stop job processing.<br>
 * - Provides controlled access to job entries through friend classes.<br>
 */
class JobTable {
private:
  Job jobs[10]{};   ///< Array of 10 Job objects<br>
  bool quitFlag;    ///< True if kids should continue working<br>

public:
  /**
   * Default Constructor<br>
   * Initializes the quit flag to true.<br>
   */
  JobTable() : quitFlag(true) {}

  /**
   * (Optional) Print Function<br>
   * Currently a stub. Intended for future display or debug purposes.<br>
   * @param out Output stream reference<br>
   */
  void print(ostream& out) {}

  // Granting friend access to allow direct job manipulation
  friend class Kid;
  friend class Mom;
};
