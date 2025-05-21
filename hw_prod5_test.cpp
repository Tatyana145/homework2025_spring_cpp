#include "hw_prod5.h"

int main() {
    Log* log = Log::Instance();

    log->message(LOG_NORMAL, "Program started");
    log->message(LOG_WARNING, "Low memory");
    log->message(LOG_ERROR, "Disk full");
    log->message(LOG_NORMAL, "User logged in");
    log->message(LOG_WARNING, "High CPU usage");
    log->message(LOG_ERROR, "Network connection lost");
    log->message(LOG_NORMAL, "Data processed");
    log->message(LOG_WARNING, "Cache almost full");
    log->message(LOG_ERROR, "Database connection failed");
    log->message(LOG_NORMAL, "Program exiting");
    log->message(LOG_NORMAL, "This is an extra message to test the limit");
    log->print();

    return 0;
}