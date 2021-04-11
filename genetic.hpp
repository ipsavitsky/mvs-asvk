#include "tinyxml2.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

using affiliation = std::pair<int16_t, int16_t>;
using entity = std::unordered_map<int16_t, int16_t>;

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
    double T;
    auto crossbreeding() -> void;    // done
    auto mutation() -> void;         // done
    auto sort_population() -> void;  // done
    auto selection() -> void;        // done

    auto calculate_target_fuction(const entity &ent);  // done
   public:
    auto generate_from_xml(tinyxml2::XMLDocument &doc) -> void;
    auto print_vals() -> void;
    auto run_generation() -> void;
    auto get_fittest();
};
