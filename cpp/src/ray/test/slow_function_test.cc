
#include <gtest/gtest.h>
#include <ray/api.h>

#include <chrono>
#include <thread>

using namespace ray::api;

int slow_function(int i) {
  std::this_thread::sleep_for(std::chrono::seconds(i));
  return i;
}

RAY_REMOTE(slow_function);

TEST(RaySlowFunctionTest, BaseTest) {
  RayConfig config;
  config.local_mode = true;
  Ray::Init(config);
  auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch());
  auto r0 = Ray::Task(slow_function).Remote(1);
  auto r1 = Ray::Task(slow_function).Remote(2);
  auto r2 = Ray::Task(slow_function).Remote(3);
  auto r3 = Ray::Task(slow_function).Remote(4);

  int result0 = *(r0.Get());
  int result1 = *(r1.Get());
  int result2 = *(r2.Get());
  int result3 = *(r3.Get());
  auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch());

  EXPECT_EQ(result0, 1);
  EXPECT_EQ(result1, 2);
  EXPECT_EQ(result2, 3);
  EXPECT_EQ(result3, 4);

  EXPECT_LT(time2.count() - time1.count(), 4200);
}
