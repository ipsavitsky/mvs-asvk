#include <iostream>

#include "entity.hpp"
#include "tinyxml2.h"

int main(int argc, char *argv[]) {
    if(argc != 2){
        std::cerr << "incorrect number of arguments" << std::endl;
        exit(0);
    }
    tinyxml2::XMLDocument doc;
    doc.LoadFile(argv[1]);
    entity_generator popl;
    popl.generate_from_xml(doc);
    // popl.print_vals();

    // auto cur_best = popl.generate_random_entity();
    auto cur_fit = 0;
    for (auto x : popl.get_affils()) {
        cur_fit += x.second;
    }
    entity cur_best;
    auto valueless_iters = 0;
    auto value_iters = 0;
    auto ever_changed = false;
    while (valueless_iters < 1000 && cur_fit != 0) {  // NOLINT
        // std::cerr << valueless_iters << std::endl;
        auto new_ent = popl.generate_random_entity();
        auto new_fit = popl.calculate_target_fuction(new_ent);
        if (new_fit >= cur_fit || !popl.check_if_fits(new_ent)) {
            ++valueless_iters;
            continue;
        }

        // std::cerr << "updating value" << std::endl;
        cur_best = new_ent;
        cur_fit = new_fit;
        valueless_iters = 0;
        ever_changed = true;
        ++value_iters;
    }

    if (ever_changed) {
        std::cout << "success" << std::endl;
        std::cout << value_iters + valueless_iters << std::endl;
        std::for_each(cur_best.begin(), cur_best.end(), [](auto &n) {
            std::cout << n.first << "\t" << n.second << std::endl;
        });
        std::cout << "fitness: " << cur_fit << std::endl;
    }
    else{
        std::cout << "failure" << std::endl;
        std::cout << value_iters + valueless_iters << std::endl;
    }
}