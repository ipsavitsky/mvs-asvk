#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

#include "tinyxml2.h"

using affiliation = std::pair<int16_t, int16_t>;
using entity = std::unordered_map<int16_t, int16_t>;

class Population {
   private:
    std::vector<int16_t> max_loads;
    std::vector<int16_t> progs;
    std::vector<std::pair<affiliation, int16_t>> affils;
    std::default_random_engine rng;
    // entity cur_fittest;
    // size_t cur_fit;

   public:
    Population();
    auto generate_random_entity() -> entity;
    auto generate_from_xml(tinyxml2::XMLDocument &doc) -> void;
    auto print_vals() -> void;
    // auto get_fittest();
    auto calculate_target_fuction(const entity &ent) -> size_t;
};
