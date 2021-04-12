#include "genetic.hpp"

#include <sstream>

enum { base_10 = 10 };

Population::Population(){
    std::cerr << "blessRNG\n";
    rng.seed(time(nullptr));
}

auto Population::generate_from_xml(tinyxml2::XMLDocument &doc) -> void {
    tinyxml2::XMLElement *txtnode = doc.FirstChildElement("processor");
    std::stringstream convert_stream;
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

        txtnode = txtnode->NextSiblingElement("connection");
    }
}

auto Population::generate_random_entity() -> entity {
    entity new_ent;
    std::vector<int16_t> vacant_power(max_loads);
    std::vector<int16_t> valid_iterators;
    // std::cerr << "new entity, MOTHERFUCKER" << std::endl;
    for (auto i = 0; i < progs.size(); ++i) {
        // std::cerr << "fucking with " << progs[i] << " at " << i << std::endl;
        for (auto j = 0; j < vacant_power.size(); ++j) {
            // std::cerr << "\tsucking with " << vacant_power[j] << " at " << j
            //           << std::endl;
            if (progs[i] < vacant_power[j]) {
                valid_iterators.emplace_back(j);
            }
        }
        std::uniform_int_distribution<> dst(0, valid_iterators.size() - 1);
        auto place = dst(rng);
        new_ent[i] = valid_iterators[place];
        vacant_power[place] -= progs[i];
        valid_iterators.clear();
    }
    return new_ent;
}

auto Population::print_vals() -> void {
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

// auto Population::get_fittest() { return cur_fittest; }

auto Population::calculate_target_fuction(const entity &ent) -> size_t{
    size_t res = 0;
    for (auto x : affils) {
        if (ent.at(x.first.first) != ent.at(x.first.second)) {
            res += x.second;
        }
    }
    return res;
}
