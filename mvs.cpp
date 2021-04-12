#include <iostream>

#include "genetic.hpp"
#include "tinyxml2.h"

int main() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("killme.xml");
    Population popl;
    popl.generate_from_xml(doc);
    popl.print_vals();

    auto cur_best = popl.generate_random_entity();
    auto cur_fit = popl.calculate_target_fuction(cur_best);
    auto valueless_iters = 0;
    while (valueless_iters < 1000) {
        auto new_ent = popl.generate_random_entity();
        auto new_fit = popl.calculate_target_fuction(new_ent);
        if (new_fit < cur_fit) {
            std::cerr << "updating value" << std::endl;
            cur_best = new_ent;
            cur_fit = new_fit;
            valueless_iters = 0;
        } else {
            ++valueless_iters;
        }
    }

    // auto ent = popl.generate_random_entity();
    // std::cout << "random entity: " << std::endl;
    for (auto cur : cur_best) {
        std::cout << cur.first << "\t" << cur.second << std::endl;
    }
    std::cout << "fitness: " << cur_fit;
}