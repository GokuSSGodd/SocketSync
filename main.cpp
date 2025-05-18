#include "tools.hpp"
#include "Mom.hpp"

/**
 * Main function<br>
 * -------------------------------------------------------<br>
 * - Seeds the random number generator for job attributes.<br>
 * - Initializes and starts the Mom server process.<br>
 * - Executes the full simulation including:<br>
 *    - Job table initialization<br>
 *    - Accepting client (Kid) connections<br>
 *    - Polling loop for chore assignment<br>
 *    - Final reporting<br>
 * - Calls the goodbye banner after simulation ends.<br>
 * -------------------------------------------------------<br>
 * @return 0 on successful execution<br>
 */
int main() {
    srand(time(nullptr));
    Mom mom;
    mom.run();
    bye();
    return 0;
}
