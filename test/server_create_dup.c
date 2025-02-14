#include "session.h"

int main() {
    struct dchat_session * session;
    if(dchat_session_init(&session) != 0) {
        return -1;
    }

    if(dchat_session_open_server(session, 20037) != 0) {
        return -1;
    }

    if(dchat_session_open_server(session, 20037) == 0) {
        return -1;
    }

    if(dchat_session_close_server(session, 20037) != 0) {
        return -1;
    }

    if(dchat_session_close(session) != 0) {
        return -1;
    }

    return 0;
}
