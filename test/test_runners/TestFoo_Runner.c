#include "../unity.h"
#include "../extras/fixture/src/unity_fixture.h"

TEST_GROUP_RUNNER(Crypt)
{
  RUN_TEST_CASE(Crypt, TestCrypt1);
  RUN_TEST_CASE(Crypt, TestCrypt2);
}
