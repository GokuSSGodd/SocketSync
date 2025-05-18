#include "Kid.hpp"

/**
 * Main function (Kid)<br>
 * -------------------------------------------------------<br>
 * - Seeds the random number generator (used for mood/job creation).<br>
 * - Initializes a Kid object which:<br>
 *    - Connects to the Mom server via sockets.<br>
 *    - Receives a Kid ID and selects a mood.<br>
 *    - Continuously requests and performs jobs.<br>
 *    - Sends job completion messages to Mom.<br>
 *    - Terminates gracefully upon receiving the QUIT signal.<br>
 * -------------------------------------------------------<br>
 * @return 0 on successful execution<br>
 */
int main() {
    srand(time(nullptr));
    Kid kid{};
    kid.run();
    return 0;
}
