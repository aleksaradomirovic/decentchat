#include "fixture.hpp"

#include "dchat/dchat.h"

#include <gtest/gtest.h>

TEST_F(dchat_test, dchat_init_fini) {}

TEST_F(dchat_test, dchat_init_server) {
    dchat_t session;
    ASSERT_EQ(dchat_open(&session), 0);
    ASSERT_EQ(dchat_close(session), 0);
}

TEST_F(dchat_session_test, dchat_open_port) {
    ASSERT_EQ(dchat_open_port(session, 20030), 0);
    ASSERT_EQ(dchat_close_port(session, 20030), 0);
}

TEST_F(dchat_session_test, dchat_port_duplicate) {
    ASSERT_EQ(dchat_open_port(session, 20030), 0);
    EXPECT_NE(dchat_open_port(session, 20030), 0);
    ASSERT_EQ(dchat_close_port(session, 20030), 0);
}

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
