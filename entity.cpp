#include "entity.hpp"

#include <sstream>

enum { base_10 = 10 };

entity_generator::entity_generator() {  // NOLINT
    rng.seed(time(nullptr));
}

auto entity_generator::generate_from_xml(tinyxml2::XMLDocument &doc) -> void {
    tinyxml2::XMLElement *txtnode = doc.FirstChildElement("processor");
    int16_t proxy = 0;
    int16_t from = 0;
    int16_t to = 0;

    tinyxml2::XMLElement *maxload = nullptr;

    while (txtnode != nullptr) {
        maxload = txtnode->FirstChildElement("max_load");
        max_loads.emplace_back(strtol(maxload->GetText(), nullptr, base_10));
        txtnode = txtnode->NextSiblingElement("processor");
    }
    txtnode = doc.FirstChildElement("process");
    while (txtnode != nullptr) {
        maxload = txtnode->FirstChildElement("load");
        progs.emplace_back(strtol(maxload->GetText(), nullptr, base_10));
        txtnode = txtnode->NextSiblingElement("process");
    }
    txtnode = doc.FirstChildElement("connection");
    while (txtnode != nullptr) {
        maxload = txtnode->FirstChildElement("first");
        from = strtol(maxload->GetText(), nullptr, base_10);

        maxload = txtnode->FirstChildElement("second");
        to = strtol(maxload->GetText(), nullptr, base_10);

        maxload = txtnode->FirstChildElement("load");
        proxy = strtol(maxload->GetText(), nullptr, base_10);

        affils.emplace_back(std::make_pair(from, to), proxy);
        affils.emplace_back(std::make_pair(to, from), proxy);

        txtnode = txtnode->NextSiblingElement("connection");
    }
}

auto entity_generator::generate_random_entity() -> entity {
    entity new_ent;
    for (auto i = 0; i < progs.size(); ++i) {
        std::uniform_int_distribution<> dst(0, max_loads.size() - 1);
        new_ent[i] = dst(rng);
    }
    return new_ent;
}

auto entity_generator::check_if_fits(entity ent) -> bool {
    std::vector<int16_t> vacant_power(max_loads);
    for (auto i = 0; i < progs.size(); ++i) {
        vacant_power[ent[i]] -= progs[i];
    }
    for (auto x : vacant_power) {
        if (x < 0) {
            return false;
        }
    }
    return true;
}

auto entity_generator::get_affils() -> std::vector<std::pair<affiliation, int16_t>>{
    return affils;
}

auto entity_generator::print_vals() -> void {
    std::cout << "max loads:" << std::endl;
    std::for_each(max_loads.begin(), max_loads.end(),
                  [](auto n) { std::cout << n << " "; });
    std::cout << std::endl << "progs:" << std::endl;
    std::for_each(progs.begin(), progs.end(),
                  [](auto n) { std::cout << n << " "; });
    std::cout << std::endl << "affils:" << std::endl;
    std::for_each(affils.begin(), affils.end(), [](auto n) {
        std::cout << n.first.first << "\t" << n.first.second << "\t" << n.second
                  << std::endl;
    });
}

// auto entity_generator::get_fittest() { return cur_fittest; }

auto entity_generator::calculate_target_fuction(const entity &ent) -> size_t {
    size_t res = 0;
    for (auto x : affils) {
        if (ent.at(x.first.first) != ent.at(x.first.second)) {
            res += x.second;
        }
    }
    return res/2;
}
