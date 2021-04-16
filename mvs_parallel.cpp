#include <chrono>
#include <future>
#include <iostream>
#include <map>
#include <thread>

#include "genetic.hpp"
// #include "threadpool.hpp"
#include "tinyxml2.h"

size_t cur_fit;
entity cur_best;
std::mutex ent_mutex;
Population popl;
int k = 0;

std::map<std::thread::id, std::pair<bool, int>> func_results;

void fit_entity() {
    auto valueless_iters = 0;
    auto value_iters = 0;
    auto ever_changed = false;
    while (valueless_iters < 1000 || cur_fit != 0) {  // NOLINT
        auto new_ent = popl.generate_random_entity();
        auto new_fit = popl.calculate_target_fuction(new_ent);
        std::unique_lock<std::mutex> lock(ent_mutex);
        if (new_fit >= cur_fit || !popl.check_if_fits(new_ent)) {
            ++valueless_iters;
            continue;
        }
        // using namespace std::chrono_literals;
        // ++k;
        // std::this_thread::sleep_for(10ms);
        cur_best = new_ent;
        cur_fit = new_fit;
        lock.unlock();
        valueless_iters = 0;
        ever_changed = true;
        ++value_iters;
    }
    func_results[std::this_thread::get_id()] =
        std::make_pair(ever_changed, value_iters + valueless_iters);
    // return std::make_pair(ever_changed, value_iters + valueless_iters);
}

int main() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("killme.xml");
    popl.generate_from_xml(doc);

    for (auto x : popl.get_affils()) {
        cur_fit += x.second;
    }

    std::vector<std::thread> threads;

    const auto num_threads = 15U;

    for (auto it = 0U; it != num_threads; ++it) {
        threads.emplace_back(fit_entity);
    }
    for (auto it = 0U; it != num_threads; ++it) {
        threads[it].join();
    }

    std::cout << "my ass: " << k << std::endl;
    for (auto x : func_results) {
        std::cout << x.first << "\t" << x.second.first << "\t"
                  << x.second.second << std::endl;
    }

    // using wtf = std::pair<std::thread::id, std::pair<bool, int>>;
    // std::vector<wtf> proxy(func_results.begin(), func_results.end());
    // auto compare_changeness = [](wtf a, wtf b){
    //     return a.second.first || b.second.first;
    // };
    // auto ever_changed = std::accumulate(proxy.begin(), proxy.end(), false,
    // compare_changeness);

    auto ever_changed = false;
    auto iter_sum = 0UL;
    for (auto cur : func_results) {
        ever_changed = ever_changed || cur.second.first;
        iter_sum += cur.second.second;
    }

    if (ever_changed) {
        std::cout << "success" << std::endl;
        std::cout << iter_sum << std::endl;
        std::for_each(cur_best.begin(), cur_best.end(), [](auto &n) {
            std::cout << n.first << "\t" << n.second << std::endl;
        });
        std::cout << "fitness: " << cur_fit << std::endl;
    } else {
        std::cout << "failure" << std::endl;
        std::cout << iter_sum << std::endl;
    }
}