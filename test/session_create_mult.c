#include "session.h"

int main() {
    struct dchat_session * session0, * session1;
    if(dchat_session_init(&session0) != 0) {
        return -1;
    }

    if(dchat_session_init(&session1) != 0) {
        return -1;
    }

    if(dchat_session_close(session0) != 0) {
        return -1;
    }

    if(dchat_session_close(session1) != 0) {
        return -1;
    }

    return 0;
}
