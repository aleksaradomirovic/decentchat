#pragma once

#include "dchat/dchat.h"

#include <gtest/gtest.h>

class dchat_test : public testing::Test {
  protected:
    void SetUp() override {
        ASSERT_EQ(dchat_init(), 0);
    }

    void TearDown() override {
        ASSERT_EQ(dchat_fini(), 0);
    }
};

class dchat_session_test : public dchat_test {
  protected:
    dchat_t session;

    void SetUp() override {
        dchat_test::SetUp();
        ASSERT_EQ(dchat_open(&session), 0);
    }

    void TearDown() override {
        ASSERT_EQ(dchat_close(session), 0);
        dchat_test::TearDown();
    }
};
