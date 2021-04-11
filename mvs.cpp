#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

typedef std::pair<int16_t, int16_t> affiliation;
// using entity = std::vector<std::pair<affiliation, int16_t>>;
using entity = std::unordered_map<int16_t, int16_t>;
// typedef std::vector<std::pair<affiliation, int16_t>> entity;

class Population {
   private:
    std::vector<int16_t> max_loads;
    std::vector<int16_t> progs;
    std::vector<std::pair<affiliation, int16_t>> affils;
    std::vector<std::pair<entity, size_t>> population;
    int16_t preserved_high_positions;
    int16_t preserved_low_positions;
    int16_t crossing_volume;
    int16_t mutation_volume;
    auto crossbreeding() -> void;      // done
    auto mutation() -> void;           // done
    auto sort_population() -> void;    // done
    auto selection(double T) -> void;  // done

    auto calculate_target_fuction(const entity &ent);  // done
   public:

    auto run_generation() -> void;
    auto get_fittest();
};

auto Population::get_fittest(){
    sort_population();
    return population[0].first;
}

auto Population::calculate_target_fuction(const entity &ent) {
    size_t res = 0;
    for (auto x : affils) {
        if (ent.at(x.first.first) != ent.at(x.first.second)) {
            res += x.second;
        }
    }
    return res;
}

auto universal_crossbreeding(entity a, entity b) -> entity {
    entity res;
    static std::default_random_engine rng;
    std::uniform_int_distribution<> dst(0, 1);
    std::vector<affiliation> a_vec(a.begin(), a.end());
    std::vector<affiliation> b_vec(b.begin(), b.end());
    std::sort(a_vec.begin(), a_vec.end(), [](const auto &a, const auto &b) {
        if (a.first == b.first) {
            return a.second > b.second;
        }
        return a.first > b.first;
    });
    std::sort(b_vec.begin(), b_vec.end(), [](const auto &a, const auto &b) {
        if (a.first == b.first) {
            return a.second > b.second;
        }
        return a.first > b.first;
    });
    for (auto i = 0; i < a.size(); ++i) {
        res[i] = dst(rng) % 2 == 0 ? a[i] : b[i];
    }
    return res;
}

auto Population::crossbreeding() -> void {
    // auto bg = population.begin() + preserved_high_positions;
    static std::default_random_engine rng;
    std::uniform_int_distribution<> dst(0, population.size() - 1);
    for (auto i = 0; i < crossing_volume; ++i) {
        // TODO(me): make sure indeces are not equal
        entity new_ent = universal_crossbreeding(population[dst(rng)].first,
                                                 population[dst(rng)].first);
        population.emplace_back(new_ent, calculate_target_fuction(new_ent));
    }
}

auto switch_mutation(entity ent) -> entity {
    static std::default_random_engine rng;
    std::uniform_int_distribution<> dst(0, ent.size() - 1);
    std::swap(ent[dst(rng)], ent[dst(rng)]);
    return ent;
}

auto Population::mutation() -> void {
    static std::default_random_engine rng;
    std::uniform_int_distribution<> dst(0, population.size() - 1);
    for (auto i = 0; i < mutation_volume; ++i) {
        entity new_ent = switch_mutation(population[dst(rng)].first);
        population.emplace_back(new_ent, calculate_target_fuction(new_ent));
    }
}

auto Population::selection(double T) -> void {
    std::vector<std::pair<entity, size_t>> new_population(population.size());
    for (int16_t i = 0; i < preserved_high_positions; ++i) {
        new_population.emplace_back(population[i]);
    }
    for (int16_t i = population.size();
         i > population.size() - preserved_low_positions; --i) {
        new_population.emplace_back(population[i]);
    }
    for (int16_t i = preserved_high_positions;
         i < preserved_high_positions +
                 (preserved_high_positions - preserved_low_positions) * T;
         ++i) {
        new_population.emplace_back(population[i]);
    }
    std::swap(population, new_population);
}

auto Population::sort_population() -> void {
    std::sort(population.begin(), population.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; });
}

auto main() -> int {}