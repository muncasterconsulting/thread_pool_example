#include <atomic>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <deque>

#include <async.h>

using namespace std;

size_t work(size_t i) {
  //std::cout << "==> Work: " << i << endl;
  auto r = std::rand() % 10;
  std::this_thread::sleep_for(std::chrono::milliseconds(100 + r));
  return i;
}

void dump_results(std::deque<size_t>& results) {
  while (!results.empty()) {
    auto result = results.front();
    std::cout << " Result:     " << result << std::endl;
    results.pop_front();
  }
}

int main(int argc, char** argv) {
  const size_t pool_size = 2;
  std::atomic<int> active_thread_count(0);
  async::pool pool(pool_size);
  async::queue lock_queue;
  std::deque<size_t> results;

  // Image capture loop
  for (size_t i = 0; i < 100; ++i) {
    // Run async process that spans multiple iterations
    // Control number of outstanding tasks via active_thread_count
    if (active_thread_count < pool_size) {
      std::cout << "Process: " << i << std::endl;
      pool.async([&, i]{
        active_thread_count++;

        auto result = work(i);

        // lock_queue makes this operation thread-safe
        lock_queue.sync([&]() {
          results.push_back(result);
        });

        active_thread_count--;
      });
    }
    else {
      std::cout << "   Skip: " << i << std::endl;
    }

    // Integrate most recent results. lock_queue makes this operation thread-safe.
    lock_queue.sync([&]() {
      dump_results(results);
    });

    // Main loop rate
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  pool.wait();

  dump_results(results);

  return 0;
}

